#pragma once
#include <Engine/EngineModule.hpp>

TODO( "Got to change this to the DEFINE_ENGINE_MODULE syntax when that's done for auto loading on engine start.")
class RenderingManager : public EngineModule
{
public:

	// Will need things like Submit, Execute, etc.
	// For submitting draw calls, constructing a render graph, and executing that render graph.

	// For stage 1, can just make a very crude inefficient system to just execute FIFO.
};