#include <character/playerfsm.h>

void PlayerFSM::init(PlayerPhysicalContext* player_context, StateInputContext* input_context) {
	_enter_tree();
	state_input_context = input_context;
	player_physical_context = player_context;
	assert(
			state_input_context != nullptr &&
			player_physical_context != nullptr);
}
void PlayerFSM::uninit() {
	_exit_tree();
	state_input_context = nullptr;
	player_physical_context = nullptr;
}

void PlayerFSM::deferred_actions() {
	if (m_current_state) {
		static_cast<PlayerState*>(m_current_state)->deferred_actions();
	}
}

// STATES ---------------------------------------------
// PlayerOnGroundState
State::StateReturn PlayerOnGroundState::enter_state() {
	// Immediate jump when entering while having just pressed jump
	if (m_state_input_context->last_valid_input_action.action == EInputAction::JUMP &&
			m_state_input_context->input_action.received_input_within_timeframe(0.1)) {
		m_player_physical_context->velocity.y += 8.0f;
		return StateReturn{
			EnumStateReturn::NEW_STATE,
			new PlayerInAirState(this, true)
		};
	}
	return StateReturn();
}
State::StateReturn PlayerOnGroundState::physics_process(real_t delta) {
	m_player_physical_context->velocity.y -= 9.81f * delta;
	return StateReturn();
}
State::StateReturn PlayerOnGroundState::handle_input() {
	if (m_state_input_context->input_action.action == EInputAction::JUMP) {
		m_player_physical_context->velocity.y += 8.0f;
		return StateReturn{
			EnumStateReturn::NEW_STATE,
			new PlayerInAirState(this, false)
		};
	}
	return StateReturn();
}

// PlayerInAirState
State::StateReturn PlayerInAirState::physics_process(real_t delta) {
	if (m_player_physical_context->is_on_ground) {
		if (!m_guarantee_one_frame) {
			return StateReturn{
				EnumStateReturn::NEW_STATE,
				new PlayerOnGroundState(this, false)
			};
		}
	}
	m_player_physical_context->velocity.y -= 9.81f * delta;
	return StateReturn();
}
