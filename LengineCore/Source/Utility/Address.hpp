#pragma once
#include <iterator>
#include <string>
#include <string_view>
#include <type_traits>

template < hash_t _Name = 0u >
struct AddressDelimiterPack {};

// Defines a delimiter pack for addresses.
// - Name refers to the delimiter pack's ID.
// - Return - token that if separated by delimiters, will refer to the current address
//   if used once, and refer to the root address if used twice. eg '.' gives:
//    '.' = ././some_path -> some_path
//    '..' = some_root/some_path/../some_other_path -> some_root/some_other_path
#define DEFINE_ADDRESS_DELIMITER_PACK( Name, Return, ... ) \
	template <> \
	struct AddressDelimiterPack< Name##_H > \
	{ \
		static constexpr char R##eturn = Return; \
		static constexpr char Delimiters[] = { __VA_ARGS__ }; \
		\
		static constexpr auto IsDelimiter = []( const auto a_Char ) \
		{ \
			if constexpr ( std::extent_v< decltype( Delimiters ) > == 1u ) \
			{ \
				return a_Char == Delimiters[ 0u ]; \
			} \
			else \
			{ \
				for ( const auto Delimiter : Delimiters ) \
					if ( Delimiter == a_Char ) \
						return true; \
				return false; \
			} \
		}; \
	}

DEFINE_ADDRESS_DELIMITER_PACK( "", '.', '/' );

template < typename _String, bool _Reverse, hash_t _DelimiterPack >
class BasicAddressIterator
{
public:

	using StringType = _String;
	using CharType = typename StringType::value_type;
	using TraitsType = typename StringType::traits_type;
	using SizeType = typename StringType::size_type;
	using StringViewType = std::basic_string_view< CharType, TraitsType >;
	using AddressDelimiterPackType = AddressDelimiterPack< _DelimiterPack >;
	using ValueType = StringViewType;
	
	static_assert( std::is_same_v< CharType, std::decay_t< decltype( AddressDelimiterPackType::Delimiters[ 0 ] ) > >, "Delimiters must be the same character type as StringType." );

	static constexpr CharType DefaultDelimiter = AddressDelimiterPackType::Delimiters[ 0u ];
	static constexpr CharType Terminator = ( CharType )'\0';

	BasicAddressIterator() = default;
	BasicAddressIterator( const BasicAddressIterator& ) = default;
	BasicAddressIterator( BasicAddressIterator&& ) = default;
	~BasicAddressIterator() = default;
	BasicAddressIterator& operator=( const BasicAddressIterator& ) = default;
	BasicAddressIterator& operator=( BasicAddressIterator&& ) = default;

	BasicAddressIterator( const CharType* a_String, const CharType* a_Start )
		: m_Start( a_Start )
	{
		if ( !a_String )
		{
			m_StringView = StringViewType{};
			return;
		}

		if constexpr ( !_Reverse )
		{
			m_StringView = FindNextView( a_String );
		}
		else
		{
			m_StringView = FindPreviousView( a_String, a_Start );
		}
	}

	template < typename _Supported >
	BasicAddressIterator( const _Supported& a_String )
	{
		using SupportedType = std::decay_t< _Supported >;

		if constexpr ( std::is_pointer_v< SupportedType > )
		{
			*this = BasicAddressIterator( StringViewType{ a_String } );
		}
		else
		{
			if constexpr ( !_Reverse )
			{
				*this = BasicAddressIterator( a_String.data(), a_String.data() - 1u );
			}
			else
			{
				*this = BasicAddressIterator( a_String.data() + a_String.size() - 1u, a_String.data() - 1u );
			}
		}
	}

	template < typename _Supported >
	BasicAddressIterator& operator=( const _Supported& a_String ) { return *this = BasicAddressIterator( a_String ); }

	BasicAddressIterator& operator++()
	{
		if constexpr ( _Reverse )
		{
			Decrement();
		}
		else
		{
			Increment();
		}

		return *this;
	}

	BasicAddressIterator operator++( int )
	{
		auto Temp = *this;
		( void )++* this;
		return Temp;
	}

