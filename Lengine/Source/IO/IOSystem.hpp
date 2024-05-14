#pragma once
#include <cstdint>
#include <string>
#include <string_view>
#include <memory>
#include <Core/Build.hpp>
#include <Utility/Name.hpp>

DECLARE_LOG_CATEGORY( IO );
#define IO_WATERMARK A,p,P,1,E

// Describes how to open a file.
enum class EIOOpenMode
{
	// Opens a file for reading only, if it exists.
	// If file exists, it will not be cleared.
	// If file doesn't exist, it will not be created.
	// Head will point to first character.
	// Equivalent to "r".
	Read, 

	// Opens a file for writing only (no reading).
	// If file exists, it will be cleared.
	// If file doesn't exist, it will be created.
	// Head will point to first character.
	// Equivalent to "w".
	Write,

	// Opens a file for appending only (no reading).
	// If file exists, it will not be cleared.
	// If file doesn't exist, it will be created.
	// Head will point to last character.
	// All writes will append to end of file.
	// Equivalent to "a".
	Append,

	// Open a file for reading and writing.
	// If file exists, it will not be cleared.
	// If file doesn't exist, it will not be created.
	// Head will point to first character.
	// Equivalent to "r+".
	ReadWrite,

	// Open a file for writing and reading.
	// If file exists, it will be cleared.
	// If file doesn't exist, it will created.
	// Head will point to first character.
	// Equivalent to "w+".
	WriteRead,

	// Open a file for appending and reading.
	// If file exists, it will not be cleared.
	// If file doesn't exist, it will be created.
	// Head will point to last character.
	// All writes will append to end of file.
	// Equivalent to "a+".
	AppendRead,
};

// Describes where in the file to seek from.
enum class EIOSeekOrigin
{
	// Seek from the beginning of the file.
	// Position: 0 + offset.
	Beginning = SEEK_SET,

	// Seek from current head position.
	// Position: head + offset.
	Current = SEEK_CUR,

	// Seek from end of file.
	// Position: size + offset.
	End = SEEK_END
};

using IOHandle = uint64_t;
using IOOffsetType = fpos_t;
using IOSizeType = size_t;
using IOStringType = std::string;
using IOStringViewType = std::string_view;

class IOSystem;
struct IOReferenceInfo;

struct IOHeader
{
	using WaterMarkType = const char[ 5 ];
	using ConfigurationTagType = char[ 3 ];
	using UserDataType = char[ 5 ];

	WaterMarkType Watermark = { FOR_EACH( CHR, IO_WATERMARK ) };
	ConfigurationTagType Configuration = { FOR_EACH( CHR, CONFIG_TOKENS ) };
	version_t MajorVersion = VERSION_MAJOR;
	version_t MinorVersion = VERSION_MINOR;
	version_t PatchVersion = VERSION_PATCH;
	UserDataType UserData = { 0, 0, 0, 0, 0 };

	static IOHeader Null;
	static IOHeader Current;

	constexpr IOHeader() = default;
	constexpr IOHeader( EBuildConfiguration a_BuildConfiguration, const BuildVersion* a_BuildVersion, const char* a_UserData );
	constexpr IOHeader( const IOHeader& a_Header );
	constexpr IOHeader& operator=( const IOHeader& a_Header );

    // Create an IOHeader with provided UserData.
	static constexpr IOHeader Create( const char* a_UserData = nullptr );

    // Create an IOHeader with the provided BuildVersion and UserData.
	static constexpr IOHeader Create( BuildVersion a_BuildVersion, const char* a_UserData = nullptr );

	// Check if the provided header contains the correct watermark.
	static constexpr bool IsValid( const IOHeader& a_Header );

	// Get the Build configuration contained in the header.
	static constexpr EBuildConfiguration GetBuildConfiguration( const IOHeader& a_Header );

	static constexpr void SetBuildConfiguration( IOHeader& a_Header, EBuildConfiguration a_BuildConfiguration );

