#pragma once
#include <string>
#include <optional>
#include <vector>
#include <map>

#include <Utils/Address.hpp>
#include <Utils/Hash.hpp>
#include <IO/Serialisation.hpp>

template < typename T, typename _String, hash_t _DelimiterPack >
class AddressMap;

template < typename T, typename _String >
struct AddressMapNode
{
	using ValueType = T;
	using StringType = _String;
	using TraitsType = typename StringType::traits_type;
	using CharType = typename StringType::value_type;
	using StringViewType = std::basic_string_view< CharType, TraitsType >;

	AddressMapNode()
		: Value( nullptr )
		, AddressHash( 0u )
		, TokenHash( 0u )
		, Parent( nullptr )
		, Index( -1 )
	{}

	AddressMapNode( const AddressMapNode& a_Node )
		: Value( a_Node.Value ? new ValueType( *a_Node.Value ) : nullptr )
		, Address( a_Node.Address )
		, Token( a_Node.Token.size() > 0u ? StringViewType{ Address.data() + ( ( size_t )a_Node.Token.data() - ( size_t )a_Node.Address.data() ), a_Node.Token.size() } : StringViewType{} )
		, AddressHash( a_Node.AddressHash )
		, TokenHash( a_Node.TokenHash )
		, Parent( a_Node.Parent )
		, Index( a_Node.Index )
	{}

	AddressMapNode( AddressMapNode&& a_Node ) noexcept
		: Value( a_Node.Value )
		, Address( std::move( a_Node.Address ) )
		, Token( a_Node.Token )
		, AddressHash( a_Node.AddressHash )
		, TokenHash( a_Node.TokenHash )
		, Parent( a_Node.Parent )
		, Index( a_Node.Index )
	{
		a_Node.Value = nullptr;
		a_Node.Token = {};
		a_Node.AddressHash = 0u;
		a_Node.TokenHash = 0u;
		a_Node.Parent = nullptr;
		a_Node.Index = 0u;
	}

	AddressMapNode& operator=( const AddressMapNode& a_Node )
	{
		Value = a_Node.Value ? new ValueType( *a_Node.Value ) : nullptr;
		Address = a_Node.Address;
		Token = a_Node.Token.size() > 0u ? StringViewType{ Address.data() + ( ( size_t )a_Node.Token.data() - ( size_t )a_Node.Address.data() ), a_Node.Token.size() } : StringViewType{};
		AddressHash = a_Node.AddressHash;
		TokenHash = a_Node.TokenHash;
		Parent = a_Node.Parent;
		Index = a_Node.Index;

		return *this;
	}

	AddressMapNode& operator=( AddressMapNode&& a_Node ) noexcept
	{
		Value = a_Node.Value;
		Address = std::move( a_Node.Address );
		Token = a_Node.Token;
		AddressHash = a_Node.AddressHash;
		TokenHash = a_Node.TokenHash;
		Parent = a_Node.Parent;
		Index = a_Node.Index;

		a_Node.Value = nullptr;
		a_Node.Token = {};
		a_Node.AddressHash = 0u;
		a_Node.TokenHash = 0u;
		a_Node.Parent = nullptr;
		a_Node.Index = 0u;

		return *this;
	}

	~AddressMapNode()
	{
		if ( Value ) delete Value;
	}

	ValueType* Value;
	StringType Address;
	StringViewType Token;
	hash_t AddressHash;
	hash_t TokenHash;
	AddressMapNode* Parent;
	size_t Index;
	std::vector< AddressMapNode* > Children;
};

template < typename T, typename _String, bool _Const, bool _IterateNodes, hash_t _DelimiterPack >
class AddressMapIteratorBase
{
public:

	using ValueType = T;
	using StringType = _String;
	using TraitsType = typename StringType::traits_type;
	using CharType = typename StringType::value_type;
	using StringViewType = std::basic_string_view< CharType, TraitsType >;
	using NodeType = std::conditional_t< _Const, const AddressMapNode< ValueType, StringType >, AddressMapNode< ValueType, StringType > >;

