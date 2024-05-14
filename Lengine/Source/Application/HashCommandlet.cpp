#if IS_EDITOR
#include <Application/HashCommandlet.hpp>
#include <Utility/CommandLineParser.hpp>

TODO( "Move iostream or logging info out into its on platform operation. Something like Platform::Print()" );
#include <iostream>

struct HashCommandLineParser : CommandLineParser
{
	HashCommandLineParser()
	{
		OnPositional( 1u, [ this ]( const CommandLineArgument<>& a_Argument )
		{
			const hash_t Hash = Hashing::Hash( a_Argument.Value );
			std::cout << Hash << std::endl;
			Abort();
		} );
	}
};

void HashCommandlet::Run( const int32_t a_ArgC, const char** a_ArgV )
{
	static HashCommandLineParser Parser;
	Parser.Run( a_ArgC, a_ArgV );
}
#endif