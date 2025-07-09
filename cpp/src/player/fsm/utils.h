#pragma once

#include <optional>

#include "input/input_state.h"
#include "mathstatics.h"
#include "typedef.h"

#include "godot_cpp/classes/camera3d.hpp"
#include "godot_cpp/classes/viewport.hpp"
#include "godot_cpp/core/math.hpp"
#include "godot_cpp/variant/vector3.hpp"

inline auto get_daelking_direction(const Context& c) -> std::optional<godot::Vector3> {
	switch (c.input.mode) {
		case InputMode::JOYPAD: {
			const auto limit = 0.1f;
			if (c.input.movement.vec3().length_squared() < limit) {
				return {};
			}
			return c.input.movement.vec3();
		}
		case InputMode::MNK: {
			const auto* camera3d = c.character.get_viewport()->get_camera_3d();
			const auto character_position = c.character.get_global_position();
			const auto character_unprojected = [camera3d, character_position] -> std::optional<godot::Vector2>
			{
				if (!camera3d->is_position_behind(character_position)) {
					return camera3d->unproject_position(character_position);
				}
				return std::nullopt;
			}();
			if (character_unprojected.has_value()) {
				const auto direction2d
					= godot::Vector2(c.input.mouse_position.vec2() - character_unprojected.value()).normalized();
				return godot::Vector3(direction2d.x, -direction2d.y, 0.f);
			}
		}
	}
	return std::nullopt;
}

// input is expected to be normalized
// We use Z for depth in this 2D game, so all gameplay rotations happen around the z-axis
inline auto get_direction_angle(const godot::Vector3& v_dir, float angle_offset_degrees) -> float {
	const float dot = mathstatics::rotation_ref_axis.dot(v_dir);
	auto angle = godot::Math::acos(dot) - godot::Math::deg_to_rad(angle_offset_degrees);

	const auto dir = [v_dir] -> int
	{
		const float dot = mathstatics::up.dot(v_dir);
		const auto limit = 0.001f;
		if (-limit < dot && dot < limit) {
			return 1;
		}
		return (0.f < dot) ? 1 : -1;
	}();
	if (dir == -1) {
		angle = Math_PI - angle;
	}
	return angle;
}
