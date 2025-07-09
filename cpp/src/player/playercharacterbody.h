#pragma once

#include "input/input_actions.h"
#include "input/input_manager.h"
#include "log.h"
#include "macros.h"

#include "godot_cpp/classes/character_body3d.hpp"
#include "godot_cpp/classes/input_event.hpp"
#include "godot_cpp/classes/scene_tree.hpp"

class PlayerCharacterBody : public godot::CharacterBody3D {
	GDCLASS(PlayerCharacterBody, godot::CharacterBody3D)

public:
	static void _bind_methods() {}

	void _enter_tree() override {
		GAME_SCOPE {
			if (auto* inputmanager = get_node<InputManager>(InputManager::get_path())) {
				inputmanager->register_input_mode_callback(get_path(),
					[this](const InputState& input)
					{
						switch (input.mode) {
							case InputMode::MNK: {
								LOG_INFO("New input mode: MnK");
								break;
							}
							case InputMode::JOYPAD: {
								LOG_INFO("New input mode: Joypad");
								break;
							}
						}
					});
			}
		}
	}
	void _exit_tree() override {
		GAME_SCOPE {
			if (auto* inputmanager = get_node<InputManager>(InputManager::get_path())) {
				inputmanager->unregister_input_mode_callback(get_path());
			}
		}
	}
	void _input(const godot::Ref<godot::InputEvent>& t_event) override {
		if (t_event->is_action_pressed(inputaction::joypad::pause_menu)
			|| t_event->is_action_pressed(inputaction::mnk::pause_menu)) {
			LOG_TRACE("Quitting from {}", godot::String(get_path()).utf8().get_data());
			get_tree()->quit(0);
		}
		if (t_event->is_action_pressed(inputaction::joypad::restart_level)
			|| t_event->is_action_pressed(inputaction::mnk::restart_level)) {
			LOG_TRACE("Restarting level");
			get_tree()->call_deferred("reload_current_scene");
		}
	}
};

/**
 * NB! Unable to test nodes that inherit from PhysicsBody. So functionality should be in separate Nodes/Resources
 * Looked like the internal Godot PhysicsServer required some initial setup to be able to be tested like that.
 */
