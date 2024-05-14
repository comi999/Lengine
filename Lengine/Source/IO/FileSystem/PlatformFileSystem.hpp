#pragma once
#include <filesystem>
#include <string>

#include <Utility/EnumTraits.hpp>
#include <IO/IOSystem.hpp>

#undef CreateDirectory
#undef CopyFile
#undef CreateHardLink

using std::move;
namespace fs = std::filesystem;
class PlatformFileSystem;

enum class ECopyOptions : std::underlying_type_t< fs::copy_options >
{
	None = ( std::underlying_type_t< fs::copy_options > )fs::copy_options::none,
	SkipExisting = ( std::underlying_type_t< fs::copy_options > )fs::copy_options::skip_existing,
	OverwriteExisting = ( std::underlying_type_t< fs::copy_options > )fs::copy_options::overwrite_existing,
	UpdateExisting = ( std::underlying_type_t< fs::copy_options > )fs::copy_options::update_existing,
	Recursive = ( std::underlying_type_t< fs::copy_options > )fs::copy_options::recursive,
	CopySymlinks = ( std::underlying_type_t< fs::copy_options > )fs::copy_options::copy_symlinks,
	SkipSymlinks = ( std::underlying_type_t< fs::copy_options > )fs::copy_options::skip_symlinks,
	DirectoriesOnly = ( std::underlying_type_t< fs::copy_options > )fs::copy_options::directories_only,
	CreateSymlinks = ( std::underlying_type_t< fs::copy_options > )fs::copy_options::create_symlinks,
	CreateHardLinks = ( std::underlying_type_t< fs::copy_options > )fs::copy_options::create_hard_links
};

DEFINE_BITMASK_OPS( ECopyOptions );

enum class EFileType : std::underlying_type_t< fs::file_type >
{
	None = ( std::underlying_type_t< fs::file_type > )fs::file_type::none,
	NotFound = ( std::underlying_type_t< fs::file_type > )fs::file_type::not_found,
	Regular = ( std::underlying_type_t< fs::file_type > )fs::file_type::regular,
	Directory = ( std::underlying_type_t< fs::file_type > )fs::file_type::directory,
	Symlink = ( std::underlying_type_t< fs::file_type > )fs::file_type::symlink,
	Block = ( std::underlying_type_t< fs::file_type > )fs::file_type::block,
	Character = ( std::underlying_type_t< fs::file_type > )fs::file_type::character,
	Fifo = ( std::underlying_type_t< fs::file_type > )fs::file_type::fifo,
	Socket = ( std::underlying_type_t< fs::file_type > )fs::file_type::socket,
	Unknown = ( std::underlying_type_t< fs::file_type > )fs::file_type::unknown,
};

enum class EFilePerm : std::underlying_type_t< fs::perms >
{
	None = ( std::underlying_type_t< fs::perms > )fs::perms::none,
	OwnerRead = ( std::underlying_type_t< fs::perms > )fs::perms::owner_read,
	OwnerWrite = ( std::underlying_type_t< fs::perms > )fs::perms::owner_write,
	OwnerExec = ( std::underlying_type_t< fs::perms > )fs::perms::owner_exec,
	OwnerAll = ( std::underlying_type_t< fs::perms > )fs::perms::owner_all,
	GroupRead = ( std::underlying_type_t< fs::perms > )fs::perms::group_read,
	GroupWrite = ( std::underlying_type_t< fs::perms > )fs::perms::group_write,
	GroupExec = ( std::underlying_type_t< fs::perms > )fs::perms::group_exec,
	GroupAll = ( std::underlying_type_t< fs::perms > )fs::perms::group_all,
	OthersRead = ( std::underlying_type_t< fs::perms > )fs::perms::others_read,
	OthersWrite = ( std::underlying_type_t< fs::perms > )fs::perms::others_write,
	OtherExec = ( std::underlying_type_t< fs::perms > )fs::perms::others_exec,
	OthersAll = ( std::underlying_type_t< fs::perms > )fs::perms::others_all,
	All = ( std::underlying_type_t< fs::perms > )fs::perms::all,
	SetUID = ( std::underlying_type_t< fs::perms > )fs::perms::set_uid,
	SetGID = ( std::underlying_type_t< fs::perms > )fs::perms::set_gid,
	StickyBit = ( std::underlying_type_t< fs::perms > )fs::perms::sticky_bit,
	Mask = ( std::underlying_type_t< fs::perms > )fs::perms::mask,
	Unknown = ( std::underlying_type_t< fs::perms > )fs::perms::unknown,
};

