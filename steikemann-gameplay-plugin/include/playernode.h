#ifndef GD_PLAYERNODE_PLUGIN_STEIKEMANNGAMEPLAY_H
#define GD_PLAYERNODE_PLUGIN_STEIKEMANNGAMEPLAY_H

#include <godot_cpp/classes/character_body3d.hpp>
#include <core/macroes.h>

using namespace godot;

class PlayerNode : public CharacterBody3D
{
    GDCLASS(PlayerNode, CharacterBody3D)
public:
    GETNAME(PlayerNode)
    static void _bind_methods();

    // void _ready() override;
    // void _process(double delta) override;
    // void _physics_process(double delta) override;
};

#endif // GD_PLAYERNODE_PLUGIN_STEIKEMANNGAMEPLAY_H