	BasicAddressIterator& operator--()
	{
		if constexpr ( !_Reverse )
		{
			Decrement();
		}
		else
		{
			Increment();
		}

		return *this;
	}

	BasicAddressIterator operator--( int )
	{
		auto Temp = *this;
		( void )--* this;
		return Temp;
	}

	StringViewType operator*() const { CHECK( *this ); return m_StringView; }

	const CharType* operator->() const { CHECK( *this ); return m_StringView.data(); }

	operator bool() const
	{
		if constexpr ( !_Reverse )
		{
			return !m_StringView.empty() && m_StringView[ 0 ] != Terminator;
		}
		else
		{
			return !m_StringView.empty() && m_StringView.data() != m_Start;
		}
	}

	bool operator==( const BasicAddressIterator& a_Iterator ) const { return m_StringView.data() == a_Iterator.m_StringView.data(); }
	bool operator!=( const BasicAddressIterator& a_Iterator ) const { return m_StringView.data() != a_Iterator.m_StringView.data(); }

private:

	void Increment()
	{
		CHECK( !m_StringView.empty() && m_StringView[ 0 ] != Terminator );
		m_StringView = FindNextView( m_StringView.data() + m_StringView.size() );
	}

	void Decrement()
	{
		CHECK( m_StringView.data() && m_StringView.data() != m_Start );
		m_StringView = FindPreviousView( m_StringView.data() - 1u, m_Start );
	}

	static StringViewType FindNextView( const CharType* a_String )
	{
		while ( *a_String != Terminator && AddressDelimiterPackType::IsDelimiter( *a_String ) ) ++a_String;
		const CharType* End = a_String;
		while ( *End != Terminator  && !AddressDelimiterPackType::IsDelimiter( *End ) ) ++End;
		return { a_String, ( size_t )End - ( size_t )a_String };
	}

	static StringViewType FindPreviousView( const CharType* a_String, const CharType* a_Start )
	{
		if ( a_String == a_Start )
		{
			return { a_Start, 1u };
		}

		while ( a_String != a_Start && AddressDelimiterPackType::IsDelimiter( *a_String ) ) --a_String;
		const CharType* End = a_String;
		while ( End != a_Start && !AddressDelimiterPackType::IsDelimiter( *End ) ) --End;
		return { End + 1u, ( size_t )a_String - ( size_t )End };
	}

	StringViewType m_StringView;
	const CharType* m_Start;
};

using AddressIterator = BasicAddressIterator< std::string, false, ""_H >;
using AddressCIterator = BasicAddressIterator< std::string, false, ""_H >;
using AddressRIterator = BasicAddressIterator< std::string, true, ""_H >;
using AddressCRIterator = BasicAddressIterator< std::string, true, ""_H >;

template < typename _String, hash_t _DelimiterPack  >
class BasicAddress
{
public:

	using StringType = _String;
	using CharType = typename StringType::value_type;
	using TraitsType = typename StringType::traits_type;
	using SizeType = typename StringType::size_type;
	using StringViewType = std::basic_string_view< CharType, TraitsType >;
	using AddressDelimiterPackType = AddressDelimiterPack< _DelimiterPack >;
	using IteratorType = BasicAddressIterator< StringType, false, _DelimiterPack >;
	using CIteratorType = IteratorType;
	using RIteratorType = BasicAddressIterator< StringType, true, _DelimiterPack >;
	using CRIteratorType = RIteratorType;

	static_assert( std::is_same_v< CharType, std::decay_t< decltype( AddressDelimiterPackType::Delimiters[ 0 ] ) > >, "Delimiters must be the same character type as StringType." );

	BasicAddress() = default;
	BasicAddress( const BasicAddress& ) = default;
	BasicAddress( BasicAddress&& ) = default;
	~BasicAddress() = default;
	BasicAddress& operator=( const BasicAddress& ) = default;
	BasicAddress& operator=( BasicAddress&& ) = default;

	template < typename _Supported >
	BasicAddress( _Supported&& a_String )
		: m_String( std::forward< _Supported >( a_String ) )
	{}

