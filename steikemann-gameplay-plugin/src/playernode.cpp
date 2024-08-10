#include <character/playernode.h>
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/input.hpp>

extern "C" {
#include <stdlib.h>
}

void PlayerNode::_bind_methods() {
	DEFAULT_PROPERTY(PlayerNode)
}

void PlayerNode::_enter_tree() {
	RETURN_IF_EDITOR()
	m_state_data = (StateData*)malloc(sizeof(StateData));
	assert(m_state_data != nullptr);
	m_state_data->player_physical_state.is_on_ground = is_on_floor();
	m_state_data->player_physical_state.position = get_position();
	m_state_data->player_physical_state.velocity = get_velocity();
	m_fsm.init(&m_state_data->player_physical_state, &m_state_data->state_input_context);
	m_fsm.force_set_state<PlayerInAirState>();
}

void PlayerNode::_exit_tree() {
	RETURN_IF_EDITOR()
	m_fsm.uninit();
	::free(m_state_data);
	m_state_data = nullptr;
	printf("PlayerNode exiting tree");
}

void PlayerNode::_process(float delta) {
	RETURN_IF_EDITOR()
}

void PlayerNode::_physics_process(float delta) {
	RETURN_IF_EDITOR()
	assert(m_state_data != nullptr);
	// capture current physics context
	m_state_data->player_physical_state.is_on_ground = is_on_floor();
	m_state_data->player_physical_state.position = get_position();
	m_state_data->player_physical_state.velocity = get_velocity();

	// Let FSM deal with physics and input context
	m_fsm.physics_process(delta);
	m_fsm.handle_input();

	// set data from context
	set_velocity(m_state_data->player_physical_state.velocity);
	move_and_slide();
	auto v = get_position();
	set_position(Vector3(v.x, v.y, 0));
}

void PlayerNode::_input(const Ref<InputEvent>& p_event) {
	RETURN_IF_EDITOR()
	if (m_state_data) {
		handle_input();
	}
}

void PlayerNode::handle_input() {
	godot::Input* input = godot::Input::get_singleton();
	m_state_data->state_input_context.input_direction = input->get_vector(
			InputMap::move_left,
			InputMap::move_right,
			InputMap::ui_down,
			InputMap::ui_up,
			0.05);
	if (input->is_action_pressed(InputMap::jump)) {
		m_state_data->state_input_context.input_action = InputAction::JUMP;
	}
	else {
		m_state_data->state_input_context.input_action = InputAction::NONE;
	}
}