	AddressMapIteratorBase() = default;
	AddressMapIteratorBase( std::nullptr_t ) : AddressMapIteratorBase() {};
	~AddressMapIteratorBase() = default;

	template < bool _OtherConst, bool _OtherIterateNodes, typename = std::enable_if_t< _Const || !_OtherConst > >
	AddressMapIteratorBase( const AddressMapIteratorBase< ValueType, StringType, _OtherConst, _OtherIterateNodes, _DelimiterPack >& a_Iterator )
		: m_Underlying( a_Iterator.m_Underlying )
	{
		if constexpr ( !_IterateNodes && _OtherIterateNodes )
		{
			if ( m_Underlying && !m_Underlying->Value )
			{
				++*this;
			}
		}
	}

	AddressMapIteratorBase& operator=( const AddressMapIteratorBase& ) = default;

	template < bool _OtherConst, bool _OtherIterateNodes, typename = std::enable_if_t< _Const || !_OtherConst > >
	AddressMapIteratorBase& operator=( const AddressMapIteratorBase< ValueType, StringType, _OtherConst, _OtherIterateNodes, _DelimiterPack >& a_Iterator )
	{
		m_Underlying = a_Iterator.m_Underlying;

		if constexpr ( !_IterateNodes && _OtherIterateNodes )
		{
			if ( m_Underlying && !m_Underlying->Value )
			{
				++*this;
			}
		}

		return *this;
	}

	AddressMapIteratorBase& operator++()
	{
		CHECK( m_Underlying );

		if constexpr ( _IterateNodes )
		{
			IncrementNode();
		}
		else
		{
			IncrementValue();
		}

		return *this;
	}

	AddressMapIteratorBase operator++( int )
	{
		auto Temp = *this;
		( void )++*this;
		return Temp;
	}

	AddressMapIteratorBase operator-() const
	{
		CHECK( m_Underlying );
		auto Result = *this;
		Result.m_Underlying = Result.m_Underlying->Parent;

		if constexpr ( _IterateNodes )
		{
			Result.IncrementNode( m_Underlying->Index );
		}
		else
		{
			Result.IncrementValue( m_Underlying->Index );
		}

		return Result;
	}

	std::conditional_t< _Const, const ValueType, ValueType >& operator*() const { return *m_Underlying->Value; }
	std::conditional_t< _Const, const ValueType, ValueType >* operator->() const { return m_Underlying->Value; }

	operator bool() const { return m_Underlying; }

	template < bool _OtherConst, bool _OtherIterateNodes >
	bool operator==( const AddressMapIteratorBase< ValueType, StringType, _OtherConst, _OtherIterateNodes, _DelimiterPack >& a_Iterator ) const { return m_Underlying == a_Iterator.m_Underlying; }

	template < bool _OtherConst, bool _OtherIterateNodes >
	bool operator!=( const AddressMapIteratorBase< ValueType, StringType, _OtherConst, _OtherIterateNodes, _DelimiterPack >& a_Iterator ) const { return m_Underlying != a_Iterator.m_Underlying; }

	// Does this iterate reference a value?
	bool HasValue() const { return m_Underlying && m_Underlying->Value; }

	// Get the address of this iterator in String form.
	const StringType& GetAddress() const { return m_Underlying->Address; }

	// Get the hash of the addres of this iterator.
	hash_t GetAddressHash() const { return m_Underlying->AddressHash; }

	// Get the trailing token of the address of this iterator in StringView form.
	const StringViewType& GetToken() const { return m_Underlying->Token; }

	// Get the hash of trailing token of the addres of this iterator.
	hash_t GetTokenHash() const { return m_Underlying->TokenHash; }

private:

	friend class AddressMap< ValueType, StringType, _DelimiterPack >;
	template < class, class, bool, bool, hash_t > friend class AddressMapIteratorBase;

	explicit AddressMapIteratorBase( NodeType* a_Node )
		: m_Underlying( a_Node )
	{}