	// Get the build version contained in the header.
	static constexpr BuildVersion GetBuildVersion( const IOHeader& a_Header );

	static constexpr void SetBuildVersion( IOHeader& a_Header, BuildVersion a_BuildVersion );

	// Get a reference to the user data contained in the header.
	static constexpr const UserDataType& GetUserData( const IOHeader& a_Header );

	// Set the user data contained in the header.
	static constexpr void SetUserData( IOHeader& a_Header, const char* a_UserData );
};

constexpr bool IOHeader::IsValid( const IOHeader& a_Header )
{
	for ( uint32_t i = 0; i < sizeof( Watermark ); ++i )
	{
		if ( Current.Watermark[ i ] != a_Header.Watermark[ i ] )
		{
			return false;
		}
	}

	return true;
}

constexpr EBuildConfiguration IOHeader::GetBuildConfiguration( const IOHeader& a_Header )
{
	switch ( Hashing::Hash( a_Header.Configuration, sizeof( a_Header.Configuration ) ) )
	{
	case COMBINE( CONFIG_TAG_DEBUG, _H ):
	{
		return EBuildConfiguration::Debug;
	}
	case COMBINE( CONFIG_TAG_RELEASE, _H ):
	{
		return EBuildConfiguration::Release;
	}
	default:
	{
		return EBuildConfiguration::Invalid;
	}
	}
}

constexpr void IOHeader::SetBuildConfiguration( IOHeader& a_Header, const EBuildConfiguration a_BuildConfiguration )
{
	const char* ConfigTag = nullptr;

	switch ( ( uint32_t )a_BuildConfiguration )
	{
	case ( uint32_t )EBuildConfiguration::Debug:
	{
		ConfigTag = CONFIG_TAG_DEBUG;
		break;
	}
	case ( uint32_t )EBuildConfiguration::Release:
	{
		ConfigTag = CONFIG_TAG_DEBUG;
		break;
	}
	default:
	{
		for ( uint32_t i = 0; i < sizeof( Configuration ); ++i )
		{
			a_Header.Configuration[ i ] = 0;
		}

		return;
	}
	}

	for ( uint32_t i = 0; i < sizeof( Configuration ); ++i )
	{
		a_Header.Configuration[ i ] = ConfigTag[ i ];
	}
}

constexpr BuildVersion IOHeader::GetBuildVersion( const IOHeader& a_Header )
{
	return { a_Header.MajorVersion, a_Header.MinorVersion, a_Header.PatchVersion };
}

constexpr void IOHeader::SetBuildVersion( IOHeader& a_Header, const BuildVersion a_BuildVersion )
{
	a_Header.MajorVersion = a_BuildVersion.Major;
	a_Header.MinorVersion = a_BuildVersion.Minor;
	a_Header.PatchVersion = a_BuildVersion.Patch;
}

constexpr const IOHeader::UserDataType& IOHeader::GetUserData( const IOHeader& a_Header )
{
	return a_Header.UserData;
}

constexpr void IOHeader::SetUserData( IOHeader& a_Header, const char* a_UserData )
{
	uint32_t i = 0;

	for ( ; a_UserData && a_UserData[ i ] && i < sizeof( UserData ); ++i )
	{
		a_Header.UserData[ i ] = a_UserData[ i ];
	}

	for ( ; i < sizeof( UserData ); ++i )
	{
		a_Header.UserData[ i ] = 0;
	}
}

constexpr IOHeader::IOHeader( const IOHeader& a_Header )
{
	*this = a_Header;
}

constexpr IOHeader::IOHeader( const EBuildConfiguration a_BuildConfiguration, const BuildVersion* a_BuildVersion, const char* a_UserData )
{
	SetBuildConfiguration( *this, a_BuildConfiguration );

	if ( a_BuildVersion )
	{
		SetBuildVersion( *this, *a_BuildVersion );
	}
	else
	{
		MajorVersion = 0;
		MinorVersion = 0;
		PatchVersion = 0;
	}

	SetUserData( *this, a_UserData );
}

