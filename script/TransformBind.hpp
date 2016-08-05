#pragma once

#include "Binder.hpp"

#include "Engine.hpp"
#include "Transform.hpp"
#include "LuaTypes.hpp"
#include "Hierarchy.hpp"

namespace TransformBind{
	const char* name = "Transform";

	inline int constructor(lua_State* L);

	inline int _add(lua_State* L);

	inline static int _invalidate(lua_State* L);

	inline int _position(lua_State* L);
	inline int _rotation(lua_State* L);
	inline int _scale(lua_State* L);

	inline int _translate(lua_State* L);
	inline int _localTranslate(lua_State* L);
	inline int _rotate(lua_State* L);
	inline int _localRotate(lua_State* L);

	inline int _lookAt(lua_State* L);

	//inline int _globalPosition(lua_State* L);
	//inline int _globalRotation(lua_State* L);
	//inline int _globalScale(lua_State* L);
	
	//inline int _parent(lua_State* L);

	inline int _x(lua_State* L, void* value);
	inline int _y(lua_State* L, void* value);
	inline int _z(lua_State* L, void* value);

	static const luaL_Reg global[] = {
		{ "add", _add },
		{ 0, 0 }
	};

	static const luaL_Reg meta[] = {
		{ "__gc", _invalidate },
		{ 0, 0 }
	};

	static const luaL_Reg methods[] = {
		{ "position", _position },
		{ "rotation", _rotation },
		{ "scale", _scale },

		{ "translate", _translate },
		{ "localTranslate", _localTranslate },
		{ "rotate", _rotate },
		{ "localRotate", _localRotate },

		{ "lookAt", _lookAt },

		//{ "globalPosition", _globalPosition },
		//{ "globalRotation", _globalRotation },
		//{ "globalScale", _globalScale },
		
		//{ "parent", _parent },
		{ 0, 0 }
	};

	static const Binder::MemberReg getters[] = {
		{ "x", _x, 0, true },
		{ "y", _y, 0, true },
		{ "z", _z, 0, true },
		{ 0, 0 }
	};

	static const Binder::MemberReg setters[] = {
		{ "x", _x, 0, true },
		{ "y", _y, 0, true },
		{ "z", _z, 0, true },
		{ 0, 0 }
	};
}

inline int TransformBind::constructor(lua_State* L){
	// {} integer

	Binder::createEntityRef(L, luaL_checkinteger(L, -1), name);

	return 1;
}

inline int TransformBind::_add(lua_State* L){
	uint64_t id = luaL_checkinteger(L, 1);
	
	Engine& engine = Binder::getEngine(L);

	if (lua_gettop(L) > 1)
		engine.manager.addComponent<Transform>(id, luaL_checkinteger(L, 2));
	else
		engine.manager.addComponent<Transform>(id);

	return 0;
}

int TransformBind::_invalidate(lua_State * L){
	EntityRef* entity = (EntityRef*)luaL_checkudata(L, 1, name);

	entity->invalidate();
	return 0;
}

inline int TransformBind::_position(lua_State* L){
	EntityRef* entity = (EntityRef*)luaL_checkudata(L, 1, name);
	Transform* transform = entity->getComponent<Transform>();

	if (lua_gettop(L) > 1){
		transform->position = *(LuaVec3*)luaL_checkudata(L, 2, "Vec3");
		return 0;
	}

	LuaVec3* vec = (LuaVec3*)lua_newuserdata(L, sizeof(LuaVec3));
	*vec = transform->position;

	luaL_getmetatable(L, "Vec3");
	lua_setmetatable(L, -2);

	return 1;
}

inline int TransformBind::_rotation(lua_State * L){
	EntityRef* entity = (EntityRef*)luaL_checkudata(L, 1, name);
	Transform* transform = entity->getComponent<Transform>();

	if (lua_gettop(L) > 1){
		transform->rotation = *(LuaQuat*)luaL_checkudata(L, 2, "Quat");
		return 0;
	}

	LuaQuat* quat = (LuaQuat*)lua_newuserdata(L, sizeof(LuaQuat));
	*quat = transform->rotation;

	luaL_getmetatable(L, "Quat");
	lua_setmetatable(L, -2);

	return 1;
}

