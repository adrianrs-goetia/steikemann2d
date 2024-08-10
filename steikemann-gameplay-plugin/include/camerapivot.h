#ifndef GD_CAMERAPIVOT_PLUGIN_STEIKEMANNGAMEPLAY_H
#define GD_CAMERAPIVOT_PLUGIN_STEIKEMANNGAMEPLAY_H

#include <core/macroes.h>
#include <godot_cpp/classes/node3d.hpp>

using namespace godot;

class PlayerNode;

class CameraPivot : public Node3D {
	GDCLASS(CameraPivot, Node3D)

public:
	GETNAME(CameraPivot)
	static void _bind_methods();

	void _enter_tree() override;
	void _physics_process(real_t delta) override;

	PlayerNode* m_playernode = nullptr;
};

#endif // GD_CAMERAPIVOT_PLUGIN_STEIKEMANNGAMEPLAY_H