DEFINE_BITMASK_OPS( EFilePerm );

enum class EFilePermOption : std::underlying_type_t< fs::perm_options >
{
	Replace = ( std::underlying_type_t< fs::perm_options > )fs::perm_options::replace,
	Add = ( std::underlying_type_t< fs::perm_options > )fs::perm_options::add,
	Remove = ( std::underlying_type_t< fs::perm_options > )fs::perm_options::remove,
	NoFollow = ( std::underlying_type_t< fs::perm_options > )fs::perm_options::nofollow
};

DEFINE_BITMASK_OPS( EFilePermOption );

class FileStatus
{
public:

	FileStatus() = default;
	FileStatus( const FileStatus& ) = default;
	FileStatus( FileStatus&& ) = default;
	~FileStatus() = default;
	FileStatus& operator=( const FileStatus& ) = default;
	FileStatus& operator=( FileStatus&& ) = default;

	EFileType GetType() const { return ( EFileType )m_FileStatus.type(); }
	void SetType( const EFileType a_FileType ) { m_FileStatus.type( ( fs::file_type )a_FileType ); }
	EFilePerm GetPermissions() const { return ( EFilePerm )m_FileStatus.permissions(); }
	void SetPermissions( const EFilePerm a_FilePerm ) { m_FileStatus.permissions( ( fs::perms )a_FilePerm ); }

private:

	friend class PlatformFileSystem;

	explicit FileStatus( const fs::file_status a_FileStatus ) : m_FileStatus( a_FileStatus ) {}

	fs::file_status m_FileStatus;
};

using FileTime = fs::file_time_type;

struct SpaceInfo
{
	IOSizeType Capacity;
	IOSizeType Free;
	IOSizeType Available;

	SpaceInfo() = default;
	SpaceInfo( const SpaceInfo& ) = default;
	SpaceInfo( SpaceInfo&& ) = default;
	SpaceInfo( const fs::space_info& a_SpaceInfo ) : Capacity( a_SpaceInfo.capacity ), Free( a_SpaceInfo.free ), Available( a_SpaceInfo.available ) {}
	~SpaceInfo() = default;
	SpaceInfo& operator=( const SpaceInfo& ) = default;
	SpaceInfo& operator=( SpaceInfo&& ) = default;

	operator fs::space_info() { return *( fs::space_info* )this; }
};

class Path
{
public:
	
	using ValueType = fs::path::value_type;
	using StringType = fs::path::string_type;
	using StringViewType = std::basic_string_view< ValueType >;
	using IteratorType = fs::path::iterator;
	using CIteratorType = fs::path::const_iterator;

	Path() = default;
	Path( const Path& ) = default;
	Path( Path&& ) = default;
	 
	template < typename _StringT >
	Path( _StringT&& a_Path )
	{
		using StringType = std::remove_cv_t< std::remove_reference_t< _StringT > >;
		constexpr bool IsPathType = std::is_same_v< StringType, Path >;

		if constexpr ( std::is_rvalue_reference_v<decltype(a_Path)> )
		{
			if constexpr ( IsPathType )
			{
				m_Path = std::move( a_Path.m_Path );
			}
			else
			{
				m_Path = std::move( a_Path );
			}
		}
		else
		{
			if constexpr ( IsPathType )
			{
				m_Path =  a_Path.m_Path;
			}
			else
			{
				m_Path = std::forward< _StringT >( a_Path );
			}
		}
	}


	template < typename _Iter >
	Path( _Iter a_Begin, _Iter a_End ) : m_Path( a_Begin, a_End ) {}

	~Path() = default;
	Path& operator=( const Path& ) = default;
	Path& operator=( Path&& ) = default;
	
	//template < typename _String, typename = std::enable_if_t< !std::is_same_v< std::remove_reference_t< _String >, Path > > >
	//Path& operator=( _String&& a_String ) { m_Path = std::forward< _String >( a_String ); return *this; }

