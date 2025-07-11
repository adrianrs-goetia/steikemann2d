#pragma once

#include <functional>

#include "godot_cpp/variant/vector2.hpp"
#include "godot_cpp/variant/vector3.hpp"

class InputParser;

enum InputMode {
	MNK,
	JOYPAD,
};

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
	void set(const godot::Vector2 vec) {
		vector = vec;
	}

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
};

struct InputState {
	InputMode mode = InputMode::MNK;

	InputVectorActionState movement;
	InputVectorActionState camera;

	InputVectorActionState mouse_position;

	InputButtonActionState daelking;

	InputButtonActionState tmp_spawnpoint;

	void set_mode(const InputMode t_mode) {
		mode = t_mode;
	}
	void set_state(const InputState& other) {
		movement = other.movement;
		camera = other.camera;
		mouse_position = other.mouse_position;
		daelking = other.daelking;
		tmp_spawnpoint = other.tmp_spawnpoint;
	}
};

using InputCallback = std::function<void(const InputState&)>;
using InputModeCallback = std::function<void(const InputState&)>;
