#pragma once

#include <cstdlib>

#include <input/input_manager.h>
#include <input/input_state.h>
#include <log.h>
#include <macros.h>

#include "fsm/state_daelking_launch.h"
#include "fsm/state_daelking_pre_launch.h"
#include "fsm/state_walking.h"
#include "fsm/typedef.h"

#include <godot_cpp/classes/character_body3d.hpp>
#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/classes/node3d.hpp>
#include <godot_cpp/classes/resource.hpp>

/**
 * Movement:
 *      - Walking
 *      - Sick Heelies
 *      - Daelking
 *      - Landing
 *
 * This component acts as the Finite State Machine for the player movement
 */
class MovementComponent : public godot::Node3D {
	GDCLASS(MovementComponent, godot::Node3D)

	std::unique_ptr<Context> m_context;
	godot::Ref<PlayerStateBase> m_current_state;

	PROPERTY(godot::Ref<WalkingState>, walking_state);
	PROPERTY(godot::Ref<DaelkingPreLaunchState>, daelking_pre_launch_state);
	PROPERTY(godot::Ref<DaelkingLaunchState>, daelking_launch_state);

public:
	static void _bind_methods() {
		BIND_RESOURCE_PROPERTY_METHODS(MovementComponent, walking_state, WalkingState);
		BIND_RESOURCE_PROPERTY_METHODS(MovementComponent, daelking_pre_launch_state, DaelkingPreLaunchState);
		BIND_RESOURCE_PROPERTY_METHODS(MovementComponent, daelking_launch_state, DaelkingLaunchState);
	}

	void _enter_tree() override {
		set_name(get_class());

		GAME_SCOPE {
			// Node hierarchy is expected to be
			// CharacterBody3D/MovementComponent
			if (auto* character = cast_to<godot::CharacterBody3D>(get_owner())) {
				m_context = std::make_unique<Context>(Context{ .owner = *this, .character = *character });
			}
			else {
				LOG_ERROR("{} failed to get_owner as CharacterBody3D.", str(get_path()));
			}

			if (auto* im = get_node<InputManager>(InputManager::get_path())) {
				im->register_input_callback(get_path(), [this](const InputState& i) { this->input_callback(i); });
			}

			// Insantiate defaults if necessary
			if (m_walking_state.is_null()) {
				m_walking_state.instantiate();
				LOG_WARN("{} missing walking_state. Using default.", str(get_path()));
			}
			if (m_daelking_pre_launch_state.is_null()) {
				m_daelking_pre_launch_state.instantiate();
				LOG_WARN("{} missing daelking_pre_launch_state_state. Using default.", str(get_path()));
			}
			if (m_daelking_launch_state.is_null()) {
				m_daelking_launch_state.instantiate();
				LOG_WARN("{} missing daelking_launch_state_state. Using default.", str(get_path()));
			}

			// Enter default state
			process_state_return(TransitionContext{ .state = EState::WALKING });
		}
	}

	void _exit_tree() override {
		GAME_SCOPE {
			m_current_state->exit(*m_context);
		}
	}

	auto _process(double delta) -> void override {
		if (m_current_state.is_valid()) {
			process_state_return(m_current_state->process(*m_context, delta));
		}
	}
	auto _physics_process(double delta) -> void override {
		if (m_current_state.is_valid()) {
			process_state_return(m_current_state->physics_process(*m_context, delta));
		}
	}
	auto input_callback(const InputState& input) -> void {
		m_context->input = input;
		if (m_current_state.is_valid()) {
			process_state_return(m_current_state->input_callback(*m_context));
		}
	}

private:
	auto process_state_return(std::optional<TransitionContext> transition_opt) -> void {
		if (!transition_opt) {
			return;
		}

		auto next_state = get_next_state(transition_opt.value().state);
		if (!next_state->can_enter(*m_context)) {
			LOG_TRACE("{} cannot enter state {}", str(get_path()), ::to_string(transition_opt.value().state));
			return;
		}

		if (m_current_state.is_valid()) {
			m_current_state->exit(*m_context);
		}
		m_current_state = next_state;
		LOG_TRACE("{} entering state {}", str(get_path()), ::to_string(transition_opt.value().state));
		process_state_return(m_current_state->enter(*m_context));
	}

	auto get_next_state(EState state) -> godot::Ref<PlayerStateBase> {
		switch (state) {
			case EState::WALKING:
				return m_walking_state;
			case EState::DAELKING_PRE_LAUNCH:
				return m_daelking_pre_launch_state;
			case EState::DAELKING_LAUNCH:
				return m_daelking_launch_state;
		}

		LOG_ERROR("{} get_next_state should never get here!!", str(get_path()));
		abort();
		return {}; // make the compiler happy...
	}
};
