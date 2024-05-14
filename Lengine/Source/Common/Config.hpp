#pragma once
#include <Common/Macro.hpp>

#include <Utility/Hash.hpp>
#include <Utility/Initializer.hpp>

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

#ifndef PATH_ENGINE_CONFIGS
	#define PATH_ENGINE_CONFIGS ""
#endif

#ifndef PATH_CONFIGS
	#define PATH_CONFIGS ""
#endif

#ifndef PATH_ENGINE_RESOURCES
	#define PATH_ENGINE_RESOURCES ""
#endif

#ifndef PATH_RESOURCES
	#define PATH_RESOURCES ""
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
	#if IS_EDITOR
		#define APPLICATION_NAME "Application Editor"
	#elif IS_CLIENT
		#define APPLICATION_NAME "Application"
	#endif
#endif

#ifndef ENGINE_NAME
	#define ENGINE_NAME "Lengine"
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
// Implementation specifiers.
//==========================================================================
#define BACKEND_NULL		0
#define BACKEND_OPENGL		1
#define BACKEND_VULKAN		2
#define BACKEND_CONSOLEGL	3
#define BACKEND_FMOD		4
#define BACKEND_PHYSX		5

#ifndef IMPLEMENTATION_RENDERING
	#define IMPLEMENTATION_RENDERING BACKEND_NULL
#endif

#ifndef IMPLEMENTATION_AUDIO
	#define IMPLEMENTATION_AUDIO BACKEND_NULL
#endif

#ifndef IMPLEMENTATION_PHYSICS
	#define IMPLEMENTATION_PHYSICS BACKEND_NULL
#endif

#define USING_RENDERING_NULL		(IMPLEMENTATION_RENDERING == BACKEND_NULL)
#define USING_RENDERING_OPENGL		(IMPLEMENTATION_RENDERING == BACKEND_OPENGL)
#define USING_RENDERING_VULKAN		(IMPLEMENTATION_RENDERING == BACKEND_VULKAN)
#define USING_RENDERING_CONSOLEGL	(IMPLEMENTATION_RENDERING == BACKEND_CONSOLEGL)
#define USING_AUDIO_NULL			(IMPLEMENTATION_AUDIO == BACKEND_NULL)
#define USING_AUDIO_FMOD			(IMPLEMENTATION_AUDIO == BACKEND_FMOD)
#define USING_PHYSICS_NULL			(IMPLEMENTATION_PHYSICS == BACKEND_NULL)
#define USING_PHYSICS_PHYSX			(IMPLEMENTATION_PHYSICS == BACKEND_PHYSX)

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

#define DEFINE_CONFIG( Name, Type, Default ) \
	namespace Configs { \
	DEFINE_INITIALIZER_IN_PLACE( ConfigInitialization_Name ) \
	{ \
		CHECK( ConfigValue::AsValueType< Type > != EConfigValueType::Unsupported, "Value type must be an integral, decimal, or string type." ); \
		GetDefaults()[ #Name ] = { #Name, Default }; \
	}; \
	}

#include <map>
using std::map;
#include <string>
using std::string;
#include <variant>
using std::variant;
#include <bitset>
using std::bitset;

enum class EConfigValueType
{
	Unsupported,
	Bool,
	Int,
	Uint,
	Decimal,
	String
};

struct ConfigValue
{
	using VariantType = variant<
		std::monostate,
		bool,
		int64_t,
		uint64_t,
		double,
		string
	>;
	
	template < typename T >
	static constexpr EConfigValueType AsValueType = EConfigValueType::Unsupported;

#define CONFIG_VALUE_GET( Type ) \
	const auto& GetAs##Type() const { return std::get< ( size_t )EConfigValueType::Type >( m_Value ); } \
	auto& GetAs##Type() { return std::get< ( size_t )EConfigValueType::Type >( m_Value ); }

	CONFIG_VALUE_GET( Bool );
	CONFIG_VALUE_GET( Int );
	CONFIG_VALUE_GET( Uint );
	CONFIG_VALUE_GET( Decimal );
	CONFIG_VALUE_GET( String );

#undef CONFIG_VALUE_GET

	ConfigValue() = default;
	ConfigValue( const ConfigValue& ) = default;
	ConfigValue( ConfigValue&& ) = default;
	ConfigValue& operator=( const ConfigValue& ) = default;
	ConfigValue& operator=( ConfigValue&& ) = default;

	template < typename _Value >
	ConfigValue( const string& a_Name, _Value&& a_Value )
		: m_Name( a_Name )
		, m_Value( std::forward< _Value >( a_Value ) )
	{}

	EConfigValueType GetType() const { return ( EConfigValueType )m_Value.index(); }
	const string& GetName() const { return m_Name; }
	bool HasValue() const { return GetType() != EConfigValueType::Unsupported; }

private:
	
	string m_Name;
	VariantType m_Value;
};

template <> constexpr EConfigValueType ConfigValue::AsValueType< uint8_t  > = EConfigValueType::Uint;
template <> constexpr EConfigValueType ConfigValue::AsValueType< uint16_t > = EConfigValueType::Uint;
template <> constexpr EConfigValueType ConfigValue::AsValueType< uint32_t > = EConfigValueType::Uint;
template <> constexpr EConfigValueType ConfigValue::AsValueType< uint64_t > = EConfigValueType::Uint;
template <> constexpr EConfigValueType ConfigValue::AsValueType< int8_t   > = EConfigValueType::Int;
template <> constexpr EConfigValueType ConfigValue::AsValueType< int16_t  > = EConfigValueType::Int;
template <> constexpr EConfigValueType ConfigValue::AsValueType< int32_t  > = EConfigValueType::Int;
template <> constexpr EConfigValueType ConfigValue::AsValueType< int64_t  > = EConfigValueType::Int;
template <> constexpr EConfigValueType ConfigValue::AsValueType< float    > = EConfigValueType::Decimal;
template <> constexpr EConfigValueType ConfigValue::AsValueType< double   > = EConfigValueType::Decimal;
template <> constexpr EConfigValueType ConfigValue::AsValueType< bool     > = EConfigValueType::Bool;
template <> constexpr EConfigValueType ConfigValue::AsValueType< string   > = EConfigValueType::String;

namespace Configs
{
	map< string, ConfigValue >& GetDefaults();
}

class Config
{
public:

	static void Load() {}
};