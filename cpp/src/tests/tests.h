#pragma once

#include "example/examplenode_test.h"
#include "godot_cpp/core/class_db.hpp"
#include "godot_cpp/variant/packed_string_array.hpp"
#include "godot_cpp/variant/string.hpp"
#include "log.h"
#include "typedef.h"

namespace tests {

/**
 * No running tests by default. Pass '--runtests=yes' as a cmd line user argument to run the tests.
 */
inline auto should_run(const godot::PackedStringArray& cmd_line_args) -> bool {
	for (const auto& arg : cmd_line_args) {
		godot::PackedStringArray key_value;
		if (arg.contains("=")) {
			key_value = arg.split("=");
		}
		else {
			key_value = arg.split(" ");
		}
		key_value[0] = key_value[0].trim_prefix("--");

		if (key_value.size() != 2) {
			LOG_ERROR("Cmd arg: {}, failed to be properly split into key/value pair.", arg.utf8().get_data());
			continue;
		}
		if (key_value[0] == "runtests" && key_value[1] == "yes") {
			return true;
		}
	}
	return false;
}

inline auto register_classes() -> std::vector<TestSuite*> {
	godot::ClassDB::register_abstract_class<TestSuite>();

	// Example test case
	godot::ClassDB::register_internal_class<ExampleNode>();
	godot::ClassDB::register_internal_class<ExampleNodeTestSuite>();

	return { memnew(ExampleNodeTestSuite) };
}

inline void run(std::vector<TestSuite*>& t_testsuites) {
	for (auto* test_suite : t_testsuites) {
		for (auto* test_case : test_suite->get_tests()) {
			test_case->start_test();
			test_case->test_body();
			test_case->end_test();
		}
		// Expected to free memory for test_cases in destructor
		memfree(test_suite);
	}
}

} //namespace tests
