#pragma once
#include <Application/Application.hpp>

DEFINE_APPLICATION( ExampleApplication )
{
public:

	DEFINE_APPLICATION_BODY( ExampleApplication );

	void OnCreate() override;
	void OnDestroy() override;
	void OnUpdate( const float a_DeltaTime ) override;
};