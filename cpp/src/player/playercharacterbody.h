#pragma once

#include <input/inputmanager.h>
#include <input/typedef.h>
#include <log.h>
#include <macros.h>

#include <godot_cpp/classes/character_body3d.hpp>
#include <godot_cpp/classes/input_event.hpp>
#include <godot_cpp/variant/vector3.hpp>

class PlayerCharacterBody : public godot::CharacterBody3D {
	GDCLASS(PlayerCharacterBody, godot::CharacterBody3D)

private:
	int m_movement_direction = 0;
	double m_movement_speed = 6.0;

public:
	static void _bind_methods() {}

	void _enter_tree() override {
		set_process_input(false);
		auto* im = get_node<InputManager>(InputManager::get_path());
		im->register_input_callback(get_path(), [this](const InputState& ci) { this->input_callback(ci); });
	}
	void _exit_tree() override {
		auto* im = get_node<InputManager>(InputManager::get_path());
		im->unregister_input_callback(get_path());
	}

	void _physics_process(double delta) override {
		set_velocity(godot::Vector3(m_movement_speed * (float)m_movement_direction, get_gravity().y, 0.f));
		move_and_slide();
	}

	void input_callback(const InputState& cachedinput) {
		m_movement_direction = get_new_movement_direction(cachedinput);

		// TODO pause menu
		if (cachedinput.pause_menu) {
			LOG_TRACE("Quitting from {}", godot::String(get_path()).utf8().get_data());
			get_tree()->quit(0);
		}
	}

	auto get_new_movement_direction(const InputState& cachedinput) -> int {
		if (cachedinput.move_left && cachedinput.move_right) {
			return 0;
		}
		else if (cachedinput.move_left) {
			return -1;
		}
		else if (cachedinput.move_right) {
			return 1;
		}
		return 0;
	}
};

/**
 * NB! Unable to test nodes that inherit from PhysicsBody. So functionality should be in separate Nodes/Resources
 * Looked like the internal Godot PhysicsServer required some initial setup to be able to be tested like that.
 */
