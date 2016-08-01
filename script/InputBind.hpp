#pragma once

#include "Binder.hpp"

#include "Engine.hpp"
#include "Input.hpp"

namespace InputBind{
	const char* name = "Input";

	inline static int _addInput(lua_State* L);

	inline static int _wasDown(lua_State* L);

	inline static int _isDown(lua_State* L);

	inline static int _mouseWasDown(lua_State* L);
	inline static int _mouseIsDown(lua_State* L);

	inline static int _mousePos(lua_State* L);
	inline static int _mouseRelativePos(lua_State* L);

	inline static int _lockMouse(lua_State* L);

	inline static int _scrollAmount(lua_State* L);

	static const luaL_Reg global[] = {
		{ "addInput", _addInput },
		{ "wasDown", _wasDown },
		{ "isDown", _isDown },
		{ "mouseWasDown", _mouseWasDown },
		{ "mouseIsDown", _mouseIsDown },
		{ "mousePos", _mousePos },
		{ "mouseRelativePos", _mouseRelativePos },
		{ "lockMouse", _lockMouse },
		{ "scrollAmount", _scrollAmount },
		{ 0, 0 }
	};
}

int InputBind::_addInput(lua_State* L){
	Input* input = Binder::getSystem<Input>(L);

	std::string name = luaL_checkstring(L, 1);

	int x = (int)luaL_checkinteger(L, 2);

	SDL_Scancode key = (SDL_Scancode)luaL_checkinteger(L, 2);

	input->addInput(name, key);

	return 0;
}

int InputBind::_wasDown(lua_State* L){
	Input* input = Binder::getSystem<Input>(L);

	lua_pushboolean(L, (int)input->wasDown(luaL_checkstring(L, 1)));

	return 1;
}

int InputBind::_isDown(lua_State* L){
	Input* input = Binder::getSystem<Input>(L);

	lua_pushboolean(L, (int)input->isDown(luaL_checkstring(L, 1)));

	return 1;
}

int InputBind::_mouseWasDown(lua_State * L){
	Input* input = Binder::getSystem<Input>(L);

	return 0;
}

int InputBind::_mouseIsDown(lua_State * L){
	Input* input = Binder::getSystem<Input>(L);

	return 0;
}

int InputBind::_mousePos(lua_State * L){
	Input* input = Binder::getSystem<Input>(L);

	LuaVec2* vec = (LuaVec2*)lua_newuserdata(L, sizeof(LuaVec2));
	*vec = input->mousePos();

	luaL_getmetatable(L, "Vec2");
	lua_setmetatable(L, -2);

	return 1;
}

int InputBind::_mouseRelativePos(lua_State * L){
	Input* input = Binder::getSystem<Input>(L);

	LuaVec2* vec = (LuaVec2*)lua_newuserdata(L, sizeof(LuaVec2));
	*vec = input->mouseRelativePos();

	luaL_getmetatable(L, "Vec2");
	lua_setmetatable(L, -2);

	return 1;
}

int InputBind::_lockMouse(lua_State * L){
	Input* input = Binder::getSystem<Input>(L);

	if (lua_gettop(L) >= 1)
		input->lockMouse((bool)lua_toboolean(L, 1));
	else
		input->lockMouse();

	return 0;
}

int InputBind::_scrollAmount(lua_State * L){
	Input* input = Binder::getSystem<Input>(L);

	return 0;
}
