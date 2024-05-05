#include <IO/FileSystem/VirtualFileSystem.hpp>
#include <IO/IOSystem/VirtualIOSystem.hpp>

//==========================================================================
using std::forward;

//==========================================================================
void VirtualFileSystem::OnPostSingletonConstructed()
{
}

//==========================================================================
bool VirtualFileSystem::AddFileIndirect( const char* a_VirtualPath, const FileSizeProvider& a_SizeProvider, const FileDataProvider& a_DataProvider, const FileDataConsumer& a_DataConsumer )
{
    ENSURE( a_VirtualPath );
    ENSURE( a_SizeProvider.IsBound() );
    ENSURE( a_DataProvider.IsBound() );
    CHECK( !m_VirtualFiles.Contains( a_VirtualPath ) );
    VirtualFileEntry& NewEntry = m_VirtualFiles[ a_VirtualPath ];
    NewEntry.IsDirect = false;
    NewEntry.SizeProvider = a_SizeProvider;
    NewEntry.DataProvider = a_DataProvider;
    NewEntry.DataConsumer = a_DataConsumer;
    return true;
}

//==========================================================================
bool VirtualFileSystem::AddFile( const char* a_VirtualPath, const void* a_FileData, int64_t a_FileSize )
{
    ENSURE( a_VirtualPath );
    ENSURE( a_FileData );
    ENSURE( a_FileSize > 0 );
    CHECK( !m_VirtualFiles.Contains( a_VirtualPath ) );
    VirtualFileEntry& NewEntry = m_VirtualFiles[ a_VirtualPath ];
    NewEntry.IsDirect = true;
    NewEntry.DirectBuffer.resize( a_FileSize ); 
    memcpy_s( NewEntry.DirectBuffer.data(), a_FileSize, a_FileData, a_FileSize );
    return true;
}

//==========================================================================
bool VirtualFileSystem::AddFile( const char* a_VirtualPath, vector< uint8_t >&& a_FileData )
{
    ENSURE( a_VirtualPath );
    CHECK( !m_VirtualFiles.Contains( a_VirtualPath ) );
    VirtualFileEntry& NewEntry = m_VirtualFiles[ a_VirtualPath ];
    NewEntry.IsDirect = true;
    NewEntry.DirectBuffer = forward< vector< uint8_t > >( a_FileData );
    return true;
}

//==========================================================================
bool VirtualFileSystem::GetFileSize( const char* a_Path, int64_t* o_FileSize )
{
    ENSURE( a_Path );
    ENSURE( o_FileSize );

    auto It = m_VirtualFiles.Find( a_Path );
    if ( It == m_VirtualFiles.End() )
    {
        return false;
    }
    auto& VirtualFile = It->second;

    if ( VirtualFile.IsDirect )
    {
        *o_FileSize = VirtualFile.DirectBuffer.size();
        return true;
    }

    if ( ASSERT( VirtualFile.SizeProvider.IsBound() ) )
    {
        *o_FileSize = VirtualFile.SizeProvider();
        return true;
    }

    return false;
}

//==========================================================================
bool VirtualFileSystem::GetFileData( const char* a_Path, void* o_FileContents, int64_t a_Offset, int64_t a_Length )
{
    ENSURE( a_Path );
    ENSURE( a_Offset >= 0 );
    ENSURE( a_Length >= 0 );
    ENSURE( o_FileContents );

    auto It = m_VirtualFiles.Find( a_Path );
    if ( It == m_VirtualFiles.End() )
    {
        return false;
    }
    auto& VirtualFile = It->second;

    if ( VirtualFile.IsDirect )
    {
        if ( !ASSERT( ( size_t )( a_Offset + a_Length ) <= VirtualFile.DirectBuffer.size() ) )
        {
            return false;
        }

        memcpy_s( o_FileContents, a_Length, VirtualFile.DirectBuffer.data() + a_Offset, a_Length );
        return true;
    }

    if ( ASSERT( VirtualFile.SizeProvider.IsBound() &&
                 VirtualFile.DataProvider.IsBound() ) )
    {
        if ( !ASSERT( a_Offset + a_Length <= VirtualFile.SizeProvider() ) )
        {
            return false;
        }

        return VirtualFile.DataProvider( o_FileContents, a_Offset, a_Length );
    }

    return false;
}

//==========================================================================
bool VirtualFileSystem::SetFileData( const char* a_Path, const void* a_FileContents, int64_t a_Offset, int64_t a_Length )
{
    ENSURE( a_Path );
    ENSURE( a_Offset >= 0 );
    ENSURE( a_Length >= 0 );
    ENSURE( a_FileContents );

    auto It = m_VirtualFiles.Find( a_Path );
    if ( It == m_VirtualFiles.End() )
    {
        return false;
    }
    auto& VirtualFile = It->second;

    if ( VirtualFile.IsReadOnly() )
    {
        return false;
    }

    if ( VirtualFile.IsDirect )
    {
        if ( ( size_t )( a_Offset + a_Length ) > VirtualFile.DirectBuffer.size() )
        {
            VirtualFile.DirectBuffer.resize( a_Offset + a_Length );
        }

        memcpy_s( VirtualFile.DirectBuffer.data() + a_Offset, a_Length, a_FileContents, a_Length );
        return true;
    }

    if ( ASSERT( VirtualFile.SizeProvider.IsBound() &&
                 VirtualFile.DataConsumer.IsBound() ) )
    {
        return VirtualFile.DataConsumer( a_FileContents, a_Offset, a_Length );
    }

    return false;
}