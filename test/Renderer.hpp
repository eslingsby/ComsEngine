#pragma once

#include "System.hpp"

#include "Transform.hpp"
#include "Mesh.hpp"
#include "Material.hpp"

class Renderer : public System<Transform, Mesh, Material>{

public:
	Renderer(EntityManager* manager);

	void onLoad() override;
	void onUpdate() override;

	void onProcess(uint64_t id, Transform* transform, Mesh* mesh, Material* material) override;
};