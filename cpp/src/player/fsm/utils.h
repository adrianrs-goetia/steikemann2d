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
inline auto get_quaternion_from_vectors(const godot::Vector3& v_dir, const float angle_degrees_offset)
	-> godot::Quaternion {
	const float dot = math_statics::rotation_ref_axis.dot(v_dir);

	// Vectors nearly the same
	if (0.9999f < dot) {
		return godot::Quaternion(0.f, 0.f, 0.f, 1.f);
	}
	// Vectors are nearly opposite
	else if (dot < -0.9999f) {
		return godot::Quaternion(1.f, 0.f, 0.f, 0.f);
	}
	else {
		const auto axis = godot::Vector3(0.f, 0.f, 1.f);
		const auto angle = godot::Math::acos(dot) - godot::Math::deg_to_rad(angle_degrees_offset);
		const auto half_angle = angle / 2.f;
		const auto s = godot::Math::sin(half_angle);
		return godot::Quaternion(0.f, 0.f, axis.z * s, godot::Math::cos(half_angle));
	}
}
