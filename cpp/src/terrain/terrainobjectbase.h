#pragma once

#include "macros.h"
#include "utils/visuallayers.h"

#include "godot_cpp/classes/collision_shape3d.hpp"
#include "godot_cpp/classes/global_constants.hpp"
#include "godot_cpp/classes/mesh_instance3d.hpp"
#include "godot_cpp/classes/static_body3d.hpp"
#include "godot_cpp/variant/node_path.hpp"
#include "godot_cpp/variant/vector3.hpp"

/**
 * This node assumes that all transforms are effectively done in global space
 * e.g. node path is somewhat equivalent to /root/<gameplay_scene>/TerrainObjectBase
 * Or at least that it is not grouped under a node with non-zeroed transforms
 */

class TerrainObjectBase : public godot::StaticBody3D {
	GDCLASS(TerrainObjectBase, godot::StaticBody3D)

	PROPERTY_CUSTOM_SET(
		EVisualLayer, visual_layer,
		{
			m_visual_layer = value;

			if (!is_node_ready()) {
				return;
			}
			set_depth(value);
		},
		EVisualLayer::GAMEPLAY);

public:
	static void _bind_methods() {
		BIND_METHOD(TerrainObjectBase, set_depth);
		BIND_PROPERTY_METHODS(TerrainObjectBase, visual_layer, INT, godot::PROPERTY_HINT_ENUM, g_visuallayer_cstr);
	}

	void _enter_tree() override {
		set_notify_transform(false);

		// Expect to find both of these nodes
		auto name = godot::MeshInstance3D::get_class_static();
		get_node<godot::MeshInstance3D>(godot::NodePath(name));
		name = godot::CollisionShape3D::get_class_static();
		get_node<godot::CollisionShape3D>(godot::NodePath(name));

		call_deferred("set_depth", get_visual_layer());
	}

	void _notification(int what) {
		if (what == godot::Node3D::NOTIFICATION_LOCAL_TRANSFORM_CHANGED) {
			if (!on_visual_layer_depth(get_visual_layer())) {
				call_deferred("set_depth", get_visual_layer());
			}
		}
	}

private:
	void set_depth(EVisualLayer layer) {
		set_global_position(godot::Vector3( //
			get_global_position().x,
			get_global_position().y,
			get_visual_layer_depth(layer)));
	}

	bool on_visual_layer_depth(EVisualLayer layer) {
		return get_visual_layer_depth(layer) == get_global_position().z;
	}
};
