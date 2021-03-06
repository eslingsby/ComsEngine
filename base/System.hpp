#pragma once

#include "BitHelper.hpp"

#include <cassert>

class Engine;

// System class used to derive all system objects within the engine.
// Systems subscribe to listen to entities with certain types of components.
// Subscribed components of interest are expressed in the template definition,
// and the onProcess event function.
//
//		class Renderer : public System<Transform, Mesh, Material>{
//		public:
//			Renderer(Engine* engine);
//		
//			void update() override;
//		
//			void onProcess(uint64_t id, Transform* transform, Mesh* mesh, Material* material) override;
//		};
//
//		Renderer::Renderer(Engine* engine) : public System(manager){}
//
//		void Renderer::update(){
//			_engine.manager.processEntities(this);
//		}
//
//		void Renderer::onProcess(uint64_t id, Transform& transform, Mesh& mesh, Material& material){
//			transform.x += 10.f;
//			transform.y += 10.f;
//		}

class BaseSystem{
protected:
	// Static counter for labelling each system type
	static uint32_t _typeCounter;

	// Required pointer to entity manager
	Engine& _engine;

public:
	// Bitmask representing types of subscribed components
	const uint32_t mask;

	BaseSystem(Engine* const engine, uint32_t mask);
	virtual ~BaseSystem() = 0;

	// Called by entity manager once after all systems are added, before running
	virtual void load(){};

	// Called on state changes (also script reloads)
	virtual void reset(){};

	virtual void reload(){};

	// Called by entity manager every tick
	virtual void update(){};

	// Called on shutdown
	virtual void exit(){};

	// Entity events called by entity manager to monitor subscribed entities
	virtual void onCreate(uint64_t id){};
	virtual void onDestroy(uint64_t id){};
	virtual void onActivate(uint64_t id){};
	virtual void onDeactivate(uint64_t id){};
};

template <class ...Ts>
class System : public BaseSystem{
	System() = delete;

public:
	System(Engine* const engine);

	// Called by entity manager on request for every entity with subscribed components
	virtual void onProcess(uint64_t id, Ts&...){};
};

template<class ...Ts>
System<Ts...>::System(Engine* const engine) : BaseSystem(engine, BitHelper::createMask<Ts...>()){}