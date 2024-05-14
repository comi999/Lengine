#pragma once
#include <cstdint>
#include <string>

#define PLATFORM_TIME_START_YEAR 2024
#define PLATFORM_MAX_PATH_LENGTH 256

using std::string_view;

struct DateTime
{
	uint32_t Seconds : 6; // 0 - 59
	uint32_t Minutes : 6; // 0 - 59
	uint32_t Hours	 : 5; // 0 - 23
	uint32_t Day	 : 5; // 0 - 30
	uint32_t Month	 : 4; // 0 - 11
	uint32_t Year	 : 6; // 0 - 63
	
	static void Format( char* a_Buffer, size_t a_BufferLength, const char* a_FormatString, DateTime a_DateTime );
};

class Platform final
{
public:

	Platform() = delete;
	static DateTime GetDateTime();
	static uint64_t GetEpochTime();
	static void Print( const string_view a_String );

	TODO( "Can we add a Console::WriteLine sort of thing here?" );
};