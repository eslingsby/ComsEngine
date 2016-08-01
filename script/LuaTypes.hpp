#pragma once

#include <lualib.h>

#include <glm\vec3.hpp>
#include <glm\gtc\quaternion.hpp>

typedef glm::tquat<lua_Number, glm::highp> LuaQuat;
typedef glm::tvec4<lua_Number, glm::highp> LuaVec4;
typedef glm::tvec3<lua_Number, glm::highp> LuaVec3;
typedef glm::tvec2<lua_Number, glm::highp> LuaVec2;