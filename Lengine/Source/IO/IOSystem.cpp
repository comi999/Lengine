#include <map>
#include <IO/IOSystem.hpp>
#include <IO/FileSystem/PlatformFileSystem.hpp>
using std::map;

IOSizeType MaxTagLength = 8u;
map< hash_t, IOSystemInfo > IOSystems;
IOSystemInfo* DefaultIOSystem = nullptr;

IOHeader IOHeader::Null = { EBuildConfiguration::Invalid, nullptr, nullptr };
IOHeader IOHeader::Current;

bool DetectTag( const IOStringViewType& a_Path, IOSizeType a_MaxLength, hash_t& o_Tag, IOSizeType& o_Offset )
{
	constexpr auto StringToUInt = []( const char* str ) constexpr -> uint16_t
	{
		return ( uint16_t )str[ 0 ] << 8u | ( uint16_t )str[ 1 ];
	};

	constexpr uint16_t Sentinel = StringToUInt( ":///" );

	for ( ; a_MaxLength > 0; --a_MaxLength )
	{
		if ( StringToUInt( a_Path.data() + a_MaxLength ) == Sentinel )
		{
			const IOStringType Tag( a_Path.data(), a_MaxLength );
			o_Tag = Hashing::Hash( Tag.c_str() );
			o_Offset = a_MaxLength + 4u;
			return true;
		}
	}

	return false;
}

// There is no guarantee that IOReferenceInfo's get deleted before the owning IOSystemInfo objects,
// so it is important that both can clean up after the other. If an IOSystemInfo object gets cleaned
// up first, it needs to go through all IOReferenceInfo's and clean them up, setting their SystemInfo
// reference to nullptr. If an IOReferenceInfo gets cleaned up first, then it will remove itself from
// the IOSystemInfo object.

IOSystemInfo::~IOSystemInfo()
{
	for ( const auto& ReferenceInfo : Opened )
	{
		CHECK_LOG( ReferenceInfo->SystemInfo, "A reference being tracked by IOSystem with tag '%s' did not have it's SystemInfo reference set.", Name.CStr() );
		ASSERT_LOG( ReferenceInfo->SystemInfo->System->Close( ReferenceInfo->Handle ), "Failed to close handle for IOSystem:'%s'.", Name.CStr() );
		ReferenceInfo->Handle = 0u;
		ReferenceInfo->Mode = EIOOpenMode::Read;
		ReferenceInfo->Path.clear();
		ReferenceInfo->Tag = "";
		ReferenceInfo->Iter = ReferenceInfo->SystemInfo->Opened.end();
		ReferenceInfo->HasHeader = false;
		ReferenceInfo->Header = IOHeader::Null;
	}

	delete System;
	Opened.clear();
}

IOReferenceInfo::~IOReferenceInfo()
{
	if ( SystemInfo )
	{
		// If Reference was open, we need to remove it from list of opened references.
		if ( Handle )
		{
			ENSURE_LOG( SystemInfo->System->Close( Handle ), "Could not close an IOHandle on IOReference deletion." );
			CHECK_LOG( Iter != SystemInfo->Opened.end(), "Opened IOReference was not registered with its linked IOSystem." );
			SystemInfo->Opened.erase( Iter );
			Iter = SystemInfo->Opened.end();
			Handle = 0u;
			Mode = EIOOpenMode::Read;
			HasHeader = false;
			Header = IOHeader::Null;
		}

		SystemInfo = nullptr;
		Path.clear();
		Tag = "";
	}
}

