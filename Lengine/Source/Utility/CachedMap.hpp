#pragma once
#include <cstdint>
#include <vector>
#include <tuple>

template < typename _Key, typename _Mapped >
class CachedMapEntry : public std::pair< _Key, _Mapped >
{
public:

	using BaseType = std::pair< _Key, _Mapped >;

	CachedMapEntry( const CachedMapEntry& ) = default;
	CachedMapEntry( CachedMapEntry&& ) = default;
	CachedMapEntry& operator=( const CachedMapEntry& ) = default;
	CachedMapEntry& operator=( CachedMapEntry&& ) = default;
	
	template < typename... Args >
	CachedMapEntry( Args&&... a_Args )
		: BaseType( std::forward< Args >( a_Args )... )
		, Accesses( 0u )
	{}

private:

	template < typename, typename, typename > friend class CachedMap;

	uint32_t Accesses = 0u;
};

template < typename _Key, typename _Mapped, typename _Allocator = std::allocator< CachedMapEntry< _Key, _Mapped > > >
class CachedMap
{
public:

	using KeyType = _Key;
	using MappedType = _Mapped;
	using ValueType = CachedMapEntry< KeyType, MappedType >;
	using AllocatorType = _Allocator;
	using AllocatorTraitsType = std::allocator_traits< AllocatorType >;
	using ContainerType = std::vector< ValueType, _Allocator >;
	using Iterator = typename ContainerType::iterator;
	using CIterator = typename ContainerType::const_iterator;
	using RIterator = typename ContainerType::reverse_iterator;
	using CRIterator = typename ContainerType::const_reverse_iterator;

	CachedMap() = default;
	CachedMap( const CachedMap& ) = default;
	CachedMap( CachedMap&& ) = default;

	Iterator Begin() { return m_Values.begin(); }
	CIterator Begin() const { return m_Values.begin(); }
	CIterator CBegin() const { return m_Values.cbegin(); }
	RIterator RBegin() { return m_Values.rbegin(); }
	CRIterator RBegin() const { return m_Values.rbegin(); }
	CRIterator CRBegin() const { return m_Values.crbegin(); }
	Iterator End() { return m_Values.end(); }
	CIterator End() const { return m_Values.end(); }
	CIterator CEnd() const { return m_Values.cend(); }
	RIterator REnd() { return m_Values.rend(); }
	CRIterator REnd() const { return m_Values.rend(); }
	CRIterator CREnd() const { return m_Values.crend(); }

	inline MappedType& At( const KeyType& a_Key ) { return Find( a_Key )->second; }

	inline const MappedType& At( const KeyType& a_Key ) const { return const_cast< CachedMap* >( this )->At( a_Key ); }

	inline ValueType& Back() { return m_Values.back(); }

	inline const ValueType& Back() const { return m_Values.back(); }

	inline size_t Capacity() const { return m_Values.capacity(); }

	inline void Clear() { m_Values.clear(); }

	inline bool Contains( const KeyType& a_Key ) const { return FindKey( a_Key ) != m_Values.end(); }

	inline ValueType* Data() { return m_Values.data(); }

	inline const ValueType* Data() const { return m_Values.data(); }

	inline std::pair< Iterator, bool > Emplace( KeyType&& a_Key, MappedType&& a_Mapped ) { return Emplace( std::piecewise_construct, std::forward_as_tuple( std::move( a_Key ) ), std::forward_as_tuple( std::move( a_Mapped ) ) ); }

	inline std::pair< Iterator, bool > Emplace( const KeyType& a_Key, MappedType&& a_Mapped ) { return Emplace( std::piecewise_construct, std::forward_as_tuple( a_Key ), std::forward_as_tuple( std::move( a_Mapped ) ) ); }

	inline std::pair< Iterator, bool > Emplace( KeyType&& a_Key, const MappedType& a_Mapped ) { return Emplace( std::piecewise_construct, std::forward_as_tuple( std::move( a_Key ) ), std::forward_as_tuple( a_Mapped ) ); }

	inline std::pair< Iterator, bool > Emplace( const KeyType& a_Key, const MappedType& a_Mapped ) { return Emplace( std::piecewise_construct, std::forward_as_tuple( a_Key ), std::forward_as_tuple( a_Mapped ) ); }

	template < typename... KeyArgs, typename... MappedArgs >
	std::pair< Iterator, bool > Emplace( std::piecewise_construct_t, std::tuple< KeyArgs... > a_KeyArgs, std::tuple< MappedArgs... > a_MappedArgs )
	{
		return Emplace( 
			a_KeyArgs, 
			a_MappedArgs,
			std::make_index_sequence< sizeof...( KeyArgs ) >{},
			std::make_index_sequence< sizeof...( MappedArgs ) >{} 
		);
	}
	
