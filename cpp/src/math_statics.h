#pragma once

#include "godot_cpp/variant/vector3.hpp"

namespace math_statics {
inline const auto rotation_ref_axis = godot::Vector3(1.f, 0.f, 0.f);

inline const auto forward = godot::Vector3(1.f, 0.f, 0.f);
inline const auto up = godot::Vector3(0.f, 1.f, 0.f);
inline const auto depth = godot::Vector3(0.f, 0.f, -1.f);

} //namespace math_statics
