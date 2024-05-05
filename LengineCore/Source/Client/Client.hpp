#pragma once

DECLARE_LOG_CATEGORY( Client );

class Client
{
	friend class Application;

public:

	Client( Application* a_Application )
		: m_Application( a_Application )
	{}

	Application* GetApplication() const { return m_Application; }

	void OnCreate() {}
	void OnDestroy() {}
	void OnUpdate( const float a_DeltaTime ) {}

private:


	Application* m_Application;
};