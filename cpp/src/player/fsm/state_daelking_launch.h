#pragma once

#include "typedef.h"
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

		const auto launch_direction = get_launch_direction(c);
		c.character.set_velocity(launch_direction * m_impulse_strength);

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
	auto get_launch_direction(const Context& c) -> godot::Vector3 {
		const auto x_value = [&c] -> float
		{
			if (c.input.move_left.pressed()) {
				return -1.f;
			}
			else if (c.input.move_right.pressed()) {
				return 1.f;
			}
			else {
				return 0.f;
			}
		}();
		const auto y_value = 1.f;
		return godot::Vector3(x_value, y_value, 0.f);
	}
};
