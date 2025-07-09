#pragma once

#include "gameplay_events/daelk_event.h"
#include "gameplay_events/gameplay_node.h"
#include "log.h"
#include "macros.h"
#include "mathstatics.h"
#include "timestamp.h"
#include "typedef.h"
#include "utils.h"

#include "godot_cpp/classes/curve.hpp"
#include "godot_cpp/classes/packed_scene.hpp"
#include "godot_cpp/variant/vector3.hpp"

class DaelkingLaunchState : public PlayerStateBase {
	GDCLASS(DaelkingLaunchState, PlayerStateBase)

private:
	PROPERTY(float, launch_time, 1.0f);
	PROPERTY(float, base_velocity_strength, 3.0f);
	PROPERTY(godot::Ref<godot::Curve>, acceleration_curve);

	TimeStamp m_launch_timestamp;

public:
	static void _bind_methods() {
		BIND_PROPERTY_METHODS(DaelkingLaunchState, launch_time, FLOAT);
		BIND_PROPERTY_METHODS(DaelkingLaunchState, base_velocity_strength, FLOAT);
		using namespace godot;
		BIND_RESOURCE_PROPERTY_METHODS(DaelkingLaunchState, acceleration_curve, Curve);
	}

	virtual auto enter(Context& c) -> std::optional<TransitionContext> override {
		m_launch_timestamp.stamp();

		c.character.set_velocity(mathstatics::vector_zero);

		if (c.daelked_node_path.has_value()) {
			send_daelking_launch_event(c.owner, c.daelk_launch_direction, c.daelked_node_path.value());
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

		const auto velocity = c.daelk_launch_direction * get_new_launch_strength();
		c.character.set_velocity(velocity);
		c.character.move_and_slide();

		return {};
	}

	virtual auto input_callback(Context& c) -> std::optional<TransitionContext> override {
		return {};
	}

private:
	auto send_daelking_launch_event(
		const godot::Node& node, const godot::Vector3& direction, const godot::NodePath node_path) -> void {
		if (auto* gameplay_node = node.get_node<GameplayNode3D>(node_path)) {
			gameplay_node->handle_daelk_launch_event(DaelkLaunchEvent{ .direction = direction });
		}
	}

	auto get_time_percentage() const {
		return std::min(1.f, m_launch_timestamp.time_since_stamp() / m_launch_time);
	}

	auto get_new_launch_strength() const -> float {
		if (m_acceleration_curve.is_null()) {
			LOG_ERROR("Missing acceleration curve on Daelk launch state");
			return 0.0;
		}

		const auto time_percentage = get_time_percentage();
		const auto velocity_strength_multiplier = m_acceleration_curve->sample(time_percentage);
		return m_base_velocity_strength * velocity_strength_multiplier;
	}

	// auto fade_lifetime_launch_velocity(const Context& c) -> godot::Vector3 {
	// 	const auto time_mod = std::min(1.f, m_launch_timestamp.time_since_stamp() / get_launch_time());
	// 	const auto strenght_mod = (get_launch_strength_fade() * time_mod) + (1.f - time_mod);
	// 	const auto current_launch_strenght = get_launch_strength() * strenght_mod;
	// 	return c.daelk_launch_direction * current_launch_strenght;
	// }
};
