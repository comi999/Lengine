#pragma once
#include <utility>
#include <type_traits>
#include <cstdint>
#include <tuple>
#include <string>
#include <string_view>
#include <array>
#include <vector>
#include <deque>
#include <forward_list>
#include <list>
#include <set>
#include <map>
#include <unordered_set>
#include <unordered_map>
#include <stack>
#include <queue>

#include <Utility/ClassTraits.hpp>
#include <Utility/TypeArray.hpp>

#define DECLARE_AS_SERIALISABLE( Version ) \
	static constexpr version_t V##ersion = Version; \
	friend class Serialisation; \
	void OnSize( SizingArchive& Archive ) const; \
	void OnSerialise( OutputArchive& Archive ) const; \
	template < version_t _Version > void OnDeserialise( InputArchive& Archive );

#define DECLARE_AS_UNVERSIONED_SERIALISABLE DECLARE_AS_SERIALISABLE( 255u )

#define DEFINE_SIZER( ... ) \
	void __VA_ARGS__::OnSize( SizingArchive& Archive ) const

#define DEFINE_SERIALISER( ... ) \
	void __VA_ARGS__::OnSerialise( OutputArchive& Archive ) const

#define DEFINE_DESERIALISER( Version, ... ) \
	template <> \
	void __VA_ARGS__::OnDeserialise< Version >( InputArchive& Archive )

#define DEFINE_UNVERSIONED_DESERIALISER( ... ) DEFINE_DESERIALISER( 0u, __VA_ARGS__ )

#define DECLARE_AS_TEMPLATED_SERIALISABLE_EXTERNAL( Version, ... ) \
	struct Serialisation::SerialisationInfo< __VA_ARGS__ > \
	{ \
		static constexpr version_t V##ersion = Version; \
		static constexpr bool IsVersioned = Version != NullVersion; \
		static constexpr bool IsSerialisable = true; \
	}

#define DECLARE_AS_UNVERSIONED_TEMPLATED_SERIALISABLE_EXTERNAL( ... ) DECLARE_AS_TEMPLATED_SERIALISABLE_EXTERNAL( 255u, __VA_ARGS__ )

#define DECLARE_AS_SERIALISABLE_EXTERNAL( Version, ... ) \
	template <> \
	DECLARE_AS_TEMPLATED_SERIALISABLE_EXTERNAL( Version, __VA_ARGS__ )

#define DECLARE_AS_UNVERSIONED_SERIALISABLE_EXTERNAL( ... ) DECLARE_AS_SERIALISABLE_EXTERNAL( 255u, __VA_ARGS__ )

#define DEFINE_TEMPLATED_SIZER_EXTERNAL( ... ) \
	auto Serialisation::Sizer< __VA_ARGS__ > = []( SizingArchive& Archive, const __VA_ARGS__& Object )

#define DEFINE_TEMPLATED_SERIALISER_EXTERNAL( ... ) \
	auto Serialisation::Serialiser< __VA_ARGS__ > = []( OutputArchive& Archive, const __VA_ARGS__& Object )

#define DEFINE_TEMPLATED_DESERIALISER_EXTERNAL( Version, ... ) \
	auto Serialisation::Deserialiser< __VA_ARGS__, Version > = []( InputArchive& Archive, __VA_ARGS__& Object )

#define DEFINE_UNVERSIONED_TEMPLATED_DESERIALISER_EXTERNAL( ... ) DEFINE_TEMPLATED_DESERIALISER_EXTERNAL( 0u, __VA_ARGS__ )

#define DEFINE_SIZER_EXTERNAL( ... ) \
	template <> \
	DEFINE_TEMPLATED_SIZER_EXTERNAL( __VA_ARGS__ )

#define DEFINE_SERIALISER_EXTERNAL( ... ) \
	template <> \
	DEFINE_TEMPLATED_SERIALISER_EXTERNAL( __VA_ARGS__ )

#define DEFINE_DESERIALISER_EXTERNAL( Version, ... ) \
	template <> \
	DEFINE_TEMPLATED_DESERIALISER_EXTERNAL( Version, __VA_ARGS__ )

#define DEFINE_UNVERSIONED_DESERIALISER_EXTERNAL( ... ) DEFINE_DESERIALISER_EXTERNAL( 255u, __VA_ARGS__ )

class SizingArchive;
class OutputArchive;
class InputArchive;
class VersionTable;

class Serialisation final
{
public:

	static constexpr version_t NullVersion = -1;

private:

	template < typename T >
	struct HasOnSize
	{
		template < typename U > static auto test( decltype( std::declval< U >().OnSize( std::declval< SizingArchive& >() ) )* ) -> std::true_type;
		template < typename U > static auto test( ... ) -> std::false_type;
		static constexpr bool Value = decltype( test< T >( nullptr ) )::value;
	};

	template < typename T >
	struct HasOnBeforeSerialise
	{
		template < typename U > static auto test( decltype( std::declval< U >().OnBeforeSerialise() )* ) -> std::true_type;
		template < typename U > static auto test( ... ) -> std::false_type;
		static constexpr bool Value = decltype( test< T >( nullptr ) )::value;
	};

	template < typename T >
	struct HasOnAfterSerialise
	{
		template < typename U > static auto test( decltype( std::declval< U >().OnAfterSerialise() )* ) -> std::true_type;
		template < typename U > static auto test( ... ) -> std::false_type;
		static constexpr bool Value = decltype( test< T >( nullptr ) )::value;
	};

	template < typename T >
	struct HasOnSerialise
	{
		template < typename U > static auto test( decltype( std::declval< U >().OnSerialise( std::declval< OutputArchive& >() ) )* ) -> std::true_type;
		template < typename U > static auto test( ... ) -> std::false_type;
		static constexpr bool Value = decltype( test< T >( nullptr ) )::value;
	};

	template < typename T >
	struct HasOnDeserialise
	{
		template < typename U > static auto test( decltype( std::declval< U >().template OnDeserialise< 0u >( std::declval< InputArchive& >() ) )* ) -> std::true_type;
		template < typename U > static auto test( ... ) -> std::false_type;
		static constexpr bool Value = decltype( test< T >( nullptr ) )::value;
	};

	template < typename T >
	struct HasOnBeforeDeserialise
	{
		template < typename U > static auto test( decltype( std::declval< U >().OnBeforeDeserialise() )* ) -> std::true_type;
		template < typename U > static auto test( ... ) -> std::false_type;
		static constexpr bool Value = decltype( test< T >( nullptr ) )::value;
	};

