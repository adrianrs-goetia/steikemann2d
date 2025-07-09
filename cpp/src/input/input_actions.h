#pragma once

#include <initializer_list>

namespace inputaction {

namespace base {

inline constexpr auto pause_menu = "pause_menu";
inline constexpr auto restart_level = "restart_level";

inline constexpr auto move_right = "move_right";
inline constexpr auto move_left = "move_left";

inline constexpr auto move_up = "move_up";
inline constexpr auto move_down = "move_down";

inline constexpr auto daelking = "daelking";
} //namespace base

namespace mnk {
inline constexpr auto prefix = "mnk_";

inline constexpr auto pause_menu = ("%s%s", prefix, base::pause_menu);
inline constexpr auto restart_level = ("%s%s", prefix, base::restart_level);

inline constexpr auto move_right = ("%s%s", prefix, base::move_right);
inline constexpr auto move_left = ("%s%s", prefix, base::move_left);

inline constexpr auto move_up = ("%s%s", prefix, base::move_up);
inline constexpr auto move_down = ("%s%s", prefix, base::move_down);

inline constexpr auto daelking = ("%s%s", prefix, base::daelking);

inline constexpr auto all = {
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
inline constexpr auto prefix = "joypad_";

inline constexpr auto pause_menu = ("%s%s", prefix, base::pause_menu);
inline constexpr auto restart_level = ("%s%s", prefix, base::restart_level);

inline constexpr auto move_right = ("%s%s", prefix, base::move_right);
inline constexpr auto move_left = ("%s%s", prefix, base::move_left);

inline constexpr auto move_up = ("%s%s", prefix, base::move_up);
inline constexpr auto move_down = ("%s%s", prefix, base::move_down);

inline constexpr auto daelking = ("%s%s", prefix, base::daelking);

inline constexpr auto joystick_r_right = "joystick_r_right";
inline constexpr auto joystick_r_left = "joystick_r_left";
inline constexpr auto joystick_r_up = "joystick_r_up";
inline constexpr auto joystick_r_down = "joystick_r_down";

inline constexpr auto all = {
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
