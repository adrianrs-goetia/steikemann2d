#pragma once

#include "math_statics.h"

#include "godot_cpp/variant/basis.hpp"
#include "godot_cpp/variant/vector3.hpp"

inline auto get_basis_scaled(const godot::Basis& basis, const godot::Vector3& scale) -> godot::Basis {
	const auto basisscale = basis.get_scale();
	auto normalizedscalebasis = basis.scaled(godot::Vector3(1, 1, 1) / basisscale);
	return normalizedscalebasis.scaled(scale);
}

inline auto get_basis_normalized(const godot::Basis& basis) -> godot::Basis {
	return get_basis_scaled(basis, math_statics::vector_one);
}
