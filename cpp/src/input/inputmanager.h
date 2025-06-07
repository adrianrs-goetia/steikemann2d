#pragma once

#include <godot_cpp/classes/input_event.hpp>
#include <godot_cpp/classes/node.hpp>

class InputManager : public godot::Node {
	GDCLASS(InputManager, godot::Node)

public:
	static void _bind_methods() {}
};
