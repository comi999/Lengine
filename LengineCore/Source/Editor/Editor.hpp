#pragma once
#if IS_EDITOR
#include <memory>

DECLARE_LOG_CATEGORY( Editor );

using std::shared_ptr;

struct EditorInfo;
class Engine;
class Client;

class Editor
{
	friend class Application;

public:

	Editor( Application* a_Application )
		: m_Application( a_Application )
	{}

	void OnCreate();
	void OnDestroy();
	void OnUpdate( const float a_DeltaTime );

	Application* GetApplication() const { return m_Application; }
	Engine* GetEngine() const;
	Client* GetClient() const;

private:

	shared_ptr< EditorInfo > m_Info;
	Application* m_Application;
};
#endif // IS_EDITOR