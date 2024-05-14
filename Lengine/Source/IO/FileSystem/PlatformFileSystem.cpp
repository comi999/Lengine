#include <IO/FileSystem/PlatformFileSystem.hpp>

IOHandle PlatformFileSystem::Open( const Path& a_Path, const EIOOpenMode a_Mode, const bool a_Binary )
{
	const char* Mode = nullptr;

	switch ( a_Mode )
	{
	case EIOOpenMode::Read:
	{
		Mode = a_Binary ? "rb" : "r";
		break;
	}
	case EIOOpenMode::Write:
	{
		Mode = a_Binary ? "wb" : "w";
		break;
	}
	case EIOOpenMode::Append:
	{
		Mode = a_Binary ? "ab" : "a";
		break;
	}
	case EIOOpenMode::ReadWrite:
	{
		Mode = a_Binary ? "r+b" : "r+";
		break;
	}
	case EIOOpenMode::WriteRead:
	{
		Mode = a_Binary ? "w+b" : "w+";
		break;
	}
	case EIOOpenMode::AppendRead:
	{
		Mode = a_Binary ? "a+b" : "a+";
		break;
	}
	}

	if ( FILE* File = fopen( a_Path.ToString().c_str(), Mode ) )
	{
		return ( IOHandle )File;
	}

	return ( IOHandle )nullptr;
}

bool PlatformFileSystem::Close( const IOHandle a_Handle )
{
	return !fclose( ( FILE* )a_Handle );
}

bool PlatformFileSystem::Read( const IOHandle a_Handle, void* a_Data, const IOSizeType a_Length )
{
	return fread( a_Data, sizeof( uint8_t ), a_Length, ( FILE* )a_Handle );
}

bool PlatformFileSystem::Write( const IOHandle a_Handle, const void* a_Data, const IOSizeType a_Length )
{
	return fwrite( a_Data, sizeof( uint8_t ), a_Length, ( FILE* )a_Handle );
}

bool PlatformFileSystem::Seek( const IOHandle a_Handle, const IOOffsetType a_Offset, const EIOSeekOrigin a_Origin )
{
	return !fseek( ( FILE* )a_Handle, ( int )a_Offset, ( int )a_Origin );
}

bool PlatformFileSystem::Tell( const IOHandle a_Handle, IOOffsetType& a_Offset )
{
	if ( const auto Position = ftell( ( FILE* )a_Handle ); Position != -1 )
	{
		a_Offset = Position;
		return true;
	}

	return false;
}

bool PlatformFileSystem::CreateFile( const Path& a_Path )
{
	if ( const IOHandle Handle = Open( a_Path, EIOOpenMode::Write ) )
	{
		ASSERT_LOG( PlatformFileSystem::Close( Handle ), "Opened handle to file '%s' but could not close it.", a_Path.ToString().c_str() );
		return true;
	}

	return false;
}