	template < typename _String >
	Path& operator=( _String&& a_Path )
	{
		if constexpr ( std::is_rvalue_reference_v<decltype(a_Path)> )
		{
			if constexpr ( std::is_same_v< std::decay_t<_String>, Path> )
			{
				m_Path = std::move( a_Path.m_Path );
			}
			else
			{
				m_Path = std::move( a_Path );
			}
		}
		else
		{
			if constexpr ( std::is_same_v< std::decay_t<_String>, Path> )
			{
				m_Path =  a_Path.m_Path;
			}
			else
			{
				m_Path = std::forward< _String >( a_Path );
			}
		}

		return *this;
	}

	const auto& NativeString() const noexcept { return m_Path.native(); }
	const auto* CString() const noexcept { return m_Path.c_str(); }
	operator const StringType& () const { return m_Path; }

	inline Path ToAbsolute() const;
	inline Path ToRelative() const;
	inline Path ToRelative( const Path& a_To ) const;
	inline Path ToProximate() const;
	inline Path ToProximate( const Path& a_To ) const;
	inline Path ToCanonical() const;
	inline Path ToWeaklyCanonical() const;

	Path& MakeAbsolute() { *this = ToAbsolute(); return *this; }
	Path& MakeRelative() { *this = ToRelative(); return *this; }
	Path& MakeRelative( const Path& a_To ) { *this = ToRelative( a_To ); return *this; }
	Path& MakeProximate() { *this = ToProximate(); return *this; }
	Path& MakeProximate( const Path& a_To ) { *this = ToProximate( a_To ); return *this; }
	Path& MakeCanonical() { *this = ToCanonical(); return *this; }
	Path& MakeWeaklyCanonical() { *this = ToWeaklyCanonical(); return *this; }
	
	Path ToLexicallyNormal() const { return Path{ m_Path.lexically_normal() }; }
	Path ToLexicallyRelative( const Path& a_To ) const { return Path{ m_Path.lexically_relative( a_To.m_Path ) }; }
	Path ToLexicallyProximate( const Path& a_To ) const { return Path{ m_Path.lexically_proximate( a_To.m_Path ) }; }

	Path& MakeLexicallyNormal() { *this = ToLexicallyNormal(); return *this; }
	Path& MakeLexicallyRelative( const Path& a_To ) { *this = ToLexicallyRelative( a_To ); return *this; }
	Path& MakeLexicallyProximate( const Path& a_To ) { *this = ToLexicallyProximate( a_To ); return *this; }

	void Clear() { m_Path.clear(); }
	Path& MakePreferred() { ( void )m_Path.make_preferred(); return *this; }
	Path& RemoveFilename() { ( void )m_Path.remove_filename(); return *this; }
	Path& RemoveExtension() { ( void )m_Path.replace_extension(); return *this; }
	Path& ReplaceFilename( const Path& a_Replacement ) { ( void )m_Path.replace_filename( a_Replacement.m_Path ); return *this; }
	Path& ReplaceExtension( const Path& a_Replacement ) { ( void )m_Path.replace_extension( a_Replacement.m_Path ); return *this; }
	void Swap( Path& a_Other ) { m_Path.swap( a_Other.m_Path ); }

	auto ToString() const { return m_Path.string(); }
	auto ToWString() const { return m_Path.wstring(); }
	auto ToU8String() const { return m_Path.u8string(); }
	auto ToU16String() const { return m_Path.u16string(); }
	auto ToU32String() const { return m_Path.u32string(); }
	auto ToGenericString() const { return m_Path.generic_string(); }
	auto ToGenericWString() const { return m_Path.generic_wstring(); }
	auto ToGenericU8String() const { return m_Path.generic_u8string(); }
	auto ToGenericU16String() const { return m_Path.generic_u16string(); }
	auto ToGenericU32String() const { return m_Path.generic_u32string(); }

	int32_t Compare( const Path& a_Path ) const { return m_Path.compare( a_Path.m_Path ); }
	int32_t Compare( const StringType& a_String ) const { return m_Path.compare( a_String ); }
	int32_t Compare( const StringViewType& a_StringView ) const { return m_Path.compare( a_StringView ); }
	int32_t Compare( const ValueType* a_String ) const { return m_Path.compare( a_String ); }

