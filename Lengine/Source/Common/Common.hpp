#pragma once

#include <Common/Config.hpp>
#include <Common/Log.hpp>
#include <Common/Assert.hpp>
#include <Common/Macro.hpp>
#include <Common/Platform.hpp>

//==========================================================================
// Common typedefs.
//==========================================================================
typedef uint8_t byte_t;
typedef uint8_t version_t;

//==========================================================================
// Disable Windows macros.
//==========================================================================
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX