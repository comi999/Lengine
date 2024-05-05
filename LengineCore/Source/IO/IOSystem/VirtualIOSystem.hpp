#pragma once
#include <IO/IOSystem.hpp>
#include <Utility/Initializer.hpp>

typedef bool( __stdcall* GetVirtualFileSizeCallback )( const char* a_Path, int64_t* o_FileSize );
typedef bool( __stdcall* GetVirtualFileDataCallback )( const char* a_Path, void* o_FileContents, int64_t a_Offset, int64_t a_Length );

DECLARE_INITIALIZER( VirtualIOSystemRegistration );

//==========================================================================
// Talks to the VirtualFileSystem.
//==========================================================================
class VirtualIOSystem final : public IOSystem
{
public:

	static constexpr Name RootTag = "virtual"_N;

	VirtualIOSystem() = default;
	~VirtualIOSystem() override = default;

	Name GetTag() const override { return RootTag; }

	// File operations.
	bool Open( const IOStringViewType& a_Path, EIOOpenMode a_OpenMode, IOHandle& o_Handle ) const override;
	bool Close( IOHandle a_Handle ) const override;
	bool Seek( IOHandle a_Handle, IOOffsetType a_Offset, EIOSeekOrigin a_Origin ) const override;
	bool Tell( IOHandle a_Handle, IOOffsetType& o_Position ) const override;
	bool Read( IOHandle a_Handle, void* a_Data, IOSizeType a_Length ) const override;
	bool Write( IOHandle a_Handle, const void* a_Data, IOSizeType a_Length ) override;
	bool Create( const IOStringViewType& a_Path ) override;
	//bool Destroy( const IOStringViewType& a_Path ) override;
	bool Exists( const IOStringViewType& a_Path ) const override;
	//bool Copy( const IOStringViewType& a_From, const IOStringViewType& a_To ) override;
	//bool Move( const IOStringViewType& a_From, const IOStringViewType& a_To ) override;
	bool Size( const IOStringViewType& a_Path, IOSizeType& o_Size ) const override;
	//bool Resize( const IOStringViewType& a_Path, IOSizeType a_Size ) override;

	// Path operations. (DEFAULT TO BASE IMPLEMENTATION)
	//bool GetFilename( const IOStringViewType& a_Path, IOStringType& o_Filename ) const override;
	//bool GetExtension( const IOStringViewType& a_Path, IOStringType& o_Extension ) const override;
	//bool GetStem( const IOStringViewType& a_Path, IOStringType& o_Stem ) const override;
	//bool GetParent( const IOStringViewType& a_Path, IOStringType& o_Parent ) const override;
	//bool GetConcatenated( const IOStringViewType& a_Left, const IOStringViewType& a_Right, IOStringType& o_Concatenated ) const override;

	static void SetVirtualFileCallbacks( GetVirtualFileSizeCallback a_GetFileSize, GetVirtualFileDataCallback a_GetFileData );

private:

	struct VirtualFile
	{
		IOStringType Path;
		IOSizeType Size;
		IOOffsetType Offset;
		std::list< VirtualFile >::iterator Iterator;
	};

	mutable std::list< VirtualFile > m_VirtualFiles;
};