	Path GetRootName() const { return Path{ m_Path.root_name() }; }
	Path GetRootDirectory() const { return Path{ m_Path.root_directory() }; }
	Path GetRootPath() const { return Path{ m_Path.root_path() }; }
	Path GetRelativePath() const { return Path{ m_Path.relative_path() }; }
	Path GetParentPath() const { return Path{ m_Path.parent_path() }; }
	Path GetFilename() const { return Path{ m_Path.filename() }; }
	Path GetExtension() const { return Path{ m_Path.extension() }; }
	Path GetStem() const { return Path{ m_Path.stem() }; }

	bool HasRootName() const { return m_Path.has_root_name(); }
	bool HasRootDirectory() const { return m_Path.has_root_directory(); }
	bool HasRootPath() const { return m_Path.has_root_path(); }
	bool HasRelativePath() const { return m_Path.has_relative_path(); }
	bool HasParentPath() const { return m_Path.has_parent_path(); }
	bool HasFilename() const { return m_Path.has_filename(); }
	bool HasExtension() const { return m_Path.has_extension(); }
	bool HasStem() const { return m_Path.has_stem(); }
	
	bool IsEmpty() const { return m_Path.empty(); }
	bool IsAbsolute() const { return m_Path.is_absolute(); }
	bool IsRelative() const { return m_Path.is_relative(); }

	CIteratorType Begin() const { return m_Path.begin(); }
	CIteratorType CBegin() const { return m_Path.begin(); }
	CIteratorType End() const { return m_Path.end(); }
	CIteratorType CEnd() const { return m_Path.end(); }

	inline bool IsBlockFile() const;
	inline bool IsCharacterFile() const;
	inline bool IsDirectory() const;
	inline bool IsFifo() const;
	inline bool IsOther() const;
	inline bool IsRegularFile() const;
	inline bool IsSocket() const;
	inline bool IsSymlink() const;

	Path& operator+=( const Path& a_Path ) { m_Path += a_Path.m_Path; return *this; }
	Path& operator/=( const Path& a_Path ) { m_Path /= a_Path.m_Path; return *this; }

	friend bool operator<( const Path& a_LHS, const Path& a_RHS ) { return a_LHS.m_Path < a_RHS.m_Path; }
	friend bool operator<=( const Path& a_LHS, const Path& a_RHS ) { return a_LHS.m_Path <= a_RHS.m_Path; }
	friend bool operator>( const Path& a_LHS, const Path& a_RHS ) { return a_LHS.m_Path > a_RHS.m_Path; }
	friend bool operator>=( const Path& a_LHS, const Path& a_RHS ) { return a_LHS.m_Path >= a_RHS.m_Path; }
	friend bool operator==( const Path& a_LHS, const Path& a_RHS ) { return a_LHS.m_Path == a_RHS.m_Path; }
	friend bool operator!=( const Path& a_LHS, const Path& a_RHS ) { return a_LHS.m_Path != a_RHS.m_Path; }
	friend Path operator+( const Path& a_LHS, const Path& a_RHS ) { return Path{ Path{ a_LHS.m_Path } += Path{ a_RHS.m_Path } }; }
	friend Path operator/( const Path& a_LHS, const Path& a_RHS ) { return Path{ a_LHS.m_Path / a_RHS.m_Path }; }

private:

	friend class PlatformFileSystem;
	friend class DirectoryIterator;
	friend class RecursiveDirectoryIterator;

	fs::path m_Path;
};

namespace std
{
	static auto begin( const Path& a_Path ) { return a_Path.Begin(); }
	static auto cbegin( const Path& a_Path ) { return a_Path.CBegin(); }
	static auto end( const Path& a_Path ) { return a_Path.End(); }
	static auto cend( const Path& a_Path ) { return a_Path.CEnd(); }
	static void swap( Path& a_LHS, Path& a_RHS ) noexcept { a_LHS.Swap( a_RHS ); }
}

class PlatformFileSystem final
{
public:

	PlatformFileSystem() = delete;

