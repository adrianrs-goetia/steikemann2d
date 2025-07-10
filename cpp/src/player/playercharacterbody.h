#pragma once

#include "groups.h"
#include "input/input_actions.h"
#include "input/input_manager.h"
#include "log.h"
#include "macros.h"
#include "mathstatics.h"
#include "notifications.h"
#include "playerspawner.h"

#include "godot_cpp/classes/character_body3d.hpp"
#include "godot_cpp/classes/input_event.hpp"
#include "godot_cpp/classes/scene_tree.hpp"

class PlayerCharacterBody : public godot::CharacterBody3D {
	GDCLASS(PlayerCharacterBody, godot::CharacterBody3D)

public:
	static void _bind_methods() {}

	void _enter_tree() override {
		add_to_group(group::playerspawn::name);

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

			get_tree()->notify_group(group::playerspawn::name, SteikeNotification::SPAWN_PLAYER);
		}
	}

	void _exit_tree() override {
		GAME_SCOPE {
			if (auto* inputmanager = get_node<InputManager>(InputManager::get_path())) {
				inputmanager->unregister_input_mode_callback(get_path());
			}
		}
	}

	void _notification(int what) {
		if (what == SteikeNotification::SPAWN_PLAYER) {
			spawn_player();
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
			get_tree()->notify_group(group::playerspawn::name, SteikeNotification::SPAWN_PLAYER);
		}
	}

private:
	void spawn_player() {
		LOG_INFO("Spawn player");
		if (auto* spawner = get_node<PlayerSpawner>(PlayerSpawner::get_path())) {
			if (const auto spawn_location = spawner->get_current_spawn_location()) {
				set_global_position(spawn_location.value());
				set_velocity(mathstatics::vector_zero);
			}
		}
	}
};

/**
 * NB! Unable to test nodes that inherit from PhysicsBody. So functionality should be in separate Nodes/Resources
 * Looked like the internal Godot PhysicsServer required some initial setup to be able to be tested like that.
 */
