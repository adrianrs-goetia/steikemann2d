#pragma once

#include <input/input_state.h>
#include <math_statics.h>

#include <godot_cpp/core/math.hpp>
#include <godot_cpp/variant/quaternion.hpp>
#include <godot_cpp/variant/vector3.hpp>

inline auto get_daelking_direction(const InputState& input) -> godot::Vector3 {
	return input.movement.vec3();
}

// input is expected to be normalized
// We use Z for depth in this 2D game, so all gameplay rotations happen around the z-axis
inline auto get_direction_angle(const godot::Vector3& v_dir, float angle_offset_degrees) -> float {
	const float dot = math_statics::rotation_ref_axis.dot(v_dir);
	auto angle = godot::Math::acos(dot) - godot::Math::deg_to_rad(angle_offset_degrees);

	const auto dir = [v_dir] -> int
	{
		const float dot = math_statics::up.dot(v_dir);
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
