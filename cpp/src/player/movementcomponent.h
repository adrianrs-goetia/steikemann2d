#pragma once

#include <input/typedef.h>
#include <log.h>
#include <macros.h>

#include <godot_cpp/classes/character_body3d.hpp>
#include <godot_cpp/classes/resource.hpp>

class MovementComponent : public godot::Resource {
	GDCLASS(MovementComponent, godot::Resource)

	PROPERTY(int, movement_direction, 0);
	PROPERTY(float, movement_speed, 450.f);
	PROPERTY(float, gravity_scale, 2.f);

	PROPERTY(float, daelking_impulse_strength, 8.0f);
	bool m_daelk_impulse = false;

public:
	static void _bind_methods() {
		BIND_PROPERTY_METHODS(MovementComponent, gravity_scale, FLOAT);
		BIND_PROPERTY_METHODS(MovementComponent, movement_speed, FLOAT);
		BIND_PROPERTY_METHODS(MovementComponent, daelking_impulse_strength, FLOAT);
	}

	void physics_process(godot::CharacterBody3D& character, double delta) {
		auto velocity = character.get_velocity();
		velocity.x = m_movement_speed * (float)m_movement_direction * delta;
		velocity.y += character.get_gravity().y * delta * get_gravity_scale();

		if (m_daelk_impulse) {
			velocity.y = get_daelking_impulse_strength();
			m_daelk_impulse = false;
		}

		character.set_velocity(velocity);
		character.move_and_slide();
	}

	void process_input(const InputState& input) {
		m_movement_direction = get_new_movement_direction(input);
		if (input.daelking.just_pressed()) {
			m_daelk_impulse = true;
		}
	}

	auto get_new_movement_direction(const InputState& input) -> int {
		if (input.move_left.pressed() && input.move_right.pressed()) {
			return 0;
		}
		else if (input.move_left.pressed()) {
			return -1;
		}
		else if (input.move_right.pressed()) {
			return 1;
		}
		return 0;
	}
};