	template < typename KeyTuple, typename MappedTuple, size_t... KeyIdxs, size_t... MappedIdxs >
	std::pair< Iterator, bool > Emplace( KeyTuple& a_Key, MappedTuple& a_Mapped, std::index_sequence< KeyIdxs... >, std::index_sequence< MappedIdxs... > )
	{
		KeyType Key( std::get< KeyIdxs >( std::move( a_Key ) )... );
		auto Iter = FindKey( Key );

		if ( Iter != m_Values.end() )
		{
			return { Iter, false };
		}

		m_Values.emplace_back(
			std::piecewise_construct,
			std::forward_as_tuple( std::move( Key ) ),
			std::forward_as_tuple( std::get< MappedIdxs >( std::move( a_Mapped ) )... )
		);

		return { --m_Values.end(), true };
	}

	inline bool Empty() const { return m_Values.empty(); }

	inline Iterator Erase( CIterator a_Where ) { return m_Values.erase( a_Where ); }

	Iterator Find( const KeyType& a_Key )
	{
		uint32_t PreviousCount = -1;

		for ( auto Begin = m_Values.begin(), End = m_Values.end(); Begin != End; ++Begin )
		{
			if ( Begin->first == a_Key )
			{
				if ( ++Begin->Accesses > PreviousCount )
				{
					SlideSortEntry( Begin );
				}

				return Begin;
			}

			PreviousCount = Begin->Accesses;
		}

		return m_Values.end();
	}

	inline CIterator Find( const KeyType& a_Key ) const { return const_cast< CachedMap* >( this )->Find( a_Key ); }

	inline ValueType& Front() { return m_Values.front(); }

	inline const ValueType& Front() const { return m_Values.front(); }

	std::pair< Iterator, bool > Insert( const KeyType& a_Key )
	{
		auto Iter = FindKey( a_Key );

		if ( Iter != m_Values.end() )
		{
			return { Iter, false };
		}

		m_Values.emplace_back(
			std::piecewise_construct,
			std::forward_as_tuple( a_Key ),
			std::forward_as_tuple()
		);

		return { --m_Values.end(), true };
	}

	std::pair< Iterator, bool > Insert( const KeyType& a_Key, const MappedType& a_Mapped )
	{
		auto Iter = FindKey( a_Key );

		if ( Iter != m_Values.end() )
		{
			return { Iter, false };
		}

		m_Values.emplace_back(
			std::piecewise_construct,
			std::forward_as_tuple( a_Key ),
			std::forward_as_tuple( a_Mapped )
		);

		return { --m_Values.end(), true };
	}

	template < typename Arg0, typename Arg1 >
	inline std::pair< Iterator, bool > Insert( const std::pair< Arg0, Arg1 >& a_Pair ) { return Emplace( a_Pair.first, a_Pair.second ); }

	inline constexpr size_t MaxSize() const { return m_Values.max_size(); }

	inline void Reserve( size_t a_Capacity ) { m_Values.reserve( a_Capacity ); }

	inline size_t Size() const { return m_Values.size(); }

	inline void Swap( CachedMap& a_CachedMap ) { m_Values.swap( a_CachedMap.m_Values ); }

	MappedType& operator[]( const KeyType& a_Key )
	{
		auto Iter = Find( a_Key );

		if ( Iter == m_Values.end() )
		{
			m_Values.emplace_back(
				std::piecewise_construct,
				std::forward_as_tuple( a_Key ),
				std::forward_as_tuple()
			);

			return m_Values.back().second;
		}

		return Iter->second;
	}

	CachedMap& operator=( const CachedMap& a_CachedMap ) = default;

	CachedMap& operator=( CachedMap&& a_CachedMap ) = default;

	bool operator==( const CachedMap& a_CachedMap ) const 
	{
		if ( m_Values.size() != a_CachedMap.m_Values.size() )
		{
			return false;
		}

		for ( auto Begin = m_Values.begin(), End = m_Values.end(); Begin != End; ++Begin )
		{
			auto Iter = a_CachedMap.Find( Begin->first );

			if ( Iter == a_CachedMap.End() || Iter->second != Begin->second )
			{
				return false;
			}
		}

		return true;
	}

	inline bool operator!=( const CachedMap& a_CachedMap ) const { return !( *this == a_CachedMap ); }

private:

	template < typename KeyTuple, size_t... Idxs >
	inline static KeyType ConstructKey( const KeyTuple& a_KeyTuple, std::index_sequence< Idxs... > )
	{
		return KeyType( std::get< Idxs >( a_KeyTuple )... );
	}

