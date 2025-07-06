#pragma once

#include "log.h"
#include "macros.h"
#include "utils/valueinterpolatonstep.h"

#include "godot_cpp/classes/camera3d.hpp"
#include "godot_cpp/classes/wrapped.hpp"
#include "godot_cpp/variant/node_path.hpp"

class PlayerCamera : public godot::Camera3D {
	GDCLASS(PlayerCamera, godot::Camera3D)

public:
	// enum EMyEnum {
	// 	FOLLOW_PLAYER,
	// 	FOCUS_ENVIRONMENT,
	// };

	PROPERTY(godot::NodePath, player_path);

private:
	// PROPERTY(EMyEnum, myenum);

public:
	static void _bind_methods() {
		BIND_PROPERTY_METHODS(PlayerCamera, player_path, NODE_PATH);
		// BIND_PROPERTY_METHODS(
		// 	PlayerCameraRootNode, myenum, INT, godot::PROPERTY_HINT_ENUM, "FOLLOW_PLAYER, FOCUS_ENVIRONMENT");
	}

	void _enter_tree() override {
		if (m_player_path.is_empty()) {
			LOG_NEW_INFO(get_path(), " is missing path to player");
		}
	}

	void _physics_process(double delta) override {
		GAME_SCOPE {
			const auto* playernode = get_node<Node3D>(get_player_path());
			if (!playernode) {
				return;
			}

			const auto playerposition = playernode->get_global_position();
			const auto new_y = ValueInterpolation{
			.start_threshhold = 0.2,
			.smoothing_length = 1,
			}.interpolate(get_global_position().y, playerposition.y, delta);
			set_global_position(godot::Vector3(playerposition.x, new_y, get_global_position().z));
		}
	}
};

// VARIANT_ENUM_CAST(PlayerCameraRootNode::EMyEnum);
