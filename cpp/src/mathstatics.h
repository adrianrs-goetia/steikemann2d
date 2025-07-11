#pragma once

#include "godot_cpp/variant/plane.hpp"
#include "godot_cpp/variant/vector3.hpp"

namespace mathstatics {
inline const auto rotation_ref_axis = godot::Vector3(1.f, 0.f, 0.f);

inline const auto vector_zero = godot::Vector3(1.f, 1.f, 1.f);
inline const auto vector_one = godot::Vector3(1.f, 1.f, 1.f);

inline const auto forward = godot::Vector3(1.f, 0.f, 0.f);
inline const auto up = godot::Vector3(0.f, 1.f, 0.f);
inline const auto depth = godot::Vector3(0.f, 0.f, -1.f);

inline const auto xy_plane = godot::Plane(0.f, 0.f, 1.f, 0.f);

} //namespace mathstatics
