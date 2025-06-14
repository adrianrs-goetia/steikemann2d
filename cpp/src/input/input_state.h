#pragma once

#include <functional>

#include <godot_cpp/variant/vector2.hpp>
#include <godot_cpp/variant/vector3.hpp>

namespace input_action {

inline const char* pause_menu = "pause_menu";

inline const char* move_right = "move_right";
inline const char* move_left = "move_left";

inline const char* move_up = "move_up";
inline const char* move_down = "move_down";

inline const char* daelking = "daelking";

inline const char* joystick_r_right = "joytstick_r_right";
inline const char* joystick_r_left = "joytstick_r_left";
inline const char* joystick_r_up = "joytstick_r_up";
inline const char* joystick_r_down = "joytstick_r_down";

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

class InputVectorActionState {
	friend class InputParser;

	godot::Vector2 vector;

public:
	auto x() const -> float {
		return vector.x;
	}
	auto y() const -> float {
		return vector.y;
	}
	auto vec2() const -> godot::Vector2 {
		return vector;
	}
	auto vec3() const -> godot::Vector3 {
		return godot::Vector3(vector.x, vector.y, 0.f);
	}
	auto vec3_transpose() const -> godot::Vector3 {
		return godot::Vector3(vector.y, vector.x, 0.f);
	}
};

struct InputState {
	InputVectorActionState movement;
	InputVectorActionState camera;

	InputButtonActionState pause_menu;
	InputButtonActionState daelking;
};

using InputCallback = std::function<void(const InputState&)>;
