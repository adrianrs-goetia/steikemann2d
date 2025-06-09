#pragma once

#include <input/inputmanager.h>
#include <input/typedef.h>
#include <log.h>
#include <macros.h>

#include <godot_cpp/classes/character_body3d.hpp>

class PlayerCharacterBody : public godot::CharacterBody3D {
	GDCLASS(PlayerCharacterBody, godot::CharacterBody3D)

private:
public:
	static void _bind_methods() {}

	void _enter_tree() override {
		GAME_SCOPE {
			if (auto* im = get_node<InputManager>(InputManager::get_path())) {
				im->register_input_callback(get_path(), [this](const InputState& i) { this->input_callback(i); });
			}
		}
	}
	void _exit_tree() override {
		GAME_SCOPE {
			if (auto* im = get_node<InputManager>(InputManager::get_path())) {
				im->unregister_input_callback(get_path());
			}
		}
	}

	void input_callback(const InputState& input) {
		GAME_SCOPE {
			// TODO pause menu
			if (input.pause_menu.just_pressed()) {
				LOG_TRACE("Quitting from {}", godot::String(get_path()).utf8().get_data());
				get_tree()->quit(0);
			}
		}
	}
};

/**
 * NB! Unable to test nodes that inherit from PhysicsBody. So functionality should be in separate Nodes/Resources
 * Looked like the internal Godot PhysicsServer required some initial setup to be able to be tested like that.
 */
