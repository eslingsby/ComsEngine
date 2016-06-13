#pragma once

#include "System.hpp"

#include "Transform.hpp"
#include "Audio.hpp"

class Sound : public System<Transform, Audio>{

public:
	Sound(EntityManager* manager);

	void load() override;
	void update() override;

	void onProcess(uint64_t id, Transform* transform, Audio* audio) override;

	void onCreate(uint64_t id) override;
	void onDestroy(uint64_t id) override;

	void onActivate(uint64_t id) override;
	void onDeactivate(uint64_t id) override;
};