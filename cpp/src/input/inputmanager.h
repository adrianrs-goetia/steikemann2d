#pragma once

#include "godot_cpp/variant/node_path.hpp"
#include "inputparser.h"
#include "typedef.h"
#include <log.h>

#include <godot_cpp/classes/global_constants.hpp>
#include <godot_cpp/classes/input.hpp>
#include <godot_cpp/classes/input_event.hpp>
#include <godot_cpp/classes/input_event_key.hpp>
#include <godot_cpp/classes/input_map.hpp>
#include <godot_cpp/classes/node.hpp>

class InputManager : public godot::Node {
	GDCLASS(InputManager, godot::Node)

	godot::Ref<InputParser> m_input_parser;

public:
	static void _bind_methods() {}

	static auto get_name() -> godot::String {
		return godot::String("InputManager");
	}

	static auto get_path() -> godot::NodePath {
		return "/root/" + InputManager::get_name();
	}

	void _enter_tree() override {
		set_name(InputManager::get_name());
		set_process_mode(godot::Node::ProcessMode::PROCESS_MODE_ALWAYS);
		get_node<godot::Node>("/root")->set_process_input(false); // Disable input events, using godot::Input singleton

		register_actions(*godot::InputMap::get_singleton());

		m_input_parser.instantiate();
	}

	void _exit_tree() override {
		LOG_TRACE("InputManager deleting all actions from InputMap");

		auto* im = godot::InputMap::get_singleton();
		im->erase_action(InputAction::move_left);
		im->erase_action(InputAction::move_right);
	}

	void _process(double _) override {
		m_input_parser->process();
	}

	void register_input_callback(godot::NodePath path, InputCallback cb) {
		m_input_parser->register_input_callback(path, cb);
	}
	void unregister_input_callback(godot::NodePath path) {
		m_input_parser->unregister_input_callback(path);
	}

	void register_actions(godot::InputMap& im) {
		LOG_TRACE("InputManager registering actions to InputMap");

		im.add_action(InputAction::pause_menu);
		im.action_add_event(InputAction::pause_menu,
			[]
			{
				godot::Ref<godot::InputEventKey> event;
				event.instantiate();
				event->set_keycode(godot::KEY_ESCAPE);
				return event;
			}());

		im.add_action(InputAction::move_left);
		im.action_add_event(InputAction::move_left,
			[]
			{
				godot::Ref<godot::InputEventKey> event;
				event.instantiate();
				event->set_keycode(godot::KEY_A);
				return event;
			}());

		im.add_action(InputAction::move_right);
		im.action_add_event(InputAction::move_right,
			[]
			{
				godot::Ref<godot::InputEventKey> event;
				event.instantiate();
				event->set_keycode(godot::KEY_D);
				return event;
			}());

		im.add_action(InputAction::daelking);
		im.action_add_event(InputAction::daelking,
			[]
			{
				godot::Ref<godot::InputEventKey> event;
				event.instantiate();
				event->set_keycode(godot::KEY_SPACE);
				return event;
			}());
	}
};
