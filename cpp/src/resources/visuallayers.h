#pragma once

#include "godot_cpp/variant/callable.hpp"
#include "log.h"
#include "macros.h"

#include "godot_cpp/classes/node3d.hpp"
#include "godot_cpp/classes/resource.hpp"
#include "godot_cpp/classes/resource_loader.hpp"
#include "godot_cpp/classes/wrapped.hpp"
#include "godot_cpp/core/binder_common.hpp"

class VisualLayerResource : public godot::Resource {
	GDCLASS(VisualLayerResource, godot::Resource)

public:
	RESOURCE_PROPERTY(float, fg_3, 15.0);
	RESOURCE_PROPERTY(float, fg_2, 10.0);
	RESOURCE_PROPERTY(float, fg_1, 5.0);
	RESOURCE_PROPERTY(float, mg_collision, 0.0);
	RESOURCE_PROPERTY(float, mg_visual, 0.1);
	RESOURCE_PROPERTY(float, bg_1, -5.0);
	RESOURCE_PROPERTY(float, bg_2, -10.0);
	RESOURCE_PROPERTY(float, bg_3, -15.0);

public:
	static auto get_res_path() {
		return "res://resources/visuallayerresource.tres";
	}

	static auto get_resource() -> godot::Ref<VisualLayerResource> {
		auto* resourceloader = godot::ResourceLoader::get_singleton();
		if (!resourceloader) {
			return {};
		}

		auto resource = resourceloader->load(get_res_path());
		if (resource.is_null()) {
			LOG_NEW_ERROR("Failed to load resource: ", get_res_path());
		}
		return resource;
	}

	static void _bind_methods() {
		BIND_PROPERTY_METHODS(VisualLayerResource, fg_3, FLOAT);
		BIND_PROPERTY_METHODS(VisualLayerResource, fg_2, FLOAT);
		BIND_PROPERTY_METHODS(VisualLayerResource, fg_1, FLOAT);
		// BIND_PROPERTY_METHODS(VisualLayerResource, mg_collision, FLOAT);
		BIND_PROPERTY_METHODS(VisualLayerResource, mg_visual, FLOAT);
		BIND_PROPERTY_METHODS(VisualLayerResource, bg_1, FLOAT);
		BIND_PROPERTY_METHODS(VisualLayerResource, bg_2, FLOAT);
		BIND_PROPERTY_METHODS(VisualLayerResource, bg_3, FLOAT);
	}
};

enum EVisualLayer {
	FG_3,
	FG_2,
	FG_1,

	MG_COLLISION,
	MG_VISUAL,

	BG_1,
	BG_2,
	BG_3,
};
VARIANT_ENUM_CAST(EVisualLayer);

namespace visuallayer {

inline const char* enum_cstr = "FG_3, FG_2, FG_1, MG_COLLISION, MG_VISUAL, BG_1, BG_2, BG_3";

inline auto get_depth(EVisualLayer layer, const godot::Ref<VisualLayerResource>& resource) -> float {
	if (resource.is_null()) {
		LOG_WARN("VisualLayerResource is null");
		return 0.0;
	}

	switch (layer) {
		case EVisualLayer::FG_3:
			return resource->get_fg_3();
		case EVisualLayer::FG_2:
			return resource->get_fg_2();
		case EVisualLayer::FG_1:
			return resource->get_fg_1();
		case EVisualLayer::MG_COLLISION:
			return resource->get_mg_collision();
		case EVisualLayer::MG_VISUAL:
			return resource->get_mg_visual();
		case EVisualLayer::BG_1:
			return resource->get_bg_1();
		case EVisualLayer::BG_2:
			return resource->get_bg_2();
		case EVisualLayer::BG_3:
			return resource->get_bg_3();
	}
	LOG_NEW_WARN("Unsupported EVisualLayer value: ", std::to_string((int)layer).c_str());
	return 0.0;
}

inline auto connect(godot::Callable callable) -> godot::Ref<VisualLayerResource> {
	auto visuallayers = VisualLayerResource::get_resource();
	if (visuallayers.is_valid()) {
		if (!visuallayers->is_connected("changed", callable)) {
			visuallayers->connect("changed", callable);
		}
	}
	return visuallayers;
}
inline auto disconnect(godot::Callable callable) -> void {
	auto visuallayers = VisualLayerResource::get_resource();
	if (visuallayers.is_valid()) {
		if (visuallayers->is_connected("changed", callable)) {
			visuallayers->disconnect("changed", callable);
		}
	}
}

inline auto set_depth(godot::Node3D& node, EVisualLayer layer) -> void {
	auto visuallayers = VisualLayerResource::get_resource();
	if (!node.is_inside_tree() || !node.is_node_ready() || visuallayers.is_null()) {
		return;
	}
	const auto new_depth = get_depth(layer, visuallayers);
	node.set_global_position(godot::Vector3(node.get_global_position().x, node.get_global_position().y, new_depth));
}

// Sanity checking if object is on the correct depth value
inline auto on_layer_depth(godot::Node3D& node, EVisualLayer layer) -> bool {
	auto visuallayers = VisualLayerResource::get_resource();
	if (!node.is_inside_tree() || !node.is_node_ready() || visuallayers.is_null()) {
		return true;
	}
	return get_depth(layer, visuallayers) == node.get_global_position().z;
}

inline auto keep_on_mgcollision_layer(godot::Node3D& node) -> void {
	if (!on_layer_depth(node, EVisualLayer::MG_COLLISION)) {
		set_depth(node, EVisualLayer::MG_COLLISION);
	}
}

} //namespace visuallayer
