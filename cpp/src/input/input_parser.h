#pragma once

#include <optional>
#include <vector>

#include "input_actions.h"
#include "input_state.h"
#include "log.h"

#include "godot_cpp/classes/input.hpp"
#include "godot_cpp/classes/input_event.hpp"
#include "godot_cpp/classes/node.hpp"
#include "godot_cpp/classes/scene_tree.hpp"

class InputParser : public godot::Node {
	GDCLASS(InputParser, godot::Node)

	InputState m_inputstate;

	std::vector<std::tuple<godot::NodePath, InputCallback>> m_input_callbacks;
	std::vector<std::tuple<godot::NodePath, InputModeCallback>> m_input_mode_callbacks;

public:
	static void _bind_methods() {}

	void process() {
		const auto* input = godot::Input::get_singleton();
		if (!input) {
			LOG_ERROR("Inputparser failed to get godot::Input singleton");
			return;
		}

		const bool input_iterated = iterate_input_states();

		const auto mnk_input = parse_mnk_input(*input, m_inputstate);
		const auto joypad_input = parse_joypad_input(*input, m_inputstate);

		const auto new_mode = [&joypad_input, &mnk_input] -> std::optional<InputMode>
		{
			if (mnk_input.has_value()) { // MnK mode always takes priority
				return InputMode::MNK;
			}
			if (joypad_input.has_value()) {
				return InputMode::JOYPAD;
			}
			return std::nullopt;
		}();

		if (new_mode.has_value()) {
			if (m_inputstate.mode != new_mode.value()) {
				m_inputstate.set_mode(new_mode.value());
				callback_input_mode_update();
			}
		}

		if (joypad_input || mnk_input) {
			switch (m_inputstate.mode) {
				case InputMode::JOYPAD: {
					m_inputstate.set_state(joypad_input.value());
					break;
				}
				case InputMode::MNK: {
					m_inputstate.set_state(mnk_input.value());
					break;
				}
			}
			callback_input_update();
		}
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

	void register_input_mode_callback(godot::NodePath path, InputCallback cb) {
		LOG_TRACE("Registering input callback for node: {}", str(path));
		m_input_mode_callbacks.emplace_back(path, cb);
	}
	void unregister_input_mode_callback(godot::NodePath path) {
		auto it = std::find_if(m_input_mode_callbacks.begin(), m_input_mode_callbacks.end(),
						[path](auto&& ele){//
						    return std::get<godot::NodePath>(ele) == path;
						});
		m_input_mode_callbacks.erase(it);
	}

private:
	void callback_input_update() {
		for (auto it = m_input_callbacks.begin(); it != m_input_callbacks.end();) {
			auto&& [path, cb] = *it;
			if (get_node<godot::Node>(path)) {
				cb(m_inputstate);
				it++;
			}
			else {
				it = m_input_callbacks.erase(it);
			}
		}
	}
	void callback_input_mode_update() {
		for (auto it = m_input_mode_callbacks.begin(); it != m_input_mode_callbacks.end();) {
			auto&& [path, cb] = *it;
			if (get_node<godot::Node>(path)) {
				cb(m_inputstate);
				it++;
			}
			else {
				it = m_input_mode_callbacks.erase(it);
			}
		}
	}

	auto parse_mnk_input(const godot::Input& t_input, InputState current_inputstate) -> std::optional<InputState> {
		bool updated = false;

		if (current_inputstate.mode == InputMode::MNK) {
			updated |= mutate_input_movement_vector(t_input,
				{ inputaction::mnk::move_left,
					inputaction::mnk::move_right,
					inputaction::mnk::move_down,
					inputaction::mnk::move_up },
				current_inputstate);
			updated |= mnk_mutate_camera_movement_vector(t_input, current_inputstate);
		}

		updated |= mutate_input_boolean_action_state(t_input, inputaction::mnk::daelking, current_inputstate.daelking);

		if (updated) {
			current_inputstate.set_mode(InputMode::MNK);
			return current_inputstate;
		}
		return std::nullopt;
	}

	auto parse_joypad_input(const godot::Input& t_input, InputState current_inputstate) -> std::optional<InputState> {
		bool updated = false;

		if (current_inputstate.mode == InputMode::JOYPAD) {
			updated |= mutate_input_movement_vector(t_input,
				{ inputaction::joypad::move_left,
					inputaction::joypad::move_right,
					inputaction::joypad::move_down,
					inputaction::joypad::move_up },
				current_inputstate);
			updated |= joypad_mutate_camera_movement_vector(t_input, current_inputstate);
		}

		updated
			|= mutate_input_boolean_action_state(t_input, inputaction::joypad::daelking, current_inputstate.daelking);

		if (updated) {
			current_inputstate.set_mode(InputMode::JOYPAD);
			return current_inputstate;
		}
		return std::nullopt;
	}

	auto iterate_input_states() -> bool {
		bool updated = false;
		updated |= m_inputstate.daelking.iterate_state();
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

	auto mutate_input_movement_vector(
		const godot::Input& t_input, const std::array<const char*, 4> actions, InputState& inputstate) -> bool {
		// input_action::move_left, input_action::move_right, input_action::move_down, input_action::move_up);
		const auto new_movement = t_input.get_vector(actions[0], actions[1], actions[2], actions[3]);
		if (inputstate.movement.vector != new_movement) {
			inputstate.movement.vector = new_movement;
			return true;
		}
		return false;
	}

	auto mnk_mutate_camera_movement_vector(const godot::Input& t_input, InputState& inputstate) -> bool {
		// Mouse movement can override joypad movement
		const auto mouse_movement = [&t_input]
		{
			auto& input = const_cast<godot::Input&>(t_input);
			return input.get_last_mouse_screen_velocity();
		}();
		if (0.1f < mouse_movement.length_squared()) {
			if (inputstate.camera.vector != mouse_movement) {
				inputstate.camera.vector = mouse_movement;
				return true;
			}
		}
		return false;
	}

	auto joypad_mutate_camera_movement_vector(const godot::Input& t_input, InputState& inputstate) -> bool {
		const auto joypad_movement = t_input.get_vector( //
			inputaction::joypad::joystick_r_right,
			inputaction::joypad::joystick_r_left,
			inputaction::joypad::joystick_r_up,
			inputaction::joypad::joystick_r_down);
		if (inputstate.camera.vector != joypad_movement) {
			inputstate.camera.vector = joypad_movement;
		}

		return false;
	}
};
