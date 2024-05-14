#include <Common/Platform.hpp>
#include <ctime>
#include <chrono>
#include <iostream>

DateTime Platform::GetDateTime()
{
	DateTime Result;
	time_t CTime;
	( void )time( &CTime );
	const tm* Time = localtime( &CTime );
	Result.Seconds = Time->tm_sec;
	Result.Minutes = Time->tm_min;
	Result.Hours = Time->tm_hour;
	Result.Day = Time->tm_mday - 1;
	Result.Month = Time->tm_mon;
	Result.Year = Time->tm_year - ( PLATFORM_TIME_START_YEAR - 1900 );
	return Result;
}

uint64_t Platform::GetEpochTime()
{
	return std::chrono::time_point_cast< std::chrono::nanoseconds >( 
		std::chrono::high_resolution_clock::now() 
	).time_since_epoch().count();
}

void DateTime::Format( char* a_Buffer, const size_t a_BufferLength, const char* a_FormatString, const DateTime a_DateTime )
{
	tm Input;
	Input.tm_sec = a_DateTime.Seconds;
	Input.tm_min = a_DateTime.Minutes;
	Input.tm_hour = a_DateTime.Hours;
	Input.tm_mday = a_DateTime.Day + 1;
	Input.tm_mon = a_DateTime.Month;
	Input.tm_year = a_DateTime.Year + ( PLATFORM_TIME_START_YEAR - 1900 );
	( void )strftime( a_Buffer, a_BufferLength * sizeof( char ), a_FormatString, &Input );
}

void Platform::Print( const string_view a_String )
{
	std::cout << a_String;
}