	static IOHandle Open( const Path& a_Path, EIOOpenMode a_Mode = EIOOpenMode::Read, bool a_Binary = true );
	static bool Close( IOHandle a_Handle );
	static bool Read( IOHandle a_Handle, void* a_Data, IOSizeType a_Length );
	static bool Write( IOHandle a_Handle, const void* a_Data, IOSizeType a_Length );
	static bool Seek( IOHandle a_Handle, IOOffsetType a_Offset, EIOSeekOrigin a_Origin );
	static bool Tell( IOHandle a_Handle, IOOffsetType& a_Offset );
	static Path Absolute( const Path& a_Path ) { return Path{ fs::absolute( a_Path.m_Path ) }; }
	static Path Canonical( const Path& a_Path ) { return Path{ fs::canonical( a_Path.m_Path ) }; }
	static Path Proximate( const Path& a_Path, const Path& a_Base = GetCurrentPath() ) { return Path{ fs::proximate( a_Path.m_Path, a_Base.m_Path ) }; }
	static Path Relative( const Path& a_Path, const Path& a_Base = GetCurrentPath() ) { return Path{ fs::relative( a_Path.m_Path, a_Base.m_Path ) }; }
	static Path WeaklyCanonical( const Path& a_Path ) { return Path{ fs::weakly_canonical( a_Path.m_Path ) }; }
	static void Copy( const Path& a_From, const Path& a_To ) { fs::copy( a_From.m_Path, a_To.m_Path ); }
	static void Copy( const Path& a_From, const Path& a_To, ECopyOptions a_CopyOptions ) { fs::copy( a_From.m_Path, a_To.m_Path, ( fs::copy_options )a_CopyOptions ); }
	static bool CopyFile( const Path& a_From, const Path& a_To ) { return fs::copy_file( a_From.m_Path, a_To.m_Path ); }
	static bool CopyFile( const Path& a_From, const Path& a_To, ECopyOptions a_CopyOptions ) { return fs::copy_file( a_From.m_Path, a_To.m_Path, ( fs::copy_options )a_CopyOptions ); }
	static void CopySymlink( const Path& a_From, const Path& a_To ) { fs::copy_symlink( a_From.m_Path, a_To.m_Path ); }
	static bool CreateFile( const Path& a_Path );
	static bool CreateDirectory( const Path& a_Path ) { return fs::create_directory( a_Path.m_Path ); }
	static bool CreateDirectory( const Path& a_Path, const Path& a_Attributes ) { return fs::create_directory( a_Path.m_Path, a_Attributes.m_Path ); }
	static bool CreateDirectories( const Path& a_Path ) { return fs::create_directories( a_Path.m_Path ); }
	static void CreateHardLink( const Path& a_Path, const Path& a_Link ) { fs::create_hard_link( a_Path.m_Path, a_Link.m_Path ); }
	static void CreateSymlink( const Path& a_Path, const Path& a_Link ) { fs::create_symlink( a_Path.m_Path, a_Link.m_Path ); }
	static void CreateDirectorySymlink( const Path& a_Path, const Path& a_Link ) { fs::create_directory_symlink( a_Path.m_Path, a_Link.m_Path ); }
	static Path GetCurrentPath() { return Path{ fs::current_path() }; }
	static bool Exists( const FileStatus& a_FileStatus ) { return fs::exists( a_FileStatus.m_FileStatus ); }
	static bool Exists( const Path& a_Path ) { return fs::exists( a_Path.m_Path ); }
	static bool Equivalent( const Path& a_LHS, const Path& a_RHS ) { return fs::equivalent( a_LHS.m_Path, a_RHS.m_Path ); }
	static uintmax_t GetFileSize( const Path& a_Path ) { return fs::file_size( a_Path.m_Path ); }
	static uintmax_t GetHardLinkCount( const Path& a_Path ) { return fs::hard_link_count( a_Path.m_Path ); }
	static FileTime GetLastWriteTime( const Path& a_Path ) { return fs::last_write_time( a_Path.m_Path ); }
	static void SetPermissions( const Path& a_Path, EFilePerm a_FilePerm, EFilePermOption a_FilePermOption = EFilePermOption::Add ) { fs::permissions( a_Path.m_Path, ( fs::perms )a_FilePerm, ( fs::perm_options )a_FilePermOption ); }
	static Path ReadSymlink( const Path& a_Path ) { return Path{ fs::read_symlink( a_Path.m_Path ) }; }
	static bool Remove( const Path& a_Path ) { return fs::remove( a_Path.m_Path ); }
	static IOSizeType RemoveAll( const Path& a_Path ) { return fs::remove_all( a_Path.m_Path ); }
	static void Rename( const Path& a_Path, const Path& a_RenameTo ) { fs::rename( a_Path.m_Path, a_RenameTo.m_Path ); }
	static void ResizeFile( const Path& a_Path, const IOSizeType a_NewSize ) { fs::resize_file( a_Path.m_Path, a_NewSize ); }
	static SpaceInfo GetSpaceInfo( const Path& a_Path ) { return fs::space( a_Path.m_Path ); }
	static FileStatus GetStatus( const Path& a_Path ) { return FileStatus{ fs::status( a_Path.m_Path ) }; }
	static FileStatus GetSymlinkStatus( const Path& a_Path ) { return FileStatus{ fs::symlink_status( a_Path.m_Path ) }; }
	static Path GetTempDirectoryPath() { return Path{ fs::temp_directory_path() }; }
	static bool IsBlockFile( const FileStatus& a_FileStatus ) { return fs::is_block_file( a_FileStatus.m_FileStatus ); }
	static bool IsBlockFile( const Path& a_Path ) { return fs::is_block_file( a_Path.m_Path ); }
	static bool IsCharacterFile( const FileStatus& a_FileStatus ) { return fs::is_character_file( a_FileStatus.m_FileStatus ); }
	static bool IsCharacterFile( const Path& a_Path ) { return fs::is_character_file( a_Path.m_Path ); }
	static bool IsDirectory( const FileStatus& a_FileStatus ) { return fs::is_directory( a_FileStatus.m_FileStatus ); }
	static bool IsDirectory( const Path& a_Path ) { return fs::is_directory( a_Path.m_Path ); }
	static bool IsEmpty( const Path& a_Path ) { return fs::is_empty( a_Path.m_Path ); }
	static bool IsFifo( const FileStatus& a_FileStatus ) { return fs::is_fifo( a_FileStatus.m_FileStatus ); }
	static bool IsFifo( const Path& a_Path ) { return fs::is_fifo( a_Path.m_Path ); }
	static bool IsOther( const FileStatus& a_FileStatus ) { return fs::is_other( a_FileStatus.m_FileStatus ); }
	static bool IsOther( const Path& a_Path ) { return fs::is_other( a_Path.m_Path ); }
	static bool IsRegularFile( const FileStatus& a_FileStatus ) { return fs::is_regular_file( a_FileStatus.m_FileStatus ); }
	static bool IsRegularFile( const Path& a_Path ) { return fs::is_regular_file( a_Path.m_Path ); }
	static bool IsSocket( const FileStatus& a_FileStatus ) { return fs::is_socket( a_FileStatus.m_FileStatus ); }
	static bool IsSocket( const Path& a_Path ) { return fs::is_socket( a_Path.m_Path ); }
	static bool IsSymlink( const FileStatus& a_FileStatus ) { return fs::is_symlink( a_FileStatus.m_FileStatus ); }
	static bool IsSymlink( const Path& a_Path ) { return fs::is_symlink( a_Path.m_Path ); }
	static bool IsStatusKnown( const FileStatus& a_FileStatus ) { return fs::status_known( a_FileStatus.m_FileStatus ); }
};

