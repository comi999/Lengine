#if IS_LOGGING_ENABLED
#include <Application/Application.hpp>
#include <Logging/Logger.hpp>
#include <Common/Platform.hpp>
#include <IO/FileSystem/PlatformFileSystem.hpp>

Logger::Logger()
	: Logger( Application::GetApplicationLogsFolder() )
{}

Logger::Logger( Path a_OutputPath )
{
	const DateTime Time = Platform::GetDateTime();
	char FileName[ PLATFORM_MAX_PATH_LENGTH ];

	const int32_t FileNameOffset = sprintf_s( FileName, PLATFORM_MAX_PATH_LENGTH, APPLICATION_NAME );
	DateTime::Format( FileName + FileNameOffset, PLATFORM_MAX_PATH_LENGTH - FileNameOffset, "-%H.%M %F.log", Time );
	PlatformFileSystem::CreateDirectories( a_OutputPath );
	a_OutputPath /= FileName;
	m_Output = PlatformFileSystem::Open( a_OutputPath, EIOOpenMode::Write, false );
}

Logger::~Logger()
{
	if ( m_Output )
	{
		PlatformFileSystem::Close( m_Output );
	}
}

void Logger::Log( const char* a_String )
{
	if ( This->m_Output )
	{
		PlatformFileSystem::Write( This->m_Output, a_String, strlen( a_String ) );
	}
}
#endif