	template < typename T >
	struct HasOnAfterDeserialise
	{
		template < typename U > static auto test( decltype( std::declval< U >().OnAfterDeserialise() )* ) -> std::true_type;
		template < typename U > static auto test( ... ) -> std::false_type;
		static constexpr bool Value = decltype( test< T >( nullptr ) )::value;
	};

	template < typename T >
	struct HasVersion
	{
		template < typename U > static constexpr auto test( decltype( U::Version )* ) -> std::true_type;
		template < typename U > static constexpr auto test( ... ) -> std::false_type;
		static constexpr bool Value = decltype( test< T >( nullptr ) )::value;
	};

	template < typename T >
	using SizerType = void( * )( SizingArchive&, const T& );

	template < typename T >
	using SerialiserType = void( * )( OutputArchive&, const T& );

	template < typename T >
	using DeserialiserType = void( * )( InputArchive&, T& );

	template < typename T >
	static constexpr auto Sizer = ( SizerType< T > )[]( SizingArchive& Archive, const T& Object ) { Object.OnSize( Archive ); };

	template < typename T >
	static constexpr auto Serialiser = ( SerialiserType< T > )[]( OutputArchive& Archive, const T& Object ) { Object.OnSerialise( Archive ); };

	template < typename T, version_t _Version >
	static constexpr auto Deserialiser = ( DeserialiserType< T > )[]( InputArchive& Archive, T& Object ) { Object.template OnDeserialise< _Version >( Archive ); };

	template < typename T >
	struct SerialisationInfo
	{
		static constexpr version_t Version = []{ if constexpr ( HasVersion< T >::Value ) { return T::Version; } else { return NullVersion; } }();
		static constexpr bool IsVersioned = Version != NullVersion;
		static constexpr bool IsSerialisable = HasOnSize< T >::Value && HasOnSerialise< T >::Value && HasOnDeserialise< T >::Value;
	};

	template < typename T >
	static constexpr auto GetDeserialiserTable()
	{
		if constexpr ( IsVersioned< T > )
		{
			return GetDeserialiserTable< T >( std::make_index_sequence< SerialisationInfo< T >::Version + 1u >{} );
		}
		else
		{
			return GetDeserialiserTable< T >( std::make_index_sequence< 1u >{} );
		}
	}

	template < typename T, size_t... Idxs >
	static constexpr auto GetDeserialiserTable( std::index_sequence< Idxs... > )
	{
		std::array< DeserialiserType< T >, sizeof...( Idxs ) > Deserialisers
		{
			Deserialiser< T, Idxs >...
		};

		return Deserialisers;
	}

	Serialisation() = delete;

	friend class VersionTable;
	friend class SizingArchive;
	friend class OutputArchive;
	friend class InputArchive;

public:

	template < typename T >
	static constexpr version_t Version = SerialisationInfo< T >::Version;

	template < typename T >
	static constexpr bool IsVersioned = SerialisationInfo< T >::IsVersioned;

	template < typename T >
	static constexpr bool IsSerialisable = SerialisationInfo< T >::IsSerialisable;

	template < typename T >
	static constexpr size_t SizeOf( const T& a_Object );

	template < typename T >
	static std::vector< byte_t > Serialise( const T& a_Object, VersionTable* o_VersionTable = nullptr );

	template < typename T >
	static void Serialise( const T& a_Object, byte_t* a_Buffer, size_t* o_BytesWritten = nullptr, VersionTable* o_VersionTable = nullptr );

	template < typename T >
	static void Serialise( const T& a_Object, byte_t* a_Buffer, size_t a_BufferSize, size_t* o_BytesWritten = nullptr, VersionTable* o_VersionTable = nullptr );

	template < typename T >
	static void Deserialise( T& o_Object, const byte_t* a_Data, size_t* o_BytesRead = nullptr, const VersionTable* a_VersionTable = nullptr );

	template < typename T >
	static void Deserialise( T& o_Object, const byte_t* a_Data, size_t a_BufferSize, size_t* o_BytesRead = nullptr, const VersionTable* a_VersionTable = nullptr );

private:

	friend struct VersionTableEntry;
};

struct VersionTableEntry
{
	hash_t TypeHash;
#if CONFIG_DEBUG
	std::string TypeName;
#endif
	version_t Version;
};

class VersionTable
{
public:

	using SizeType = uint16_t;
	using IndexerType = TypeArray< VersionTableEntry, TypeIndexer< TypeAlias< "VersionTable"_H > > >;
#if CONFIG_DEBUG
	using LookupType = std::map< hash_t, VersionTableEntry >;
#else
	using LookupType = std::map< hash_t, version_t >;
#endif

	VersionTable() = default;
	VersionTable( const VersionTable& ) = default;
	VersionTable( VersionTable&& ) = default;
	VersionTable& operator=( const VersionTable& ) = default;
	VersionTable& operator=( VersionTable&& ) = default;

	SizeType Size() const { return ( SizeType )m_TypeVersions.size(); }
	static constexpr SizeType MaxSize() { return -1; }

	template < typename T >
	void SetVersion( std::in_place_type_t< T > = std::in_place_type< T > ) { SetVersion< T >( Serialisation::Version< T > ); }

	template < typename T >
	void SetVersion( version_t a_Version, std::in_place_type_t< T > = std::in_place_type< T > );

	template < typename T >
	version_t GetVersion() const;

	inline version_t GetVersion( hash_t a_TypeHash ) const;

	template < typename T >
	bool Contains() const { return GetVersion< T >() != Serialisation::NullVersion; }

	void Clear();

private:

	DECLARE_AS_UNVERSIONED_SERIALISABLE;

	mutable IndexerType m_VersionTable;
	LookupType m_TypeVersions;
};

template < typename T >
void VersionTable::SetVersion( const version_t a_Version, std::in_place_type_t< T > )
{
	auto& Entry = *m_VersionTable.Assure< T >();
	Entry.TypeHash = ClassTraits< T >::GetTypeHash();
#if CONFIG_DEBUG
	Entry.TypeName = typeid( T ).name();
#endif
	Entry.Version = a_Version;

#if CONFIG_DEBUG
	m_TypeVersions[ Entry.TypeHash ] = Entry;
#else
	m_TypeVersions[ Entry.TypeHash ] = a_Version;
#endif
}

