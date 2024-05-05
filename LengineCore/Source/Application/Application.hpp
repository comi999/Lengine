#pragma once
#include <memory>
#include <string>
#include <Utility/Initializer.hpp>

class Application;
class Engine;
class Client;
#if IS_EDITOR
class Editor;
#endif // IS_EDITOR

using std::string;
using std::shared_ptr;

class ApplicationModule
{
public:

	ApplicationModule( Application* m_Application );

};

class Application
{
public:

	Application( const char* a_Name = APPLICATION_NAME
#if IS_EDITOR
		" Editor"
#endif // IS_EDITOR
	);

	virtual ~Application() = default;

	const string& GetName() const { return m_Name; }
	Engine* GetEngine() const { return m_Engine.get(); }
	Client* GetClient() const { return m_Client.get(); }
#if IS_EDITOR
	Editor* GetEditor() const { return m_Editor.get(); }
#endif

protected:

	virtual void OnCreate() {}
	virtual void OnDestroy() {}
	virtual void OnUpdate( const float a_DeltaTime ) {}

public:

	void Run();
	void Quit() { m_QuitRequested = true; }
	bool IsQuitting() const { return m_QuitRequested; }

private:

	bool m_QuitRequested;
	string m_Name;
	shared_ptr< Engine > m_Engine;
	shared_ptr< Client > m_Client;
#if IS_EDITOR
	shared_ptr< Editor > m_Editor;
#endif // IS_EDITOR
};