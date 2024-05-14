#pragma once
#include <Common/Log.hpp>
#include <Common/Config.hpp>

#if CONFIG_DEBUG
	#define CHECK(x) (::Check((x), __FUNCTION__, __FILE__, __LINE__))
	#define CHECK_LOG(x, ...) (::Check_Log((x), __FUNCTION__, __FILE__, __LINE__, __VA_ARGS__))
	void Check( bool a_Condition, const char* a_Function, const char* a_File, int a_Line );
	void Check_Log( bool a_Condition, const char* a_Function, const char* a_File, int a_Line, const char* a_FormatString, ... );
#else
	#define CHECK(x)
	#define CHECK_LOG(x, ...)
#endif

#if CONFIG_DEBUG || CONFIG_RELEASE
	#define ASSERT(x) (::Assert((x), __FUNCTION__, __FILE__, __LINE__))
	#define ASSERT_LOG(x, ...) (::Assert_Log((x), __FUNCTION__, __FILE__, __LINE__, __VA_ARGS__))
	bool Assert( bool a_Condition, const char* a_Function, const char* a_File, int a_Line );
	bool Assert_Log( bool a_Condition, const char* a_Function, const char* a_File, int a_Line, const char* a_FormatString, ...  );
#else
	#define ASSERT(x) (x)
	#define ASSERT_LOG(x, ...) (x)
#endif

#if CONFIG_DEBUG || CONFIG_RELEASE
	#define ENSURE(x) (::Ensure((x), __FUNCTION__, __FILE__, __LINE__))
	#define ENSURE_LOG(x, ...) (::Ensure_Log((x), __FUNCTION__, __FILE__, __LINE__, __VA_ARGS__))
	bool Ensure( bool a_Condition, const char* a_Function, const char* a_File, int a_Line );
	bool Ensure_Log( bool a_Condition, const char* a_Function, const char* a_File, int a_Line, const char* a_FormatString, ... );
#else
	#define ENSURE(x) (::Ensure((x)))
	#define ENSURE_LOG(x, ...) (::Ensure_Log((x)))
	bool Ensure( bool a_Condition );
	bool Ensure_Log( bool a_Condition );
#endif