constexpr IOHeader& IOHeader::operator=( const IOHeader& a_Header )
{
	for ( uint32_t i = 0; i < sizeof( Configuration ); ++i )
	{
		Configuration[ i ] = a_Header.Configuration[ i ];
	}

	MajorVersion = a_Header.MajorVersion;
	MinorVersion = a_Header.MinorVersion;
	PatchVersion = a_Header.PatchVersion;

	for ( uint32_t i = 0; i < sizeof( UserData ); ++i )
	{
		UserData[ i ] = a_Header.UserData[ i ];
	}

	return *this;
}

constexpr IOHeader IOHeader::Create( const char* a_UserData )
{
	IOHeader Result;
    SetUserData( Result, a_UserData );
	return Result;
}

constexpr IOHeader IOHeader::Create( const BuildVersion a_BuildVersion, const char* a_UserData )
{
	IOHeader Result = Create( a_UserData );
    SetBuildVersion( Result, a_BuildVersion );
	return Result;
}

struct IOSystemInfo
{
	Name Name;
	IOSystem* System;
	std::list< IOReferenceInfo* > Opened;

	~IOSystemInfo();
};

struct IOReferenceInfo
{
	IOSystemInfo* SystemInfo = nullptr;
	IOHandle Handle = 0u;
	EIOOpenMode Mode = EIOOpenMode::Read;
	IOStringType Path;
	IOStringViewType Tag;
	IOHeader Header = IOHeader::Null;
	bool HasHeader = false;
	std::list< IOReferenceInfo* >::iterator Iter;

	~IOReferenceInfo();
};

class IOReference final
{
public:

	IOReference() = default;
	IOReference( const IOReference& ) = delete;
	IOReference( IOReference&& ) = default;
	IOReference( const IOStringViewType& a_Path, const IOSystem* a_IOSystem = nullptr );
	IOReference( const IOStringViewType& a_Path, hash_t a_IOSystem );
	~IOReference() = default;
	IOReference& operator=( const IOReference& ) = delete;
	IOReference& operator=( IOReference&& ) = default;

	// Is the IOReference bound to a path?
	bool IsBound() const { return m_Reference.get(); }

	// Is the IOReference referring to an opened file?
	bool IsOpen() const;

	// Is the IOReference referring to an existing file?
	bool Exists() const;

	// Open the file with the given mode. Using Open(..., false) with a non-write mode will check for a file header.
	bool Open( EIOOpenMode a_OpenMode, bool a_IgnoreHeader = false ) const;

	// Close the file. This will clear any information about any headers the file had.
	bool Close() const;

	// Create a file that doesn't exist. If IncludeHeader is true, either a Current header, or a provided header
	// will be written into the head of the file.
	bool Create( bool a_IncludeHeader = false, const IOHeader* a_Header = &IOHeader::Current ) const;

	// Destroy a file that exists.
	bool Destroy() const;

	// Resize a file. Must not be open before calling this. If IncludeHeader is false,
	// IOHeader size will be added additionally.
	bool Resize( IOSizeType a_Size, bool a_IncludeHeader = false ) const;

	// Clear will unbind the reference.
	void Clear();

	// Get the IOSystem that the bound file path is part of.
	IOSystem* GetSystem() const { return m_Reference->SystemInfo->System; }

	// Get the open mode of the opened file. If file is closed, this will be meaningless.
	EIOOpenMode GetMode() const { return m_Reference->Mode; }

	// Get the path to the referenced file. If IncludeTag is false, only the path after tag:/// will be included.
	IOStringViewType GetPath( const bool a_IncludeTag = false ) const { if ( a_IncludeTag ) return m_Reference->Path; return m_Reference->Path.c_str() + m_Reference->Tag.size(); }

	// Write data to the opened file.
	bool Write( const void* a_Data, IOSizeType a_Length ) const;

	// Read data from the opened file.
	bool Read( void* a_Data, IOSizeType a_Length ) const;

