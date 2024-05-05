#pragma once

#include <Common/Config.hpp>
#include <Common/Macro.hpp>

enum class ELogLevel { None, Info, Warning, Critical };

#define LOG_MAX_LENGTH 1024

#define LOG( Category, Level, ... ) ::Log_##Level( &LogCategory::Category[ 0 ], __VA_ARGS__ );

#if ENABLE_MESSAGES
	#define MESSAGE( x ) __pragma( message( x ) )
#else
	#define MESSAGE( x )
#endif

#define LOG_OUTPUT_LINK __FILE__ "(" WRAP( STR, __LINE__ ) ")"
#define TODO( x ) MESSAGE( "TODO: "x"\n\t\t" LOG_OUTPUT_LINK )

#if ENABLE_MESSAGES
	#define DEPRECATED( Replacement ) [[deprecated( "This is deprecated. Please use " Replacement " instead.\n\t\t" LOG_OUTPUT_LINK )]]
#else
	#define DEPRECATED( Replacement )
#endif

#define NOT_IMPLEMENTED LOG( Log, Warning, "Invoking function '" __FUNCTION__ "' @\n\t\t" LOG_OUTPUT_LINK )

#define DECLARE_LOG_CATEGORY( Category ) namespace LogCategory { static constexpr auto Category = #Category; }

DECLARE_LOG_CATEGORY( Log );

void Log( const char* a_LogCategory, ELogLevel a_LogLevel, const char* a_Formatting, ... );
void Log( const char* a_String );

#define DECLARE_LOG_FUNCTION( Level ) \
	void Log_##Level( const char* a_LogCategory, const char* a_Formatting, ... )

DECLARE_LOG_FUNCTION( Info );
DECLARE_LOG_FUNCTION( Warning );
DECLARE_LOG_FUNCTION( Critical );

#undef DECLARE_LOG_FUNCTION