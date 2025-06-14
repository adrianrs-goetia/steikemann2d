#pragma once

#include <vector>

#include "input_mode.h"
#include "input_state.h"
#include <log.h>

#include <godot_cpp/classes/input.hpp>
#include <godot_cpp/classes/input_event.hpp>
#include <godot_cpp/classes/resource.hpp>
#include <godot_cpp/classes/scene_tree.hpp>

class InputParser : public godot::Resource {
	GDCLASS(InputParser, godot::Resource)

	InputState m_inputstates;

	std::vector<std::tuple<godot::NodePath, InputCallback>> m_input_callbacks;

public:
	static void _bind_methods() {}

	void process(const InputModeManager& inputmode) {
		bool updated = false;

		updated |= iterate_input_states();

		if (auto* input = godot::Input::get_singleton()) {
			updated |= parse_input_singleton(*input);
		}

		if (updated) {
			for (auto&& [_, cb] : m_input_callbacks) {
				cb(m_inputstates);
			}
		}
	}

	auto iterate_input_states() -> bool {
		bool updated = false;
		// Move input state from `just_released -> none` once released.
		// No input event for this
		updated |= m_inputstates.move_left.iterate_state();
		updated |= m_inputstates.move_right.iterate_state();
		updated |= m_inputstates.pause_menu.iterate_state();
		updated |= m_inputstates.daelking.iterate_state();
		return updated;
	}

	auto parse_input_singleton(const godot::Input& t_input) -> bool {
		bool updated = false;

		updated |= mutate_input_action_state(t_input, input_action::move_left, m_inputstates.move_left);
		updated |= mutate_input_action_state(t_input, input_action::move_right, m_inputstates.move_right);
		updated |= mutate_input_action_state(t_input, input_action::pause_menu, m_inputstates.pause_menu);
		updated |= mutate_input_action_state(t_input, input_action::daelking, m_inputstates.daelking);

		return updated;
	}

	auto mutate_input_action_state(const godot::Input& t_input, const char* action, InputButtonActionState& inputstate)
		-> bool {
		if (t_input.is_action_just_pressed(action)) {
			inputstate.state = InputButtonActionState::JUST_PRESSED;
			return true;
		}
		else if (t_input.is_action_just_released(action)) {
			inputstate.state = InputButtonActionState::JUST_RELEASED;
			return true;
		}
		return false;
	};

	void register_input_callback(godot::NodePath path, InputCallback cb) {
		m_input_callbacks.emplace_back(path, cb);
	}

	void unregister_input_callback(godot::NodePath path) {
		auto it = std::find_if(m_input_callbacks.begin(), m_input_callbacks.end(),
						[path](auto&& ele){//
						    return std::get<godot::NodePath>(ele) == path;
						});
		m_input_callbacks.erase(it);
	}

	void parse_gamepad_input(const godot::Input& t_input) {
		const auto movement_vector = t_input.get_vector(
			input_action::move_left, input_action::move_right, input_action::move_up, input_action::move_down);
	}
};
