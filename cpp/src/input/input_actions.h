#pragma once

#include <initializer_list>

namespace inputaction {

namespace base {

constexpr auto pause_menu = "pause_menu";
constexpr auto restart_level = "restart_level";

constexpr auto move_right = "move_right";
constexpr auto move_left = "move_left";

constexpr auto move_up = "move_up";
constexpr auto move_down = "move_down";

constexpr auto daelking = "daelking";
} //namespace base

namespace mnk {
constexpr auto prefix = "mnk_";

constexpr auto pause_menu = "mnk_pause_menu";
constexpr auto restart_level = "mnk_restart_level";

constexpr auto move_right = "mnk_move_right";
constexpr auto move_left = "mnk_move_left";

constexpr auto move_up = "mnk_move_up";
constexpr auto move_down = "mnk_move_down";

constexpr auto daelking = "mnk_daelking";

constexpr auto all = {
	pause_menu,
	restart_level,
	move_right,
	move_left,
	move_up,
	move_down,
	daelking,
};

} //namespace mnk

namespace joypad {
constexpr auto prefix = "joypad_";

constexpr auto pause_menu = "joypad_pause_menu";
constexpr auto restart_level = "joypad_restart_level";

constexpr auto move_right = "joypad_move_right";
constexpr auto move_left = "joypad_move_left";

constexpr auto move_up = "joypad_move_up";
constexpr auto move_down = "joypad_move_down";

constexpr auto daelking = "joypad_daelking";

constexpr auto joystick_r_right = "joystick_r_right";
constexpr auto joystick_r_left = "joystick_r_left";
constexpr auto joystick_r_up = "joystick_r_up";
constexpr auto joystick_r_down = "joystick_r_down";

constexpr auto all = {
	pause_menu,
	restart_level,
	move_right,
	move_left,
	move_up,
	move_down,
	daelking,
	joystick_r_right,
	joystick_r_left,
	joystick_r_up,
	joystick_r_down,
};

} //namespace joypad

} //namespace inputaction