	// Move the iterator one node. CurrentIndex refers to the simulated index of the current node:
	// -1 refers to the node itself, 0+ refers to the indices of the children.
	//   -1
	//  / | \
	// 0  1  2
	void IncrementNode( size_t a_CurrentIndex = -1 )
	{
		// While we are not at the end node.
		while ( m_Underlying )
		{
			// If this node has a child, and the simulated index is not the last index of this nodes children,
			// then we can move to the next child index.
			if ( a_CurrentIndex != m_Underlying->Children.size() - 1u )
			{
				m_Underlying = m_Underlying->Children[ a_CurrentIndex + 1u ];

				// Break recursion. We found our next node.
				break;
			}

			// If our similated index was the last, we should track what index of the parent
			// we are, and move upwards.
			a_CurrentIndex = m_Underlying->Index;
			m_Underlying = m_Underlying->Parent;
		}
	}

	// Move the iterator one value. CurrentIndex refers to the simulated index of the current node:
	// -1 refers to the node itself, 0+ refers to the indices of the children. This will continue until
	// it either hits the end, or finds a node with a value.
	//   -1
	//  / | \
	// 0  1  2
	void IncrementValue( const size_t a_CurrentIndex = -1 )
	{
		IncrementNode( a_CurrentIndex );

		while ( m_Underlying && !m_Underlying->Value )
		{
			IncrementNode();
		}
	}

	NodeType* GetNode() { return m_Underlying; }
	const NodeType* GetNode() const { return m_Underlying; }

	mutable NodeType* m_Underlying = nullptr;
};

template < typename T, typename _String, hash_t _DelimiterPack = ""_H >
using AddressMapIterator = AddressMapIteratorBase< T, _String, false, false, _DelimiterPack >;

template < typename T, typename _String, hash_t _DelimiterPack = ""_H >
using AddressMapCIterator = AddressMapIteratorBase< T, _String, true, false, _DelimiterPack >;

template < typename T, typename _String, hash_t _DelimiterPack = ""_H >
using AddressMapNodeIterator = AddressMapIteratorBase< T, _String, false, true, _DelimiterPack >;

template < typename T, typename _String, hash_t _DelimiterPack = ""_H >
using AddressMapNodeCIterator = AddressMapIteratorBase< T, _String, true, true, _DelimiterPack  >;

template < typename T, typename _String, bool _Const, bool _IterateNodes, hash_t _DelimiterPack >
struct std::iterator_traits< AddressMapIteratorBase< T, _String, _Const, _IterateNodes, _DelimiterPack > >
{
	using iterator_category = std::forward_iterator_tag;
	using difference_type = std::ptrdiff_t;
	using value_type = std::conditional_t< _Const, const T, T >;
	using reference = value_type&;
	using pointer = value_type*;
};

template < typename T, typename _String = std::string, hash_t _DelimiterPack = ""_H >
class AddressMap
{
public:

	using NodeType = AddressMapNode< T, _String >;
	using ValueType = T;
	using ContainerType = std::map< hash_t, NodeType >;
	using StringType = _String;
	using TraitsType = typename StringType::traits_type;
	using CharType = typename StringType::value_type;
	using SizeType = typename StringType::size_type;
	using StringViewType = std::basic_string_view< CharType, TraitsType >;
	using AddressType = BasicAddress< StringType, _DelimiterPack >;
	using IteratorType = AddressMapIterator< ValueType, StringType, _DelimiterPack >;
	using CIteratorType = AddressMapCIterator< ValueType, StringType, _DelimiterPack >;
	using NodeIteratorType = AddressMapNodeIterator< ValueType, StringType, _DelimiterPack >;
	using NodeCIteratorType = AddressMapNodeCIterator< ValueType, StringType, _DelimiterPack >;

	AddressMap()
		: m_Count( 0u )
	{
		m_Root = &m_Nodes.emplace( 0u, NodeType{} ).first->second;
	}

