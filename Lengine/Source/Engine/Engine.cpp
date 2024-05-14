#include <Engine/Engine.hpp>

DEFINE_INITIALIZER( EngineInitialization )
{
	if ( ActionType == EInitializerActionType::Startup )
	{
		LOG( Engine, Info, "Engine starting." );
	}
	else if ( ActionType == EInitializerActionType::Teardown )
	{
		LOG( Engine, Info, "Engine stopping." );
	}
};