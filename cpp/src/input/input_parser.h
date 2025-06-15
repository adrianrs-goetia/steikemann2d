#pragma once

#include <vector>

#include "log.h"
#include "typedef.h"

#include "godot_cpp/classes/input.hpp"
#include "godot_cpp/classes/input_event.hpp"
#include "godot_cpp/classes/node.hpp"
#include "godot_cpp/classes/scene_tree.hpp"

class InputParser : public godot::Node {
	GDCLASS(InputParser, godot::Node)

	InputState m_inputstates;

	std::vector<std::tuple<godot::NodePath, InputCallback>> m_input_callbacks;

public:
	static void _bind_methods() {}

	void process() {
		bool updated = false;

		updated |= iterate_input_states();

		if (auto* input = godot::Input::get_singleton()) {
			updated |= parse_input_singleton(*input);
		}

		if (updated) {
			for (auto it = m_input_callbacks.begin(); it != m_input_callbacks.end();) {
				auto&& [path, cb] = *it;
				if (get_node<godot::Node>(path)) {
					cb(m_inputstates);
					it++;
				}
				else {
					it = m_input_callbacks.erase(it);
				}
			}
		}
	}

	auto iterate_input_states() -> bool {
		bool updated = false;
		// Move input state from `just_released -> none` once released.
		updated |= m_inputstates.daelking.iterate_state();
		return updated;
	}

	auto parse_input_singleton(const godot::Input& t_input) -> bool {
		bool updated = false;

		updated |= mutate_input_boolean_action_state(t_input, input_action::daelking, m_inputstates.daelking);
		updated |= mutate_input_movement_vector(t_input);
		updated |= mutate_camera_movement_vector(t_input);

		return updated;
	}

	auto mutate_input_boolean_action_state(
		const godot::Input& t_input, const char* action, InputButtonActionState& inputstate) -> bool {
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

	auto mutate_input_movement_vector(const godot::Input& t_input) -> bool {
		// Should return movement for MnK-wasd or the Joypad left stick
		const auto new_movement = t_input.get_vector(
			input_action::move_left, input_action::move_right, input_action::move_down, input_action::move_up);
		if (m_inputstates.movement.vector != new_movement) {
			m_inputstates.movement.vector = new_movement;
			return true;
		}
		return false;
	}

	auto mutate_camera_movement_vector(const godot::Input& t_input) -> bool {
		// Mouse movement can override joypad movement
		const auto mouse_movement = [&t_input]
		{
			auto& input = const_cast<godot::Input&>(t_input);
			return input.get_last_mouse_screen_velocity();
		}();
		if (0.1f < mouse_movement.length_squared()) {
			if (m_inputstates.camera.vector != mouse_movement) {
				m_inputstates.camera.vector = mouse_movement;
				return true;
			}
		}

		const auto joypad_movement = t_input.get_vector(input_action::joystick_r_right,
			input_action::joystick_r_left,
			input_action::joystick_r_up,
			input_action::joystick_r_down);
		if (m_inputstates.camera.vector != joypad_movement) {
			m_inputstates.camera.vector = joypad_movement;
		}

		return false;
	}

	void register_input_callback(godot::NodePath path, InputCallback cb) {
		LOG_TRACE("Registering input callback for node: {}", str(path));
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