	AddressMap( const AddressMap& a_AddressMap )
		: m_Nodes( a_AddressMap.m_Nodes )
		, m_Count( a_AddressMap.m_Count )
	{
		// Connect up all the copied nodes.
		for ( auto& NodePair : m_Nodes )
		{
			auto& FromNode = a_AddressMap.m_Nodes.at( NodePair.first );

			if ( FromNode.Parent )
			{
				NodePair.second.Parent = &m_Nodes.at( FromNode.Parent->AddressHash );
			}

			NodePair.second.Children.reserve( FromNode.Children.size() );

			for ( size_t i = 0u; i < FromNode.Children.size(); ++i )
			{
				NodePair.second.Children.push_back( &m_Nodes.at( FromNode.Children[ i ]->AddressHash ) );
			}
		}

		m_Root = &m_Nodes.at( 0u );
	}

	AddressMap( AddressMap&& a_AddressMap ) noexcept
		: m_Root( std::move( a_AddressMap.m_Root ) )
		, m_Nodes( std::move( a_AddressMap.m_Nodes ) )
		, m_Count( a_AddressMap.m_Count )
	{
		a_AddressMap.m_Root = nullptr;
		a_AddressMap.m_Count = 0u;
	}

	AddressMap& operator=( const AddressMap& a_AddressMap )
	{
		m_Nodes = a_AddressMap.m_Nodes;
		m_Count = a_AddressMap.m_Count;

		// Connect up all the copied nodes.
		for ( auto& NodePair : m_Nodes )
		{
			auto& FromNode = a_AddressMap.m_Nodes.at( NodePair.first );

			if ( FromNode.Parent )
			{
				NodePair.second.Parent = &m_Nodes.at( FromNode.Parent->AddressHash );
			}

			NodePair.second.Children.reserve( FromNode.Children.size() );

			for ( size_t i = 0u; i < FromNode.Children.size(); ++i )
			{
				NodePair.second.Children.push_back( &m_Nodes.at( FromNode.Children[ i ]->AddressHash ) );
			}
		}

		m_Root = &m_Nodes.at( 0u );

		return *this;
	}

	AddressMap& operator=( AddressMap&& a_AddressMap ) noexcept
	{
		m_Root = a_AddressMap.m_Root;
		m_Nodes = std::move( a_AddressMap.m_Nodes );
		m_Count = a_AddressMap.m_Count;
		a_AddressMap.m_Root = nullptr;
		a_AddressMap.m_Count = 0u;

		return *this;
	}

	~AddressMap() = default;

	IteratorType Begin() { return IteratorType{ m_Root }; }
	CIteratorType Begin() const { return CIteratorType{ m_Root }; }
	CIteratorType CBegin() const { return CIteratorType{ m_Root }; }
	IteratorType End() { return IteratorType{ nullptr }; }
	CIteratorType End() const { return CIteratorType{ nullptr }; }
	CIteratorType CEnd() const { return CIteratorType{ nullptr }; }

	NodeIteratorType NodeBegin() { return NodeIteratorType{ m_Root }; }
	NodeCIteratorType NodeBegin() const { return NodeCIteratorType{ m_Root }; }
	NodeCIteratorType NodeCBegin() const { return NodeCIteratorType{ m_Root }; }
	NodeIteratorType NodeEnd() { return NodeIteratorType{ nullptr }; }
	NodeCIteratorType NodeEnd() const { return NodeCIteratorType{ nullptr }; }
	NodeCIteratorType NodeCEnd() const { return NodeCIteratorType{ nullptr }; }

	// Get the object at the given address. If value does not exist at location, this will be undefined behaviour.
	const T& At( const StringViewType& a_Address ) const
	{
		return const_cast< AddressMap* >( this )->At( a_Address );
	}

	// Get the object at the given address. If value does not exist at location, this will be undefined behaviour.
	T& At( const StringViewType& a_Address )
	{
		return *FindNode( a_Address, false )->Value;
	}

	// Get the object at the given address hash. If value does not exist at location, this will be undefined behaviour.
	const T& At( const hash_t a_Address ) const
	{
		return const_cast< AddressMap* >( this )->At( a_Address );
	}

	// Get the object at the given address hash. If value does not exist at location, this will be undefined behaviour.
	T& At( const hash_t a_Address )
	{
		return *FindNode( a_Address )->Value;
	}

	// Is the AddressMap empty?
	bool Empty() const { return !m_Count; }

