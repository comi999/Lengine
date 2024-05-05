#include "Name.hpp"

using std::to_string;
const Name Name::Null = ""_N;

string ToString( const Name& a_Object, const char* a_Style )
{
#ifdef ENABLE_NAME_STORAGE
    return a_Object.CStr();
#else
    return std::to_string( a_Object.Hash() );
#endif
}