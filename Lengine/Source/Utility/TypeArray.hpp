#pragma once
#include <vector>
#include <optional>
#include <typeinfo>

#include <Utility/ClassTraits.hpp>

struct TypeArrayEntryInfo
{
	const std::type_info* Info = nullptr;
	hash_t Name = 0u;
};

// TypeArray stores its elements in a static way. Each index is associated with a unique type.
// This makes finding elements by type extremely fast.
template < typename T, typename _Indexer = TypeIndexer< TypeAlias< 0u > > >
class TypeArray
{
public:

	using ValueType = T;
	using IndexerType = _Indexer;
	using ArrayType = std::vector< ValueType >;
	using EntryType = TypeArrayEntryInfo;
	using InfoArrayType = std::vector< EntryType >;
	using IteratorType = typename ArrayType::iterator;
	using CIteratorType = typename ArrayType::const_iterator;
	using RIteratorType = typename ArrayType::reverse_iterator;
	using CRIteratorType = typename ArrayType::const_reverse_iterator;

	TypeArray() = default;
	TypeArray( const TypeArray& ) = default;
	TypeArray( TypeArray&& ) = default;
	TypeArray& operator=( const TypeArray& ) = default;
	TypeArray& operator=( TypeArray&& ) = default;

	CIteratorType Begin() const { return m_ValueArray.begin(); }
	IteratorType Begin() { return m_ValueArray.begin(); }
	CIteratorType CBegin() const { return m_ValueArray.cbegin(); }
	RIteratorType RBegin() const { return m_ValueArray.rbegin(); }
	RIteratorType RBegin() { return m_ValueArray.rbegin(); }
	RIteratorType CRBegin() const { return m_ValueArray.rbegin(); }
	CIteratorType End() const { return m_ValueArray.end(); }
	IteratorType End() { return m_ValueArray.end(); }
	CIteratorType CEnd() const { return m_ValueArray.cend(); }
	CRIteratorType REnd() const { return m_ValueArray.rend(); }
	RIteratorType REnd() { return m_ValueArray.rend(); }
	CRIteratorType CREnd() const { return m_ValueArray.crrend(); }

	const T& At( size_t a_Index ) const { return m_ValueArray.at( a_Index ); }
	T& At( size_t a_Index ) { return m_ValueArray.at( a_Index ); }
	const T& Back() const { return m_ValueArray.back(); }
	T& Back() { return m_ValueArray.back(); }
	size_t Capacity() const { return m_ValueArray.capacity(); }
	void Clear() { m_ValueArray.clear(); m_InfoArray.clear(); }
	bool Empty() const { return m_ValueArray.empty(); }
	const T& Front() const { return m_ValueArray.front(); }
	T& Front() { return m_ValueArray.front(); }
	size_t MaxSize() const { return m_ValueArray.max_size(); }
	void Reserve( const size_t a_NewCapacity ) { m_ValueArray.reserve( a_NewCapacity ); m_InfoArray.reserve( a_NewCapacity ); }
	size_t Size() const { return m_ValueArray.size(); }
	void Swap( TypeArray& a_TypeArray ) { m_ValueArray.swap( a_TypeArray.m_ValueArray ); m_InfoArray.swap( a_TypeArray.m_InfoArray ); }
	const T& operator[]( const size_t a_Index ) const { return At( a_Index ); }
	T& operator[]( const size_t a_Index ) { return At( a_Index ); }

	hash_t GetType( CIteratorType a_Where ) const
	{
		return m_InfoArray[ a_Where - m_ValueArray.cbegin() ].Name;
	}

	template < typename U >
	const T& At( const std::in_place_type_t< U > = std::in_place_type_t< U >{} ) const
	{
		return m_ValueArray.at( IndexerType::template Value< U >() );
	}

	template < typename U >
	T& At( std::in_place_type_t< U > = std::in_place_type_t< U >{} )
	{
		return m_ValueArray.at( IndexerType::template Value< U >() );
	}

	template < typename U >
	IteratorType Assure()
	{
		static const size_t Index = IndexerType::template Value< U >();

		if ( !( Index < m_ValueArray.size() ) )
		{
			m_ValueArray.resize( Index + 1u );
			m_InfoArray.resize( Index + 1u );
			m_InfoArray[ Index ] = { &typeid( U ), ClassTraits< U >::GetTypeHash() };
		}

		return m_ValueArray.begin() + Index;
	}

	template < typename U, typename... Args >
	T& Emplace( Args&&... a_Args )
	{
		T& Value = Assure< U >();
		Value = T( std::forward< Args >()... );
		return Value;
	}

	IteratorType Erase( CIteratorType a_Where )
	{
		auto Index = a_Where - m_ValueArray.begin();
		auto InfoIter = m_InfoArray.begin() + Index;
		auto ValueIter = m_ValueArray.begin() + Index;
		InfoIter->Info = nullptr;
		InfoIter->Name = 0u;
		ShrinkToFit();
		return Index >= m_ValueArray.size() ? m_ValueArray.end() : m_ValueArray.begin() + Index;
	}

	void ShrinkToFit() 
	{
		while ( m_InfoArray.size() && m_InfoArray.back().Info )
		{
			m_InfoArray.erase( --m_InfoArray.end() );
			m_ValueArray.erase( --m_ValueArray.end() );
		}

		m_ValueArray.shrink_to_fit();
		m_InfoArray.shrink_to_fit(); 
	}