	// How many elements are in the AddressMap?
	SizeType Size() const { return m_Count; }

	// The max amount of elements that can be inserted into the AddressMap.
	constexpr SizeType MaxSize() const { return m_Nodes.max_size(); }

	// Clear the AddressMap.
	void Clear()
	{
		m_Nodes.clear();
		m_Root = &m_Nodes.emplace( 0u, NodeType{} ).first->second;
		m_Count = 0u;
	}

	// Insert a value at a given address. If the address already exists, no insertion will take place.
	template < typename _Value >
	std::pair< IteratorType, bool > Insert( const StringViewType& a_Address, _Value&& a_Value )
	{
		NodeType* Node = AssureNode( a_Address, false );
		bool AlreadyExists = true;

		if ( !Node->Value )
		{
			Node->Value = new ValueType( std::forward< _Value >( a_Value ) );
			AlreadyExists = false;
			++m_Count;
		}

		return { IteratorType{ Node }, !AlreadyExists };
	}

	// Find a node by the given address. A value is not guaranteed to exist at this address.
	NodeIteratorType FindNode( const StringViewType& a_Address )
	{
		return NodeIteratorType{ FindNodeByAddress( a_Address, false ) };
	}

	// Find a node by the given address. A value is not guaranteed to exist at this address.
	NodeCIteratorType FindNode( const StringViewType& a_Address ) const
	{
		return NodeCIteratorType{ const_cast< AddressMap* >( this )->FindNodeByAddress( a_Address, false ) };
	}

	// Find a node by the given address. A value is not guaranteed to exist at this address.
	NodeIteratorType FindNode( const hash_t a_Address )
	{
		return NodeIteratorType{ FindNodeByHash( a_Address ) };
	}

	// Find a node by the given address. A value is not guaranteed to exist at this address.
	NodeCIteratorType FindNode( const hash_t a_Address ) const
	{
		return const_cast< AddressMap* >( this )->FindNode( a_Address );
	}

	// Get an iterator to an element at the given address. If no element exists, end iterator will be returned.
	IteratorType Find( const StringViewType& a_Address )
	{
		auto Iter = FindNode( a_Address );
		return IteratorType{ Iter.HasValue() ? Iter : NodeEnd() };
	}

	// Get an iterator to an element at the given address. If no element exists, end iterator will be returned.
	CIteratorType Find( const StringViewType& a_Address ) const
	{
		return const_cast< AddressMap* >( this )->Find( a_Address );
	}

	// Get an iterator to an element at the given address has. If no element exists, end iterator will be returned.
	IteratorType Find( const hash_t a_Address )
	{
		auto Iter = FindNode( a_Address );
		return Iter.HasValue() ? IteratorType{ Iter } : End();
	}

	// Get an iterator to an element at the given address hash. If no element exists, end iterator will be returned.
	CIteratorType Find( const hash_t a_Address ) const
	{
		return const_cast< AddressMap* >( this )->Find( a_Address );
	}

	// Construct an element in place at the given address. If element already exists, it will be deleted first and
	// reconstructed using provided args.
	template < typename... Args >
	T& Emplace( const StringViewType& a_Address, Args&&... a_Args )
	{
		NodeType* Node = AssureNode( a_Address, false );

		if ( Node->Value )
		{
			delete Node->Value;
		}
		else
		{
			++m_Count;
		}

		return *( Node->Value = new ValueType( std::forward< Args >( a_Args )... ) );
	}

	// Construct an element in place at the given address. If one already exists, will do nothing.
	template < typename... Args >
	T& TryEmplace( const StringViewType& a_Address, Args&&... a_Args )
	{
		NodeType* Node = AssureNode( a_Address, false );

		if ( !Node->Value )
		{
			Node->Value = new ValueType( std::forward< Args >( a_Args )... );
			++m_Count;
		}

		return *Node->Value;
	}

	// Find or construct an element at a given address.
	T& operator[]( const StringViewType& a_Address )
	{
		return TryEmplace( a_Address );
	}

