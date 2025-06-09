#pragma once

#include <functional>

namespace InputAction {

inline const char* pause_menu = "pause_menu";

inline const char* move_left = "move_left";
inline const char* move_right = "move_right";
inline const char* daelking = "daelking";

} //namespace InputAction

class InputParser;

struct InputActionState {
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

struct InputState {
	InputActionState pause_menu;
	InputActionState move_left;
	InputActionState move_right;
	InputActionState daelking;
};

using InputCallback = std::function<void(const InputState&)>;
