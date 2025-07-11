#pragma once

#include "groups.h"
#include "input/input_manager.h"
#include "input/input_state.h"
#include "macros.h"
#include "mathstatics.h"
#include "playerspawner.h"

#include "godot_cpp/classes/button.hpp"
#include "godot_cpp/classes/camera3d.hpp"
#include "godot_cpp/classes/control.hpp"
#include "godot_cpp/classes/node3d.hpp"
#include "godot_cpp/classes/wrapped.hpp"
#include "godot_cpp/variant/callable_method_pointer.hpp"

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

/**
 * For Editor usage.
 * Override spawnpoints by placing a temporary spawnpoint instead.
 * Only supports a single temporary spawnpoint, unlike regular spawnpoints.
 */
class TemporaryPlayerSpawnPoint : public godot::Node3D {
	GDCLASS(TemporaryPlayerSpawnPoint, godot::Node3D)

	PROPERTY_CUSTOM_SET(
		bool, active,
		{
			m_active = value;
			if (is_node_ready() && in_game()) {
				update_register_to_player_spawner();
				set_tmpspawnpoint_visibility();
			}
		},
		false);

public:
	static void _bind_methods() {
		BIND_PROPERTY_METHODS(TemporaryPlayerSpawnPoint, active, BOOL);
	}

	void _ready() override {
		set_notify_transform(true);
	}

	void _enter_tree() override {
		add_to_group(group::playerspawn::name);
		if (in_game()) {
			update_register_to_player_spawner();
			set_tmpspawnpoint_visibility();

			if (auto* im = get_node<InputManager>(InputManager::get_path())) {
				im->register_input_callback(get_path(), [this](const InputState& i) { this->input_callback(i); });
			}

			if (auto* gui_button = get_node<godot::Button>("Control/Button")) {
				gui_button->connect("pressed", callable_mp(this, &TemporaryPlayerSpawnPoint::on_gui_button_pressed));
			}
		}
	}

	void _exit_tree() override {
		if (in_game()) {
			if (auto* spawner = get_node<PlayerSpawner>(PlayerSpawner::get_path())) {
				spawner->unregister_temporary_spawn_point();
			}

			if (auto* gui_button = get_node<godot::Button>("Control/Button")) {
				gui_button->disconnect("pressed", callable_mp(this, &TemporaryPlayerSpawnPoint::on_gui_button_pressed));
			}
		}
	}

	void _notification(int what) {
		if (what == godot::Node3D::NOTIFICATION_TRANSFORM_CHANGED) {
			if (in_game()) {
				update_register_to_player_spawner();
			}
		}
	}

private:
	void update_register_to_player_spawner() {
		if (auto* playerspawner = get_node<PlayerSpawner>(PlayerSpawner::get_path())) {
			if (m_active) {
				playerspawner->register_temporary_spawn_point(get_global_position());
			}
			else {
				playerspawner->unregister_temporary_spawn_point();
			}
		}
	}

	void input_callback(const InputState& i) {
		if (i.tmp_spawnpoint.just_pressed()) {
			position_on_xy_plane_intersection();
			set_active(true);
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

	void on_gui_button_pressed() {
		set_active(false);
	}

	void set_tmpspawnpoint_visibility() {
		set_visible(m_active);
		if (auto* control = get_node<godot::Control>("Control")) {
			control->set_visible(m_active);
		}
	}
};
