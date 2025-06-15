#pragma once

#include <type_traits>

#include "input_parser.h"
#include "typedef.h"
#include <log.h>

#include <godot_cpp/classes/global_constants.hpp>
#include <godot_cpp/classes/input.hpp>
#include <godot_cpp/classes/input_event.hpp>
#include <godot_cpp/classes/input_event_joypad_button.hpp>
#include <godot_cpp/classes/input_event_joypad_motion.hpp>
#include <godot_cpp/classes/input_event_key.hpp>
#include <godot_cpp/classes/input_map.hpp>
#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/variant/node_path.hpp>

class InputManager : public godot::Node {
	GDCLASS(InputManager, godot::Node)

	InputParser* m_input_parser = nullptr;

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

		register_actions(*godot::InputMap::get_singleton());

		if (!m_input_parser) {
			m_input_parser = memnew(InputParser);
			m_input_parser->set_name(m_input_parser->get_class());
			add_child(m_input_parser);
		}
	}

	void _process(double _) override {
		if (m_input_parser) {
			m_input_parser->process();
		}
	}

	void register_input_callback(godot::NodePath path, InputCallback cb) {
		if (m_input_parser) {
			m_input_parser->register_input_callback(path, cb);
		}
	}
	void unregister_input_callback(godot::NodePath path) {
		if (m_input_parser) {
			m_input_parser->unregister_input_callback(path);
		}
	}

private:
	template <class T>
	auto create_event(int key_or_button_or_axis, float scalar = 0.f) -> godot::Ref<T> {
		godot::Ref<T> event;
		event.instantiate();
		if constexpr (std::is_same_v<T, godot::InputEventKey>) {
			event->set_keycode((godot::Key)key_or_button_or_axis);
		}
		else if constexpr (std::is_same_v<T, godot::InputEventJoypadButton>) {
			event->set_button_index((godot::JoyButton)key_or_button_or_axis);
		}
		else if constexpr (std::is_same_v<T, godot::InputEventJoypadMotion>) {
			event->set_axis((godot::JoyAxis)key_or_button_or_axis);
			event->set_axis_value(scalar);
		}
		return event;
	}

	void register_actions(godot::InputMap& im) {
		LOG_TRACE("InputManager registering actions to InputMap");

		{
			const auto action = input_action::pause_menu;
			im.add_action(action);
			im.action_add_event(action, create_event<godot::InputEventKey>(godot::KEY_ESCAPE));
			im.action_add_event(action, create_event<godot::InputEventJoypadButton>(godot::JOY_BUTTON_START));
		}
		{
			const auto action = input_action::restart_level;
			im.add_action(action);
			im.action_add_event(action, create_event<godot::InputEventKey>(godot::KEY_R));
			im.action_add_event(action, create_event<godot::InputEventJoypadButton>(godot::JOY_BUTTON_BACK));
		}

		_register_wasd_and_leftjoystick_actions(im);
		_register_right_joystick_actions(im);

		{
			const auto action = input_action::daelking;
			im.add_action(action);
			im.action_add_event(action, create_event<godot::InputEventKey>(godot::KEY_SPACE));
			im.action_add_event(action, create_event<godot::InputEventJoypadButton>(godot::JOY_BUTTON_A));
		}
	}

	void _register_wasd_and_leftjoystick_actions(godot::InputMap& im) {
		{
			const auto action = input_action::move_left;
			im.add_action(action);
			im.action_add_event(action, create_event<godot::InputEventKey>(godot::KEY_A));
			im.action_add_event(action, create_event<godot::InputEventJoypadMotion>(godot::JOY_AXIS_LEFT_X, -1.f));
		}

		{
			const auto action = input_action::move_right;
			im.add_action(action);
			im.action_add_event(action, create_event<godot::InputEventKey>(godot::KEY_D));
			im.action_add_event(action, create_event<godot::InputEventJoypadMotion>(godot::JOY_AXIS_LEFT_X, 1.f));
		}

		{
			const auto action = input_action::move_up;
			im.add_action(action);
			im.action_add_event(action, create_event<godot::InputEventKey>(godot::KEY_W));
			im.action_add_event(action, create_event<godot::InputEventJoypadMotion>(godot::JOY_AXIS_LEFT_Y, -1.f));
		}

		{
			const auto action = input_action::move_down;
			im.add_action(action);
			im.action_add_event(action, create_event<godot::InputEventKey>(godot::KEY_S));
			im.action_add_event(action, create_event<godot::InputEventJoypadMotion>(godot::JOY_AXIS_LEFT_Y, 1.f));
		}
	}

	void _register_right_joystick_actions(godot::InputMap& im) {
		{
			const auto action = input_action::joystick_r_right;
			im.add_action(action);
			im.action_add_event(action, create_event<godot::InputEventJoypadMotion>(godot::JOY_AXIS_RIGHT_X, 1.f));
		}
		{
			const auto action = input_action::joystick_r_left;
			im.add_action(action);
			im.action_add_event(action, create_event<godot::InputEventJoypadMotion>(godot::JOY_AXIS_RIGHT_X, -1.f));
		}
		{
			const auto action = input_action::joystick_r_up;
			im.add_action(action);
			im.action_add_event(action, create_event<godot::InputEventJoypadMotion>(godot::JOY_AXIS_RIGHT_Y, 1.f));
		}
		{
			const auto action = input_action::joystick_r_down;
			im.add_action(action);
			im.action_add_event(action, create_event<godot::InputEventJoypadMotion>(godot::JOY_AXIS_RIGHT_Y, -1.f));
		}
	}
};
