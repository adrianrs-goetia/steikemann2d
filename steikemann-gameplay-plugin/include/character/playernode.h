#ifndef GD_PLAYERNODE_PLUGIN_STEIKEMANNGAMEPLAY_H
#define GD_PLAYERNODE_PLUGIN_STEIKEMANNGAMEPLAY_H

#include <character/playerfsm.h>
#include <core/core.h>
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
	void _input(const Ref<InputEvent> &p_event);

	PlayerFSM m_fsm;
	struct StateData{
		PlayerPhysicalContext player_physical_state;
		StateInputContext state_input_context;
	};
	StateData* m_state_data = nullptr;
};

#endif // GD_PLAYERNODE_PLUGIN_STEIKEMANNGAMEPLAY_H