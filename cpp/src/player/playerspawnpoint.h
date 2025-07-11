#pragma once

#include "groups.h"
#include "input/input_manager.h"
#include "input/input_state.h"
#include "macros.h"
#include "mathstatics.h"
#include "playerspawner.h"

#include "godot_cpp/classes/camera3d.hpp"
#include "godot_cpp/classes/node3d.hpp"
#include "godot_cpp/classes/wrapped.hpp"

class PlayerSpawnPoint : public godot::Node3D {
	GDCLASS(PlayerSpawnPoint, godot::Node3D)

public:
	static void _bind_methods() {}

	void _enter_tree() override {
		add_to_group(group::playerspawn::name);
		GAME_SCOPE {
			if (auto* spawner = get_node<PlayerSpawner>(PlayerSpawner::get_path())) {
				spawner->register_spawn_point(get_path());
			}
		}
	}

	void _exit_tree() override {
		GAME_SCOPE {
			if (auto* spawner = get_node<PlayerSpawner>(PlayerSpawner::get_path())) {
				spawner->unregister_spawn_point(get_path());
			}
		}
	}
};

/**
 * For Editor usage.
 * Override spawnpoints by placing a temporary spawnpoint instead.
 * Only supports a single temporary spawnpoint, unlike regular spawnpoints.
 */
class TemporaryPlayerSpawnPoint : public godot::Node3D {
	GDCLASS(TemporaryPlayerSpawnPoint, godot::Node3D)

public:
	static void _bind_methods() {}

	void _ready() override {
		set_notify_transform(true);
		// set_notify_local_transform(true);
	}

	void _enter_tree() override {
		add_to_group(group::playerspawn::name);
		GAME_SCOPE {
			if (auto* spawner = get_node<PlayerSpawner>(PlayerSpawner::get_path())) {
				spawner->register_temporary_spawn_point(get_global_position());
			}

			if (auto* im = get_node<InputManager>(InputManager::get_path())) {
				im->register_input_callback(get_path(), [this](const InputState& i) { this->input_callback(i); });
			}
		}
	}

	void _exit_tree() override {
		GAME_SCOPE {
			if (auto* spawner = get_node<PlayerSpawner>(PlayerSpawner::get_path())) {
				spawner->unregister_temporary_spawn_point();
			}
		}
	}

	void _notification(int what) {
		if (what == godot::Node3D::NOTIFICATION_TRANSFORM_CHANGED) {
			GAME_SCOPE {
				if (auto* spawner = get_node<PlayerSpawner>(PlayerSpawner::get_path())) {
					spawner->register_temporary_spawn_point(get_global_position());
				}
			}
		}
	}

private:
	void input_callback(const InputState& i) {
		if (i.tmp_spawnpoint.just_pressed()) {
			position_on_xy_plane_intersection();
		}
	}

	void position_on_xy_plane_intersection() {
		const auto mouse_pos = get_viewport()->get_mouse_position();

		auto* camera3d = get_viewport()->get_camera_3d();
		const auto projection_position = camera3d->project_ray_origin(mouse_pos);
		const auto projection_normal = camera3d->project_ray_normal(mouse_pos);

		auto intersection = mathstatics::vector_zero;
		if (mathstatics::xy_plane.intersects_ray(projection_position, projection_normal, &intersection)) {
			set_global_position(intersection);
		}
	}
};