bool Path::IsBlockFile() const { return PlatformFileSystem::IsBlockFile( *this ); }
bool Path::IsCharacterFile() const { return PlatformFileSystem::IsCharacterFile( *this ); }
bool Path::IsDirectory() const { return PlatformFileSystem::IsDirectory( *this ); }
bool Path::IsFifo() const { return PlatformFileSystem::IsFifo( *this ); }
bool Path::IsOther() const { return PlatformFileSystem::IsOther( *this ); }
bool Path::IsRegularFile() const { return PlatformFileSystem::IsRegularFile( *this ); }
bool Path::IsSocket() const { return PlatformFileSystem::IsSocket( *this ); }
bool Path::IsSymlink() const { return PlatformFileSystem::IsSymlink( *this ); }
Path Path::ToAbsolute() const { return PlatformFileSystem::Absolute( *this ); }
Path Path::ToRelative() const { return PlatformFileSystem::Relative( *this ); }
Path Path::ToRelative( const Path& a_To ) const { return PlatformFileSystem::Relative( *this, a_To ); }
Path Path::ToProximate() const { return PlatformFileSystem::Proximate( *this ); }
Path Path::ToProximate( const Path& a_To ) const { return PlatformFileSystem::Proximate( *this, a_To ); }
Path Path::ToCanonical() const { return PlatformFileSystem::Canonical( *this ); }
Path Path::ToWeaklyCanonical() const { return PlatformFileSystem::WeaklyCanonical( *this ); }

