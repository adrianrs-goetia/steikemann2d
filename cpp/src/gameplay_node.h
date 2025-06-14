#pragma once

#include <events/daelk_event.h>
#include <log.h>

#include <godot_cpp/classes/node3d.hpp>

class GameplayNode3D : public godot::Node3D {
	GDCLASS(GameplayNode3D, godot::Node3D)

public:
	static void _bind_methods() {}

	auto handle_daelk_event(const DaelkEvent& event) -> void {
		LOG_TRACE("{} received daelk event", str(get_path()));
	}
};
