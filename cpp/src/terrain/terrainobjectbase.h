#pragma once

#include "godot_cpp/classes/box_shape3d.hpp"
#include "godot_cpp/classes/mesh.hpp"
#include "godot_cpp/classes/static_body3d.hpp"
#include "godot_cpp/variant/node_path.hpp"
#include "godot_cpp/variant/vector3.hpp"
#include "godotutils.h"
#include "log.h"
#include "macros.h"

#include "godot_cpp/classes/collision_shape3d.hpp"
#include "godot_cpp/classes/mesh_instance3d.hpp"
#include "godot_cpp/classes/node3d.hpp"
#include "godot_cpp/classes/shape3d.hpp"

class TerrainObjectBase : public godot::StaticBody3D {
	GDCLASS(TerrainObjectBase, godot::StaticBody3D)

	PROPERTY(godot::Ref<godot::Mesh>, mesh);
	PROPERTY(godot::Ref<godot::Shape3D>, shape);

public:
	static void _bind_methods() {}

	void _enter_tree() override {
		ensure_correct_children();

		set_notify_transform(true);
		set_disable_scale(true);
	}

	void _notification(int what) {
		if (what == godot::Node3D::NOTIFICATION_TRANSFORM_CHANGED) {
			auto basis = get_global_basis();
			const auto scale = get_basis_scale(basis);

			update_collisionshape_scale(scale);
			update_meshinstance_scale(scale);

			basis.scale(godot::Vector3(1, 1, 1));
			set_global_basis(basis);
		}
	}

private:
	/**
	 * Ensure terrain objects do not scale their collision shapes, as that can
	 * cause unexpected colllision behaviour.
	 * Instead resize the shape inside of collision shape based on terrain global scale.
	 */
	void update_collisionshape_scale(const godot::Vector3& scale) {
		const auto* collisionshape
			= get_node<godot::CollisionShape3D>(godot::NodePath(godot::CollisionShape3D::get_class_static()));
		if (!collisionshape) {
			return;
		}

		auto shape = collisionshape->get_shape();

		if (auto* boxshape = cast_to<godot::BoxShape3D>(shape.ptr())) {
			boxshape->set_size(scale);
		}
	}

	void update_meshinstance_scale(const godot::Vector3& scale) {
		const auto* meshinstance
			= get_node<godot::MeshInstance3D>(godot::NodePath(godot::MeshInstance3D::get_class_static()));
		if (!meshinstance) {
			return;
		}
	}

	void ensure_correct_children() {
		auto name = godot::MeshInstance3D::get_class_static();
		if (!has_node(godot::NodePath(name))) {
			LOG_TRACE("{} missing {}. Instantiating one now", str(get_path()), str(name));
			const auto meshinstance = memnew(godot::MeshInstance3D);
			meshinstance->set_name(name);
			if (m_mesh.is_valid()) {
				meshinstance->set_mesh(m_mesh);
			}
			meshinstance->set_owner(this);
			add_child(meshinstance);
		}

		name = godot::CollisionShape3D::get_class_static();
		if (!has_node(godot::NodePath(name))) {
			LOG_TRACE("{} missing {}. Instantiating one now", str(get_path()), str(name));
			const auto collisionshape = memnew(godot::CollisionShape3D);
			collisionshape->set_name(name);
			if (m_shape.is_valid()) {
				collisionshape->set_shape(m_shape);
			}
			collisionshape->set_disable_scale(true);
			collisionshape->set_owner(this);
			add_child(collisionshape);
		}
	}
};