template < typename T >
version_t VersionTable::GetVersion() const
{
	if ( auto Iter = m_VersionTable.Find< T >(); Iter == m_VersionTable.End() )
	{
		if ( auto VersionIter = m_TypeVersions.find( ClassTraits< T >::GetTypeHash() ); VersionIter != m_TypeVersions.end() )
		{
			const_cast< VersionTable* >( this )->SetVersion< T >(
#if CONFIG_DEBUG
				VersionIter->second.Version
#else
				VersionIter->second
#endif
			);

#if CONFIG_DEBUG
			return VersionIter->second.Version;
#else
			return VersionIter->second;
#endif
		}

		return Serialisation::NullVersion;
	}
	else
	{
		return Iter->Version;
	}
}

version_t VersionTable::GetVersion( const hash_t a_TypeHash ) const
{
	if ( const auto Iter = m_TypeVersions.find( a_TypeHash ); Iter != m_TypeVersions.end() )
	{
#if CONFIG_DEBUG
		return Iter->second.Version;
#else
		return Iter->second;
#endif
	}

	return Serialisation::NullVersion;
}

class SizingArchive
{
public:

	constexpr SizingArchive() = default;
	constexpr SizingArchive( const SizingArchive& ) = default;
	constexpr SizingArchive& operator=( const SizingArchive& ) = default;

	template < typename T >
	constexpr SizingArchive& AddSize( const T& a_Object );

	constexpr SizingArchive& AddSizeExplicit( const size_t a_Size ) { m_BytesCounted += a_Size; return *this; }

	template < typename T >
	constexpr SizingArchive& AddSizeExplicit() { return AddSizeExplicit( sizeof( T ) ); }

	template < typename T >
	constexpr SizingArchive& AddSizeExplicit( const size_t a_Size ) { return AddSizeExplicit( sizeof( T ) * a_Size ); }

	template < typename T >
	constexpr SizingArchive& operator+=( const T& a_Object ) { return AddSize( a_Object ); }

	constexpr size_t GetBytesCounted() const { return m_BytesCounted; }

private:
	
	size_t m_BytesCounted = 0u;
};

class OutputArchive
{
public:

	void Open( void* a_Buffer, size_t a_Length, VersionTable* o_VersionTable = nullptr );

	void Close();

	bool IsOpen() const { return m_Buffer; }

	template < typename T >
	void Serialise( const T& a_Object );

	template < typename T >
	void SerialiseExplicit( const T& a_Object ) { Write( &a_Object, sizeof( std::decay_t< T > ) ); }

	void Write( const void* a_Data, const size_t a_Length );

	template < typename T >
	OutputArchive& operator<<( const T& a_Object ) { Serialise( a_Object ); return *this; }

	size_t GetBytesWritten() const { return m_BytesWritten; }

private:

	VersionTable* m_VersionTable = nullptr;
	size_t m_BytesWritten = 0u;
	void* m_Buffer = nullptr;
	size_t m_Length = 0u;
};

class InputArchive
{
public:

	void Open( const void* a_Buffer, size_t a_Length, const VersionTable* a_VersionTable );

	void Close();

	bool IsOpen() const { return m_Buffer; }

	template < typename T >
	void Deserialise( T& a_Object );

	template < typename T >
	void DeserialiseExplicit( T& a_Object ) { Read( ( void* )&a_Object, sizeof( std::decay_t< T > ) ); }

	void Read( void* a_Destination, const size_t a_Length );

	template < typename T >
	InputArchive& operator>>( T& a_Object ) { Deserialise( a_Object ); return *this; }

	size_t GetBytesRead() const { return m_BytesRead; }

private:

	const VersionTable* m_VersionTable = nullptr;
	size_t	m_BytesRead = 0u;
	const void* m_Buffer = nullptr;
	size_t m_Length = 0u;
};

template < typename T >
constexpr size_t Serialisation::SizeOf( const T& a_Object )
{
	using Type = std::conditional_t< std::is_array_v< T >, T, std::decay_t< T > >;

	if constexpr ( IsSerialisable< Type > )
	{
		SizingArchive Archive;
		Archive += a_Object;
		return Archive.GetBytesCounted();
	}
	else
	{
		return sizeof( Type );
	}
}

template < typename T >
std::vector< byte_t > Serialisation::Serialise( const T& a_Object, VersionTable* o_VersionTable )
{
	std::vector< byte_t > Data( SizeOf( a_Object ) );
	OutputArchive Archive;
	Archive.Open( Data.data(), Data.size(), o_VersionTable );
	Archive << a_Object;
	CHECK( Data.size() == Archive.GetBytesWritten() );
	return Data;
}

template < typename T >
void Serialisation::Serialise( const T& a_Object, byte_t* a_Buffer, size_t* o_BytesWritten, VersionTable* o_VersionTable )
{
	Serialise( a_Object, a_Buffer, -1, o_BytesWritten, o_VersionTable );
}

template < typename T >
void Serialisation::Serialise( const T& a_Object, byte_t* a_Buffer, const size_t a_BufferSize, size_t* o_BytesWritten, VersionTable* o_VersionTable )
{
	if ( !a_Buffer || !a_BufferSize )
	{
		*o_BytesWritten = 0u;
		return;
	}

	OutputArchive Archive;
	Archive.Open( a_Buffer, a_BufferSize, o_VersionTable );
	Archive << a_Object;
	CHECK( Archive.GetBytesWritten() <= a_BufferSize );

	if ( o_BytesWritten )
	{
		*o_BytesWritten = Archive.GetBytesWritten();
	}
}

template < typename T >
void Serialisation::Deserialise( T& o_Object, const byte_t* a_Data, size_t* o_BytesRead, const VersionTable* a_VersionTable )
{
	Deserialise( o_Object, a_Data, -1, o_BytesRead, a_VersionTable );
}

template < typename T >
void Serialisation::Deserialise( T& o_Object, const byte_t* a_Data, const size_t a_BufferSize, size_t* o_BytesRead, const VersionTable* a_VersionTable )
{
	if ( !a_Data || !a_BufferSize )
	{
		if ( o_BytesRead )
		{
			*o_BytesRead = 0u;
		}

		return;
	}

	InputArchive Archive;
	Archive.Open( a_Data, a_BufferSize, a_VersionTable );
	Archive >> o_Object;
	CHECK( Archive.GetBytesRead() <= a_BufferSize );
	
	if ( o_BytesRead )
	{
		*o_BytesRead = Archive.GetBytesRead();
	}
}

