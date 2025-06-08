#pragma once

#include "inputcomponent.h"
#include <input/actions.h>
#include <log.h>
#include <macros.h>

#include <godot_cpp/classes/character_body3d.hpp>
#include <godot_cpp/classes/input_event.hpp>
#include <godot_cpp/variant/vector3.hpp>

class PlayerCharacterBody : public godot::CharacterBody3D {
	GDCLASS(PlayerCharacterBody, godot::CharacterBody3D)

private:
	InputComponent* m_inputcomponent = nullptr;
	double m_movement_speed = 6.0;

public:
	static void _bind_methods() {}

	void _enter_tree() override {
		m_inputcomponent = memnew(InputComponent);
		add_child(m_inputcomponent);
	}

	void _physics_process(double delta) override {
		set_velocity(
			godot::Vector3(m_movement_speed * m_inputcomponent->get_movement_direction(), get_gravity().y, 0.f));
		move_and_slide();
	}
};

/**
 * NB! Unable to test nodes that inherit from PhysicsBody. So functionality should be in separate Nodes/Resources
 * Looked like the internal Godot PhysicsServer required some initial setup to be able to be tested like that.
 */
