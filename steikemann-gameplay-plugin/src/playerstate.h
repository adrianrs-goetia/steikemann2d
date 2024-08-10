
#ifndef GD_PLAYERSTATE_PLUGINSTEIKEMANNGAMEPLAY_H
#define GD_PLAYERSTATE_PLUGINSTEIKEMANNGAMEPLAY_H

#include <state.h>

// Forward declarations
class IdleState;
class WalkState;

// States
class IdleState : public State
{
public:
    CLASS_NAME(IdleState)
    State::StateReturn process(double delta) override{ return State::StateReturn(); }
    State::StateReturn handle_input(const godot::Ref<godot::InputEvent> &p_event) override;
};

class WalkState : public State
{
public:
    CLASS_NAME(WalkState)
    State::StateReturn process(double delta) override{ return State::StateReturn(); }

    State::StateReturn handle_input(const godot::Ref<godot::InputEvent> &p_event) override;
};

#endif // GD_PLAYERSTATE_PLUGINSTEIKEMANNGAMEPLAY_H