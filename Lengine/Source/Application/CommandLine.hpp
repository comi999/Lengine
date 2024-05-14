#pragma once
#include <iostream>
#include <Utility/CommandLineParser.hpp>
#include <Utility/Singleton.hpp>

struct ApplicationCommandLineParser : private ISingleton< ApplicationCommandLineParser >, protected CommandLineParser
{
	friend class Application;
	friend class ISingleton;

private:
	
	ApplicationCommandLineParser( const int32_t a_ArgC, const char** a_ArgV );

	static void RunHelp();

public:

	template < typename T >
	static T GetArgument( const size_t a_Index ) { return This->operator[]( a_Index ); }

	template < typename T >
	static bool TryGetArgument( const size_t a_Index, T& o_Value )
	{
		CommandLineArgument< T > Argument = This->operator[]( a_Index );

		if ( Argument.CastFail )
		{
			return false;
		}

		o_Value = std::move( Argument.Value );
		return true;
	}

	// Commandlet info.
	bool IsCommandlet = false;
	string_view CommandletName;

	// Path info.
	bool LogsFolderOverride = false;
	string_view LogsFolder;

	bool EngineConfigsOverride = false;
	string_view EngineConfigsPath;

	bool ConfigsOverride = false;
	string_view ConfigsPath;

	bool EngineResourcesOverride = false;
	string_view EngineResourcesFolder;

	bool ResourcesOverride = false;
	string_view ResourcesFolder;
};

struct CommandLine final : ApplicationCommandLineParser { CommandLine() = delete; };