#pragma once

#include "macros.h"
#include "resources/visuallayers.h"

#include "godot_cpp/classes/collision_shape3d.hpp"
#include "godot_cpp/classes/mesh_instance3d.hpp"
#include "godot_cpp/classes/static_body3d.hpp"
#include "godot_cpp/variant/callable_method_pointer.hpp"
#include "godot_cpp/variant/node_path.hpp"
#include "godot_cpp/variant/vector3.hpp"

/**
 * This node assumes that all transforms are effectively done in global space
 * e.g. node path is somewhat equivalent to /root/<gameplay_scene>/TerrainObjectBase
 * Or at least that it is not grouped under a node with non-zeroed transforms
 */

class TerrainObjectBase : public godot::StaticBody3D {
	GDCLASS(TerrainObjectBase, godot::StaticBody3D)

public:
	static void _bind_methods() {
		BIND_METHOD(TerrainObjectBase, set_depth);
	}

	void _enter_tree() override {
		set_notify_transform(false);

		// Expect to find both of these nodes
		auto name = godot::MeshInstance3D::get_class_static();
		get_node<godot::MeshInstance3D>(godot::NodePath(name));
		name = godot::CollisionShape3D::get_class_static();
		get_node<godot::CollisionShape3D>(godot::NodePath(name));

		visuallayer::connect(callable_mp(this, &TerrainObjectBase::on_visuallayerresource_update));

		call_deferred("set_depth");
	}

	void _notification(int what) {
		if (what == godot::Node3D::NOTIFICATION_TRANSFORM_CHANGED) {
			visuallayer::keep_on_mgcollision_layer(*this);
			if (!correct_rotational_transform()) {
				set_correct_rotational_transform();
			}
		}
		if (what == godot::Node::NOTIFICATION_PREDELETE) {
			visuallayer::disconnect(callable_mp(this, &TerrainObjectBase::on_visuallayerresource_update));
		}
	}

	void on_visuallayerresource_update() {
		visuallayer::keep_on_mgcollision_layer(*this);
	}

private:
	void set_depth() {
		visuallayer::keep_on_mgcollision_layer(*this);
	}

	auto correct_rotational_transform() const -> bool {
		return get_global_rotation_degrees().x == 0 && get_global_rotation_degrees().y == 0;
	}

	void set_correct_rotational_transform() {
		set_global_rotation_degrees(godot::Vector3(0.f, 0.f, get_rotation_degrees().z));
	}
};