IOReference::IOReference( const IOStringViewType& a_Path, const IOSystem* a_IOSystem )
{
	// If there is no path provided, default to empty reference.
	if ( !a_Path.size() )
	{
		return;
	}

	// If there is an IOSystem provided, we can jump straight to trying to
	// producing an IOReference.
	if ( a_IOSystem )
	{
		// First we need to find the associated IOSystemInfo object.
		const auto SystemInfoIter = IOSystems.find( a_IOSystem->GetTag() );

		if ( ASSERT_LOG( SystemInfoIter != IOSystems.end(), "Provided IOSystem with tag '%s' is not registered.", a_IOSystem->GetTag().CStr() ) )
		{
			IOReferenceInfo* ReferenceInfo = new IOReferenceInfo;
			ReferenceInfo->SystemInfo = &SystemInfoIter->second;
			ReferenceInfo->Path = a_Path;
			ReferenceInfo->Iter = SystemInfoIter->second.Opened.end();
			m_Reference.reset( ReferenceInfo );
		}

		return;
	}

	hash_t Tag; IOSizeType Offset;

	// If we don't have an IOSystem reference provided and no tag is detected in front of path,
	// then search through all IOSystems for one that can open the path.
	if ( !DetectTag( a_Path, MaxTagLength, Tag, Offset ) )
	{
		for ( auto& Info : IOSystems )
		{
			if ( Info.second.System->Exists( a_Path ) )
			{
				IOReferenceInfo* ReferenceInfo = new IOReferenceInfo;
				ReferenceInfo->SystemInfo = &Info.second;
				ReferenceInfo->Path = a_Path;
				ReferenceInfo->Iter = Info.second.Opened.end();
				m_Reference.reset( ReferenceInfo );
				return;
			}
		}

		// If none was found, then it's possible the path doesn't exist yet. Use Default IOSystem if one is set.
		if ( DefaultIOSystem )
		{
			IOReferenceInfo* ReferenceInfo = new IOReferenceInfo;
			ReferenceInfo->SystemInfo = DefaultIOSystem;
			ReferenceInfo->Path = a_Path;
			ReferenceInfo->Iter = DefaultIOSystem->Opened.end();
			m_Reference.reset( ReferenceInfo );
		}

		return;
	}

	// If a tag was detected, let's look for a matching IOSystem.
	const auto Iter = IOSystems.find( Tag );

	// If no IOSystem is found for the given tag, default to empty reference.
	if ( Iter == IOSystems.end() )
	{
		LOG( IO, Warning, "Trying to create reference to %s, but %s is not a registered IO tag.", IOStringType( a_Path ).c_str(), IOStringType( a_Path.data(), Offset ).c_str() );
		return;
	}

	// At this point, we have an IOReference to a System/Path pair, where the path here may not exist yet.
	// Exists() can be used to check if the path exists. Open(EIOOpenMode::Read) will fail for a non-existant
	// path, but Open(EIOOpenMode::Write) will create the file.
	IOReferenceInfo* ReferenceInfo = new IOReferenceInfo;
	ReferenceInfo->SystemInfo = &Iter->second;
	ReferenceInfo->Path = a_Path;
	ReferenceInfo->Iter = Iter->second.Opened.end();
	m_Reference.reset( ReferenceInfo );
}

IOReference::IOReference( const IOStringViewType& a_Path, const hash_t a_IOSystem )
{
	// First we need to find the associated IOSystemInfo object.
	const auto SystemInfoIter = IOSystems.find( a_IOSystem );

	if ( ASSERT_LOG( SystemInfoIter != IOSystems.end(), "IOSystem not found." ) )
	{
		IOReferenceInfo* ReferenceInfo = new IOReferenceInfo;
		ReferenceInfo->SystemInfo = &SystemInfoIter->second;
		ReferenceInfo->Path = a_Path;
		ReferenceInfo->Iter = SystemInfoIter->second.Opened.end();
		m_Reference.reset( ReferenceInfo );
	}
}

bool IOReference::IsOpen() const
{
	return IsBound() && m_Reference->Handle != 0u;
}

bool IOReference::Exists() const
{
	if ( !IsBound() )
	{
		return false;
	}

	return m_Reference->SystemInfo->System->Exists( GetPath() );
}

