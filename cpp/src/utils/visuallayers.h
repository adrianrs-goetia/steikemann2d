#pragma once

#include "godot_cpp/core/binder_common.hpp"

#include "log.h"
// #include "utils/globalesettings.h"

enum EVisualLayer {
	CLOSE_FOREGROUND,
	FOREGROUND,

	GAMEPLAY,

	BACKGROUND,
	FAR_BACKGROUND,
};

inline const char* g_visuallayer_cstr = "CLOSE_FOREGROUND, FOREGROUND, GAMEPLAY, BACKGROUND, FAR_BACKGROUND";

inline auto get_visual_layer_depth(EVisualLayer layer) -> float {
	// auto* global_settings = globalsettings::get();
	// if (!global_settings) {
	// 	LOG_NEW_ERROR(globalsettings::error_msg);
	// 	return 0.0;
	// }

	switch (layer) {
		case EVisualLayer::CLOSE_FOREGROUND:
			return 10.0;
		case EVisualLayer::FOREGROUND:
			return 5.0;
		case EVisualLayer::GAMEPLAY:
			return 0.0;
		case EVisualLayer::BACKGROUND:
			return -5.0;
		case EVisualLayer::FAR_BACKGROUND:
			return -10.0;
	}
	LOG_NEW_ERROR("Unsupported EVisualLayer value: ", std::to_string((int)layer).c_str());
	return 0.0;
}

VARIANT_ENUM_CAST(EVisualLayer);
