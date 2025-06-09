#pragma once

#include <input/typedef.h>
#include <macros.h>

#include <godot_cpp/classes/character_body3d.hpp>
#include <godot_cpp/classes/resource.hpp>

class MovementComponent : public godot::Resource {
	GDCLASS(MovementComponent, godot::Resource)

	PROPERTY(int, movement_direction, 0);
	PROPERTY(float, movement_speed, 6.f);

public:
	static void _bind_methods() {
		BIND_PROPERTY_METHODS(MovementComponent, movement_speed, FLOAT);
	}

	void physics_process(godot::CharacterBody3D& character, double delta) {
		character.set_velocity(
			godot::Vector3(m_movement_speed * (float)m_movement_direction * delta, character.get_gravity().y, 0.f));
		character.move_and_slide();
	}

	void process_input(const InputState& input) {
		m_movement_direction = get_new_movement_direction(input);
	}

	auto get_new_movement_direction(const InputState& input) -> int {
		if (input.move_left && input.move_right) {
			return 0;
		}
		else if (input.move_left) {
			return -1;
		}
		else if (input.move_right) {
			return 1;
		}
		return 0;
	}
};
