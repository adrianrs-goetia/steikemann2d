#pragma once

#include <godot_cpp/classes/character_body3d.hpp>

namespace player_character{

class PlayerCharacterBody : public godot::CharacterBody3D {
    GDCLASS(PlayerCharacterBody, godot::CharacterBody3D)

private:

public:
    static void _bind_methods() {}
};

}