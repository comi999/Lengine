#pragma once
#include <charconv>
#include <vector>
#include <list>
#include <span>
#include <map>
#include <string>
#include <variant>
#include <optional>
#include <Utility/Invoker.hpp>

using std::pair;
using std::vector;
using std::list;
using std::span;
using std::map;
using std::variant;
using std::optional;
using std::string;
using std::string_view;

enum class ECommandLineArgumentType
{
	Malformed,
	Positional,
	Option,
	Flag,
};

template < typename _Value >
struct CommandLineConverterDefault
{
	using ValueType = _Value;

	bool operator()( const string_view a_Input, _Value& o_Value ) const
	{
		// If this value can be assigned from a string_view, ie a string_view or string...
		if constexpr ( std::is_constructible_v< _Value, string_view > )
		{
			o_Value = _Value{ a_Input };
			return true;
		}

		// If this value is a boolean, attempt to convert it.
		if constexpr ( std::is_same_v< _Value, bool > )
		{
			// Attempt to convert a string version of boolean.
			if ( a_Input.find_first_of( "tTfFyYnN" ) == 0u )
			{
				string Temp{ a_Input };

				for ( auto& Char : Temp )
				{
					if ( !std::isalpha( Char ) )
					{
						return false;
					}

					Char = std::tolower( Char );
				}

				bool IsValid = false;

					 if ( Temp == "true"	) o_Value = true , IsValid = true;
				else if ( Temp == "t"		) o_Value = true , IsValid = true;
				else if ( Temp == "y"		) o_Value = true , IsValid = true;
				else if ( Temp == "yes"		) o_Value = true , IsValid = true;
				else if ( Temp == "false"	) o_Value = false, IsValid = true;
				else if ( Temp == "f"		) o_Value = false, IsValid = true;
				else if ( Temp == "n"		) o_Value = false, IsValid = true;
				else if ( Temp == "no"		) o_Value = false, IsValid = true;

				return IsValid;
			}
			else
			{
				double Temp = 0.0f;

				if ( std::from_chars( a_Input.data(), a_Input.data() + a_Input.size(), Temp ).ec == std::errc{} )
				{
					o_Value = ( bool )Temp;
					return true;
				}

				return false;
			}
		}
		
		else if constexpr ( std::is_arithmetic_v< _Value > )
		{
			return std::from_chars( a_Input.data(), a_Input.data() + a_Input.size(), o_Value ).ec == std::errc{};
		}

		return false;
	}
};

template < typename _Value = string_view, typename _Converter = CommandLineConverterDefault< _Value > >
struct CommandLineArgument
{
public:

	using ValueType = _Value;
	using ConverterType = _Converter;

	CommandLineArgument() = default;

	CommandLineArgument( const int32_t a_ArgC, const char** a_ArgV )
		: ArgC( a_ArgC )
		, ArgV{ a_ArgV }
		, Token{ a_ArgC > 0u ? a_ArgV[ 0u ] : nullptr }
		, Value{}
		, Type( ECommandLineArgumentType::Malformed )
		, CastFail( false )
	{
		if ( a_ArgC == 0u )
		{
			return;
		}

		string_view Temp = a_ArgV[ 0u ];

		// If this is an empty argument, continue.
		if ( Temp.empty() )
		{
			return;
		}

		// Check to see if this is either a flag or option
		if ( Temp.front() == '-' )
		{
			Temp = Temp.substr( 1u );

			// Check to see if it was a "-" by itself, "--" or "-"".
			if ( Temp.empty() || Temp.front() == '-' || Temp.front() == '"' )
			{
				return;
			}

			// Next we want to see if there's any assignment operators. That
			// will make this an option.
			if ( const size_t Offset = Temp.find( '=' ); Offset != string_view::npos )
			{
				string_view Option = Temp.substr( Offset + 1u );
				Temp = Temp.substr( 0u, Offset );

				// If the value was empty (eg -x=), then we have nothing else to do, so we can add and continue.
				if ( Option.empty() )
				{

				}

				// We should now check to see if the value begins with a quotation mark.
				else if ( Option.front() == '"' )
				{
					// Consume the starting quotation.
					Option = Option.substr( 1u );

					// We should consume any quotation marks on the back as well.
					if ( !Option.empty() && Option.back() == '"' )
					{
						Option = Option.substr( 0u, Option.size() - 1u );
					}
				}

				// If there are no beginning quotations, yet there is still an equality somewhere in the value, this is malformed.
				else if ( Option.find( '=' ) != string_view::npos )
				{
					return;
				}

				if ( ConverterType{}( Option, Value ) )
				{
					Type = ECommandLineArgumentType::Option;
					Token = Temp;
				}
				else
				{
					CastFail = true;
				}
			}

			// If we get to this part, this must be a flag.
			else
			{
				Type = ECommandLineArgumentType::Flag;
				Token = Temp;
			}

			return;
		}

		// If we are here (didn't begin with flag/option operator '-'), then this is a positional arg.

		// Consume any quotations.
		if ( Temp.front() == '"' )
		{
			Temp = Temp.substr( 1u );

			if ( !Temp.empty() && Temp.back() == '"' )
			{
				Temp = Temp.substr( 0u, Temp.size() - 1u );
			}
		}

		if ( ConverterType{}( Temp, Value ) )
		{
			Type = ECommandLineArgumentType::Positional;
			Token = Temp;
		}
		else
		{
			CastFail = true;
		}
	}

