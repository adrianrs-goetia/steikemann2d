#ifndef GD_CAMERAPIVOT_PLUGIN_STEIKEMANNGAMEPLAY_H
#define GD_CAMERAPIVOT_PLUGIN_STEIKEMANNGAMEPLAY_H

#include <godot_cpp/classes/node3d.hpp>
#include <core/macroes.h>

using namespace godot;

class PlayerNode;

class CameraPivot : public Node3D
{
    GDCLASS(CameraPivot, Node3D)

public:
    GETNAME(CameraPivot)
    static void _bind_methods();

    void _enter_tree() override;
    void _physics_process(double delta) override;

    PlayerNode* m_playernode = nullptr;
};

#endif // GD_CAMERAPIVOT_PLUGIN_STEIKEMANNGAMEPLAY_H