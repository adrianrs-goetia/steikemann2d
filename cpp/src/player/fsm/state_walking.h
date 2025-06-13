#pragma once

#include "typedef.h"
#include <log.h>
#include <macros.h>

class WalkingState : public PlayerStateBase {
	GDCLASS(WalkingState, PlayerStateBase)

	PROPERTY(int, movement_direction, 0);
	PROPERTY(float, walking_speed, 450.f);
	PROPERTY(float, gravity_scale, 2.f);

public:
	static void _bind_methods() {
		BIND_PROPERTY_METHODS(WalkingState, gravity_scale, FLOAT);
		BIND_PROPERTY_METHODS(WalkingState, walking_speed, FLOAT);
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
		if (input.daelking.just_pressed()) {
			return TransitionContext{ .state = EState::DAELKING_PRE_LAUNCH };
		}
		return {};
	}
};