bool IOReference::Open( const EIOOpenMode a_OpenMode, const bool a_IgnoreHeader ) const
{
	CHECK( !IsOpen() );

	bool Success;

	if ( IOSizeType Size = 0u;
		!a_IgnoreHeader &&
		a_OpenMode != EIOOpenMode::Write && 
		a_OpenMode != EIOOpenMode::WriteRead && 
		Exists() &&
		IOReference::Size( Size ) &&
		Size >= sizeof( IOHeader ) )
	{
		IOHandle Handle = 0u;

		// Out of all other modes other than Write/WriteRead, Append doesn't allow reading. So we
		// need to first open the file in Read mode, read in head, and then reopen in the requested mode.
		if ( !ASSERT_LOG( m_Reference->SystemInfo->System->Open( GetPath(), 
			a_OpenMode == EIOOpenMode::Append ? EIOOpenMode::Read : a_OpenMode, Handle ) && Handle, 
			"Failed to open file '%s'%s", GetPath( true ).data(), a_OpenMode == EIOOpenMode::Append ? " to check for header." : "." ) )
		{
			return false;
		}

		IOHeader Header;
		ENSURE_LOG( m_Reference->SystemInfo->System->Read( Handle, &Header, sizeof( Header ) ), "" );

		// Check to see if the read in header contains the correct watermark.
		if ( IOHeader::IsValid( Header ) )
		{
			m_Reference->HasHeader = true;
			m_Reference->Header = Header;
		}

		// If we had to open it in Read due to the open mode being Append, we need to close it and reopen it with the requested mode.
		// We also need to seek past the head to set it in the right start position.
		if ( a_OpenMode == EIOOpenMode::Append )
		{
			ENSURE_LOG( m_Reference->SystemInfo->System->Close( Handle ), "Failed to close file '%s' after checking for header.", GetPath( true ).data() );
			Handle = 0u;
			ASSERT_LOG( m_Reference->SystemInfo->System->Open( GetPath(), a_OpenMode, Handle ) && Handle, "Failed to open file '%s'.", GetPath( true ).data() );
		}

		ENSURE_LOG( m_Reference->SystemInfo->System->Seek( Handle, m_Reference->HasHeader ? sizeof( Header ) : 0u, EIOSeekOrigin::Beginning ), "Failed to seek to '%s' file '%s'.", GetPath( true ).data(), m_Reference->HasHeader ? "past header for" : "start of" );
		m_Reference->Handle = Handle;
		Success = true;
	}
	else
	{
		Success = ASSERT_LOG( m_Reference->SystemInfo->System->Open( GetPath(), a_OpenMode, m_Reference->Handle ) && m_Reference->Handle, "Failed to open file '%s'.", GetPath( true ).data() );
	}

	if ( Success )
	{
		m_Reference->SystemInfo->Opened.push_back( m_Reference.get() );
		m_Reference->Iter = --m_Reference->SystemInfo->Opened.end();
	}
	
	return Success;
}

bool IOReference::Close() const
{
	CHECK( IsOpen() );

	if ( ASSERT_LOG( m_Reference->SystemInfo->System->Close( m_Reference->Handle ), "Failed to close file '%s'.", GetPath( true ).data() ) )
	{
		m_Reference->Handle = 0u;
		m_Reference->Mode = EIOOpenMode::Read;
		m_Reference->SystemInfo->Opened.erase( m_Reference->Iter );
		m_Reference->Iter = m_Reference->SystemInfo->Opened.end();
		m_Reference->HasHeader = false;
		m_Reference->Header = IOHeader::Null;
		return true;
	}

	return false;
}

bool IOReference::Create( const bool a_IncludeHeader, const IOHeader* a_Header ) const
{
	CHECK( !Exists() );

	// If we are including a header...
	if ( a_IncludeHeader )
	{
		ENSURE( a_Header );

		IOHandle Handle = 0u;

		// Then we need to open the file in write mode to create and write the header to it.
		if ( !ASSERT_LOG( m_Reference->SystemInfo->System->Open( GetPath(), EIOOpenMode::Write, Handle ) && Handle, "Could not open file '%s' to write header.", GetPath( true ).data() ) )
		{
			return false;
		}

		ENSURE_LOG( m_Reference->SystemInfo->System->Write( Handle, a_Header, sizeof( *a_Header ) ), "Could not write header into file '%s'.", GetPath( true ).data() );
		ENSURE_LOG( m_Reference->SystemInfo->System->Close( Handle ), "Could not close file after writing header into file '%s'.", GetPath( true ).data() );

		// We just wrote the header.
		m_Reference->HasHeader = true;
		m_Reference->Header = *a_Header;

		return true;
	}
	
	return m_Reference->SystemInfo->System->Create( GetPath() );
}

bool IOReference::Destroy() const
{
	CHECK( Exists() && !IsOpen() );

	return m_Reference->SystemInfo->System->Destroy( GetPath() );
}

bool IOReference::Resize( const IOSizeType a_Size, const bool a_IncludeHeader ) const
{
	CHECK( Exists() && !IsOpen() );

	return m_Reference->SystemInfo->System->Resize( GetPath(), a_Size + a_IncludeHeader ? sizeof( IOHeader ) : 0u );
}

void IOReference::Clear()
{
	m_Reference.reset();
}

bool IOReference::Write( const void* a_Data, const IOSizeType a_Length ) const
{
	CHECK( IsOpen() );

	return m_Reference->SystemInfo->System->Write( m_Reference->Handle, a_Data, a_Length );
}

