#include <Application/Application.hpp>
#include <Application/CommandLine.hpp>
#include <Application/Commandlet.hpp>

#if IS_EDITOR
struct CommandletCommandLineParser : public CommandLineParser
{
	CommandletCommandLineParser()
	{
		OnOption( "run", [ this ]( const CommandLineArgument< string >& arg)
		{
			if ( Commandlet* Found = Commandlet::GetCommandlet( arg.Value ) )
			{
				Application::Get()->m_IsCommandlet = true;
				Application::Get()->m_CommandletName = Found->GetName();
				Found->Run( arg.ArgC, arg.ArgV );
				Abort();
			}
		} );

		OnPositional( 1u, []( const CommandLineArgument<>& arg )
		{
			std::cout << "Cooking folder: " << arg.Value << std::endl;
		} );
	}
};
#endif

ApplicationCommandLineParser::ApplicationCommandLineParser( const int32_t a_ArgC, const char** a_ArgV )
{
#if IS_EDITOR
	OnOption( "run", [ this ]( const CommandLineArgument<>& a_Argument )
	{
		// If "-run=commandlet", then pass this off to the Commandlet CommandLine parser to handle.
		CommandletCommandLineParser Parser;
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
	std::cout << "Help page: " << std::endl;
}