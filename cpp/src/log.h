#pragma once

#include <fmt/format.h>

namespace LogColor {
inline const char* color_default = "0";
inline const char* color_fg_gray = "90";
inline const char* color_fg_green = "32";
inline const char* color_fg_yellow = "33";
inline const char* color_fg_red = "31";
} //namespace LogColor

#define LOG_TRACE(string, ...)                                                                                         \
	printf("\033[%sm%s \n\033[%sm",                                                                                    \
		LogColor::color_fg_gray,                                                                                       \
		fmt::format(string __VA_OPT__(, ) __VA_ARGS__).c_str(),                                                        \
		LogColor::color_default)

#define LOG_INFO(string, ...)                                                                                          \
	printf("\033[%sm%s \n\033[%sm",                                                                                    \
		LogColor::color_fg_green,                                                                                      \
		fmt::format(string __VA_OPT__(, ) __VA_ARGS__).c_str(),                                                        \
		LogColor::color_default)

#define LOG_WARN(string, ...)                                                                                          \
	printf("\033[%sm%s \n\033[%sm",                                                                                    \
		LogColor::color_fg_yellow,                                                                                     \
		fmt::format(string __VA_OPT__(, ) __VA_ARGS__).c_str(),                                                        \
		LogColor::color_default)

#define LOG_ERROR(string, ...)                                                                                         \
	printf("\033[%sm%s \n\033[%sm",                                                                                    \
		LogColor::color_fg_red,                                                                                        \
		fmt::format(string __VA_OPT__(, ) __VA_ARGS__).c_str(),                                                        \
		LogColor::color_default)
