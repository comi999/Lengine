#pragma once
#include <Utility/Delegate.hpp>
#include <Utility/Initializer.hpp>

//==========================================================================
// List of delegates for various points in the lifetime of the application.
//==========================================================================
struct CoreDelegates
{
	static Delegate<> OnStartup;
	static Delegate< void, float > OnUpdate;
	static Delegate<> OnRestart;
	static Delegate<> OnShutdown;
};

//==========================================================================
DECLARE_INITIALIZER( CoreInitialisation );

//==========================================================================
// Perform all the initial setup, the very first thing that must happen before
// calling any subsequent FGN functions.
//==========================================================================
void Initialise();

//==========================================================================
// Reloads all game data. Used to reload mods when the player requests
// a different set of active mods to play with.
//==========================================================================
void Reload();

//==========================================================================
// Perform the final shutdown, the very last thing that must happen before
// the app closes down.
//==========================================================================
void ShutDown( bool a_IsEndOfProcess );