//==========================================================================
//
//=========================SERIALISATION DEFINITIONS========================
//
//==========================================================================

//==========================================================================
// PAIR
//==========================================================================

template < typename T0, typename T1 >
struct Serialisation::SerialisationInfo< std::pair< T0, T1 > >
{
	static constexpr version_t Version = NullVersion;
	static constexpr bool IsVersioned = false;
	static constexpr bool IsSerialisable = SerialisationInfo< typename std::pair< T0, T1 >::first_type >::IsSerialisable || SerialisationInfo< typename std::pair< T0, T1 >::second_type >::IsSerialisable;
};

template < typename T0, typename T1 >
DEFINE_TEMPLATED_SIZER_EXTERNAL( std::pair< T0, T1 > )
{
	Archive += Object.first;
	Archive += Object.second;
};

template < typename T0, typename T1 >
DEFINE_TEMPLATED_SERIALISER_EXTERNAL( std::pair< T0, T1 > )
{
	Archive << Object.first;
	Archive << Object.second;
};

template < typename T0, typename T1 >
DEFINE_UNVERSIONED_TEMPLATED_DESERIALISER_EXTERNAL( std::pair< T0, T1 > )
{
	Archive >> Object.first;
	Archive >> Object.second;
};

//==========================================================================
// TUPLE
//==========================================================================

template <>
struct Serialisation::SerialisationInfo< std::tuple<> >
{
	static constexpr version_t Version = NullVersion;
	static constexpr bool IsVersioned = false;
	static constexpr bool IsSerialisable = true;
};

template < typename... T >
struct Serialisation::SerialisationInfo< std::tuple< T... > >
{
	static constexpr version_t Version = NullVersion;
	static constexpr bool IsVersioned = false;
	static constexpr bool IsSerialisable = SerialisationInfo< typename std::tuple< T... >::_This_type >::IsSerialisable || SerialisationInfo< typename std::tuple< T... >::_Mybase >::IsSerialisable;
};

template < typename... T >
DEFINE_TEMPLATED_SIZER_EXTERNAL( std::tuple< T... > )
{
	if constexpr ( sizeof...( T ) )
	{
		Archive += Object._Myfirst._Val;
		Archive += Object._Get_rest();
	}
};

template < typename... T >
DEFINE_TEMPLATED_SERIALISER_EXTERNAL( std::tuple< T... > )
{
	if constexpr ( sizeof...( T ) )
	{
		Archive << Object._Myfirst._Val;
		Archive << Object._Get_rest();
	}
};

template < typename... T >
DEFINE_UNVERSIONED_TEMPLATED_DESERIALISER_EXTERNAL( std::tuple< T... > )
{
	if constexpr ( sizeof...( T ) )
	{
		Archive >> Object._Myfirst._Val;
		Archive >> Object._Get_rest();
	}
};

//==========================================================================
// STRING
//==========================================================================

template < typename T0, typename T1, typename T2 >
DECLARE_AS_UNVERSIONED_TEMPLATED_SERIALISABLE_EXTERNAL( std::basic_string< T0, T1, T2 > );

template < typename T0, typename T1, typename T2 >
DEFINE_TEMPLATED_SIZER_EXTERNAL( std::basic_string< T0, T1, T2 > )
{
	using SizeType = typename std::basic_string< T0, T1, T2 >::size_type;
	using ValueType = typename std::basic_string< T0, T1, T2 >::value_type;

	Archive.AddSizeExplicit< SizeType >();
	Archive.AddSizeExplicit< ValueType >( Object.length() );
};

template < typename T0, typename T1, typename T2 >
DEFINE_TEMPLATED_SERIALISER_EXTERNAL( std::basic_string< T0, T1, T2 > )
{
	using ValueType = typename std::basic_string< T0, T1, T2 >::value_type;

	Archive << Object.length();
	Archive.Write( Object.data(), Object.length() * sizeof( ValueType ) );
};

template < typename T0, typename T1, typename T2 >
DEFINE_UNVERSIONED_TEMPLATED_DESERIALISER_EXTERNAL( std::basic_string< T0, T1, T2 > )
{
	using SizeType = typename std::basic_string< T0, T1, T2 >::size_type;
	using ValueType = typename std::basic_string< T0, T1, T2 >::value_type;

	SizeType Length = 0u;
	Archive >> Length;

	Object.resize( Length );
	Archive.Read( Object.data(), Object.length() * sizeof( ValueType ) );
};

//==========================================================================
// C-ARRAY
//==========================================================================
template < typename T, size_t _Size >
using CArray = T[ _Size ];

template < typename T, size_t _Size >
DECLARE_AS_UNVERSIONED_TEMPLATED_SERIALISABLE_EXTERNAL( CArray< T, _Size > );

template < typename T, size_t _Size >
DEFINE_TEMPLATED_SIZER_EXTERNAL( CArray< T, _Size > )
{
	if constexpr ( _Size == 0u )
	{
		return;
	}

	if constexpr ( IsSerialisable< T > )
	{
		for ( auto& Entry : Object )
		{
			Archive += Entry;
		}
	}
	else
	{
		Archive.AddSizeExplicit< T >( _Size );
	}
};

template < typename T, size_t _Size >
DEFINE_TEMPLATED_SERIALISER_EXTERNAL( CArray< T, _Size > )
{
	if constexpr ( _Size == 0u )
	{
		return;
	}

	if constexpr ( IsSerialisable< T > )
	{
		for ( auto& Entry : Object )
		{
			Archive << Entry;
		}
	}
	else
	{
		Archive.Write( &Object[ 0 ], _Size * sizeof( T ) );
	}
};

template < typename T, size_t _Size >
DEFINE_UNVERSIONED_TEMPLATED_DESERIALISER_EXTERNAL( CArray< T, _Size > )
{
	if constexpr ( _Size == 0u )
	{
		return;
	}

	if constexpr ( IsSerialisable< T > )
	{
		for ( auto& Entry : Object )
		{
			Archive >> Entry;
		}
	}
	else
	{
		Archive.Read( &Object[ 0 ], _Size * sizeof( T ) );
	}
};

//==========================================================================
// ARRAY
//==========================================================================

template < typename T, size_t _Size >
DECLARE_AS_UNVERSIONED_TEMPLATED_SERIALISABLE_EXTERNAL( std::array< T, _Size > );

