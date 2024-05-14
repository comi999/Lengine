#include <Application/CommandLine.hpp>

struct CommandletCommandLineParser : public CommandLineParser
{
	CommandletCommandLineParser()
	{
		OnOption( "run", []( const CommandLineArgument<>& arg)
		{
			std::cout << "Running commandlet: " << arg.Value << std::endl;
		} );

		OnPositional( 1u, []( const CommandLineArgument<>& arg )
		{
			std::cout << "Cooking folder: " << arg.Value << std::endl;
		} );
	}
};

ApplicationCommandLineParser::ApplicationCommandLineParser( const int32_t a_ArgC, const char** a_ArgV )
{
	OnOption( "run", []( const CommandLineArgument<>& a_Argument )
	{
		// If "-run=commandlet", then pass this off to the Commandlet CommandLine parser to handle.
		CommandletCommandLineParser Parser;
		Parser.Run( a_Argument.ArgC, a_Argument.ArgV );
	} );

	OnFlag( "help", [ this ]( const CommandLineArgument<>& a_Argument )
	{
		Abort();
		RunHelp();
	} );

	// ... and so on.

	Run( a_ArgC, a_ArgV );
}

void ApplicationCommandLineParser::RunHelp()
{
	std::cout << "Help page: " << std::endl;
}