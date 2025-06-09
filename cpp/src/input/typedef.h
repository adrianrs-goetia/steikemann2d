#pragma once

#include <functional>

namespace InputAction {

inline const char* pause_menu = "pause_menu";

inline const char* move_left = "move_left";
inline const char* move_right = "move_right";
inline const char* daelking = "daelking";

} //namespace InputAction

struct InputState {
	bool pause_menu = false;
	bool move_left = false;
	bool move_right = false;
	bool daelking = false;
};

using InputCallback = std::function<void(const InputState&)>;