template < typename T, size_t _Size >
DEFINE_TEMPLATED_SIZER_EXTERNAL( std::array< T, _Size > )
{
	using ValueType = typename std::array< T, _Size >::value_type;

	if constexpr ( _Size == 0u )
	{
		return;
	}

	if constexpr ( IsSerialisable< ValueType > )
	{
		for ( auto& Entry : Object )
		{
			Archive += Entry;
		}
	}
	else
	{
		Archive.AddSizeExplicit< ValueType >( _Size );
	}
};

template < typename T, size_t _Size >
DEFINE_TEMPLATED_SERIALISER_EXTERNAL( std::array< T, _Size > )
{
	using ValueType = typename std::array< T, _Size >::value_type;

	if constexpr ( _Size == 0u )
	{
		return;
	}

	if constexpr ( IsSerialisable< ValueType > )
	{
		for ( auto& Entry : Object )
		{
			Archive << Entry;
		}
	}
	else
	{
		Archive.Write( Object.data(), _Size * sizeof( ValueType ) );
	}
};

template < typename T, size_t _Size >
DEFINE_UNVERSIONED_TEMPLATED_DESERIALISER_EXTERNAL( std::array< T, _Size > )
{
	using ValueType = typename std::array< T, _Size >::value_type;

	if constexpr ( _Size == 0u )
	{
		return;
	}

	if constexpr ( IsSerialisable< ValueType > )
	{
		for ( auto& Entry : Object )
		{
			Archive >> Entry;
		}
	}
	else
	{
		Archive.Read( Object.data(), _Size * sizeof( ValueType ) );
	}
};

//==========================================================================
// VECTOR
//==========================================================================

template < typename T0, typename T1 >
DECLARE_AS_UNVERSIONED_TEMPLATED_SERIALISABLE_EXTERNAL( std::vector< T0, T1 > );

template < typename T0, typename T1 >
DEFINE_TEMPLATED_SIZER_EXTERNAL( std::vector< T0, T1 > )
{
	using ValueType = typename std::vector< T0, T1 >::value_type;
	using SizeType = typename std::vector< T0, T1 >::size_type;

	Archive.AddSizeExplicit< SizeType >();

	if constexpr ( IsSerialisable< ValueType > )
	{
		for ( auto& Entry : Object )
		{
			Archive += Entry;
		}
	}
	else
	{
		Archive.AddSizeExplicit< ValueType >( Object.size() );
	}
};

template < typename T0, typename T1 >
DEFINE_TEMPLATED_SERIALISER_EXTERNAL( std::vector< T0, T1 > )
{
	using ValueType = typename std::vector< T0, T1 >::value_type;

	Archive << Object.size();

	if constexpr ( IsSerialisable< ValueType > )
	{
		for ( auto& Entry : Object )
		{
			Archive << Entry;
		}
	}
	else
	{
		Archive.Write( Object.data(), Object.size() * sizeof( ValueType ) );
	}
};

template < typename T0, typename T1 >
DEFINE_UNVERSIONED_TEMPLATED_DESERIALISER_EXTERNAL( std::vector< T0, T1 > )
{
	using ValueType = typename std::vector< T0, T1 >::value_type;
	using SizeType = typename std::vector< T0, T1 >::size_type;

	SizeType Size = 0u;
	Archive >> Size;

	if constexpr ( IsSerialisable< ValueType > )
	{
		Object.reserve( Size );

		for ( SizeType i = 0; i < Size; ++i )
		{
			Archive >> Object.emplace_back();
		}
	}
	else
	{
		Object.resize( Size );
		Archive.Read( Object.data(), Object.size() * sizeof( ValueType ) );
	}
};

//==========================================================================
// DEQUE
//==========================================================================

template < typename T0, typename T1 >
DECLARE_AS_UNVERSIONED_TEMPLATED_SERIALISABLE_EXTERNAL( std::deque< T0, T1 > );

template < typename T0, typename T1 >
DEFINE_TEMPLATED_SIZER_EXTERNAL( std::deque< T0, T1 > )
{
	using ValueType = typename std::deque< T0, T1 >::value_type;
	using SizeType = typename std::deque< T0, T1 >::size_type;

	Archive.AddSizeExplicit< SizeType >();

	if constexpr ( IsSerialisable< ValueType > )
	{
		for ( auto& Entry : Object )
		{
			Archive += Entry;
		}
	}
	else
	{
		Archive.AddSizeExplicit< ValueType >( Object.size() );
	}
};

template < typename T0, typename T1 >
DEFINE_TEMPLATED_SERIALISER_EXTERNAL( std::deque< T0, T1 > )
{
	Archive << Object.size();

	for ( auto& Entry : Object )
	{
		Archive << Entry;
	}
};

template < typename T0, typename T1 >
DEFINE_UNVERSIONED_TEMPLATED_DESERIALISER_EXTERNAL( std::deque< T0, T1 > )
{
	using SizeType = typename std::vector< T0, T1 >::size_type;

	SizeType Size = 0u;
	Archive >> Size;

	Object.resize( Size );

	for ( SizeType i = 0; i < Size; ++i )
	{
		Archive >> Object[ i ];
	}
};

//==========================================================================
// FORWARD-LIST
//==========================================================================

template < typename T0, typename T1 >
DECLARE_AS_UNVERSIONED_TEMPLATED_SERIALISABLE_EXTERNAL( std::forward_list< T0, T1 > );

template < typename T0, typename T1 >
DEFINE_TEMPLATED_SIZER_EXTERNAL( std::forward_list< T0, T1 > )
{
	using SizeType = typename std::forward_list< T0, T1 >::size_type;

	Archive.AddSizeExplicit< SizeType >();

	for ( auto& Entry : Object )
	{
		Archive += Entry;
	}
};

template < typename T0, typename T1 >
DEFINE_TEMPLATED_SERIALISER_EXTERNAL( std::forward_list< T0, T1 > )
{
	using SizeType = typename std::forward_list< T0, T1 >::size_type;

	SizeType Size = 0u;

	for ( auto& Pair : Object )
	{
		++Size;
	}

	Archive << Size;

	for ( auto& Pair : Object )
	{
		Archive << Pair;
	}
};

template < typename T0, typename T1 >
DEFINE_UNVERSIONED_TEMPLATED_DESERIALISER_EXTERNAL( std::forward_list< T0, T1 > )
{
	using SizeType = typename std::forward_list< T0, T1 >::size_type;

	SizeType Size;
	Archive >> Size;

	for ( auto Begin = Object.before_begin(); Size > 0u; --Size )
	{
		Begin = Object.emplace_after( Begin );
		Archive >> *Begin;
	}
};

