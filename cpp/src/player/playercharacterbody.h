#pragma once

#include <input/input_manager.h>
#include <input/typedef.h>
#include <log.h>
#include <macros.h>

#include <godot_cpp/classes/character_body3d.hpp>
#include <godot_cpp/classes/input_event.hpp>

class PlayerCharacterBody : public godot::CharacterBody3D {
	GDCLASS(PlayerCharacterBody, godot::CharacterBody3D)

public:
	static void _bind_methods() {}

	void _input(const godot::Ref<godot::InputEvent>& t_event) override {
		if (t_event->is_action_pressed(input_action::pause_menu)) {
			LOG_TRACE("Quitting from {}", godot::String(get_path()).utf8().get_data());
			get_tree()->quit(0);
		}
		if (t_event->is_action_pressed(input_action::restart_level)) {
			LOG_TRACE("Restarting level");
			get_tree()->call_deferred("reload_current_scene");
		}
	}
};

/**
 * NB! Unable to test nodes that inherit from PhysicsBody. So functionality should be in separate Nodes/Resources
 * Looked like the internal Godot PhysicsServer required some initial setup to be able to be tested like that.
 */
