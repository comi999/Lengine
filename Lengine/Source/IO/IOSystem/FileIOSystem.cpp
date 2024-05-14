#include <IO/IOSystem/FileIOSystem.hpp>
#include <IO/FileSystem/PlatformFileSystem.hpp>

DEFINE_INITIALIZER( FileIOSystemRegistration )
{
	if ( ActionType == EInitializerActionType::Startup )
	{
		IOSystem::AddSystem( new FileIOSystem, true );
	}
	else if ( ActionType == EInitializerActionType::Teardown )
	{
		IOSystem::RemoveSystem( FileIOSystem::RootTag );
	}
};

bool FileIOSystem::Open( const IOStringViewType& a_Path, const EIOOpenMode a_OpenMode, IOHandle& o_Handle ) const
{
	if ( const IOHandle Handle = PlatformFileSystem::Open( a_Path, a_OpenMode ) )
	{
		o_Handle = Handle;
		return true;
	}

	return false;
}

bool FileIOSystem::Close( const IOHandle a_Handle ) const
{
	return PlatformFileSystem::Close( a_Handle );
}

bool FileIOSystem::Seek( const IOHandle a_Handle, const IOOffsetType a_Offset, const EIOSeekOrigin a_Origin ) const
{
	return PlatformFileSystem::Seek( a_Handle, a_Offset, a_Origin );
}

bool FileIOSystem::Tell( const IOHandle a_Handle, IOOffsetType& o_Position ) const
{
	return PlatformFileSystem::Tell( a_Handle, o_Position );
}

bool FileIOSystem::Read( const IOHandle a_Handle, void* a_Data, const IOSizeType a_Length ) const
{
	return PlatformFileSystem::Read( a_Handle, a_Data, a_Length );
}

bool FileIOSystem::Write( const IOHandle a_Handle, const void* a_Data, const IOSizeType a_Length )
{
	return PlatformFileSystem::Write( a_Handle, a_Data, a_Length );
}

bool FileIOSystem::Create( const IOStringViewType& a_Path )
{
	const Path FilePath = a_Path;
	PlatformFileSystem::CreateDirectories( FilePath.GetParentPath() );
	return PlatformFileSystem::CreateFile( FilePath );
}

bool FileIOSystem::Destroy( const IOStringViewType& a_Path )
{
	return PlatformFileSystem::Remove( a_Path );
}

bool FileIOSystem::Exists( const IOStringViewType& a_Path ) const
{
	return PlatformFileSystem::Exists( a_Path ) && PlatformFileSystem::IsRegularFile( a_Path );
}

bool FileIOSystem::Copy( const IOStringViewType& a_From, const IOStringViewType& a_To )
{
	const Path To = a_To;

	if ( !To.IsDirectory() )
	{
		return false;
	}

	if ( const Path From = a_From; From.IsDirectory() )
	{
		PlatformFileSystem::Copy( From, To );
	}
	else
	{
		PlatformFileSystem::CopyFile( From, To );
	}

	return true;
}

bool FileIOSystem::Move( const IOStringViewType& a_From, const IOStringViewType& a_To )
{
	if ( !Copy( a_From, a_To ) )
	{
		return false;
	}

	ASSERT_LOG( Destroy( a_From ), "Failed to delete file/directory during move of '%s' to 's'", a_From, a_To );

	return true;
}

bool FileIOSystem::Size( const IOStringViewType& a_Path, IOSizeType& o_Size ) const
{
	if ( !Exists( a_Path ) )
	{
		o_Size = 0u;
		return false;
	}

	o_Size = PlatformFileSystem::GetFileSize( a_Path );
	return true;
}

bool FileIOSystem::Resize( const IOStringViewType& a_Path, const IOSizeType a_Size )
{
	if ( !Exists( a_Path ) )
	{
		return false;
	}

	PlatformFileSystem::ResizeFile( a_Path, a_Size );
	return true;
}