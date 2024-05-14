#include <Application/Application.hpp>
#include <Application/CommandLine.hpp>
#include <Application/Commandlet.hpp>

#if IS_EDITOR
struct CommandletCommandLineParser : CommandLineParser
{
	CommandletCommandLineParser()
	{
		OnOption( "run", [ this ]( const CommandLineArgument< string >& a_Argument )
		{
			Application::Get()->m_IsCommandlet = true;

			// If this was a "-run=", then display all registered commandlets.
			if ( a_Argument.Value.empty() )
			{
				PrintAvailableCommandlets();
			}

			// If a commandlet was specified that exists, run it.
			else if ( Commandlet* Found = Commandlet::GetCommandlet( a_Argument.Value ) )
			{
				Application::Get()->m_CommandletName = Found->GetName();
				Found->Run( a_Argument.ArgC, a_Argument.ArgV );
				Abort();
			}

			// If commandlet not found, warn.
			else
			{
				PrintUnavailableCommandlet( a_Argument.Value );
			}
		} );
	}

	static void PrintAvailableCommandlets()
	{
		const auto& Lookup = Commandlet::GetLookup();

		Platform::Print( "Available commandlets: " );

		for ( auto& Commandlet : Lookup )
		{
			Platform::Print( "\n\t" );
			Platform::Print( Commandlet.first );
		}

		Platform::Print( "\n" );
	}

	static void PrintUnavailableCommandlet( const string_view a_Name )
	{
		Platform::Print( "Commandlet '" );
		Platform::Print( a_Name );
		Platform::Print( "' not found.\n" );
	}
};
#endif

ApplicationCommandLineParser::ApplicationCommandLineParser( const int32_t a_ArgC, const char** a_ArgV )
{
#if IS_EDITOR
	OnOption( "run", [ this ]( const CommandLineArgument<>& a_Argument )
	{
		// If "-run=commandlet", then pass this off to the Commandlet CommandLine parser to handle.
		static CommandletCommandLineParser Parser;
		Parser.Run( a_Argument.ArgC, a_Argument.ArgV );
		Abort();
	} );

	OnFlag( "help", [ this ]( const CommandLineArgument<>& a_Argument )
	{
		Abort();
		RunHelp();
	} );
#endif
	// ... and so on.

	Run( a_ArgC, a_ArgV );
}

void ApplicationCommandLineParser::RunHelp()
{
	Platform::Print( "Help page: \n" );
}