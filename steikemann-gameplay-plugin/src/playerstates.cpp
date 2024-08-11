#include <character/playerstates.h>

// PlayerOnGroundState
StateReturn PlayerOnGroundState::enter_state() {
	// Immediate jump when entering while having just pressed jump
	if (m_context->input.last_valid_input_action.action == EInputAction::JUMP &&
			m_context->input.input_action.received_input_within_timeframe(0.1)) {
		m_context->physics.velocity.y += 8.0f;
		return StateReturn{
			EStateReturn::NEW_STATE,
			new PlayerInAirState(this, true)
		};
	}
	return {};
}
StateReturn PlayerOnGroundState::physics_process(real_t delta) {
	m_context->physics.velocity.y -= 9.81f * delta;
	return {};
}
StateReturn PlayerOnGroundState::handle_input() {
	if (m_context->input.input_action.action == EInputAction::JUMP) {
		m_context->physics.velocity.y += 8.0f;
		return StateReturn{
			EStateReturn::NEW_STATE,
			new PlayerInAirState(this, false)
		};
	}
	return {};
}

// PlayerInAirState
StateReturn PlayerInAirState::physics_process(real_t delta) {
	if (m_context->physics.is_on_ground) {
		if (!m_guarantee_one_frame) {
			return StateReturn{
				EStateReturn::NEW_STATE,
				new PlayerOnGroundState(this, false)
			};
		}
	}
	m_context->physics.velocity.y -= 9.81f * delta;
	return {};
}
