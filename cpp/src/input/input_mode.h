#pragma once

#include <functional>
#include <optional>
#include <vector>

#include <godot_cpp/classes/input.hpp>
#include <godot_cpp/classes/input_event.hpp>
#include <godot_cpp/classes/input_event_joypad_button.hpp>
#include <godot_cpp/classes/input_event_joypad_motion.hpp>
#include <godot_cpp/classes/input_event_key.hpp>
#include <godot_cpp/classes/input_event_mouse_button.hpp>
#include <godot_cpp/classes/input_event_mouse_motion.hpp>

enum class EInputMode {
	MNK,
	JOYPAD,
};
using InputModeChangeCallback = std::function<void(const EInputMode&)>;

class InputModeManager {
	// Determines the current mode manager will report
	EInputMode m_current_mode = EInputMode::MNK;

	// Input mode determined from godots propogated input events
	// Will not contain mouse movements..
	EInputMode m_previous_inputevent_mode = EInputMode::MNK;

	std::vector<std::tuple<godot::NodePath, InputModeChangeCallback>> m_input_mode_callbacks;

public:
	auto determine_input_mode_change(const godot::Input& input) -> std::optional<EInputMode> {
		const auto mouse_has_moved = has_mouse_moved(input);
		if (mouse_has_moved) {
			m_current_mode = EInputMode::MNK;
		}
		return m_current_mode;
	}

	void handle_input_event(const godot::Ref<godot::InputEvent>& t_event) {}

	void register_input_mode_callback(godot::NodePath path, InputModeChangeCallback cb) {
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
	auto has_mouse_moved(const godot::Input& input) -> bool {}
	auto determine_input_mode_from_cached_input_event(const godot::Input& input) -> EInputMode {}
};