	CIteratorType Find( const T& a_Value ) const
	{
		for ( size_t i = 0; i < m_ValueArray.size(); ++i )
		{
			if ( !m_InfoArray[ i ].Info )
			{
				continue;
			}

			if ( m_ValueArray[ i ] == a_Value )
			{
				return m_ValueArray.begin() + i;
			}
		}

		return m_ValueArray.end();
	}

	IteratorType Find( const T& a_Value )
	{
		for ( size_t i = 0; i < m_ValueArray.size(); ++i )
		{
			if ( !m_InfoArray[ i ].Info )
			{
				continue;
			}

			if ( m_ValueArray[ i ] == a_Value )
			{
				return m_ValueArray.begin() + i;
			}
		}

		return m_ValueArray.end();
	}

	CIteratorType Find( const std::type_info* a_TypeInfo ) const
	{
		for ( size_t i = 0; i < m_ValueArray.size(); ++i )
		{
			if ( m_InfoArray[ i ].Info != a_TypeInfo )
			{
				continue;
			}

			return m_ValueArray.begin() + i;
		}

		return m_ValueArray.end();
	}

	IteratorType Find( const std::type_info* a_TypeInfo )
	{
		for ( size_t i = 0; i < m_ValueArray.size(); ++i )
		{
			if ( m_InfoArray[ i ].Info != a_TypeInfo )
			{
				continue;
			}

			return m_ValueArray.begin() + i;
		}

		return m_ValueArray.end();
	}

	CIteratorType Find( const hash_t a_TypeHash ) const
	{
		for ( size_t i = 0; i < m_ValueArray.size(); ++i )
		{
			if ( m_InfoArray[ i ].Name != a_TypeHash )
			{
				continue;
			}

			return m_ValueArray.begin() + i;
		}

		return m_ValueArray.end();
	}

	IteratorType Find( const hash_t a_TypeHash )
	{
		for ( size_t i = 0; i < m_ValueArray.size(); ++i )
		{
			if ( m_InfoArray[ i ].Name != a_TypeHash )
			{
				continue;
			}

			return m_ValueArray.begin() + i;
		}

		return m_ValueArray.end();
	}

	template < typename U >
	CIteratorType Find( std::in_place_type_t< U > = std::in_place_type_t< U >{} ) const
	{
		return Find( ClassTraits< U >::GetTypeHash() );
	}

	template < typename U >
	IteratorType Find( std::in_place_type_t< U > = std::in_place_type_t< U >{} )
	{
		return Find( ClassTraits< U >::GetTypeHash() );
	}

	template < typename U >
	IteratorType Insert( const T& a_Value )
	{
		IteratorType Value = Assure< U >();
		*Value = a_Value;
		return Value;
	}

	template < typename U >
	IteratorType Insert( T&& a_Value )
	{
		IteratorType Value = Assure< U >();
		*Value = std::move( a_Value );
		return Value;
	}

	template < typename U >
	const T& operator[]( std::in_place_type_t< U > ) const { return At< U >(); }

	template < typename U >
	T& operator[]( std::in_place_type_t< U > ) { return At< U >(); }

private:

	InfoArrayType	m_InfoArray;
	ArrayType		m_ValueArray;
};

namespace std
{
	template < typename T, typename _Indexer > static auto begin( TypeArray< T, _Indexer >& a_TypeArray ) { return a_TypeArray.Begin(); }
	template < typename T, typename _Indexer > static auto begin( const TypeArray< T, _Indexer >& a_TypeArray ) { return a_TypeArray.Begin(); }
	template < typename T, typename _Indexer > static auto cbegin( const TypeArray< T, _Indexer >& a_TypeArray ) { return a_TypeArray.CBegin(); }
	template < typename T, typename _Indexer > static auto end( TypeArray< T, _Indexer >& a_TypeArray ) { return a_TypeArray.End(); }
	template < typename T, typename _Indexer > static auto end( const TypeArray< T, _Indexer >& a_TypeArray ) { return a_TypeArray.End(); }
	template < typename T, typename _Indexer > static auto cend( const TypeArray< T, _Indexer >& a_TypeArray ) { return a_TypeArray.CEnd(); }
	template < typename T, typename _Indexer > static auto rbegin( TypeArray< T, _Indexer >& a_TypeArray ) { return a_TypeArray.RBegin(); }
	template < typename T, typename _Indexer > static auto rbegin( const TypeArray< T, _Indexer >& a_TypeArray ) { return a_TypeArray.RBegin(); }
	template < typename T, typename _Indexer > static auto crbegin( const TypeArray< T, _Indexer >& a_TypeArray ) { return a_TypeArray.CRBegin(); }
	template < typename T, typename _Indexer > static auto rend( TypeArray< T, _Indexer >& a_TypeArray ) { return a_TypeArray.REnd(); }
	template < typename T, typename _Indexer > static auto rend( const TypeArray< T, _Indexer >& a_TypeArray ) { return a_TypeArray.REnd(); }
	template < typename T, typename _Indexer > static auto crend( const TypeArray< T, _Indexer >& a_TypeArray ) { return a_TypeArray.CREnd(); }
}