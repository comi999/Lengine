#include <IO/IOSystem/VirtualIOSystem.hpp>
#include <IO/FileSystem/VirtualFileSystem.hpp>

DEFINE_INITIALIZER( VirtualIOSystemRegistration )
{
	if ( ActionType == EInitializerActionType::Startup )
	{
		IOSystem::AddSystem( new VirtualIOSystem );
	}
	else if ( ActionType == EInitializerActionType::Teardown )
	{
		IOSystem::RemoveSystem( VirtualIOSystem::RootTag );
	}
};

bool VirtualIOSystem::Open( const IOStringViewType& a_Path, const EIOOpenMode a_OpenMode, IOHandle& o_Handle ) const
{
	int64_t Size = 0;
	if ( !VirtualFileSystem::Get()->GetFileSize( a_Path.data(), &Size ) )
	{
		// When writing, create the file if it doesn't exist.
		if ( a_OpenMode == EIOOpenMode::Read ||
			 !VirtualFileSystem::Get()->AddFile( a_Path.data() ) )
		{
			return false;
		}
	}

	const auto Iterator = m_VirtualFiles.insert( m_VirtualFiles.end(), VirtualFile{} );
	VirtualFile& File = *Iterator;
	File.Path = a_Path;
	File.Size = ( IOSizeType )Size;
	File.Offset = 0u;
	File.Iterator = Iterator;

	o_Handle = ( IOHandle )&File;
	return true;
}

bool VirtualIOSystem::Close( const IOHandle a_Handle ) const
{
	if ( !a_Handle )
	{
		return false;
	}

	const VirtualFile& File = *( const VirtualFile* )a_Handle;	
	m_VirtualFiles.erase( File.Iterator );
	return true;
}

bool VirtualIOSystem::Seek( const IOHandle a_Handle, const IOOffsetType a_Offset, const EIOSeekOrigin a_Origin ) const
{
	if ( !a_Handle )
	{
		return false;
	}

	VirtualFile& File = *( VirtualFile* )a_Handle;

	switch ( a_Origin )
	{
	case EIOSeekOrigin::Beginning:
	{
		CHECK( a_Offset >= 0 && a_Offset <= ( IOOffsetType )File.Size );
		File.Offset = a_Offset;
		break;
	}
	case EIOSeekOrigin::Current:
	{
		CHECK( File.Offset + a_Offset >= 0 && File.Offset + a_Offset <= ( IOOffsetType )File.Size );
		File.Offset += a_Offset;
		break;
	}
	case EIOSeekOrigin::End:
	{
		CHECK( a_Offset <= 0 && a_Offset >= -( int64_t )File.Size );
		File.Offset += a_Offset;
		break;
	}
	}

	return true;
}

bool VirtualIOSystem::Tell( const IOHandle a_Handle, IOOffsetType& o_Position ) const
{
	if ( !a_Handle )
	{
		return false;
	}

	const VirtualFile& File = *( VirtualFile* )a_Handle;
	o_Position = File.Offset;
	return true;
}

bool VirtualIOSystem::Read( const IOHandle a_Handle, void* a_Data, const IOSizeType a_Length ) const
{
	if ( !a_Handle )
	{
		return false;
	}

	VirtualFile& File = *( VirtualFile* )a_Handle;

	if ( !VirtualFileSystem::Get()->GetFileData( File.Path.c_str(), a_Data, File.Offset, a_Length ) )
	{
		return false;
	}

	File.Offset += a_Length;
	return true;
}

bool VirtualIOSystem::Write( IOHandle a_Handle, const void* a_Data, IOSizeType a_Length )
{
	if ( !a_Handle )
	{
		return false;
	}

	VirtualFile& File = *( VirtualFile* )a_Handle;

	if ( !VirtualFileSystem::Get()->SetFileData( File.Path.c_str(), a_Data, File.Offset, a_Length ) )
	{
		return false;
	}

	File.Offset += a_Length;
	return true;
}

bool VirtualIOSystem::Create( const IOStringViewType& a_Path )
{
	return VirtualFileSystem::Get()->AddFile( a_Path.data() );
}

bool VirtualIOSystem::Exists( const IOStringViewType& a_Path ) const
{
	int64_t Size;
	return VirtualFileSystem::Get()->GetFileSize( a_Path.data(), &Size );
}

bool VirtualIOSystem::Size( const IOStringViewType& a_Path, IOSizeType& o_Size ) const
{
	for ( auto& VirtualFile : m_VirtualFiles )
	{
		if ( VirtualFile.Path == a_Path )
		{
			o_Size = VirtualFile.Size;
			return true;
		}
	}

	return false;
}