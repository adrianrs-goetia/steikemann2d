#pragma once

#include "typedef.h"
#include <collisionmasks.h>
#include <log.h>
#include <macros.h>

#include <godot_cpp/classes/shape3d.hpp>
#include <godot_cpp/classes/shape_cast3d.hpp>

class WalkingState : public PlayerStateBase {
	GDCLASS(WalkingState, PlayerStateBase)

public:
	static inline const char* p_shapecast_name = "ShapeCast3D";

	PROPERTY(int, movement_direction, 0);
	PROPERTY(float, walking_speed, 450.f);
	PROPERTY(float, gravity_scale, 2.f);

	PROPERTY(godot::Ref<godot::Shape3D>, daelking_collision_shape);
	godot::ShapeCast3D* m_shapecast = nullptr;

public:
	static void _bind_methods() {
		BIND_PROPERTY_METHODS(WalkingState, gravity_scale, FLOAT);
		BIND_PROPERTY_METHODS(WalkingState, walking_speed, FLOAT);

		using godot::Shape3D;
		BIND_RESOURCE_PROPERTY_METHODS(WalkingState, daelking_collision_shape, Shape3D);
	}

	virtual auto enter(Context& c) -> std::optional<TransitionContext> override {
		m_shapecast = allocate_daelking_shapecast_node(c);
		return {};
	}

	virtual auto exit(Context& c) -> void override {
		deallocate_nodes(c);
		return;
	}

	virtual auto process(Context&, double) -> std::optional<TransitionContext> override {
		return {};
	}

	virtual auto physics_process(Context& c, double delta) -> std::optional<TransitionContext> override {
		auto velocity = c.character.get_velocity();
		velocity.x = m_walking_speed * (float)m_movement_direction * delta;
		velocity.y += c.character.get_gravity().y * delta * get_gravity_scale();

		c.character.set_velocity(velocity);
		c.character.move_and_slide();
		return {};
	}

	virtual auto handle_input(Context& c, const InputState& input) -> std::optional<TransitionContext> override {
		m_movement_direction = get_new_movement_direction(input);
		if (input.daelking.just_pressed()) {
			if (detect_daelking_collision(c)) {
				return TransitionContext{ .state = EState::DAELKING_PRE_LAUNCH };
			}
		}
		return {};
	}

private:
	auto get_new_movement_direction(const InputState& input) -> int {
		if (input.move_left.pressed() && input.move_right.pressed()) {
			return 0;
		}
		else if (input.move_left.pressed()) {
			return -1;
		}
		else if (input.move_right.pressed()) {
			return 1;
		}
		return 0;
	}

	auto detect_daelking_collision(const Context& c) -> bool {
		if (!m_shapecast) {
			LOG_ERROR("{} missing daelking shapecast3d ptr", str(get_name()));
			return false;
		}

		m_shapecast->set_enabled(true);
		m_shapecast->set_target_position(c.owner.get_position());
		m_shapecast->force_shapecast_update();

		if (m_shapecast->is_colliding()) {
			m_shapecast->set_enabled(false);
			return true;
		}
		return false;
	}
	auto allocate_daelking_shapecast_node(Context& c) -> godot::ShapeCast3D* {
		auto* shapecast = memnew(godot::ShapeCast3D); // Expects parent to take memory ownership
		shapecast->set_name(p_shapecast_name);
		shapecast->set_enabled(false);
		shapecast->set_collide_with_areas(true);
		shapecast->set_collision_mask(collisionflag::daelking_collision);

		if (get_daelking_collision_shape().is_null()) {
			LOG_ERROR("{} is missing a daelking collision shape resource.", str(get_path()));
			return nullptr;
		}
		shapecast->set_shape(get_daelking_collision_shape());

		c.owner.add_child(shapecast);
		return shapecast;
	}
	auto deallocate_nodes(Context& c) -> void {
		if (m_shapecast) {
			// Bug?
			// Godot Jolt complained about an RID node being abandoned and not
			// properly freed. Setting shape to nothing here before freeing
			// this node looks to have helped.
			m_shapecast->set_shape({});

			c.owner.remove_child(m_shapecast);
			m_shapecast = nullptr;
		}
	}
};
