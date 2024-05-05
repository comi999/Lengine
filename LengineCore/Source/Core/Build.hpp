#pragma once
#include <string>

DECLARE_LOG_CATEGORY( BuildVersion );

enum class EBuildConfiguration : uint8_t
{
	Debug = CONFIG_ID_DEBUG,
	Release = CONFIG_ID_RELEASE,
	Invalid,
	Current = CONFIG_ID
};

struct BuildVersion
{
	version_t Major;
	version_t Minor;
	version_t Patch;

	constexpr BuildVersion()
		: Major( VERSION_MAJOR )
		, Minor( VERSION_MINOR )
		, Patch( VERSION_PATCH )
	{}
	constexpr BuildVersion( const version_t a_Major, const version_t a_Minor, const version_t a_Patch )
		: Major( a_Major )
		, Minor( a_Minor )
		, Patch( a_Patch )
	{}
	constexpr BuildVersion( const BuildVersion& a_BuildVersion ) = default;
	constexpr BuildVersion& operator=( const BuildVersion& a_BuildVersion ) = default;
	
	static constexpr version_t MinMajor = VERSION_MIN_MAJOR;
	static constexpr version_t MinMinor = VERSION_MIN_MINOR;
	static constexpr version_t MinPatch = VERSION_MIN_PATCH;
	static constexpr BuildVersion GetMinSupported();
	static constexpr BuildVersion GetCurrent();

	// SPrints version numbers into a format string. Returns characters written.
	// %M - Major version.
	// %m - Minor version.
	// %p - Patch version.
	static size_t Format( char* a_Buffer, size_t a_BufferSize, const char* a_FormatString = "%M.%m.%p", BuildVersion a_Version = GetCurrent() );

	// Will parse a version string and populate a BuildVersion.
	// Version string must be "%i.%i.%i" where . represents the version delimiter.
	static bool Parse( const char* a_String, BuildVersion* o_BuildVersion );
	static constexpr BuildVersion Parse( const char* a_String );

	bool operator==( const BuildVersion a_BuildVersion ) const { return Major == a_BuildVersion.Major && Minor == a_BuildVersion.Minor && Patch == a_BuildVersion.Patch; }
	bool operator!=( const BuildVersion a_BuildVersion ) const { return Major != a_BuildVersion.Major || Minor != a_BuildVersion.Minor || Patch != a_BuildVersion.Patch; }
	bool operator< ( const BuildVersion a_BuildVersion ) const { return Major < a_BuildVersion.Major || Minor < a_BuildVersion.Minor || Patch < a_BuildVersion.Patch; }
	bool operator> ( const BuildVersion a_BuildVersion ) const { return Major < a_BuildVersion.Major || Minor < a_BuildVersion.Minor || Patch < a_BuildVersion.Patch; }
	bool operator<=( const BuildVersion a_BuildVersion ) const { return *this == a_BuildVersion || *this < a_BuildVersion; }
	bool operator>=( const BuildVersion a_BuildVersion ) const { return *this == a_BuildVersion || *this > a_BuildVersion; }
};

constexpr BuildVersion BuildVersion::GetMinSupported() { return { VERSION_MIN_MAJOR, VERSION_MIN_MINOR, VERSION_MIN_PATCH }; }
constexpr BuildVersion BuildVersion::GetCurrent() { return {}; }

constexpr BuildVersion BuildVersion::Parse( const char* a_String )
{
	version_t Versions[ 3 ] = { 0, 0, 0 };

	uint32_t Start = 0u;
	uint32_t End = 0u;
	uint32_t Index = 0u;

	auto ProcessToken = []( const uint32_t a_Start, uint32_t a_End, const char* a_VersionString ) -> version_t
	{
		uint32_t Multiplier = 1u;
		version_t Token = 0u;

		while ( a_Start != a_End )
		{
			Token += ( a_VersionString[ --a_End ] - '0' ) * Multiplier;
			Multiplier *= 10;
		}

		return Token;
	};

	while ( true )
	{
		if ( a_String[ End ] == WRAP(CHR,VERSION_DELIMITER) || a_String[ End ] == '\0' )
		{
			Versions[ Index++ ] = ProcessToken( Start, End, a_String );
			Start = End + 1;

			if ( a_String[ End ] == '\0' )
			{
				break;
			}
		}
		
		++End;
	}
	return { Versions[ 0 ], Versions[ 1 ], Versions[ 2 ] };
}

static constexpr BuildVersion operator ""_BV( const char* a_VersionString, const size_t )
{
	return BuildVersion::Parse( a_VersionString );
}