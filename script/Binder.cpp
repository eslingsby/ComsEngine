#include "Binder.hpp"

#include "ScriptBind.hpp"

void Binder::bind(lua_State* L){
	// -
	luaL_newmetatable(L, "Script");
	luaL_setfuncs(L, ScriptBind::meta, 0);
}