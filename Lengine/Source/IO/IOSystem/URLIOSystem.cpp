#include <IO/IOSystem/URLIOSystem.hpp>

DEFINE_INITIALIZER( URLIOSystemRegistration )
{
	if ( ActionType == EInitializerActionType::Startup )
	{
		IOSystem::AddSystem( new URLIOSystem );
	}
	else if ( ActionType == EInitializerActionType::Teardown )
	{
		IOSystem::RemoveSystem( URLIOSystem::RootTag );
	}
};