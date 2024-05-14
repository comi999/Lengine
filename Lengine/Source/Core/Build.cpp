#include <Core/Build.hpp>

size_t BuildVersion::Format( char* a_Buffer, const size_t a_BufferSize, const char* a_FormatString, const BuildVersion a_Version )
{
	bool ExpectingSymbol = false;
	const char* End = a_Buffer + a_BufferSize;

	while ( *a_FormatString && a_Buffer != End )
	{
		if ( ExpectingSymbol )
		{
			switch ( *a_FormatString )
			{
			case 'M':
			{
				a_Buffer += sprintf( a_Buffer, "%i", ( int32_t )a_Version.Major );
				break;
			}
			case 'm':
			{
				a_Buffer += sprintf( a_Buffer, "%i", ( int32_t )a_Version.Minor );
				break;
			}
			case 'p':
			{
				a_Buffer += sprintf( a_Buffer, "%i", ( int32_t )a_Version.Patch );
				break;
			}
			default:
			{
				LOG( BuildVersion, Warning, R"(Encountered unsupported format symbol in BuildVersion::Format. Please use %M, %m, %p.)" );
				break;
			}
			}

			ExpectingSymbol = false;
		}
		else if ( *a_FormatString == '%' )
		{
			ExpectingSymbol = true;
		}
		else
		{
			*a_Buffer = *a_FormatString;
			++a_Buffer;
		}
		
		++a_FormatString;
	}

	const size_t CharactersWritten = a_BufferSize - ( size_t )( End - a_Buffer );

	if ( CharactersWritten != a_BufferSize )
	{
		*a_Buffer = '\0';
	}

	return CharactersWritten;
}

bool BuildVersion::Parse( const char* a_String, BuildVersion* o_BuildVersion )
{
	if ( !a_String || !o_BuildVersion )
	{
		return false;
	}

	int32_t Versions[ 3 ];
	const int32_t Filled = sscanf( a_String, "%u" WRAP(STR, VERSION_DELIMITER) "%u" WRAP(STR, VERSION_DELIMITER) "%u", &Versions[ 0 ], &Versions[ 1 ], &Versions[ 2 ] );

	if ( Filled == 0u )
	{
		return false;
	}

	for ( uint32_t i = Filled; i < 3; ++i )
	{
		Versions[ i ] = 0u;
	}

	o_BuildVersion->Major = Versions[ 0 ];
	o_BuildVersion->Minor = Versions[ 1 ];
	o_BuildVersion->Patch = Versions[ 2 ];
	return true;
}