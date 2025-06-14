#pragma once

#include <type_traits>

#include <fmt/format.h>
#include <godot_cpp/variant/node_path.hpp>
#include <godot_cpp/variant/string.hpp>
#include <godot_cpp/variant/string_name.hpp>
#include <godot_cpp/variant/vector2.hpp>
#include <godot_cpp/variant/vector3.hpp>

namespace LogColor {
inline const char* color_default = "0";
inline const char* color_fg_gray = "90";
inline const char* color_fg_green = "32";
inline const char* color_fg_yellow = "33";
inline const char* color_fg_red = "31";
} //namespace LogColor

/**
 * Util function for easy conversion to C string
 */
template <typename T>
const char* str(T t) {
	if constexpr (std::is_same_v<T, godot::String>) {
		return t.utf8().get_data();
	}
	if constexpr (std::is_same_v<T, godot::StringName> || std::is_same_v<T, godot::NodePath>) {
		return godot::String(t).utf8().get_data();
	}
	if constexpr (std::is_same_v<T, godot::Vector2> || std::is_same_v<T, godot::Vector3>) {
		return godot::String(t).utf8().get_data();
	}
}

#define LOG(color, string, ...)                                                                                        \
	printf("\033[%sm%s \033[%sm\n",                                                                                    \
		color,                                                                                                         \
		fmt::format(string __VA_OPT__(, ) __VA_ARGS__).c_str(),                                                        \
		LogColor::color_default)

#define LOG_TRACE(string, ...) LOG(LogColor::color_fg_gray, string, __VA_ARGS__)

#define LOG_INFO(string, ...) LOG(LogColor::color_fg_green, string, __VA_ARGS__)

#define LOG_WARN(string, ...) LOG(LogColor::color_fg_yellow, string, __VA_ARGS__)

#define LOG_ERROR(string, ...) LOG(LogColor::color_fg_red, string, __VA_ARGS__)
