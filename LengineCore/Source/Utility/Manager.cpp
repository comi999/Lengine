#include <Utility/Manager.hpp>

Delegate<> ManagerCore::OnStartup;
Delegate< void, float > ManagerCore::OnUpdate;
Delegate<> ManagerCore::OnRestart;
Delegate<> ManagerCore::OnShutdown;
Delegate<> ManagerCore::Destructors;