	// Get the position of the head relative to just after the header, or the start of the file if there is no header.
	bool Tell( IOOffsetType& o_Position ) const;

	// Seek the position of the header relative to just after the header, or the start of the file if there is no header.
	bool Seek( IOOffsetType a_Offset, EIOSeekOrigin a_Origin ) const;

	// Find the size of the file. If IncludeHeader is false, size of IOHeader will be excluded.
	bool Size( IOSizeType& o_Size, bool a_IncludeHeader = false ) const;

	// Does the open file contain a header?
	bool HasHeader() const { return m_Reference->HasHeader; }

	// Get the BuildConfiguration found in the file's header.
	EBuildConfiguration GetBuildConfiguration() const { return IOHeader::GetBuildConfiguration( m_Reference->Header ); }

	// Get the BuildVersion found in the file's header.
	BuildVersion GetBuildVersion() const { return IOHeader::GetBuildVersion( m_Reference->Header ); }

	// Get the UserData founder in the file's header.
	const IOHeader::UserDataType& GetUserData() const { return IOHeader::GetUserData( m_Reference->Header ); }

private:

	std::unique_ptr< IOReferenceInfo > m_Reference;
};

class IOSystem
{
public:

	virtual ~IOSystem() = default;
	virtual Name GetTag() const = 0;
	
	// File operations.
	virtual bool Open( const IOStringViewType& a_Path, EIOOpenMode a_OpenMode, IOHandle& o_Handle ) const { NOT_IMPLEMENTED; return false; }
	virtual bool Close( IOHandle a_Handle ) const { NOT_IMPLEMENTED; return false; }
	virtual bool Seek( IOHandle a_Handle, IOOffsetType a_Offset, EIOSeekOrigin a_Origin ) const { NOT_IMPLEMENTED; return false; }
	virtual bool Tell( IOHandle a_Handle, IOOffsetType& o_Position ) const { NOT_IMPLEMENTED; return false; }
	virtual bool Read( IOHandle a_Handle, void* a_Data, IOSizeType a_Length ) const { NOT_IMPLEMENTED; return false; }
	virtual bool Write( IOHandle a_Handle, const void* a_Data, IOSizeType a_Length ) { NOT_IMPLEMENTED; return false; }
	virtual bool Create( const IOStringViewType& a_Path ) { NOT_IMPLEMENTED; return false; }
	virtual bool Destroy( const IOStringViewType& a_Path ) { NOT_IMPLEMENTED; return false; }
	virtual bool Exists( const IOStringViewType& a_Path ) const { NOT_IMPLEMENTED; return false; }
	virtual bool Copy( const IOStringViewType& a_From, const IOStringViewType& a_To ) { NOT_IMPLEMENTED; return false; }
	virtual bool Move( const IOStringViewType& a_From, const IOStringViewType& a_To ) { NOT_IMPLEMENTED; return false; }
	virtual bool Size( const IOStringViewType& a_Path, IOSizeType& o_Size ) const { NOT_IMPLEMENTED; return false; }
	virtual bool Resize( const IOStringViewType& a_Path, IOSizeType a_Size ) { NOT_IMPLEMENTED; return false; }

	// Path operations.
	virtual bool GetFilename( const IOStringViewType& a_Path, IOStringType& o_Filename ) const;
	virtual bool GetExtension( const IOStringViewType& a_Path, IOStringType& o_Extension ) const;
	virtual bool GetStem( const IOStringViewType& a_Path, IOStringType& o_Stem ) const;
	virtual bool GetParent( const IOStringViewType& a_Path, IOStringType& o_Parent ) const;
	virtual bool GetConcatenated( const IOStringViewType& a_Left, const IOStringViewType& a_Right, IOStringType& o_Concatenated ) const;

	static IOSystem* FindSystem( hash_t a_Hash );
	static void AddSystem( IOSystem* a_IOSystem, bool a_MakeDefault = false );
	static void RemoveSystem( hash_t a_Name );
	static void SetDefaultSystem( hash_t a_Name );
};