	inline Iterator FindKey( const KeyType& a_Key )
	{
		auto End = m_Values.end();

		for ( auto Begin = m_Values.begin(); Begin != End; ++Begin )
		{
			if ( Begin->first == a_Key )
			{
				return Begin;
			}
		}

		return End;
	}

	inline CIterator FindKey( const KeyType& a_Key ) const
	{
		auto End = m_Values.end();

		for ( auto Begin = m_Values.begin(); Begin != End; ++Begin )
		{
			if ( Begin->first == a_Key )
			{
				return Begin;
			}
		}

		return End;
	}

	void SlideSortEntry( Iterator& a_Where )
	{
		auto Begin = m_Values.begin();
		ValueType Temp( std::move( *a_Where ) );
		AllocatorType Allocator = m_Values.get_allocator();

		for ( ; a_Where != Begin; )
		{
			ValueType& Current = *a_Where;
			--a_Where;

			if ( a_Where->Accesses >= Temp.Accesses )
			{
				++a_Where;
				break;
			}

			AllocatorTraitsType::destroy( Allocator, &Current );
			AllocatorTraitsType::construct( Allocator, &Current, std::move( *a_Where ) );
		}

		AllocatorTraitsType::destroy( Allocator, &*a_Where );
		AllocatorTraitsType::construct( Allocator, &*a_Where, std::move( Temp ) );
	}

	ContainerType m_Values;
};

namespace std
{
	template < typename _Key, typename _Mapped, typename _Allocator > typename CachedMap< _Key, _Mapped, _Allocator >::Iterator begin( CachedMap< _Key, _Mapped, _Allocator >& a_CachedMap ) { return a_CachedMap.Begin(); }
	template < typename _Key, typename _Mapped, typename _Allocator > typename CachedMap< _Key, _Mapped, _Allocator >::CIterator begin( const CachedMap< _Key, _Mapped, _Allocator >& a_CachedMap ) { return a_CachedMap.Begin(); }
	template < typename _Key, typename _Mapped, typename _Allocator > typename CachedMap< _Key, _Mapped, _Allocator >::CIterator cbegin( const CachedMap< _Key, _Mapped, _Allocator >& a_CachedMap ) { return a_CachedMap.CBegin(); }
	template < typename _Key, typename _Mapped, typename _Allocator > typename CachedMap< _Key, _Mapped, _Allocator >::Iterator end( CachedMap< _Key, _Mapped, _Allocator >& a_CachedMap ) { return a_CachedMap.End(); }
	template < typename _Key, typename _Mapped, typename _Allocator > typename CachedMap< _Key, _Mapped, _Allocator >::CIterator end( const CachedMap< _Key, _Mapped, _Allocator >& a_CachedMap ) { return a_CachedMap.End(); }
	template < typename _Key, typename _Mapped, typename _Allocator > typename CachedMap< _Key, _Mapped, _Allocator >::CIterator cend( const CachedMap< _Key, _Mapped, _Allocator >& a_CachedMap ) { return a_CachedMap.CEnd(); }	
	template < typename _Key, typename _Mapped, typename _Allocator > typename CachedMap< _Key, _Mapped, _Allocator >::RIterator rbegin( CachedMap< _Key, _Mapped, _Allocator >& a_CachedMap ) { return a_CachedMap.RBegin(); }
	template < typename _Key, typename _Mapped, typename _Allocator > typename CachedMap< _Key, _Mapped, _Allocator >::CRIterator rbegin( const CachedMap< _Key, _Mapped, _Allocator >& a_CachedMap ) { return a_CachedMap.RBegin(); }
	template < typename _Key, typename _Mapped, typename _Allocator > typename CachedMap< _Key, _Mapped, _Allocator >::CRIterator crbegin( const CachedMap< _Key, _Mapped, _Allocator >& a_CachedMap ) { return a_CachedMap.CRBegin(); }
	template < typename _Key, typename _Mapped, typename _Allocator > typename CachedMap< _Key, _Mapped, _Allocator >::RIterator rend( CachedMap< _Key, _Mapped, _Allocator >& a_CachedMap ) { return a_CachedMap.REnd(); }
	template < typename _Key, typename _Mapped, typename _Allocator > typename CachedMap< _Key, _Mapped, _Allocator >::CRIterator rend( const CachedMap< _Key, _Mapped, _Allocator >& a_CachedMap ) { return a_CachedMap.REnd(); }
	template < typename _Key, typename _Mapped, typename _Allocator > typename CachedMap< _Key, _Mapped, _Allocator >::CRIterator crend( const CachedMap< _Key, _Mapped, _Allocator >& a_CachedMap ) { return a_CachedMap.CREnd(); }
}