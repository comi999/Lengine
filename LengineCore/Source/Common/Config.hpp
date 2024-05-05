#pragma once
#include <Common/Macro.hpp>

//==========================================================================
// Output specifiers.
//==========================================================================

// Is the output of this project a shared library? ie. .dll, .so
#ifndef IS_SHARED_LIB
	#define IS_SHARED_LIB 0
#endif

// Is the output of this project a static library? ie. .lib, .a
#ifndef IS_STATIC_LIB
	#define IS_STATIC_LIB 0
#endif

// Is the output of this project an executable? ie. .exe
#ifndef IS_EXE
	#define IS_EXE 0
#endif

#ifndef PATH_LOGS
	#define PATH_LOGS ""
#endif

#ifndef PATH_CONFIGS
	#define PATH_CONFIGS ""
#endif

#ifndef PATH_RESOURCES
	#define PATH_RESOURCES ""
#endif

#ifndef PATH_PRIMITIVE_RESOURCES
	#define PATH_PRIMITIVE_RESOURCES PATH_RESOURCES ## "/Primitive"
#endif

//==========================================================================
// Runtime specifiers.
//==========================================================================

// Is this code that will be executing on a game instance?
#ifndef IS_CLIENT
	#define IS_CLIENT 0
#endif

// Is code that will be executing for the AppleSeed editor?
#ifndef IS_EDITOR
	#define IS_EDITOR 0
#endif

#ifndef ENVIRONMENT_NAME
	#if IS_EDITOR
		#define ENVIRONMENT_NAME "Editor"
	#elif IS_CLIENT
		#define ENVIRONMENT_NAME "Client"
	#endif
#endif

#ifndef APPLICATION_NAME
	#define APPLICATION_NAME "Application"
#endif

//==========================================================================
// Feature specifiers.
//==========================================================================

#if defined(LOGGING_LEVEL) && LOGGING_LEVEL > 0
	#define IS_LOGGING_ENABLED 1
#elif defined(ENABLE_LOGGING)
	#define IS_LOGGING_ENABLED 1
	#ifndef LOGGING_LEVEL
		#define LOGGING_LEVEL 1
	#endif
#else
	#define IS_LOGGING_ENABLED 0
	#ifndef LOGGING_LEVEL
		#define LOGGING_LEVEL 0
	#endif
#endif

#if LOGGING_LEVEL > 2
	#define LOGGING_LEVEL_CRITICAL 1
	#define LOGGING_LEVEL_WARNING 0
	#define LOGGING_LEVEL_INFO 0
#elif LOGGING_LEVEL > 1
	#define LOGGING_LEVEL_CRITICAL 1
	#define LOGGING_LEVEL_WARNING 1
	#define LOGGING_LEVEL_INFO 0
#elif LOGGING_LEVEL > 0
	#define LOGGING_LEVEL_CRITICAL 1
	#define LOGGING_LEVEL_WARNING 1
	#define LOGGING_LEVEL_INFO 1
#else
	#define LOGGING_LEVEL_CRITICAL 0
	#define LOGGING_LEVEL_WARNING 0
	#define LOGGING_LEVEL_INFO 0
#endif

#ifndef BREAK_ON_ASSERT_FAIL
	#define BREAK_ON_ASSERT_FAIL 0
#endif

#ifndef ENABLE_MESSAGES
	#define ENABLE_MESSAGES 0
#endif

//==========================================================================
// Configuration specifiers.
//==========================================================================

#ifndef CONFIG_DEBUG
	#define CONFIG_DEBUG 0
#endif

#ifndef CONFIG_RELEASE
	#define CONFIG_RELEASE 0
#endif

#define CONFIG_TOKENS_DEBUG D,B,G
#define CONFIG_TOKENS_RELEASE R,L,S
#define CONFIG_TAG_DEBUG COMBINE(WRAP(STR,COMBINE(CONFIG_TOKENS_DEBUG)))
#define CONFIG_TAG_RELEASE COMBINE(WRAP(STR,COMBINE(CONFIG_TOKENS_RELEASE)))
#define CONFIG_NAME_DEBUG "DEBUG"
#define CONFIG_NAME_RELEASE "RELEASE"
#define CONFIG_ID_DEBUG 0
#define CONFIG_ID_RELEASE 1

#if CONFIG_DEBUG
	#define CONFIG_TOKENS CONFIG_TOKENS_DEBUG
	#define CONFIG_TAG CONFIG_TAG_DEBUG
	#define CONFIG_NAME CONFIG_NAME_DEBUG
	#define CONFIG_ID CONFIG_ID_DEBUG
#elif CONFIG_RELEASE
	#define CONFIG_TOKENS CONFIG_TOKENS_RELEASE
	#define CONFIG_NAME CONFIG_NAME_RELEASE
	#define CONFIG_TAG CONFIG_TAG_RELEASE
	#define CONFIG_ID CONFIG_ID_RELEASE
#endif

//==========================================================================
// Platform specifiers.
//==========================================================================

// Is the platform x86_64?
#ifndef PLATFORM_WIN64
	#define PLATFORM_WIN64 0
#endif

// Is the platform ARM64?
#ifndef PLATFORM_ARM64
	#define PLATFORM_ARM64 0
#endif

// Is the platform Windows?
#if PLATFORM_WIN64
	#define IS_WINDOWS 1
#else
	#define IS_WINDOWS 0
#endif

// Is the platform Android?
#if PLATFORM_ARM64
	#define IS_ANDROID 1
#else
	#define IS_ANDROID 0
#endif

// Is the platform 32 bit?
#define IS_32BIT 0

// Is the platform 64 bit?
#define IS_64BIT 1

//==========================================================================
// Build specifiers.
//==========================================================================

#define VERSION_DELIMITER .

#define VERSION_MAJOR 0
#define VERSION_MINOR 0
#define VERSION_PATCH 1
#define VERSION_STRING WRAP(STR, VERSION_MAJOR) WRAP(STR, VERSION_DELIMITER) WRAP(STR, VERSION_MINOR) WRAP(STR, VERSION_DELIMITER) WRAP(STR, VERSION_PATCH)

#define VERSION_MIN_MAJOR 0
#define VERSION_MIN_MINOR 0
#define VERSION_MIN_PATCH 1
#define VERSION_MIN_STRING WRAP(STR, VERSION_MIN_MAJOR) WRAP(STR, VERSION_DELIMITER) WRAP(STR, VERSION_MIN_MINOR) WRAP(STR, VERSION_DELIMITER) WRAP(STR, VERSION_MIN_PATCH)

//==========================================================================
// Platform decorators.
//==========================================================================
#if IS_ANDROID
	#define __cdecl
	#define __stdcall
#endif //IS_ANDROID

//==========================================================================
// Output decorators.
//==========================================================================
#if IS_SHARED_LIB
	#ifdef __GNUC__
		#define LENGINE_API extern "C" __attribute__((dllexport))
	#else
		#define LENGINE_API extern "C" __declspec(dllexport)
	#endif
#else
	#define LENGINE_API
#endif

//==========================================================================
// Function decorators.
//==========================================================================
#define NO_INLINE __declspec(noinline)

#if CONFIG_DEBUG
	#define FORCE_INLINE
#else
	#define FORCE_INLINE __forceinline
#endif