enum class EDirectoryOptions : std::underlying_type_t< fs::directory_options >
{
	None = ( std::underlying_type_t< fs::directory_options > )fs::directory_options::none,
	FollowDirectorySymlink = ( std::underlying_type_t< fs::directory_options > )fs::directory_options::follow_directory_symlink,
	SkipPermissionDenied = ( std::underlying_type_t< fs::directory_options > )fs::directory_options::skip_permission_denied,
};

class DirectoryIterator
{
public:

	DirectoryIterator() noexcept = default;
	DirectoryIterator( const DirectoryIterator& ) noexcept = default;
	DirectoryIterator( DirectoryIterator&& ) noexcept = default;
	DirectoryIterator& operator=( const DirectoryIterator& ) noexcept = default;
	DirectoryIterator& operator=( DirectoryIterator&& ) noexcept = default;
	explicit DirectoryIterator( const Path& a_Path, const EDirectoryOptions a_DirectoryOptions = EDirectoryOptions::None ) : m_Underlying( a_Path.m_Path, ( fs::directory_options )a_DirectoryOptions ) {}

	DirectoryIterator& operator++() { ( void )++m_Underlying; return *this; }
	DirectoryIterator operator++( int ) { auto Result = *this; ++*this; return Result; }
	const Path* operator->() const { return reinterpret_cast< const Path* >( &m_Underlying.operator->()->path() ); }
	const Path& operator*() const { return reinterpret_cast< const Path& >( m_Underlying.operator*().path() ); }
	bool operator==( const DirectoryIterator& a_DirectoryIterator ) const { return m_Underlying == a_DirectoryIterator.m_Underlying; }
	bool operator!=( const DirectoryIterator& a_DirectoryIterator ) const { return m_Underlying != a_DirectoryIterator.m_Underlying; }

private:

	fs::directory_iterator m_Underlying;
};

class RecursiveDirectoryIterator
{
public:

	RecursiveDirectoryIterator() noexcept = default;
	RecursiveDirectoryIterator( const RecursiveDirectoryIterator& ) noexcept = default;
	RecursiveDirectoryIterator( RecursiveDirectoryIterator&& ) noexcept = default;
	RecursiveDirectoryIterator& operator=( const RecursiveDirectoryIterator& ) noexcept = default;
	RecursiveDirectoryIterator& operator=( RecursiveDirectoryIterator&& ) noexcept = default;
	explicit RecursiveDirectoryIterator( const Path& a_Path, const EDirectoryOptions a_DirectoryOptions = EDirectoryOptions::None ) : m_Underlying( a_Path.m_Path, ( fs::directory_options )a_DirectoryOptions ) {}

	RecursiveDirectoryIterator& operator++() { ( void )++m_Underlying; return *this; }
	RecursiveDirectoryIterator operator++( int ) { auto Result = *this; ++* this; return Result; }
	const Path* operator->() const { return reinterpret_cast< const Path* >( &m_Underlying.operator->()->path() ); }
	const Path& operator*() const { return reinterpret_cast< const Path& >( m_Underlying.operator*().path() ); }
	bool operator==( const RecursiveDirectoryIterator& a_DirectoryIterator ) const { return m_Underlying == a_DirectoryIterator.m_Underlying; }
	bool operator!=( const RecursiveDirectoryIterator& a_DirectoryIterator ) const { return m_Underlying != a_DirectoryIterator.m_Underlying; }
	operator bool() const { return *this != RecursiveDirectoryIterator(); }
	Path GetPath() const { return Path{ m_Underlying->path() }; }

private:

	fs::recursive_directory_iterator m_Underlying;
};

template <>
struct std::iterator_traits< DirectoryIterator >
{
	using iterator_category = std::input_iterator_tag;
	using value_type = Path;
	using difference_type = ptrdiff_t;
	using pointer = const Path*;
	using reference = const Path&;
};

template <>
struct std::iterator_traits< RecursiveDirectoryIterator >
{
	using iterator_category = std::input_iterator_tag;
	using value_type = Path;
	using difference_type = ptrdiff_t;
	using pointer = const Path*;
	using reference = const Path&;
};