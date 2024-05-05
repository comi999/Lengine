#pragma once
#if IS_LOGGING_ENABLED

#include <Utility/Manager.hpp>
#include <IO/IOSystem.hpp>
#include <IO/FileSystem/PlatformFileSystem.hpp>

class Logger final : public Manager< Logger >
{
public:
	
	Logger();
	Logger( Path a_OutputPath );
	~Logger() override;

	static void Log( const char* a_String );

private:

	void OnStartup()
	{

	}
	void OnShutdown()
	{

	}

	IOHandle m_Output;
};
#endif