#pragma once

/**
 * SCALE NOT WORKING AS EXPECT...
 * Abandoned test
 */

#include "godot_cpp/variant/packed_string_array.hpp"
#include "godot_cpp/variant/transform3d.hpp"
#include "godotutils.h"
#include "log.h"

#include "godot_cpp/classes/box_shape3d.hpp"
#include "godot_cpp/classes/collision_shape3d.hpp"
#include "godot_cpp/classes/mesh_instance3d.hpp"
#include "godot_cpp/classes/node3d.hpp"
#include "godot_cpp/classes/static_body3d.hpp"
#include "godot_cpp/variant/node_path.hpp"
#include "godot_cpp/variant/vector3.hpp"
#include "mathstatics.h"

/**
 * This node assumes that all transforms are effectively done in global space
 * e.g. node path is somewhat equivalent to /root/<gameplay_scene>/TerrainObjectBase
 * Or at least that it is not grouped under a node with non-zeroed transforms
 */

class TerrainObjectBase : public godot::StaticBody3D {
	GDCLASS(TerrainObjectBase, godot::StaticBody3D)

public:
	static void _bind_methods() {}

	void _enter_tree() override {
		// Expect to find both of these nodes
		auto name = godot::MeshInstance3D::get_class_static();
		get_node<godot::MeshInstance3D>(godot::NodePath(name));
		name = godot::CollisionShape3D::get_class_static();
		get_node<godot::CollisionShape3D>(godot::NodePath(name));
		update_configuration_warnings();
	}

	void _notification(int what) {
		if (what == godot::Node3D::NOTIFICATION_LOCAL_TRANSFORM_CHANGED) {
			update_collisionshape_transform(get_global_transform());
		}
	}

	godot::PackedStringArray _get_configuration_warnings() const override {
		return godot::PackedStringArray();
		// auto warnings = godot::Node::_get_configuration_warnings();
		// godot::TypedArray<godot::String> filtered;

		// for (int i = 0; i < warnings.size(); i++) {
		// 	godot::String warning = warnings[i];
		// 	if (!warning.contains("non-uniform scale")) {
		// 		filtered.append(warning);
		// 	}
		// }

		// return filtered;
	}

private:
	/**
	 * Ensure terrain objects do not scale their collision shapes, as that can
	 * cause unexpected colllision behaviour.
	 * Instead resize the shape inside of collision shape based on terrain global scale.
	 */
	void update_collisionshape_transform(const godot::Transform3D& transform) {
		auto* collisionshape
			= get_node<godot::CollisionShape3D>(godot::NodePath(godot::CollisionShape3D::get_class_static()));
		if (!collisionshape) {
			return;
		}

		const auto parent_scale = transform.get_basis().get_scale();
		const auto inverse_scale = mathstatics::vector_one / parent_scale;
		collisionshape->set_scale(inverse_scale);

		auto shape = collisionshape->get_shape();
		if (auto* boxshape = cast_to<godot::BoxShape3D>(shape.ptr())) {
			boxshape->set_size(parent_scale);
		}
	}
};
