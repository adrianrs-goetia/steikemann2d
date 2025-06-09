#pragma once

#include "godot_cpp/variant/node_path.hpp"
#include <optional>
#include <vector>

#include <input/typedef.h>
#include <log.h>

#include <godot_cpp/classes/input_event.hpp>
#include <godot_cpp/classes/resource.hpp>
#include <godot_cpp/classes/scene_tree.hpp>

class InputParser : public godot::Resource {
	GDCLASS(InputParser, godot::Resource)

	int m_movement_direction = 0;

	InputState cached;

	std::vector<std::tuple<godot::NodePath, InputCallback>> m_input_callbacks;

public:
	static void _bind_methods() {}

	void parse_input_event(const godot::Ref<godot::InputEvent>& t_event) {
		bool updated = false;

		if (auto move_left_opt = get_input_action_state(t_event, InputAction::move_left)) {
			cached.move_left = move_left_opt.value();
			updated = true;
		}
		if (auto move_right_opt = get_input_action_state(t_event, InputAction::move_right)) {
			cached.move_right = move_right_opt.value();
			updated = true;
		}

		if (auto pause_menu_opt = get_input_action_state(t_event, InputAction::pause_menu)) {
			cached.pause_menu = pause_menu_opt.value();
			updated = true;
		}

		if (auto daelking_opt = get_input_action_state(t_event, InputAction::daelking)) {
			cached.daelking = daelking_opt.value();
			updated = true;
		}

		if (updated) {
			for (auto&& [_, cb] : m_input_callbacks) {
				cb(cached);
			}
		}
	}

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

	auto get_input_action_state(const godot::Ref<godot::InputEvent>& t_event, const char* action)
		-> std::optional<bool> {
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
};
