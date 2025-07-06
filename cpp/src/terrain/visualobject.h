#pragma once

#include "macros.h"
#include "resources/visuallayers.h"

#include "godot_cpp/classes/global_constants.hpp"
#include "godot_cpp/classes/mesh_instance3d.hpp"
#include "godot_cpp/variant/callable_method_pointer.hpp"
#include "godot_cpp/variant/node_path.hpp"
#include "godot_cpp/variant/vector3.hpp"

/**
 * This node assumes that all transforms are effectively done in global space
 * e.g. node path is somewhat equivalent to /root/<gameplay_scene>/TerrainObjectBase
 * Or at least that it is not grouped under a node with non-zeroed transforms
 */

class VisualObject : public godot::MeshInstance3D {
	GDCLASS(VisualObject, godot::MeshInstance3D)

	PROPERTY_CUSTOM_SET(
		EVisualLayer, visual_layer,
		{
			m_visual_layer = value;

			if (!is_node_ready() || !is_inside_tree()) {
				return;
			}
			visuallayer::set_depth(*this, value);
		},
		EVisualLayer::MG_VISUAL);

public:
	static void _bind_methods() {
		BIND_METHOD(VisualObject, set_depth);
		BIND_PROPERTY_METHODS(VisualObject, visual_layer, INT, godot::PROPERTY_HINT_ENUM, visuallayer::enum_cstr);
	}

	void _enter_tree() override {
		set_notify_transform(false);

		visuallayer::connect(callable_mp(this, &VisualObject::on_visuallayerresource_update));

		call_deferred("set_depth");
	}

	void _notification(int what) {
		if (what == godot::Node3D::NOTIFICATION_TRANSFORM_CHANGED) {
			if (!visuallayer::on_layer_depth(*this, get_visual_layer())) {
				visuallayer::set_depth(*this, get_visual_layer());
			}
		}
		if (what == godot::Node::NOTIFICATION_PREDELETE) {
			visuallayer::disconnect(callable_mp(this, &VisualObject::on_visuallayerresource_update));
		}
	}

	void on_visuallayerresource_update() {
		if (!visuallayer::on_layer_depth(*this, get_visual_layer())) {
			visuallayer::set_depth(*this, get_visual_layer());
		}
	}

private:
	void set_depth() {
		visuallayer::set_depth(*this, get_visual_layer());
	}
};
