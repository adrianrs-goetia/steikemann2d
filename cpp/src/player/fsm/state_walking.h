#pragma once

#include "typedef.h"
#include <collisionmasks.h>
#include <gameplay_node.h>
#include <log.h>
#include <macros.h>
#include <player/fsm/utils.h>

#include <godot_cpp/classes/area3d.hpp>
#include <godot_cpp/classes/shape3d.hpp>
#include <godot_cpp/classes/shape_cast3d.hpp>

class WalkingState : public PlayerStateBase {
	GDCLASS(WalkingState, PlayerStateBase)

public:
	static inline const char* p_shapecast_name = "ShapeCast3D";

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
		velocity.x = m_walking_speed * c.input.movement.x() * delta;
		velocity.y += c.character.get_gravity().y * delta * get_gravity_scale();

		c.character.set_velocity(velocity);
		c.character.move_and_slide();
		return {};
	}

	virtual auto input_callback(Context& c) -> std::optional<TransitionContext> override {
		if (c.input.daelking.just_pressed()) {
			if (detect_daelking_collision(c)) {
				c.daelked_node_path = get_gameplay_node_path(c);
				return TransitionContext{ .state = EState::DAELKING_PRE_LAUNCH };
			}
		}
		return {};
	}

private:
	auto detect_daelking_collision(const Context& c) -> bool {
		if (!m_shapecast) {
			LOG_ERROR("{} missing daelking shapecast3d ptr", str(get_name()));
			return false;
		}

		m_shapecast->set_enabled(true);
		m_shapecast->set_max_results(1); // Only do daelk collision with one object at a time
		m_shapecast->set_target_position(c.owner.get_position());
		m_shapecast->force_shapecast_update();

		if (m_shapecast->is_colliding()) {
			m_shapecast->set_enabled(false);
			return true;
		}
		return false;
	}

	auto get_gameplay_node_path(const Context& c) -> std::optional<godot::NodePath> {
		if (!m_shapecast) {
			LOG_ERROR("{} missing daelking shapecast3d ptr", str(get_name()));
			return {};
		}

		if (m_shapecast->get_collision_count() == 0) {
			LOG_ERROR("{} daelk collision count is somehow 0", str(get_name()));
			return {};
		}

		// Only daelking with a single thing at a time
		auto* area = c.owner.cast_to<godot::Area3D>(m_shapecast->get_collider(0));
		if (!area) {
			LOG_ERROR("{} could not cast shapecast collider to Area3D", str(get_name()));
			return {};
		}

		auto* gameplay_node = c.owner.cast_to<GameplayNode3D>(area->get_parent());
		if (!gameplay_node) {
			LOG_ERROR("{} could not cast area->get_parent to GameplayNode3D", str(get_name()));
			return {};
		}

		return gameplay_node->get_path();
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
			m_shapecast->queue_free();
			m_shapecast = nullptr;
		}
	}
};
