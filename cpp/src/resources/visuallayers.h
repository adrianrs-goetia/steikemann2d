#pragma once

#include "log.h"
#include "macros.h"

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

inline const char* g_visuallayer_cstr = "CLOSE_FOREGROUND, FOREGROUND, GAMEPLAY, BACKGROUND, FAR_BACKGROUND";

inline auto get_visual_layer_depth(EVisualLayer layer, const godot::Ref<VisualLayerResource>& resource) -> float {
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

VARIANT_ENUM_CAST(EVisualLayer);
