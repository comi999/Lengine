#pragma once

// --- STD ---
#include <memory>
#include <string>
#include <vector>
#include <map>

using std::string;
using std::vector;
using std::map;

#define DEFINE_RESOURCE_TYPE( Name ) \
	class R##Name; \
	using Name##Handle = std::shared_ptr< R##Name >; \
	template <> constexpr EResourceType ResourceType< R##Name > = EResourceType::Name; \
	class R##Name : public RResource

#define DEFINE_RESOURCE_BODY( Name ) \
	friend class RResource; \
	R##Name() = default; \
	R##Name( const R##Name& ) = default; \
	R##Name( R##Name&& ) = default; \
	R##Name( const string& a_Name, const string& a_Path ) \
		: RResource( a_Name, a_Path ) {} \
	~R##Name() = default; \
	R##Name& operator=( const R##Name& ) = default; \
	R##Name& operator=( R##Name&& ) = default; \
	EResourceType GetType() const override { return EResourceType::Name; }

class RResource;

template < typename T >
using TResourceHandle = std::shared_ptr< T >;

using ResourceHandle = TResourceHandle< RResource >;

enum class EResourceType
{
	None,
	Mesh,
	Material,
	Texture,
	Skeleton,
	Animation,
	Light,
};

template < typename _Resource >
static constexpr EResourceType ResourceType = EResourceType::None;

// Represents a bundle of resources. Maintains handles to resources, not
// the resources themselves.
class ResourcePack
{
public:

	ResourcePack() = default;
	ResourcePack( const ResourcePack& ) = default;
	ResourcePack( ResourcePack&& ) = default;
	
	ResourcePack& operator=( const ResourcePack& ) = default;
	ResourcePack& operator=( ResourcePack&& ) = default;

	template < typename T >
	TResourceHandle< T > GetResource( const size_t a_Index ) const;

	ResourceHandle GetResource( const size_t a_Index ) const;

	template < typename T >
	TResourceHandle< T > GetResourceByAlias( const string& a_Alias ) const;

	ResourceHandle GetResourceByAlias( const string& a_Alias ) const;

	template < typename T >
	TResourceHandle< T > GetResourceByName( const string& a_Name ) const;

	ResourceHandle GetResourceByName( const string& a_Name ) const;

	template < typename T >
	void AddResource( const TResourceHandle< T >& a_Handle, const string& a_Alias = {} )
	{
		if ( !a_Alias.empty() )
		{
			// CHECK( m_AliasLookup.find( a_Alias ) == m_AliasLookup.end(), "Alias already being used." );
			m_AliasLookup[ a_Alias ] = m_Handles.size();
		}

		m_Handles.push_back( std::static_pointer_cast< RResource >( a_Handle ) );
	}

	void Clear() { m_Handles.clear(); }
	size_t Size() const { return m_Handles.size(); }

private:

	vector< ResourceHandle > m_Handles;
	map< string, size_t > m_AliasLookup;
};

class RResource
{
public:

	// Declare non-trivial constructors.
	virtual ~RResource() = default;

protected:

	RResource() = default;
	RResource( const RResource& ) = default;
	RResource( RResource&& ) = default;
	RResource& operator=( const RResource& ) = default;
	RResource& operator=( RResource&& ) = default;

	RResource( const string& a_Name, const string& a_Path )
		: m_Name( a_Name )
		, m_Path( a_Path )
	{}

public:

	// Get the type of resource contained.
	virtual EResourceType GetType() const { return EResourceType::None; }

	// Get the path of the file that this resource was loaded from.
	const string& GetPath()	const { return m_Path; }

	// Get the name of the resource.
	const string& GetName() const { return m_Name; }

	// Get the resource pack of all child resources that this resource depends on.
	// ie. Mesh -> Skeleton.
	const ResourcePack& GetChildren() const { return m_Children; }

private:


	
	string		 m_Name;
	string		 m_Path;
	ResourcePack m_Children;

public:

	// Load a file and mount all resources found inside.
	// eg. fbx -> (Mesh, Material, Light, Skeleton... ).
	static ResourcePack LoadFile( const string& a_Name, const string& a_Path );

	template < typename T >
	static TResourceHandle< T > MakeResource( const string& a_Name, const string& a_Path = {} )
	{
		return std::make_shared< T >( a_Name, a_Path );
	}

private:

	static ResourcePack s_LoadedResources;
};

template < typename T >
TResourceHandle< T > ResourcePack::GetResource( const size_t a_Index ) const
{
	// CHECK( !( a_Index < m_Handles.size() ), "Index out of range." );
	// CHECK( ResourceType< T > == m_Handles[ a_Index ].GetType(), "Incorrect type!" );

	return std::shared_ptr< T >( m_Handles[ a_Index ], ( T* )m_Handles[ a_Index ].get() );
}

inline ResourceHandle ResourcePack::GetResource( const size_t a_Index ) const
{
	// CHECK( !( a_Index < m_Handles.size() ), "Index out of range." );

	return m_Handles[ a_Index ];
}

template < typename T >
TResourceHandle< T > ResourcePack::GetResourceByAlias( const string& a_Alias ) const
{
	const auto Iter = m_AliasLookup.find( a_Alias );

	if ( Iter == m_AliasLookup.end() )
	{
		return {};
	}

	return GetResource< T >( Iter->second );
}

inline ResourceHandle ResourcePack::GetResourceByAlias( const string& a_Alias ) const
{
	const auto Iter = m_AliasLookup.find( a_Alias );

	if ( Iter == m_AliasLookup.end() )
	{
		return {};
	}

	return GetResource( Iter->second );
}

template < typename T >
TResourceHandle< T > ResourcePack::GetResourceByName( const string& a_Name ) const
{
	for ( size_t i = 0; i < m_Handles.size(); ++i )
	{
		if ( m_Handles[ i ]->GetName() == a_Name )
		{
			return GetResource< T >( i );
		}
	}

	return {};
}

inline ResourceHandle ResourcePack::GetResourceByName( const string& a_Name ) const
{
	for ( size_t i = 0; i < m_Handles.size(); ++i )
	{
		if ( m_Handles[ i ]->GetName() == a_Name )
		{
			return GetResource( i );
		}
	}

	return {};
}