	template < typename _Other >
	CommandLineArgument( const CommandLineArgument< _Other >& a_Other )
		: ArgC( a_Other.ArgC )
		, ArgV( a_Other.ArgV )
		, Token( a_Other.Token )
		, Value{}
		, Type( a_Other.Type )
		, CastFail( a_Other.CastFail )
	{
		if ( !ConverterType{}( a_Other.Value, Value ) )
		{
			Type = ECommandLineArgumentType::Malformed;
			CastFail = true;
		}
	}

	template < typename _Other >
	CommandLineArgument& operator=( const CommandLineArgument< _Other >& a_Other )
	{
		ArgC = a_Other.ArgC;
		ArgV = a_Other.ArgV;
		Token = a_Other.Token;
		Value = {};
		Type = a_Other.Type;
		CastFail = a_Other.CastFail;

		if ( !ConverterType{}( a_Other.Value, Value ) )
		{
			Type = ECommandLineArgumentType::Malformed;
			CastFail = true;
		}

		return *this;
	}

	template < typename _Other >
	operator optional< _Other >() const
	{
		CommandLineArgument< _Other > Result = this;
		return Result.CastFail ? std::make_optional< _Other >() : std::make_optional< _Other >( std::move( Result.Value ) );
	}

	template < typename _Other >
	operator _Other() const { CommandLineArgument< _Other > Result = this; return Result.CastFail ? _Other{} : Result.Value; }

 	int32_t						ArgC = 0u;
	const char**				ArgV = nullptr;
	string_view					Token;
	ValueType					Value;
	ECommandLineArgumentType	Type;
	bool						CastFail;
};

enum class ECommandLineErrorType
{
	None,
	MalformedArgument,
	ArgumentNotFound,
	PositionalExpected,
	FlagExpected,
	OptionExpected,
	InvalidCast,
};

struct CommandLineError
{
	ECommandLineErrorType Reason;
	string_view Argument;
	int32_t Index;
};

class CommandLineParser
{
	template < typename T >
	static constexpr bool IsValidType = false;

	template < typename T >
	static constexpr bool IsCommandLineArgument = false;

	template < typename T >
	static constexpr bool IsCommandLineArgument< CommandLineArgument< T > > = true;

	// Checks to see if _Action is void(CommandLineArgument<T>&).
	template < typename _Action > static constexpr bool IsValidAction =
		std::function_arity_v< std::decay_t< _Action > > == 1u &&
		!std::is_rvalue_reference_v< std::function_argument_t< 0u, std::decay_t< _Action > > > &&
		IsCommandLineArgument< std::function_argument_t< 0u, std::decay_t< _Action > > >;

public:

	// Will invoke action on an action on an optional argument.
	// Function type must be void(const CommandLineArgument<T>&).
	template < typename _Action >
	void OnOption( const string& a_Option, _Action&& a_Action )
	{
		using ActionType = std::decay_t< _Action >;
		using ArgumentType = std::decay_t< std::function_argument_t< 0u, ActionType > >;

		m_Actions[ a_Option ] = [ Action = std::forward< _Action >( a_Action ) ]( const CommandLineArgument<>& a_Argument ) -> ECommandLineErrorType
		{
			const ArgumentType Argument( a_Argument );

			if ( Argument.CastFail )
			{
				return ECommandLineErrorType::InvalidCast;
			}

			if ( Argument.Type != ECommandLineArgumentType::Option )
			{
				return ECommandLineErrorType::OptionExpected;
			}

			Action( Argument );
			return ECommandLineErrorType::None;
		};
	}
	
