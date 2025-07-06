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
	enum {
		NOTIFICATION_VALUE_CHANGED = 1000,
	};

	RESOURCE_PROPERTY(float, close_foreground, 10.0);
	RESOURCE_PROPERTY(float, foreground, 5.0);
	RESOURCE_PROPERTY(float, gameplay, 0.0);
	RESOURCE_PROPERTY(float, background, -5.0);
	RESOURCE_PROPERTY(float, far_background, -10.0);

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
		BIND_PROPERTY_METHODS(VisualLayerResource, close_foreground, FLOAT);
		BIND_PROPERTY_METHODS(VisualLayerResource, foreground, FLOAT);
		BIND_PROPERTY_METHODS(VisualLayerResource, gameplay, FLOAT);
		BIND_PROPERTY_METHODS(VisualLayerResource, background, FLOAT);
		BIND_PROPERTY_METHODS(VisualLayerResource, far_background, FLOAT);
	}
};

enum EVisualLayer {
	CLOSE_FOREGROUND,
	FOREGROUND,

	GAMEPLAY,

	BACKGROUND,
	FAR_BACKGROUND,
};
VARIANT_ENUM_CAST(EVisualLayer);

namespace visuallayer {

inline const char* enum_cstr = "CLOSE_FOREGROUND, FOREGROUND, GAMEPLAY, BACKGROUND, FAR_BACKGROUND";

inline auto get_depth(EVisualLayer layer, const godot::Ref<VisualLayerResource>& resource) -> float {
	if (resource.is_null()) {
		LOG_WARN("VisualLayerResource is null");
		return 0.0;
	}

	switch (layer) {
		case EVisualLayer::CLOSE_FOREGROUND:
			return resource->get_close_foreground();
		case EVisualLayer::FOREGROUND:
			return resource->get_foreground();
		case EVisualLayer::GAMEPLAY:
			return resource->get_gameplay();
		case EVisualLayer::BACKGROUND:
			return resource->get_background();
		case EVisualLayer::FAR_BACKGROUND:
			return resource->get_far_background();
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

} //namespace visuallayer
