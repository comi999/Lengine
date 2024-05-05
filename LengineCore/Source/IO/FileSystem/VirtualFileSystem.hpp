#pragma once
#include <Utility/Manager.hpp>
#include <unordered_map>
#include <vector>
#include <Utility/CachedMap.hpp>

//==========================================================================
using std::unordered_map;
using std::vector;
using std::move;

//==========================================================================
// Provides the data for VirtualIOSystem.
//==========================================================================
class VirtualFileSystem : public ISingleton< VirtualFileSystem, false >
{
public:

    // Provides the total size of a virtual file.
    using FileSizeProvider = Invoker< int64_t >;

    // Provides the address to the requested block of data
    using FileDataProvider = Invoker< bool, void*, int64_t, int64_t >;

    // Updates the block of data with the provided data
    using FileDataConsumer = Invoker< bool, const void*, int64_t, int64_t >;


public:

    //======================================================================
    void OnPostSingletonConstructed();

    //======================================================================
    // Store an indirect file provider. The data will be copied from a permanent source elsewhere.
    bool AddFileIndirect( const char* a_VirtualPath, const FileSizeProvider& a_SizeProvider, const FileDataProvider& a_DataProvider, const FileDataConsumer& a_DataConsumer = nullptr );

    //======================================================================
    // Copy and store data for a file from somewhere. Useful if the data is not expected to hang around.
    bool AddFile( const char* a_VirtualPath, const void* a_FileData, int64_t a_FileSize );

    //======================================================================
    // Move and store data for a file from somewhere. Useful if the data is not expected to hang around.
    bool AddFile( const char* a_VirtualPath, vector< uint8_t >&& a_FileData );

    //======================================================================
    // Similar to AddFile above, but just creates an empty file.
    bool AddFile( const char* a_VirtualPath )
    {
        vector< uint8_t > NoData;
        return AddFile( a_VirtualPath, move( NoData ) );
    }

public:

    //======================================================================
    bool GetFileSize( const char* a_Path, int64_t* o_FileSize );
    bool GetFileData( const char* a_Path, void* o_FileContents, int64_t a_Offset, int64_t a_Length );
    bool SetFileData( const char* a_Path, const void* a_FileContents, int64_t a_Offset, int64_t a_Length );

private:

    //==================================================================
    // Describes a single direct or indirect virtual file.
    struct VirtualFileEntry
    {
        bool IsDirect   = false;
        FileSizeProvider  SizeProvider;
        FileDataProvider  DataProvider;
        FileDataConsumer  DataConsumer;
        vector< uint8_t > DirectBuffer;

        bool IsReadOnly() const
        {
            return !IsDirect && !DataConsumer.IsBound();
        }
    };

    CachedMap< string, VirtualFileEntry > m_VirtualFiles;
};

