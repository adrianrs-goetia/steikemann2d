#pragma once

#include "godot_cpp/classes/object.hpp"
#include "macros.h"

#include <vector>

struct TestCase {
	virtual auto start_test() -> void = 0;
	virtual auto test_body() -> void = 0;
	virtual auto end_test() -> void = 0;
};

class TestSuite : public godot::Object {
	GDCLASS(TestSuite, godot::Object)
public:
	static void _bind_methods() {}

	virtual auto get_tests() -> std::vector<TestCase*> = 0;
};

#define TEST_BODY(t_classname)                                                                                         \
	GDCLASS(t_classname, TestSuite)                                                                                    \
public:                                                                                                                \
	static void _bind_methods() {}                                                                                     \
	virtual ~t_classname() {                                                                                           \
		for (auto* test_case : get_tests()) {                                                                          \
			memfree(test_case);                                                                                        \
		}                                                                                                              \
	}                                                                                                                  \
	virtual auto get_tests() -> std::vector<TestCase*> override

#define TEST_CASE(t_classname, t_testcase, t_testbody)                                                                 \
	class t_testcase : public TestCase {                                                                               \
		auto start_test() -> void override {                                                                           \
			LOG_INFO("{}_{} -- Start", TOSTRING(t_classname), TOSTRING(t_testcase));                                   \
		}                                                                                                              \
		auto test_body() -> void override t_testbody;                                                                  \
		auto end_test() -> void override {                                                                             \
			LOG_INFO("{}_{} -- End", TOSTRING(t_classname), TOSTRING(t_testcase));                                     \
		}                                                                                                              \
	};

#define FAIL(t_message, ...)                                                                                           \
	LOG_ERROR(t_message __VA_OPT__(, ) __VA_ARGS__);                                                                   \
	return
