#include <fsm/state.h>

#include <fsm/state.h>
#include <godot_cpp/core/class_db.hpp>

void FSM::_enter_tree() {
	// assert(_current_state != nullptr);
}

void FSM::_exit_tree() {
	if (m_current_state) {
		delete m_current_state;
		m_current_state = nullptr;
	}
	assert(m_current_state == nullptr);
}

void FSM::_process(real_t delta) {
	if (m_current_state) {
		process_state(m_current_state->process(delta));
	}
}

void FSM::_physics_process(real_t delta) {
	if (m_current_state) {
		process_state(m_current_state->physics_process(delta));
	}
}

void FSM::_unhandled_input(const godot::Ref<godot::InputEvent>& p_event) {
	if (m_current_state) {
		process_state(m_current_state->handle_input(p_event));
	}
}

void FSM::process_state(State::StateReturn state_return) {
	assert(m_current_state != nullptr);
	switch (state_return.ret_enum) {
		case State::EnumStateReturn::NONE:
		case State::EnumStateReturn::CONTINUE:
			break;
		case State::EnumStateReturn::NEW_STATE:
			if (!state_return.new_state) {
				printf("%s new state was nullptr. Abort changing state", __FUNCTION__);
				break;
			}
			m_current_state->exit_state();
			delete m_current_state;
			m_current_state = nullptr;
			m_current_state = state_return.new_state;
			process_state(m_current_state->enter_state());
			break;
		default:
			break;
	}
}
