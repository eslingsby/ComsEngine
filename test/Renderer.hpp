#pragma once

#include "System.hpp"

#include "Transform.hpp"
#include "Mesh.hpp"
#include "Material.hpp"

class Renderer : public System<Transform, Mesh, Material>{

public:
	Renderer(EntityManager* manager);

	void load() override;
	void update() override;

	void onProcess(uint64_t id, Transform* transform, Mesh* mesh, Material* material) override;

	void onCreate(uint64_t id) override;
	void onDestroy(uint64_t id) override;

	void onActivate(uint64_t id) override;
	void onDeactivate(uint64_t id) override;
};