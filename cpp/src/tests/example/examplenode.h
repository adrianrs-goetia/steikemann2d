#pragma once

#include "log.h"
#include "macros.h"

#include "godot_cpp/classes/node.hpp"

class ExampleNode : public godot::Node {
	GDCLASS(ExampleNode, godot::Node)
public:
	static void _bind_methods() {
		BIND_METHOD(ExampleNode, simple_call);
	}

	void simple_call() {
		LOG_INFO("string, ..... !!!!!!!");
	}
};
