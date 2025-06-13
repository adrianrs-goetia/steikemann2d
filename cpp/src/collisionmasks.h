#pragma once

namespace collisionflag {

inline constexpr auto static_world = (1 << 0);
inline constexpr auto dynamic_world = (1 << 1);
inline constexpr auto player = (1 << 2);
inline constexpr auto enemies = (1 << 3);
inline constexpr auto daelking_collision = (1 << 4);

} //namespace collisionflag
