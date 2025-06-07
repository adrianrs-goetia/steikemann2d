#pragma once

#include <godot_cpp/classes/character_body3d.hpp>
#include <godot_cpp/classes/resource.hpp>

#include <log.h>
#include <macros.h>

class PlayerCharacterBody : public godot::CharacterBody3D {
	GDCLASS(PlayerCharacterBody, godot::CharacterBody3D)

private:
	int m_myint = 0;

public:
	auto get_myint() const -> int {
		return m_myint;
	}
	auto set_myint(int t_int) -> void {
		m_myint = t_int;
	}

	static void _bind_methods() {
		BIND_METHOD(PlayerCharacterBody, my_function);
		BIND_PROPERTY_METHODS(PlayerCharacterBody, myint, INT);
	}

	void _enter_tree() override {
		call_deferred("my_function");
	}

	void my_function() {
		LOG_INFO("Calling my function!");
	}
};
