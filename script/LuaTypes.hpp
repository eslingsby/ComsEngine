#pragma once

#include <lualib.h>

#include <glm\vec3.hpp>
#include <glm\gtc\quaternion.hpp>

typedef glm::tquat<lua_Number, glm::highp> LuaQuat;
typedef glm::tvec3<lua_Number, glm::highp> LuaVec3;