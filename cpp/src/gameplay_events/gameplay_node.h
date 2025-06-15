#pragma once

#include "daelk_event.h"
#include "log.h"
#include "macros.h"

#include "godot_cpp/classes/area3d.hpp"
#include "godot_cpp/classes/node3d.hpp"
#include "godot_cpp/classes/timer.hpp"
#include "godot_cpp/variant/callable_method_pointer.hpp"

/**
 * Node allowing for player to send gameplay events
 */
class GameplayNode3D : public godot::Node3D {
	GDCLASS(GameplayNode3D, godot::Node3D)

	godot::Timer* m_daelked_timer = nullptr;

	PROPERTY(float, daelked_time, 1.f);

public:
	static void _bind_methods() {
		BIND_PROPERTY_METHODS(GameplayNode3D, daelked_time, FLOAT);
	}

	void _enter_tree() override {
		if (!m_daelked_timer) {
			m_daelked_timer = memnew(godot::Timer);
			add_child(m_daelked_timer);
			m_daelked_timer->set_one_shot(true);
			m_daelked_timer->connect("timeout", callable_mp(this, &GameplayNode3D::on_timeout));
		}
	}

	void _exit_tree() override {
		if (m_daelked_timer) {
			m_daelked_timer->queue_free();
			m_daelked_timer = nullptr;
		}
	}

	auto handle_daelk_pre_launch_event(const DaelkPreLaunchEvent& event) -> void {}

	auto handle_daelk_launch_event(const DaelkLaunchEvent& event) -> void {
		if (!m_daelked_timer) {
			LOG_ERROR("{} received daelk event. But has no timer allocated", str(get_path()));
			return;
		}

		m_daelked_timer->start(m_daelked_time);

		if (auto* area3d = get_node<godot::Area3D>("Area3D")) {
			area3d->set_process_mode(Node::ProcessMode::PROCESS_MODE_DISABLED);
		}
		set_visible(false);
	}

	void on_timeout() {
		LOG_TRACE("{} on_timeout", str(get_path()));

		if (auto* area3d = get_node<godot::Area3D>("Area3D")) {
			area3d->set_process_mode(Node::ProcessMode::PROCESS_MODE_INHERIT);
		}
		set_visible(true);
	}
};