//==========================================================================
// LIST
//==========================================================================

template < typename T0, typename T1 >
DECLARE_AS_UNVERSIONED_TEMPLATED_SERIALISABLE_EXTERNAL( std::list< T0, T1 > );

template < typename T0, typename T1 >
DEFINE_TEMPLATED_SIZER_EXTERNAL( std::list< T0, T1 > )
{
	using ValueType = typename std::list< T0, T1 >::value_type;
	using SizeType = typename std::list< T0, T1 >::size_type;

	Archive.AddSizeExplicit< SizeType >();

	if constexpr ( IsSerialisable< ValueType > )
	{
		for ( auto& Entry : Object )
		{
			Archive += Entry;
		}
	}
	else
	{
		Archive.AddSizeExplicit< ValueType >( Object.size() );
	}
};

template < typename T0, typename T1 >
DEFINE_TEMPLATED_SERIALISER_EXTERNAL( std::list< T0, T1 > )
{
	Archive << Object.size();

	for ( auto& Entry : Object )
	{
		Archive << Entry;
	}
};

template < typename T0, typename T1 >
DEFINE_UNVERSIONED_TEMPLATED_DESERIALISER_EXTERNAL( std::list< T0, T1 > )
{
	using SizeType = typename std::list< T0, T1 >::size_type;

	SizeType Size = 0u;
	Archive >> Size;

	for ( ; Size > 0; --Size )
	{
		Archive >> Object.emplace_back();
	}
};

//==========================================================================
// SET
//==========================================================================

template < typename T0, typename T1, typename T2 >
DECLARE_AS_UNVERSIONED_TEMPLATED_SERIALISABLE_EXTERNAL( std::set< T0, T1, T2 > );

template < typename T0, typename T1, typename T2 >
DEFINE_TEMPLATED_SIZER_EXTERNAL( std::set< T0, T1, T2 > )
{
	using ValueType = typename std::set< T0, T1, T2 >::value_type;
	using SizeType = typename std::set< T0, T1, T2 >::size_type;

	Archive.AddSizeExplicit< SizeType >();

	if constexpr ( IsSerialisable< ValueType > )
	{
		for ( auto& Entry : Object )
		{
			Archive += Entry;
		}
	}
	else
	{
		Archive.AddSizeExplicit< ValueType >( Object.size() );
	}
};

template < typename T0, typename T1, typename T2 >
DEFINE_TEMPLATED_SERIALISER_EXTERNAL( std::set< T0, T1, T2 > )
{
	Archive << Object.size();

	for ( auto& Entry : Object )
	{
		Archive << Entry;
	}
};

template < typename T0, typename T1, typename T2 >
DEFINE_UNVERSIONED_TEMPLATED_DESERIALISER_EXTERNAL( std::set< T0, T1, T2 > )
{
	using ValueType = typename std::set< T0, T1, T2 >::value_type;
	using SizeType = typename std::set< T0, T1, T2 >::size_type;

	SizeType Size;
	Archive >> Size;

	for ( ; Size > 0u; --Size )
	{
		ValueType Entry;
		Archive >> Entry;
		Object.emplace( std::move( Entry ) );
	}
};

//==========================================================================
// MAP
//==========================================================================

template < typename T0, typename T1, typename T2, typename T3 >
DECLARE_AS_UNVERSIONED_TEMPLATED_SERIALISABLE_EXTERNAL( std::map< T0, T1, T2, T3 > );

template < typename T0, typename T1, typename T2, typename T3 >
DEFINE_TEMPLATED_SIZER_EXTERNAL( std::map< T0, T1, T2, T3 > )
{
	using SizeType = typename std::map< T0, T1, T2, T3 >::size_type;
	using ValueType = typename std::map< T0, T1, T2, T3 >::value_type;

	Archive.AddSizeExplicit< SizeType >();

	if constexpr ( IsSerialisable< ValueType > )
	{
		for ( auto& Pair : Object )
		{
			Archive += Pair;
		}
	}
	else
	{
		Archive.AddSizeExplicit< ValueType >( Object.size() );
	}
};

template < typename T0, typename T1, typename T2, typename T3 >
DEFINE_TEMPLATED_SERIALISER_EXTERNAL( std::map< T0, T1, T2, T3 > )
{
	Archive << Object.size();

	for ( auto& Pair : Object )
	{
		Archive << Pair;
	}
};

template < typename T0, typename T1, typename T2, typename T3 >
DEFINE_UNVERSIONED_TEMPLATED_DESERIALISER_EXTERNAL( std::map< T0, T1, T2, T3 > )
{
	using SizeType = typename std::map< T0, T1, T2, T3 >::size_type;
	using KeyType = typename std::map< T0, T1, T2, T3 >::key_type;

	SizeType Size = 0u;
	Archive >> Size;

	for ( ; Size > 0u; --Size )
	{
		KeyType Key;
		Archive >> Key;
		Archive >> Object[ std::move( Key ) ];
	}
};

//==========================================================================
// MULTI-SET
//==========================================================================

template < typename T0, typename T1, typename T2 >
DECLARE_AS_UNVERSIONED_TEMPLATED_SERIALISABLE_EXTERNAL( std::multiset< T0, T1, T2 > );

template < typename T0, typename T1, typename T2 >
DEFINE_TEMPLATED_SIZER_EXTERNAL( std::multiset< T0, T1, T2 > )
{
	using ValueType = typename std::multiset< T0, T1, T2 >::value_type;
	using SizeType = typename std::multiset< T0, T1, T2 >::size_type;

	Archive.AddSizeExplicit< SizeType >();

	if constexpr ( IsSerialisable< ValueType > )
	{
		for ( auto& Entry : Object )
		{
			Archive += Entry;
		}
	}
	else
	{
		Archive.AddSizeExplicit< ValueType >( Object.size() );
	}
};

template < typename T0, typename T1, typename T2 >
DEFINE_TEMPLATED_SERIALISER_EXTERNAL( std::multiset< T0, T1, T2 > )
{
	Archive << Object.size();

	for ( auto& Entry : Object )
	{
		Archive << Entry;
	}
};

