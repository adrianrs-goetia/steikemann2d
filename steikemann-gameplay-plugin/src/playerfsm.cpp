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

// STATES ---------------------------------------------
// PlayerOnGroundState
State::StateReturn PlayerOnGroundState::physics_process(real_t delta) {
	m_player_physical_context->velocity.y -= 9.81f * delta;
	return StateReturn();
}
State::StateReturn PlayerOnGroundState::handle_input() {
	if (m_state_input_context->input_action == InputAction::JUMP) {
		m_player_physical_context->velocity.y += 8.0f;
		return StateReturn{
			EnumStateReturn::NEW_STATE,
			new PlayerInAirState(this)
		};
	}
	return StateReturn();
}

// PlayerInAirState
State::StateReturn PlayerInAirState::physics_process(real_t delta) {
	if (m_player_physical_context->is_on_ground) {
		return StateReturn{
			EnumStateReturn::NEW_STATE,
			new PlayerOnGroundState(this)
		};
	}
	m_player_physical_context->velocity.y -= 9.81f * delta;
	return StateReturn();
}
