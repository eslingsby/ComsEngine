#pragma once

#include "System.hpp"

#include "Transform.hpp"
#include "Audio.hpp"

class Sound : public System<Transform, Audio>{

public:
	Sound(EntityManager* manager);

	void onLoad() override;
	void onUpdate() override;

	void onProcess(uint64_t id, Transform* transform, Audio* audio) override;
};