template < typename T0, typename T1, typename T2 >
DEFINE_UNVERSIONED_TEMPLATED_DESERIALISER_EXTERNAL( std::multiset< T0, T1, T2 > )
{
	using ValueType = typename std::multiset< T0, T1, T2 >::value_type;
	using SizeType = typename std::multiset< T0, T1, T2 >::size_type;

	SizeType Size;
	Archive >> Size;

	for ( ; Size > 0u; --Size )
	{
		ValueType Entry;
		Archive >> Entry;
		Object.emplace( std::move( Entry ) );
	}
};

//==========================================================================
// MULTI-MAP
//==========================================================================

template < typename T0, typename T1, typename T2, typename T3 >
DECLARE_AS_UNVERSIONED_TEMPLATED_SERIALISABLE_EXTERNAL( std::multimap< T0, T1, T2, T3 > );

template < typename T0, typename T1, typename T2, typename T3 >
DEFINE_TEMPLATED_SIZER_EXTERNAL( std::multimap< T0, T1, T2, T3 > )
{
	using SizeType = typename std::multimap< T0, T1, T2, T3 >::size_type;
	using ValueType = typename std::multimap< T0, T1, T2, T3 >::value_type;

	Archive.AddSizeExplicit< SizeType >();

	if constexpr ( IsSerialisable< ValueType > )
	{
		for ( auto& Pair : Object )
		{
			Archive += Pair;
		}
	}
	else
	{
		Archive.AddSizeExplicit< ValueType >( Object.size() );
	}
};

template < typename T0, typename T1, typename T2, typename T3 >
DEFINE_TEMPLATED_SERIALISER_EXTERNAL( std::multimap< T0, T1, T2, T3 > )
{
	Archive << Object.size();

	for ( auto& Pair : Object )
	{
		Archive << Pair;
	}
};

template < typename T0, typename T1, typename T2, typename T3 >
DEFINE_UNVERSIONED_TEMPLATED_DESERIALISER_EXTERNAL( std::multimap< T0, T1, T2, T3 > )
{
	using SizeType = typename std::map< T0, T1, T2, T3 >::size_type;
	using ValueType = typename std::map< T0, T1, T2, T3 >::value_type;

	SizeType Size = 0u;
	Archive >> Size;

	for ( ; Size > 0u; --Size )
	{
		ValueType Value;
		Archive >> Value;
		Object.emplace( std::move( Value ) );
	}
};

//==========================================================================
// UNORDERED-SET
//==========================================================================

template < typename T0, typename T1, typename T2, typename T3 >
DECLARE_AS_UNVERSIONED_TEMPLATED_SERIALISABLE_EXTERNAL( std::unordered_set< T0, T1, T2, T3 > );

template < typename T0, typename T1, typename T2, typename T3 >
DEFINE_TEMPLATED_SIZER_EXTERNAL( std::unordered_set< T0, T1, T2, T3 > )
{
	using ValueType = typename std::unordered_set< T0, T1, T2, T3 >::value_type;
	using SizeType = typename std::unordered_set< T0, T1, T2, T3 >::size_type;

	Archive.AddSizeExplicit< SizeType >();

	if constexpr ( IsSerialisable< ValueType > )
	{
		for ( auto& Entry : Object )
		{
			Archive += Entry;
		}
	}
	else
	{
		Archive.AddSizeExplicit< ValueType >( Object.size() );
	}
};

template < typename T0, typename T1, typename T2, typename T3 >
DEFINE_TEMPLATED_SERIALISER_EXTERNAL( std::unordered_set< T0, T1, T2, T3 > )
{
	Archive << Object.size();

	for ( auto& Entry : Object )
	{
		Archive << Entry;
	}
};

template < typename T0, typename T1, typename T2, typename T3 >
DEFINE_UNVERSIONED_TEMPLATED_DESERIALISER_EXTERNAL( std::unordered_set< T0, T1, T2, T3 > )
{
	using ValueType = typename std::unordered_set< T0, T1, T2, T3 >::value_type;
	using SizeType = typename std::unordered_set< T0, T1, T2, T3 >::size_type;

	SizeType Size;
	Archive >> Size;

	for ( ; Size > 0u; --Size )
	{
		ValueType Entry;
		Archive >> Entry;
		Object.emplace( std::move( Entry ) );
	}
};

//==========================================================================
// UNORDERED-MAP
//==========================================================================

template < typename T0, typename T1, typename T2, typename T3, typename T4 >
DECLARE_AS_UNVERSIONED_TEMPLATED_SERIALISABLE_EXTERNAL( std::unordered_map< T0, T1, T2, T3, T4 > );

template < typename T0, typename T1, typename T2, typename T3, typename T4 >
DEFINE_TEMPLATED_SIZER_EXTERNAL( std::unordered_map< T0, T1, T2, T3, T4 > )
{
	using SizeType = typename std::unordered_map< T0, T1, T2, T3, T4 >::size_type;
	using ValueType = typename std::unordered_map< T0, T1, T2, T3, T4 >::value_type;

	Archive.AddSizeExplicit< SizeType >();

	if constexpr ( IsSerialisable< ValueType > )
	{
		for ( auto& Pair : Object )
		{
			Archive += Pair;
		}
	}
	else
	{
		Archive.AddSizeExplicit< ValueType >( Object.size() );
	}
};

template < typename T0, typename T1, typename T2, typename T3, typename T4 >
DEFINE_TEMPLATED_SERIALISER_EXTERNAL( std::unordered_map< T0, T1, T2, T3, T4 > )
{
	Archive << Object.size();

	for ( auto& Pair : Object )
	{
		Archive << Pair;
	}
};

template < typename T0, typename T1, typename T2, typename T3, typename T4 >
DEFINE_UNVERSIONED_TEMPLATED_DESERIALISER_EXTERNAL( std::unordered_map< T0, T1, T2, T3, T4 > )
{
	using SizeType = typename std::unordered_map< T0, T1, T2, T3, T4 >::size_type;
	using ValueType = typename std::unordered_map< T0, T1, T2, T3, T4 >::value_type;

	SizeType Size = 0u;
	Archive >> Size;

	for ( ; Size > 0u; --Size )
	{
		ValueType Value;
		Archive >> Value;
		Object.emplace( std::move( Value ) );
	}
};

//==========================================================================
// STACK
//==========================================================================

template < typename T0, typename T1 >
DECLARE_AS_UNVERSIONED_TEMPLATED_SERIALISABLE_EXTERNAL( std::stack< T0, T1 > );

