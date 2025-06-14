#pragma once

#include <input/typedef.h>

#include <godot_cpp/classes/character_body3d.hpp>
#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/classes/node3d.hpp>
#include <godot_cpp/classes/resource.hpp>
#include <godot_cpp/variant/node_path.hpp>
#include <godot_cpp/variant/string.hpp>

struct Context {
	godot::Node3D& owner;
	godot::CharacterBody3D& character;
	InputState input;
	std::optional<godot::NodePath> daelked_node_path;
};

enum class EState {
	WALKING,
	// FALLING,
	DAELKING_PRE_LAUNCH,
	DAELKING_LAUNCH,
};
inline constexpr const char* to_string(EState state) {
	switch (state) {
		case EState::WALKING:
			return "Walking";
		case EState::DAELKING_PRE_LAUNCH:
			return "Daelking Pre Launch";
		case EState::DAELKING_LAUNCH:
			return "Daelking Launch";
	}
	return "";
}

struct TransitionContext {
	EState state;
};

class PlayerStateBase : public godot::Resource {
	GDCLASS(PlayerStateBase, Resource)
public:
	static void _bind_methods() {}
	virtual ~PlayerStateBase() = default;
	virtual auto can_enter(Context& c) -> bool {
		return true;
	}
	virtual auto enter(Context&) -> std::optional<TransitionContext> = 0;
	virtual auto exit(Context&) -> void = 0;
	virtual auto process(Context&, double) -> std::optional<TransitionContext> = 0;
	virtual auto physics_process(Context&, double) -> std::optional<TransitionContext> = 0;
	virtual auto input_callback(Context&) -> std::optional<TransitionContext> = 0;
};