	// Erase a secific address node. All address nodes below will also be erased.
	// An iterator to the next available address node will be returned.
	NodeIteratorType EraseNode( NodeCIteratorType a_Where )
	{
		CHECK( a_Where != NodeEnd() );

		// We want to delete a node and all of its children, we need to recurse and find them all.
		NodeType* Node = const_cast< NodeType* >( a_Where.GetNode() );
		NodeType* Parent = const_cast< NodeType* >( Node->Parent );
		const size_t Index = Node->Index;

		// First disconnect node from parent.
		if ( Parent )
		{
			DisconnectNode( Node );
		}

		// We can now iterate on this node and gather the keys to remove.
		NodeIteratorType Begin{ Node }, End;
		std::vector< hash_t > KeysToErase;

		for ( ; Begin != End; ++Begin )
		{
			KeysToErase.push_back( Begin.GetAddressHash() );
			m_Count -= !!Begin.m_Underlying->Value;
		}

		// Erase all keys found at or below the iterator.
		for ( auto KeyToErase : KeysToErase )
		{
			m_Nodes.erase( KeyToErase );
		}

		// Find the next available iterator with a value, starting from erased index.
		NodeIteratorType Result{ Parent };
		Result.IncrementNode( Index - 1u );
		return Result;
	}

	// Erase an element. An iterator to the next available address node will be returned.
	IteratorType Erase( CIteratorType a_Where )
	{
		CHECK( a_Where != End() );

		NodeType* Node = const_cast< NodeType* >( a_Where.GetNode() );

		if ( Node->Value )
		{
			delete Node->Value;
			Node->Value = nullptr;
			--m_Count;
		}

		auto Result = ++a_Where;

		if ( !Node->Children.size() )
		{
			DisconnectNode( Node );
			m_Nodes.erase( Node->AddressHash );
		}

		return IteratorType( const_cast< NodeType* >( Result.GetNode() ) );
	}

	// Find all iterators that have values at or below the given iterator.
	std::vector< CIteratorType > FindAll( NodeCIteratorType a_Where ) const
	{
		std::vector< CIteratorType > Result;

		for ( NodeCIteratorType End = -a_Where; a_Where != End; ++a_Where )
		{
			if ( a_Where.HasValue() )
			{
				Result.push_back( CIteratorType( a_Where ) );
			}
		}

		return Result;
	}

	// Find all iterators that have values at or below the given iterator.
	std::vector< IteratorType > FindAll( NodeIteratorType a_Where )
	{
		std::vector< IteratorType > Result;

		for ( NodeIteratorType End = -a_Where; a_Where != End; ++a_Where )
		{
			if ( a_Where.HasValue() )
			{
				Result.push_back( IteratorType( a_Where ) );
			}
		}

		return Result;
	}

	// Find all iterators that have values and which satisfy the predicate at or below the given iterator.
	template < typename _Predicate >
	std::vector< CIteratorType > FindAll( NodeCIteratorType a_Where, const _Predicate& a_Predicate ) const
	{
		std::vector< CIteratorType > Result;

		for ( NodeCIteratorType End = -a_Where; a_Where != End; ++a_Where )
		{
			if ( a_Where.HasValue() && a_Predicate( *a_Where ) )
			{
				Result.push_back( CIteratorType( a_Where ) );
			}
		}

		return Result;
	}
	
	// Find all iterators that have values and which satisfy the predicate at or below the given iterator.
	template < typename _Predicate >
	std::vector< IteratorType > FindAll( NodeIteratorType a_Where, const _Predicate& a_Predicate )
	{
		std::vector< IteratorType > Result;

		for ( NodeIteratorType End = -a_Where; a_Where != End; ++a_Where )
		{
			if ( a_Where.HasValue() && a_Predicate( *a_Where ) )
			{
				Result.push_back( IteratorType( a_Where ) );
			}
		}

		return Result;
	}

	// Count all nodes that have values at or below an iterator.
	SizeType Count( NodeCIteratorType a_Where )
	{
		SizeType Count = 0u;

		for ( NodeCIteratorType End = -a_Where; a_Where != End; ++a_Where )
		{
			if ( a_Where.HasValue() )
			{
				++Count;
			}
		}

		return Count;
	}

