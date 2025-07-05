#pragma once

#include "godot_cpp/variant/basis.hpp"
#include "godot_cpp/variant/vector3.hpp"

inline auto get_basis_scale(const godot::Basis& basis) -> godot::Vector3 {
	return godot::Vector3( //
		basis.get_column(0).length(),
		basis.get_column(1).length(),
		basis.get_column(2).length());
}
