
#ifndef GD_PLAYERSTATE_PLUGIN_STEIKEMANNGAMEPLAY_H
#define GD_PLAYERSTATE_PLUGIN_STEIKEMANNGAMEPLAY_H

#include <core/core.h>
#include <fsm/state.h>

// Data som en playerstate alltid er interessert i
//  Forer inn en ny struct som dette i hver _physics_process
struct PlayerPhysicalContext {
	bool is_on_ground;
	Vector3 position;
	Vector3 velocity;
};

class PlayerState : public State {
public:
#define PLAYER_STATE_CONSTRUCTORS(classname)                                             \
	classname(PlayerState* state) :                                                      \
			PlayerState(state) {}                                                        \
	classname(PlayerPhysicalContext* player_context, StateInputContext* input_context) : \
			PlayerState(player_context, input_context) {}

	PlayerState(PlayerState* state) :
			m_player_physical_context(state->m_player_physical_context), m_state_input_context(state->m_state_input_context) {}
	PlayerState(PlayerPhysicalContext* player_context, StateInputContext* input_context) :
			m_player_physical_context(player_context), m_state_input_context(input_context) {}

	PlayerPhysicalContext* m_player_physical_context = nullptr;
	StateInputContext* m_state_input_context = nullptr;
};

// statuses: Free, HoldingEnemy
// states: OnGround, Falling, HoldingEnemy_OnGround, HoldingEnemy_Falling
class PlayerFSM : public FSM {
public:
	// Sets the pointers for the FSM so it can pass them along to the given states
	// Memory is allocated and freed by the body/entity governing the FSM
	void init(PlayerPhysicalContext* player_context, StateInputContext* input_context);
	void uninit();

	template <typename T>
	void force_set_state() {
		static_assert(std::is_base_of_v<PlayerState, T>, "FSM requires class State as base");
		if (m_current_state) {
			delete m_current_state;
			m_current_state = nullptr;
		}
		// m_current_state = new T(player_physical_context, state_input_context);
		m_current_state = new T(player_physical_context, state_input_context);
		assert(m_current_state != nullptr);
	}

	PlayerPhysicalContext* player_physical_context = nullptr;
	StateInputContext* state_input_context = nullptr;
};

// Forward declare
class PlayerOnGroundState;
class PlayerInAirState;

class PlayerOnGroundState : public PlayerState {
public:
	PLAYER_STATE_CONSTRUCTORS(PlayerOnGroundState)
	CLASS_NAME(PlayerOnGroundState)

	StateReturn process(real_t delta) override { return State::StateReturn(); }
	StateReturn physics_process(real_t delta) override;
	StateReturn handle_input() override;
};

class PlayerInAirState : public PlayerState {
public:
	PLAYER_STATE_CONSTRUCTORS(PlayerInAirState)
	CLASS_NAME(PlayerInAirState)

	StateReturn process(real_t delta) override { return State::StateReturn(); }
	StateReturn physics_process(real_t delta) override;
	StateReturn handle_input() override { return State::StateReturn(); }
};

#endif // GD_PLAYERSTATE_PLUGIN_STEIKEMANNGAMEPLAY_H