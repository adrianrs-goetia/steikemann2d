#pragma once

#include "godot_cpp/core/math.hpp"

struct ValueInterpolation {
	float start_threshhold;
	float smoothing_length;

	auto interpolate(float start, float end, float deltatime) -> float {
		const auto diff = end - start;

		if (godot::Math::abs(diff) < start_threshhold) {
			return start;
		}

		const auto diff_direction = 0.f < diff ? 1.f : -1.f;
		const auto interpolation_delta = godot::Math::pow(godot::Math::abs(diff) / smoothing_length, 2);
		return start + (interpolation_delta * diff_direction * deltatime);
	}
};
