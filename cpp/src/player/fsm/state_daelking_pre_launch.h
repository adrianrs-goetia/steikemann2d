#pragma once

#include "typedef.h"
#include <log.h>
#include <macros.h>

#include <godot_cpp/classes/packed_scene.hpp>
#include <godot_cpp/classes/shape3d.hpp>

class DaelkingPreLaunchState : public PlayerStateBase {
	GDCLASS(DaelkingPreLaunchState, PlayerStateBase)

	PROPERTY(float, daelking_impulse_strength, 8.0f);
	PROPERTY(bool, daelk_impulse, false);
	PROPERTY(godot::Ref<godot::Shape3D>, daelking_collision_shape);

	PROPERTY(godot::Ref<godot::PackedScene>, arrow_scene);

public:
	static void _bind_methods() {
		BIND_PROPERTY_METHODS(DaelkingPreLaunchState, daelking_impulse_strength, FLOAT);

		using godot::Shape3D, godot::PackedScene;
		BIND_RESOURCE_PROPERTY_METHODS(DaelkingPreLaunchState, daelking_collision_shape, Shape3D);
		BIND_RESOURCE_PROPERTY_METHODS(DaelkingPreLaunchState, arrow_scene, PackedScene);
	}

	virtual auto enter(Context&) -> std::optional<TransitionContext> override {
		return {};
	}

	virtual auto exit(Context&) -> void override {
		return;
	}

	virtual auto process(Context&, double) -> std::optional<TransitionContext> override {
		return {};
	}

	virtual auto physics_process(Context&, double) -> std::optional<TransitionContext> override {
		return {};
	}

	virtual auto handle_input(Context&, const InputState& input) -> std::optional<TransitionContext> override {
		if (input.daelking.just_released()) {
			return TransitionContext{ .state = EState::WALKING };
		}
		return {};
	}
};
