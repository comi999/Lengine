#pragma once
#include <Utility/Initializer.hpp>

DECLARE_LOG_CATEGORY( Engine );
DECLARE_INITIALIZER( EngineInitialization );

//enum class EEngineStage
//{
//	// Objects are static initialized.
//	// Nothing is set up at this point.
//	PreStart,
//#if IS_EDITOR
//	// Editor is starting up and loading states, scanning for content.
//	EditorInitializing,
//
//	// Editor is running.
//	EditorRunning,
//#endif
//	// Client is initializing.
//	ClientInitializing,
//
//	// Client is running.
//	ClientRunning,
//};

class Engine
{
	friend class Application;

public:

	Engine( Application* a_Application )
		: m_Application( a_Application )
	{}

private:

	Application* m_Application;

};