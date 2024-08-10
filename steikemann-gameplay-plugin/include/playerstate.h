
#ifndef GD_PLAYERSTATE_PLUGIN_STEIKEMANNGAMEPLAY_H
#define GD_PLAYERSTATE_PLUGIN_STEIKEMANNGAMEPLAY_H

#include <fsm/state.h>

// Forward declarations
class IdleState;
class WalkState;

// States
class IdleState : public State {
public:
	CLASS_NAME(IdleState)
	State::StateReturn process(real_t delta) override { return State::StateReturn(); }
	State::StateReturn handle_input(const godot::Ref<godot::InputEvent>& p_event) override;
};

class WalkState : public State {
public:
	CLASS_NAME(WalkState)
	State::StateReturn process(real_t delta) override { return State::StateReturn(); }

	State::StateReturn handle_input(const godot::Ref<godot::InputEvent>& p_event) override;
};

#endif // GD_PLAYERSTATE_PLUGIN_STEIKEMANNGAMEPLAY_H