	// Count all nodes that have values that satisfy the predicate at or below an iterator.
	template < typename _Predicate >
	SizeType Count( NodeCIteratorType a_Where, const _Predicate& a_Predicate )
	{
		SizeType Count = 0u;

		for ( NodeIteratorType End = -a_Where; a_Where != End; ++a_Where )
		{
			if ( a_Where.HasValue() && a_Predicate( *a_Where ) )
			{
				++Count;
			}
		}

		return Count;
	}

private:

	friend class Serialisation;

	NodeType* AssureNode( const StringViewType& a_Address, bool a_IsSanitised = false )
	{
		// If a node by this address already exists, return it.
		if ( NodeType* Node = FindNodeByAddress( a_Address, a_IsSanitised ) )
		{
			return Node;
		}

		// If not, we need to create it.
		NodeType* Current = m_Root;

		// Iterate through all tokens and create a node path with the same path.
		for ( auto TokenIter = typename AddressType::IteratorType{ a_Address }; TokenIter; ++TokenIter )
		{
			auto Token = *TokenIter;
			hash_t TokenHash = Hashing::Hash( Token );
			bool ChildExists = false;

			for ( auto Child : Current->Children )
			{
				if ( Child->TokenHash == TokenHash )
				{
					Current = Child;
					ChildExists = true;
					break;
				}
			}

			if ( ChildExists )
			{
				continue;
			}

			const CharType* Start = a_Address.data();
			const CharType* End = Token.data();
			End += Token.size();
			StringViewType CurrentAddress = { Start, ( size_t )End - ( size_t )Start };
			hash_t CurrentAddressHash = Hashing::Hash( CurrentAddress );
			auto& Node = m_Nodes[ CurrentAddressHash ];
			Node.Value = nullptr;
			Node.Address = CurrentAddress;
			Node.Token = StringViewType{ Node.Address.data() + ( ( size_t )Token.data() - ( size_t )a_Address.data() ), Token.size() };
			Node.AddressHash = CurrentAddressHash;
			Node.TokenHash = TokenHash;
			Node.Parent = Current;
			Node.Index = Current->Children.size();
			Current->Children.emplace_back( &Node );
			Current = &Node;
		}

		return Current;
	}

	NodeType* FindNodeByAddress( const StringViewType& a_Address, bool a_IsSanitised = false )
	{
		// If string is empty, must be root.
		if ( a_Address.empty() )
		{
			return m_Root;
		}

		hash_t AddressHash = 0u;

		// Calculate the hash. If not sanitised, sanitise first.
		if ( !a_IsSanitised )
		{
			// Convert string into unique address.
			auto Address = AddressType{ a_Address };
			Address.Sanitise();
			AddressHash = Hashing::Hash( Address.String() );
		}
		else
		{
			AddressHash = Hashing::Hash( a_Address );
		}

		return FindNodeByHash( AddressHash );
	}

	NodeType* FindNodeByHash( const hash_t a_Address )
	{
		// If hash came out to 0, must be root.
		if ( a_Address == 0u )
		{
			return m_Root;
		}

		// Check to see if it can be found in node lookup.
		if ( auto Iter = m_Nodes.find( a_Address ); Iter != m_Nodes.end() )
		{
			return &Iter->second;
		}

		return nullptr;
	}

	static void DisconnectNode( NodeType* a_Node )
	{
		a_Node->Parent->Children[ a_Node->Index ] = a_Node->Parent->Children.back();
		a_Node->Parent->Children[ a_Node->Index ]->Index = a_Node->Index;
		a_Node->Parent->Children.pop_back();
		a_Node->Parent = nullptr;
		a_Node->Index = -1;
	}

	NodeType* m_Root;
	std::map< hash_t, NodeType > m_Nodes;
	SizeType m_Count = 0u;
};

