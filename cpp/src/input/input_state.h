#pragma once

#include <functional>

#include <godot_cpp/variant/vector2.hpp>

namespace input_action {

inline const char* pause_menu = "pause_menu";

inline const char* move_right = "move_right";
inline const char* move_left = "move_left";

inline const char* move_up = "move_up";
inline const char* move_down = "move_down";

inline const char* daelking = "daelking";

} //namespace input_action

class InputParser;

struct InputButtonActionState {
	enum State {
		NONE,
		JUST_PRESSED,
		IS_HELD,
		JUST_RELEASED,
	};

private:
	friend class InputParser;

	State state = State::NONE;

	// Iterate state from just_pressed/released to its designated next state
	auto iterate_state() -> bool {
		if (just_pressed()) {
			state = IS_HELD;
			return true;
		}
		else if (just_released()) {
			state = NONE;
			return true;
		}
		return false;
	}

public:
	bool none() const {
		return state == NONE;
	}
	bool pressed() const {
		return just_pressed() || state == IS_HELD;
	}
	bool just_pressed() const {
		return state == JUST_PRESSED;
	}
	bool just_released() const {
		return state == JUST_RELEASED;
	}
};

struct InputAxisActionState {
	godot::Vector2 axis;
};

struct InputState {
	InputAxisActionState movement;
	InputAxisActionState camera;

	InputButtonActionState pause_menu;
	InputButtonActionState move_left;
	InputButtonActionState move_right;
	InputButtonActionState daelking;
};

using InputCallback = std::function<void(const InputState&)>;