template < typename T0, typename T1 >
DEFINE_TEMPLATED_SIZER_EXTERNAL( std::stack< T0, T1 > )
{
	struct StackHelper : std::stack< T0, T1 >
	{
		auto& GetContainer() const { return this->c; }
	};

	Archive += static_cast< const StackHelper& >( Object ).GetContainer();
};

template < typename T0, typename T1 >
DEFINE_TEMPLATED_SERIALISER_EXTERNAL( std::stack< T0, T1 > )
{
	struct StackHelper : std::stack< T0, T1 >
	{
		auto& GetContainer() const { return this->c; }
	};

	Archive << static_cast< const StackHelper& >( Object ).GetContainer();
};

template < typename T0, typename T1 >
DEFINE_UNVERSIONED_TEMPLATED_DESERIALISER_EXTERNAL( std::stack< T0, T1 > )
{
	struct StackHelper : std::stack< T0, T1 >
	{
		auto& GetContainer() { return this->c; }
	};

	Archive >> static_cast< StackHelper& >( Object ).GetContainer();
};

//==========================================================================
// QUEUE
//==========================================================================

template < typename T0, typename T1 >
DECLARE_AS_UNVERSIONED_TEMPLATED_SERIALISABLE_EXTERNAL( std::queue< T0, T1 > );

template < typename T0, typename T1 >
DEFINE_TEMPLATED_SIZER_EXTERNAL( std::queue< T0, T1 > )
{
	struct QueueHelper : std::queue< T0, T1 >
	{
		auto& GetContainer() const { return this->c; }
	};

	Archive += static_cast< const QueueHelper& >( Object ).GetContainer();
};

template < typename T0, typename T1 >
DEFINE_TEMPLATED_SERIALISER_EXTERNAL( std::queue< T0, T1 > )
{
	struct QueueHelper : std::queue< T0, T1 >
	{
		auto& GetContainer() const { return this->c; }
	};

	Archive << static_cast< const QueueHelper& >( Object ).GetContainer();
};

template < typename T0, typename T1 >
DEFINE_UNVERSIONED_TEMPLATED_DESERIALISER_EXTERNAL( std::queue< T0, T1 > )
{
	struct QueueHelper : std::queue< T0, T1 >
	{
		auto& GetContainer() { return this->c; }
	};

	Archive >> static_cast< QueueHelper& >( Object ).GetContainer();
};

//==========================================================================
// PRIORITY-QUEUE
//==========================================================================

template < typename T0, typename T1, typename T2 >
DECLARE_AS_UNVERSIONED_TEMPLATED_SERIALISABLE_EXTERNAL( std::priority_queue< T0, T1, T2 > );

template < typename T0, typename T1, typename T2 >
DEFINE_TEMPLATED_SIZER_EXTERNAL( std::priority_queue< T0, T1, T2 > )
{
	struct PriorityQueueHelper : std::priority_queue< T0, T1, T2 >
	{
		auto& GetContainer() const { return this->c; }
	};

	Archive += static_cast< const PriorityQueueHelper& >( Object ).GetContainer();
};

template < typename T0, typename T1, typename T2 >
DEFINE_TEMPLATED_SERIALISER_EXTERNAL( std::priority_queue< T0, T1, T2 > )
{
	struct PriorityQueueHelper : std::priority_queue< T0, T1, T2 >
	{
		auto& GetContainer() const { return this->c; }
	};

	Archive << static_cast< const PriorityQueueHelper& >( Object ).GetContainer();
};

template < typename T0, typename T1, typename T2 >
DEFINE_UNVERSIONED_TEMPLATED_DESERIALISER_EXTERNAL( std::priority_queue< T0, T1, T2 > )
{
	struct PriorityQueueHelper : std::priority_queue< T0, T1, T2 >
	{
		auto& GetContainer() { return this->c; }
	};

	Archive >> static_cast< PriorityQueueHelper& >( Object ).GetContainer();
};

template < typename T >
constexpr SizingArchive& SizingArchive::AddSize( const T& a_Object )
{
	using Type = T;// std::conditional_t< std::is_array_v< T >, T, std::decay_t< T > >;

	if constexpr ( Serialisation::IsSerialisable< Type > )
	{
		Serialisation::Sizer< Type >( *this, a_Object );
	}
	else
	{
		m_BytesCounted += sizeof( Type );
	}

	return *this;
}

template < typename T >
void OutputArchive::Serialise( const T& a_Object )
{
	using Type = std::conditional_t< std::is_array_v< T >, T, std::decay_t< T > >;
	static_assert( std::is_array_v< Type > || !std::is_pointer_v< Type >, "Pointer type serialisation is not supported." );

	if constexpr ( Serialisation::IsSerialisable< Type > )
	{
		if constexpr ( Serialisation::IsVersioned< Type > )
		{
			if ( m_VersionTable )
			{
				m_VersionTable->SetVersion< Type >();
			}
		}

		if constexpr ( Serialisation::HasOnBeforeSerialise< Type >::Value )
		{
			a_Object.OnBeforeSerialise();
		}

		Serialisation::Serialiser< Type >( *this, a_Object );

		if constexpr ( Serialisation::HasOnAfterSerialise< Type >::Value )
		{
			a_Object.OnAfterSerialise();
		}
	}
	else
	{
		SerialiseExplicit( a_Object );
	}
}

template < typename T >
void InputArchive::Deserialise( T& a_Object )
{
	using Type = std::conditional_t< std::is_array_v< T >, T, std::decay_t< T > >;
	static_assert( !std::is_pointer_v< Type >, "Pointer type deserialisation is not supported." );

	if constexpr ( Serialisation::IsSerialisable< Type > )
	{
		version_t Version = 0u;

		if constexpr ( Serialisation::IsVersioned< Type > )
		{
			Version = Serialisation::Version< Type >;

			if ( m_VersionTable )
			{
				CHECK( m_VersionTable->Contains< T >() );
				Version = m_VersionTable->GetVersion< T >();
			}
		}

		if constexpr ( Serialisation::HasOnBeforeDeserialise< Type >::Value )
		{
			a_Object.OnBeforeDeserialise();
		}

		Serialisation::GetDeserialiserTable< T >()[ Version ]( *this, a_Object );

		if constexpr ( Serialisation::HasOnAfterDeserialise< Type >::Value )
		{
			a_Object.OnAfterDeserialise();
		}
	}
	else
	{
		DeserialiseExplicit( a_Object );
	}
}