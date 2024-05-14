#pragma once
#if IS_EDITOR
#include <Application/Commandlet.hpp>

DEFINE_COMMANDLET( HashCommandlet, "hash" )
{
public:

	DEFINE_COMMANDLET_BODY( HashCommandlet );

	void Run( const int32_t a_ArgC, const char** a_ArgV ) override;
};
#endif