// https://github.com/vinniefalco/LuaBridge
// Copyright 2020, Dmitry Tarakanov
// Copyright 2012, Vinnie Falco <vinnie.falco@gmail.com>
// Copyright 2007, Nathan Reed
// SPDX-License-Identifier: MIT

#pragma once

// All #include dependencies are listed here
// instead of in the individual header files.
//

#define LUABRIDGE_MAJOR_VERSION 2
#define LUABRIDGE_MINOR_VERSION 7
#define LUABRIDGE_VERSION 207

#ifndef LUA_VERSION_NUM
#error "Lua headers must be included prior to LuaBridge ones"
#endif

#include <luabridge/detail/CFunctions.h>
#include <luabridge/detail/ClassInfo.h>
#include <luabridge/detail/Constructor.h>
#include <luabridge/detail/FuncTraits.h>
#include <luabridge/detail/Iterator.h>
#include <luabridge/detail/LuaException.h>
#include <luabridge/detail/LuaHelpers.h>
#include <luabridge/detail/LuaRef.h>
#include <luabridge/detail/Namespace.h>
#include <luabridge/detail/Security.h>
#include <luabridge/detail/Stack.h>
#include <luabridge/detail/TypeList.h>
#include <luabridge/detail/TypeTraits.h>
#include <luabridge/detail/Userdata.h>
