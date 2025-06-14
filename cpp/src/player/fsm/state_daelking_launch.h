#pragma once

#include "typedef.h"
#include "utils.h"
#include <events/daelk_event.h>
#include <gameplay_node.h>
#include <input/typedef.h>
#include <log.h>
#include <macros.h>
#include <timestamp.h>

#include <godot_cpp/classes/packed_scene.hpp>
#include <godot_cpp/classes/time.hpp>

class DaelkingLaunchState : public PlayerStateBase {
	GDCLASS(DaelkingLaunchState, PlayerStateBase)

private:
	PROPERTY(float, launch_time, 1.0f);
	PROPERTY(float, impulse_strength, 8.0f);
	PROPERTY(float, gravity_scale, 1.0f);

	TimeStamp m_launch_timestamp;

public:
	static void _bind_methods() {
		BIND_PROPERTY_METHODS(DaelkingLaunchState, launch_time, FLOAT);
		BIND_PROPERTY_METHODS(DaelkingLaunchState, impulse_strength, FLOAT);
		BIND_PROPERTY_METHODS(DaelkingLaunchState, gravity_scale, FLOAT);
	}

	virtual auto enter(Context& c) -> std::optional<TransitionContext> override {
		m_launch_timestamp.stamp();

		const auto launch_direction = get_daelking_direction(c.input);
		c.character.set_velocity(launch_direction * m_impulse_strength);

		if (c.daelked_node_path.has_value()) {
			send_daelking_launch_event(c.owner, c.input, c.daelked_node_path.value());
			c.daelked_node_path = {}; // Consume node_path
		}

		return {};
	}

	virtual auto exit(Context& c) -> void override {
		return;
	}

	virtual auto process(Context&, double) -> std::optional<TransitionContext> override {
		return {};
	}

	virtual auto physics_process(Context& c, double delta) -> std::optional<TransitionContext> override {
		if (!m_launch_timestamp.in_range(m_launch_time)) {
			return TransitionContext{ .state = EState::WALKING };
		}
		auto velocity = c.character.get_velocity();
		velocity.y += c.character.get_gravity().y * delta * m_gravity_scale;
		c.character.set_velocity(velocity);
		c.character.move_and_slide();
		return {};
	}

	virtual auto input_callback(Context& c) -> std::optional<TransitionContext> override {
		return {};
	}

private:
	auto send_daelking_launch_event(const godot::Node& node, const InputState& input, const godot::NodePath node_path)
		-> void {
		if (auto* gameplay_node = node.get_node<GameplayNode3D>(node_path)) {
			gameplay_node->handle_daelk_launch_event(DaelkLaunchEvent{ .direction = get_daelking_direction(input) });
		}
	}
};