bool IOReference::Read( void* a_Data, const IOSizeType a_Length ) const
{
	CHECK( IsOpen() );

	return m_Reference->SystemInfo->System->Read( m_Reference->Handle, a_Data, a_Length );
}

bool IOReference::Tell( IOOffsetType& o_Position ) const
{
	CHECK( IsOpen() );

	if ( IOOffsetType Position = 0u; m_Reference->SystemInfo->System->Tell( m_Reference->Handle, Position ) )
	{
		o_Position = Position - ( m_Reference->HasHeader ? sizeof( IOHeader ) : 0u );
		return true;
	}

	return false;
}

bool IOReference::Seek( const IOOffsetType a_Offset, const EIOSeekOrigin a_Origin ) const
{
	CHECK( IsOpen() );

	const IOOffsetType Offset = a_Offset + ( m_Reference->HasHeader && a_Origin == EIOSeekOrigin::Beginning ? sizeof( IOHeader ) : 0u );

	if ( m_Reference->SystemInfo->System->Seek( m_Reference->Handle, Offset , a_Origin ) )
	{
		return true;
	}

	return false;
}

bool IOReference::Size( IOSizeType& o_Size, const bool a_IncludeHeader ) const
{
	if ( IOSizeType Size = 0u; m_Reference->SystemInfo->System->Size( m_Reference->Path.c_str(), Size ) )
	{
		o_Size = Size - ( a_IncludeHeader ? sizeof( IOHeader ) : 0u );
		return true;
	}

	return false;
}

bool IOSystem::GetFilename( const IOStringViewType& a_Path, IOStringType& o_Filename ) const
{
	o_Filename = Path( a_Path ).GetFilename().ToString();
	return true;
}

bool IOSystem::GetExtension( const IOStringViewType& a_Path, IOStringType& o_Extension ) const
{
	o_Extension = Path( a_Path ).GetExtension().ToString();
	return true;
}

bool IOSystem::GetStem( const IOStringViewType& a_Path, IOStringType& o_Stem ) const
{
	o_Stem = Path( a_Path ).GetStem().ToString();
	return true;
}

bool IOSystem::GetParent( const IOStringViewType& a_Path, IOStringType& o_Parent ) const
{
	o_Parent = Path( a_Path ).GetParentPath().ToString();
	return true;
}

bool IOSystem::GetConcatenated( const IOStringViewType& a_Left, const IOStringViewType& a_Right, IOStringType& o_Concatenated ) const
{
	o_Concatenated = ( Path( a_Left ) / Path( a_Right ) ).ToString();
	return true;
}

IOSystem* IOSystem::FindSystem( const hash_t a_Hash )
{
	if ( const auto Iter = IOSystems.find( a_Hash ); Iter != IOSystems.end() )
	{
		return Iter->second.System;
	}

	return nullptr;
}

void IOSystem::AddSystem( IOSystem* a_IOSystem, const bool a_MakeDefault )
{
	if ( !ASSERT_LOG( a_IOSystem, "Can not add a nullptr IOSystem." ) )
	{
		return;
	}

	if ( !ASSERT_LOG( IOSystems.find( a_IOSystem->GetTag() ) == IOSystems.end(), "IOSystem with tag '%s' is already registered.", a_IOSystem->GetTag().CStr() ) )
	{
		return;
	}

	IOSystemInfo& SystemInfo = IOSystems[ a_IOSystem->GetTag() ];
	SystemInfo.Name = a_IOSystem->GetTag();
	SystemInfo.System = a_IOSystem;

	if ( a_MakeDefault )
	{
		DefaultIOSystem = &SystemInfo;
	}
}

void IOSystem::RemoveSystem( const hash_t a_Name )
{
	const auto Iter = IOSystems.find( a_Name );

	if ( !ASSERT_LOG( Iter != IOSystems.end(), "No IOSystem found." ) )
	{
		return;
	}

	if ( DefaultIOSystem && DefaultIOSystem->Name == a_Name )
	{
		DefaultIOSystem = nullptr;
	}

	IOSystems.erase( Iter );
}

void IOSystem::SetDefaultSystem( const hash_t a_Name )
{
	if ( !a_Name )
	{
		DefaultIOSystem = nullptr;
		return;
	}

	const auto Iter = IOSystems.find( a_Name );

	if ( !ASSERT_LOG( Iter != IOSystems.end(), "No IOSystem registered with given tag. Clearing default." ) )
	{
		DefaultIOSystem = nullptr;
	}
	else
	{
		DefaultIOSystem = &Iter->second;
	}
}