	template < typename _Supported >
	BasicAddress& operator=( _Supported&& a_String ) { m_String = std::forward< _Supported >( a_String ); return *this; }

	const StringType& String() const { return m_String; }
	StringType& String() { return m_String; }
	const CharType* Data() const noexcept { return m_String.data(); }
	CharType* Data() noexcept { return m_String.data(); }
	SizeType Size() const { return m_String.size(); }

	explicit operator const StringType& ( ) const { return m_String; }
	explicit operator StringType& ( ) { return m_String; }
	explicit operator StringViewType() const { return ( StringViewType )m_String; }
	explicit operator const CharType* ( ) const { return m_String.data(); }
	explicit operator CharType* ( ) { return m_String.data(); }

	// Is the address an empty address?
	bool IsEmpty() const
	{
		return m_String.empty();
	}

	// Get the parent address of this address.
	// eg. "some/path/to/something" -> "some/path/to"
	// eg. "some_path" -> ""
	// eg. "" -> ""
	StringViewType GetParent() const
	{
		if ( m_String.size() == 0u )
		{
			return StringViewType{};
		}

		auto Last = --End();

		if ( Last == Begin() )
		{
			return StringViewType{};
		}

		return { m_String.data(), ( size_t ) & ( *--Last ).back() - ( size_t )m_String.data() + 1u };
	}

	// Get the destination name.
	// eg. "some/path/to/something" -> "something"
	// eg. "some_path" -> "some_path"
	// eg. "" -> ""
	StringViewType GetStem() const
	{
		if ( m_String.size() == 0u )
		{
			return StringViewType{};
		}

		return *--End();
	}

	// Return an address made into unique form. ".." and "." operators will be collapsed.
	// eg. "some/path/to/something" -> "some/path/to/something"
	// eg. "some/////path///to///something" -> "some/path/to/something"
	// eg. ""some/path/.././to/something/.." -> "some/to"
	// eg. "" -> ""
	BasicAddress ToSanitised() const
	{
		if ( m_String.empty() )
		{
			return {};
		}

		std::vector< StringViewType > Tokens;

		static constexpr CharType Return = AddressDelimiterPackType::Return;

		static constexpr auto IsCurrent = []( const StringViewType& a_StringView )
		{
			return a_StringView.size() == 1u && a_StringView[ 0 ] == Return;
		};

		static auto IsRoot = []( const StringViewType& a_StringView )
		{
			return a_StringView.size() == 2u && a_StringView[ 0 ] == Return && a_StringView[ 1 ] == Return;
		};

		size_t TotalLength = 0u;

		for ( auto Token : *this )
		{
			if ( IsCurrent( Token ) )
			{
				continue;
			}

			if ( IsRoot( Token ) )
			{
				if ( Tokens.size() > 0u )
				{
					Tokens.erase( --Tokens.end() );
				}
			}
			else
			{
				Tokens.push_back( Token );
				TotalLength += Token.size() + 1u;
			}
		}

		if ( Tokens.empty() )
		{
			return {};
		}

		StringType Result;
		Result.reserve( TotalLength + Tokens.size() );

		for ( auto& Token : Tokens )
		{
			Result += Token;
			Result += IteratorType::DefaultDelimiter;
		}

		Result.resize( Result.size() - 1u );
		return { Result };
	}

	// Make sanitise this address. See ToSanitised.
	void Sanitise()
	{
		*this = ToSanitised();
	}

	BasicAddress operator+( const BasicAddress& a_Address ) { return BasicAddress{ m_String + a_Address.m_String }; }
	BasicAddress& operator+=( const BasicAddress& a_Address ) { m_String += a_Address.m_String; return *this; }
	BasicAddress operator/( const BasicAddress& a_Address ) { StringType Result; Result.reserve( m_String.size() + 1u + a_Address.m_String.size() ); Result += m_String; Result += IteratorType::DefaultDelimiter; Result += a_Address.m_String; return { Result }; }
	BasicAddress& operator/=( const BasicAddress& a_Address ) { m_String.reserve( 1u + a_Address.m_String.size() ); m_String += IteratorType::DefaultDelimiter += a_Address.m_String; return *this; }

