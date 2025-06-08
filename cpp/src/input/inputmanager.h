#pragma once

#include "actions.h"
#include <log.h>

#include <godot_cpp/classes/global_constants.hpp>
#include <godot_cpp/classes/input.hpp>
#include <godot_cpp/classes/input_event.hpp>
#include <godot_cpp/classes/input_event_key.hpp>
#include <godot_cpp/classes/input_map.hpp>
#include <godot_cpp/classes/node.hpp>

class InputManager : public godot::Node {
	GDCLASS(InputManager, godot::Node)

public:
	static void _bind_methods() {}

	void _enter_tree() override {
		LOG_TRACE("InputManager registering actions to InputMap");

		auto* im = godot::InputMap::get_singleton();
		im->add_action(InputAction::move_left);
		im->add_action(InputAction::move_right);

		im->action_add_event(InputAction::move_left,
			[]
			{
				godot::Ref<godot::InputEventKey> event;
				event.instantiate();
				event->set_keycode(godot::KEY_A);
				return event;
			}());
		im->action_add_event(InputAction::move_right,
			[]
			{
				godot::Ref<godot::InputEventKey> event;
				event.instantiate();
				event->set_keycode(godot::KEY_D);
				return event;
			}());
	}

	void _exit_tree() override {
		LOG_TRACE("InputManager deleting all actions from InputMap");

		auto* im = godot::InputMap::get_singleton();
		im->erase_action(InputAction::move_left);
		im->erase_action(InputAction::move_right);
	}
};
