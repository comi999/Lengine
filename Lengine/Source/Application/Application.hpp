#pragma once
#include <memory>
#include <string>
#include <IO/FileSystem/PlatformFileSystem.hpp>
#include <Utility/Singleton.hpp>

#define DEFINE_APPLICATION_MAIN( Name ) \
	int main( const int32_t a_ArgC, const char** a_ArgV ) \
	{ \
		Name::BindExisting( new Name( a_ArgC, a_ArgV ) ); \
		return Name::Get()->Run(); \
	}

#define DEFINE_APPLICATION( Name ) \
	class Name : public Application

#define DEFINE_APPLICATION_BODY( Name ) \
	Name( const int32_t a_ArgC, const char** a_ArgV ) \
		: Application( a_ArgC, a_ArgV ) \
	{}

class Application;
class Engine;
class Client;
#if IS_EDITOR
class Editor;
#endif // IS_EDITOR

using std::string;
using std::shared_ptr;

class Application : public ISingleton< Application >
{
private:

	friend struct ApplicationCommandLineParser;
	friend struct CommandletCommandLineParser;

public:

	Application( const int32_t a_ArgC, const char** a_ArgV, const char* a_Name = APPLICATION_NAME );

	virtual ~Application() override = default;

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

	int Run();
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

	Path m_ExecutablePath;
	Path m_EngineResourcesPath;
	Path m_ApplicationResourcesFolder;
	Path m_EngineConfigsFolder;
	Path m_ApplicationConfigsFolder;
	Path m_LogsFolder;

	int32_t m_ArgC;
	const char** m_ArgV;

#if IS_EDITOR
	bool m_IsCommandlet;
	string m_CommandletName;
#endif

public:

	static Application& GetApplication() { return *This; }
	static const Path& GetExecutablePath() { return This->m_ExecutablePath; }
	static const Path& GetEngineResourcesFolder() { return This->m_EngineResourcesPath; }
	static const Path& GetApplicationResourcesFolder() { return This->m_ApplicationResourcesFolder; }
	static const Path& GetEngineConfigsFolder() { return This->m_EngineConfigsFolder; }
	static const Path& GetApplicationConfigsFolder() { return This->m_ApplicationConfigsFolder; }
	static const Path& GetApplicationLogsFolder() { return This->m_LogsFolder; }

#if IS_EDITOR
	static bool IsCommandlet() { return This->m_IsCommandlet; }
#endif
};