#ifndef GD_PLAYERNODE_PLUGIN_STEIKEMANNGAMEPLAY_H
#define GD_PLAYERNODE_PLUGIN_STEIKEMANNGAMEPLAY_H

#include <core/macroes.h>
#include <fsm/state.h>
#include <godot_cpp/classes/character_body3d.hpp>

using namespace godot;

class PlayerNode : public CharacterBody3D {
	GDCLASS(PlayerNode, CharacterBody3D)
public:
	GETNAME(PlayerNode)
	static void _bind_methods();

	void _enter_tree() override;
	void _exit_tree() override;
	void _process(float delta) override;
	void _physics_process(float delta) override;
	virtual void _unhandled_input(const Ref<InputEvent>& p_event) override;

	FSM m_fsm;
};

#endif // GD_PLAYERNODE_PLUGIN_STEIKEMANNGAMEPLAY_H