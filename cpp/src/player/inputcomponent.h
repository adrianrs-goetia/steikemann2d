#pragma once

#include <optional>

#include <input/actions.h>
#include <log.h>

#include <godot_cpp/classes/input_event.hpp>
#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/classes/scene_tree.hpp>

class InputComponent : public godot::Node {
	GDCLASS(InputComponent, godot::Node)

	int m_movement_direction = 0;

	struct CachedInput {
		bool move_left = false;
		bool move_right = false;
	} cached;

public:
	static void _bind_methods() {}

	void _enter_tree() override {
		set_name(get_class());
		set_process_input(true);
		set_process_mode(godot::Node::ProcessMode::PROCESS_MODE_ALWAYS);
	}

	void _input(const godot::Ref<godot::InputEvent>& t_event) override {
		if (t_event->is_action(InputAction::move_left) || t_event->is_action(InputAction::move_right)) {
			m_movement_direction = get_new_movement_direction(t_event);
		}

		if (t_event->is_action_pressed(InputAction::pause_menu)) {
			activate_pause_menu();
		}
	}

	auto get_movement_direction() const -> int {
		return m_movement_direction;
	}

	void activate_pause_menu() {
		// TODO toggle pause menu here
		LOG_INFO("Quitting game from InputComponent");
		get_tree()->quit(0);
	}

	auto get_new_movement_direction(const godot::Ref<godot::InputEvent>& t_event) -> int {
		auto input_direction_pressed = [&t_event](const char* action) -> std::optional<bool>
		{
			if (t_event->is_action(action)) {
				if (t_event->is_pressed()) {
					return true;
				}
				if (t_event->is_released()) {
					return false;
				}
			}
			return std::nullopt;
		};

		if (auto move_left_opt = input_direction_pressed(InputAction::move_left); move_left_opt.has_value()) {
			cached.move_left = move_left_opt.value();
		}
		if (auto move_right_opt = input_direction_pressed(InputAction::move_right); move_right_opt.has_value()) {
			cached.move_right = move_right_opt.value();
		}

		if (cached.move_left && cached.move_right) {
			return 0;
		}
		else if (cached.move_left) {
			return -1;
		}
		else if (cached.move_right) {
			return 1;
		}
		return 0;
	}
};