	// Will invoke action on a flag argument.
	// Function type must be void(const CommandLineArgument<T>&).
	template < typename _Action >
	void OnFlag( const string& a_Flag, _Action&& a_Action )
	{
		using ActionType = std::decay_t< _Action >;
		using ArgumentType = std::decay_t< std::function_argument_t< 0u, ActionType > >;

		m_Actions[ a_Flag ] = [ Action = std::forward< _Action >( a_Action ) ]( const CommandLineArgument<>& a_Argument ) -> ECommandLineErrorType
		{
			const ArgumentType Argument{ a_Argument };

			if ( Argument.CastFail )
			{
				return ECommandLineErrorType::InvalidCast;
			}

			if ( Argument.Type != ECommandLineArgumentType::Flag )
			{
				return ECommandLineErrorType::FlagExpected;
			}

			Action( Argument );
			return ECommandLineErrorType::None;
		};
	}
	
	// Will invoke action on a positional argument.
	// Function type must be void(const CommandLineArgument<T>&).
	template < typename _Action >
	void OnPositional( const size_t a_Position, _Action&& a_Action )
	{
		using ActionType = std::decay_t< _Action >;
		using ArgumentType = std::decay_t< std::function_argument_t< 0u, ActionType > >;

		m_Actions[ std::to_string( a_Position ) ] = [ Action = std::forward< _Action >( a_Action ) ]( const CommandLineArgument<>& a_Argument ) -> ECommandLineErrorType
		{
			const ArgumentType Argument{ a_Argument };

			if ( Argument.CastFail )
			{
				return ECommandLineErrorType::InvalidCast;
			}

			if ( Argument.Type != ECommandLineArgumentType::Positional )
			{
				return ECommandLineErrorType::PositionalExpected;
			}

			Action( Argument );
			return ECommandLineErrorType::None;
		};
	}

	// Fires when an error occurs.
	// Function type must be void(const CommandLineError&).
	template < typename _Action >
	void OnError( _Action&& a_Action )
	{
		m_OnError = std::forward< _Action >( a_Action );
	}

	// This will be fired on all tokens.
	// Function type must be void(const CommandLineArgument<string_view>&).
	template < typename _Action >
	void OnToken( _Action&& a_Action )
	{
		m_OnToken = std::forward< _Action >( a_Action );
	}

	// Run this parser on a set of tokenized commandline arguments.
	// Flag:
	//    -x
	// Option:
	//    -x=y
	// Positional:
	//    x or "x y"
	void Run( const int32_t a_ArgC, const char** a_ArgV );

	// Abort the parser if it is running.
	void Abort() { m_Parsing = false; }

	// Clear all parsed tokens and optionally all actions as well.
	void Clear( const bool a_ClearActions = false )
	{
		if ( a_ClearActions )
		{
			m_Actions.clear();
			m_OnToken.Unbind();
			m_OnError.Unbind();
		}

		m_Tokens.clear();
		m_Parsing = false;
	}

	CommandLineArgument<> operator[]( const size_t a_Index ) const { return a_Index < m_Tokens.size() ? m_Tokens[ a_Index ] : CommandLineArgument{}; }

	CommandLineArgument<> operator[]( const string& a_Token ) const;

private:

	map< string, Invoker< ECommandLineErrorType, const CommandLineArgument<>& > > m_Actions;
	vector< CommandLineArgument<> > m_Tokens;
	Action< const CommandLineArgument<>& > m_OnToken;
	Action< const CommandLineError& > m_OnError;
	bool m_Parsing = false;
};

template <> constexpr bool CommandLineParser::IsValidType< uint8_t > = true;
template <> constexpr bool CommandLineParser::IsValidType< uint16_t > = true;
template <> constexpr bool CommandLineParser::IsValidType< uint32_t > = true;
template <> constexpr bool CommandLineParser::IsValidType< uint64_t > = true;
template <> constexpr bool CommandLineParser::IsValidType< int8_t > = true;
template <> constexpr bool CommandLineParser::IsValidType< int16_t > = true;
template <> constexpr bool CommandLineParser::IsValidType< int32_t > = true;
template <> constexpr bool CommandLineParser::IsValidType< int64_t > = true;
template <> constexpr bool CommandLineParser::IsValidType< float > = true;
template <> constexpr bool CommandLineParser::IsValidType< double > = true;
template <> constexpr bool CommandLineParser::IsValidType< bool > = true;
template <> constexpr bool CommandLineParser::IsValidType< string_view > = true;
template <> constexpr bool CommandLineParser::IsValidType< string > = true;