#pragma once

#include "groups.h"
#include "macros.h"
#include "playerspawner.h"

#include "godot_cpp/classes/node3d.hpp"
#include "godot_cpp/classes/wrapped.hpp"

class PlayerSpawnPoint : public godot::Node3D {
	GDCLASS(PlayerSpawnPoint, godot::Node3D)

public:
	static void _bind_methods() {}

	void _enter_tree() override {
		add_to_group(group::playerspawn::name);
		if (in_game()) {
			if (auto* spawner = get_node<PlayerSpawner>(PlayerSpawner::get_path())) {
				spawner->register_spawn_point(get_path());
			}
		}
	}

	void _exit_tree() override {
		if (in_game()) {
			if (auto* spawner = get_node<PlayerSpawner>(PlayerSpawner::get_path())) {
				spawner->unregister_spawn_point(get_path());
			}
		}
	}
};
