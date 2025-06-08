#pragma once

#include <algorithm>

#include <input/actions.h>
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

	void _enter_tree() override {}

	void _physics_process(double delta) override {
		set_velocity(godot::Vector3(m_movement_speed * m_movement_direction, get_gravity().y, 0.f));
		move_and_slide();
	}

	void _input(const godot::Ref<godot::InputEvent>& t_event) override {
		// Set movement direction
		if (t_event->is_action(InputAction::move_left)) {
			if (t_event->is_pressed()) {
				m_movement_direction -= 1;
			}
			else if (t_event->is_released()) {
				m_movement_direction += 1;
			}
		}
		if (t_event->is_action(InputAction::move_right)) {
			if (t_event->is_pressed()) {
				m_movement_direction += 1;
			}
			else if (t_event->is_released()) {
				m_movement_direction -= 1;
			}
		}

		LOG_TRACE("Movement direction. {}", m_movement_direction);
	}
};

/**
 * NB! Unable to test nodes that inherit from PhysicsBody. So functionality should be in separate Nodes/Resources
 * Looked like the internal Godot PhysicsServer required some initial setup to be able to be tested like that.
 */