inline int TransformBind::_scale(lua_State * L){
	EntityRef* entity = (EntityRef*)luaL_checkudata(L, 1, name);
	Transform* transform = entity->getComponent<Transform>();

	if (lua_gettop(L) > 1){
		transform->scale = *(LuaVec3*)luaL_checkudata(L, 2, "Vec3");
		return 0;
	}

	LuaVec3* vec = (LuaVec3*)lua_newuserdata(L, sizeof(LuaVec3));
	*vec = transform->position;

	luaL_getmetatable(L, "Vec3");
	lua_setmetatable(L, -2);

	return 1;
}

int TransformBind::_translate(lua_State* L){
	Hierarchy* hierarchy = Binder::getSystem<Hierarchy>(L);
	EntityRef* entity = (EntityRef*)luaL_checkudata(L, 1, name);

	LuaVec3* vec = (LuaVec3*)luaL_checkudata(L, 2, "Vec3");

	hierarchy->translate(entity->id(), *vec);

	return 0;
}

int TransformBind::_localTranslate(lua_State * L){
	Hierarchy* hierarchy = Binder::getSystem<Hierarchy>(L);
	EntityRef* entity = (EntityRef*)luaL_checkudata(L, 1, name);

	LuaVec3* vec = (LuaVec3*)luaL_checkudata(L, 2, "Vec3");

	hierarchy->localTranslate(entity->id(), *vec);

	return 0;
}

int TransformBind::_rotate(lua_State * L){
	Hierarchy* hierarchy = Binder::getSystem<Hierarchy>(L);
	EntityRef* entity = (EntityRef*)luaL_checkudata(L, 1, name);

	LuaQuat* quat = (LuaQuat*)luaL_checkudata(L, 2, "Quat");

	hierarchy->rotate(entity->id(), *quat);

	return 0;
}

int TransformBind::_localRotate(lua_State * L){
	Hierarchy* hierarchy = Binder::getSystem<Hierarchy>(L);
	EntityRef* entity = (EntityRef*)luaL_checkudata(L, 1, name);

	LuaQuat* quat = (LuaQuat*)luaL_checkudata(L, 2, "Quat");

	hierarchy->localRotate(entity->id(), *quat);

	return 0;
}

int TransformBind::_lookAt(lua_State* L){
	Hierarchy* hierarchy = Binder::getSystem<Hierarchy>(L);
	EntityRef* entity = (EntityRef*)luaL_checkudata(L, 1, name);

	LuaVec3* target = (LuaVec3*)luaL_checkudata(L, 2, "Vec3");
	LuaVec3* up = (LuaVec3*)luaL_checkudata(L, 3, "Vec3");
	
	hierarchy->lookAt(entity->id(), *target, *up);

	return 0;
}

//int TransformBind::_globalPosition(lua_State * L){
//	Hierarchy* hierarchy = Binder::getSystem<Hierarchy>(L);
//
//	return 0;
//}

//int TransformBind::_globalRotation(lua_State * L){
//	Hierarchy* hierarchy = Binder::getSystem<Hierarchy>(L);
//
//	return 0;
//}

//int TransformBind::_globalScale(lua_State * L){
//	Hierarchy* hierarchy = Binder::getSystem<Hierarchy>(L);
//
//	return 0;
//}

//int TransformBind::_parent(lua_State * L){
//	Hierarchy* hierarchy = Binder::getSystem<Hierarchy>(L);
//
//	return 0;
//}

inline int TransformBind::_x(lua_State * L, void * value){
	EntityRef* entity = (EntityRef*)luaL_checkudata(L, 1, name);

	if (lua_gettop(L) > 2){
		entity->getComponent<Transform>()->position.x = (float)luaL_checknumber(L, 3);
		return 0;
	}

	lua_pushnumber(L, entity->getComponent<Transform>()->position.x);
	return 1;
}

inline int TransformBind::_y(lua_State * L, void * value){
	EntityRef* entity = (EntityRef*)luaL_checkudata(L, 1, name);

	if (lua_gettop(L) > 2){
		entity->getComponent<Transform>()->position.y = (float)luaL_checknumber(L, 3);
		return 0;
	}

	lua_pushnumber(L, entity->getComponent<Transform>()->position.y);
	return 1;
}

inline int TransformBind::_z(lua_State * L, void * value){
	EntityRef* entity = (EntityRef*)luaL_checkudata(L, 1, name);

	if (lua_gettop(L) > 2){
		entity->getComponent<Transform>()->position.z = (float)luaL_checknumber(L, 3);
		return 0;
	}

	lua_pushnumber(L, entity->getComponent<Transform>()->position.z);
	return 1;
}
