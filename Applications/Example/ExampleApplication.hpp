#pragma once
#include <Application/Application.hpp>

class ExampleApplication : public Application
{
public:

	void OnCreate() override;
	void OnDestroy() override;
	void OnUpdate( const float a_DeltaTime ) override;
};