	IteratorType Begin() { return IteratorType{ m_String.c_str(), m_String.c_str() - 1u }; }
	CIteratorType Begin() const { return const_cast< BasicAddress* >( this )->Begin(); }
	CIteratorType CBegin() const { return Begin(); }
	RIteratorType RBegin() { return RIteratorType{ m_String.c_str() + m_String.size() - 1u, m_String.c_str() - 1u }; }
	CRIteratorType RBegin() const { return const_cast< BasicAddress* >( this )->RBegin(); }
	CRIteratorType CRBegin() const { return RBegin(); }
	IteratorType End() { return IteratorType{ m_String.c_str() + m_String.size(), m_String.c_str() - 1u }; }
	CIteratorType End() const { return const_cast< BasicAddress* >( this )->End(); }
	CIteratorType CEnd() const { return End(); }
	RIteratorType REnd() { return RIteratorType{ m_String.c_str() - 1u, m_String.c_str() - 1u }; }
	CRIteratorType REnd() const { return const_cast< BasicAddress* >( this )->REnd(); }
	CRIteratorType CREnd() const { return REnd(); }

private:

	StringType m_String;
};

using Address = BasicAddress< std::string, ""_H >;

namespace std
{
	template < typename _String, hash_t _DelimiterPack > static auto begin( BasicAddress< _String, _DelimiterPack >& a_Address ) { return a_Address.Begin(); }
	template < typename _String, hash_t _DelimiterPack > static auto begin( const BasicAddress< _String, _DelimiterPack >& a_Address ) { return a_Address.Begin(); }
	template < typename _String, hash_t _DelimiterPack > static auto cbegin( const BasicAddress< _String, _DelimiterPack >& a_Address ) { return a_Address.CBegin(); }
	template < typename _String, hash_t _DelimiterPack > static auto rbegin( BasicAddress< _String, _DelimiterPack >& a_Address ) { return a_Address.RBegin(); }
	template < typename _String, hash_t _DelimiterPack > static auto rbegin( const BasicAddress< _String, _DelimiterPack >& a_Address ) { return a_Address.RBegin(); }
	template < typename _String, hash_t _DelimiterPack > static auto crbegin( const BasicAddress< _String, _DelimiterPack >& a_Address ) { return a_Address.CRBegin(); }
	template < typename _String, hash_t _DelimiterPack > static auto end( BasicAddress< _String, _DelimiterPack >& a_Address ) { return a_Address.End(); }
	template < typename _String, hash_t _DelimiterPack > static auto end( const BasicAddress< _String, _DelimiterPack >& a_Address ) { return a_Address.End(); }
	template < typename _String, hash_t _DelimiterPack > static auto cend( const BasicAddress< _String, _DelimiterPack >& a_Address ) { return a_Address.CEnd(); }
	template < typename _String, hash_t _DelimiterPack > static auto rend( BasicAddress< _String, _DelimiterPack >& a_Address ) { return a_Address.REnd(); }
	template < typename _String, hash_t _DelimiterPack > static auto rend( const BasicAddress< _String, _DelimiterPack >& a_Address ) { return a_Address.REnd(); }
	template < typename _String, hash_t _DelimiterPack > static auto crend( const BasicAddress< _String, _DelimiterPack >& a_Address ) { return a_Address.CREnd(); }
}

template < typename _String, bool _Reverse, hash_t _DelimiterPack >
struct std::iterator_traits< BasicAddressIterator< _String, _Reverse, _DelimiterPack > >
{
	using iterator_category = std::bidirectional_iterator_tag;
	using difference_type = std::ptrdiff_t;
	using value_type = typename BasicAddressIterator< _String, _Reverse, _DelimiterPack >::ValueType;
	using reference = typename BasicAddressIterator< _String, _Reverse, _DelimiterPack >::ValueType;
	using pointer = typename BasicAddressIterator< _String, _Reverse, _DelimiterPack >::ValueType;
};