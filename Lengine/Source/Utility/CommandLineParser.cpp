#include <Utility/CommandLineParser.hpp>

void CommandLineParser::Run( const int32_t a_ArgC, const char** a_ArgV )
{
	m_Parsing = true;

	for ( int32_t i = 0; i < a_ArgC && m_Parsing; ++i )
	{
		const CommandLineArgument Argument{ a_ArgC - i, a_ArgV + i };

		// Add argument to array.
		m_Tokens.push_back( Argument );

		// Call OnToken regardless of the state of the argument.
		m_OnToken.InvokeSafe( Argument );

		// If it was malformed, let's determine why and send off an error. Can be malformed argument, or bad cast.
		if ( Argument.Type == ECommandLineArgumentType::Malformed )
		{
			CommandLineError Error;
			Error.Index = i;
			Error.Argument = a_ArgV[ i ];
			Error.Reason = Argument.CastFail ? ECommandLineErrorType::InvalidCast : ECommandLineErrorType::MalformedArgument;
			m_OnError.InvokeSafe( Error );
			continue;
		}

		// If it was a positional argument, then we need to look for the stringicized version of the number.
		if ( Argument.Type == ECommandLineArgumentType::Positional )
		{
			const string Token = std::to_string( i );

			if ( auto Iter = m_Actions.find( Token ); Iter != m_Actions.end() )
			{
				const ECommandLineErrorType Result = Iter->second.Invoke( Argument );

				if ( Result != ECommandLineErrorType::None )
				{
					CommandLineError Error;
					Error.Index = i;
					Error.Argument = a_ArgV[ i ];
					Error.Reason = Result;
					m_OnError.InvokeSafe( Error );
				}
			}
			else
			{
				CommandLineError Error;
				Error.Index = i;
				Error.Argument = a_ArgV[ i ];
				Error.Reason = ECommandLineErrorType::ArgumentNotFound;
				m_OnError.InvokeSafe( Error );
			}

			continue;
		}

		// If it was a flag or option, then we should process it the same way.
		if ( auto Iter = m_Actions.find( string{ Argument.Token } ); Iter != m_Actions.end() )
		{
			const ECommandLineErrorType Result = Iter->second.Invoke( Argument );

			if ( Result != ECommandLineErrorType::None )
			{
				CommandLineError Error;
				Error.Index = i;
				Error.Argument = a_ArgV[ i ];
				Error.Reason = Result;
				m_OnError.InvokeSafe( Error );
			}
		}
		else
		{
			CommandLineError Error;
			Error.Index = i;
			Error.Argument = a_ArgV[ i ];
			Error.Reason = ECommandLineErrorType::ArgumentNotFound;
			m_OnError.InvokeSafe( Error );
		}
	}
}

CommandLineArgument<> CommandLineParser::operator[]( const string& a_Token ) const
{
	for ( const auto& Token : m_Tokens )
	{
		if ( Token.Token == a_Token )
		{
			return Token;
		}
	}

	return {};
}