#pragma once
#if IS_EDITOR
#include <Application/Commandlet.hpp>

DEFINE_COMMANDLET( CookCommandlet, "cook" )
{
public:

	DEFINE_COMMANDLET_BODY( CookCommandlet );

	void Run( const int32_t a_ArgC, const char** a_ArgV ) override;
};
#endif