#include <Core/Core.hpp>

Delegate<>              CoreDelegates::OnStartup;
Delegate< void, float > CoreDelegates::OnUpdate;
Delegate<>              CoreDelegates::OnRestart;
Delegate<>              CoreDelegates::OnShutdown;

DEFINE_INITIALIZER( CoreInitialisation )
{
    // Initialize core here.
};