#if IS_EDITOR
#include <Application/Commandlet.hpp>

map< string, shared_ptr< Commandlet > >& Commandlet::GetLookup()
{
	static map< string, shared_ptr< Commandlet > > Lookup;
	return Lookup;
}
#endif