namespace std
{
	template < typename T, typename _String, hash_t _DelimiterPack > static auto begin( AddressMap< T, _String, _DelimiterPack >& a_AddressMap ) { return a_AddressMap.Begin(); }
	template < typename T, typename _String, hash_t _DelimiterPack > static auto begin( const AddressMap< T, _String, _DelimiterPack >& a_AddressMap ) { return a_AddressMap.Begin(); }
	template < typename T, typename _String, hash_t _DelimiterPack > static auto cbegin( const AddressMap< T, _String, _DelimiterPack >& a_AddressMap ) { return a_AddressMap.CBegin(); }
	template < typename T, typename _String, hash_t _DelimiterPack > static auto end( AddressMap< T, _String, _DelimiterPack >& a_AddressMap ) { return a_AddressMap.End(); }
	template < typename T, typename _String, hash_t _DelimiterPack > static auto end( const AddressMap< T, _String, _DelimiterPack >& a_AddressMap ) { return a_AddressMap.End(); }
	template < typename T, typename _String, hash_t _DelimiterPack > static auto cend( const AddressMap< T, _String, _DelimiterPack >& a_AddressMap ) { return a_AddressMap.CEnd(); }

	template < typename T, typename _String, bool _Const, hash_t _DelimiterPack > static auto begin( const AddressMapIteratorBase< T, _String, _Const, false, _DelimiterPack >& a_AddressMapIterator ) { return a_AddressMapIterator; }
	template < typename T, typename _String, bool _Const, hash_t _DelimiterPack > static auto cbegin( const AddressMapIteratorBase< T, _String, _Const, false, _DelimiterPack >& a_AddressMapIterator ) { return a_AddressMapIterator; }
	template < typename T, typename _String, bool _Const, hash_t _DelimiterPack > static auto end( const AddressMapIteratorBase< T, _String, _Const, false, _DelimiterPack >& a_AddressMapIterator ) { return -a_AddressMapIterator; }
	template < typename T, typename _String, bool _Const, hash_t _DelimiterPack > static auto cend( const AddressMapIteratorBase< T, _String, _Const, false, _DelimiterPack >& a_AddressMapIterator ) { return -a_AddressMapIterator; }
}

template < typename T, typename _String, hash_t _DelimiterPack >
DECLARE_AS_UNVERSIONED_TEMPLATED_SERIALISABLE_EXTERNAL( AddressMap< T, _String, _DelimiterPack > );

template < typename T, typename _String, hash_t _DelimiterPack >
DEFINE_TEMPLATED_SIZER_EXTERNAL( AddressMap< T, _String, _DelimiterPack > )
{
	Archive.AddSizeExplicit< decltype( Object.m_Count ) >();

	for ( auto& Node : Object.m_Nodes )
	{
		if ( Node.second.Value )
		{
			Archive += *Node.second.Value;
			Archive += Node.second.Address;
		}
	}
};

template < typename T, typename _String, hash_t _DelimiterPack >
DEFINE_TEMPLATED_SERIALISER_EXTERNAL( AddressMap< T, _String, _DelimiterPack > )
{
	Archive << Object.m_Count;

	for ( auto& Node : Object.m_Nodes )
	{
		if ( Node.second.Value )
		{
			Archive << *Node.second.Value;
			Archive << Node.second.Address;
		}
	}
};

template <typename T, typename _String, hash_t _DelimiterPack >
DEFINE_UNVERSIONED_TEMPLATED_DESERIALISER_EXTERNAL( AddressMap< T, _String, _DelimiterPack > )
{
	using Type = AddressMap< T, _String, _DelimiterPack >;
	using ValueType = typename Type::ValueType;
	using StringType = typename Type::StringType;
	using NodeType = typename Type::NodeType;

	Archive >> Object.m_Count;

	for ( size_t i = 0u; i < Object.m_Count; ++i )
	{
		ValueType* Value = new T;
		StringType Address;

		Archive >> *Value;
		Archive >> Address;

		NodeType* Node = Object.AssureNode( Address, true );
		Node->Value = Value;
		Node->Address = std::move( Address );
		Node->AddressHash = Hashing::Hash( Node->Address );
		Node->Token = *AddressCRIterator{ Node->Address };
		Node->TokenHash = Hashing::Hash( Node->Token );
	}
};