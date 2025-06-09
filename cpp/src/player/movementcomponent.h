#pragma once

#include <collisionmasks.h>
#include <input/typedef.h>
#include <log.h>
#include <macros.h>

#include <godot_cpp/classes/character_body3d.hpp>
#include <godot_cpp/classes/resource.hpp>
#include <godot_cpp/classes/shape3d.hpp>
#include <godot_cpp/classes/shape_cast3d.hpp>

class MovementComponent : public godot::Resource {
	GDCLASS(MovementComponent, godot::Resource)

	godot::Vector3 m_position;

	PROPERTY(int, movement_direction, 0);
	PROPERTY(float, walking_speed, 450.f);
	PROPERTY(float, gravity_scale, 2.f);

	PROPERTY(float, daelking_impulse_strength, 8.0f);
	bool m_daelk_impulse = false;
	PROPERTY(godot::Ref<godot::Shape3D>, daelking_collision_shape);
	godot::ShapeCast3D* m_daelking_shapecast = nullptr;

public:
	static void _bind_methods() {
		BIND_PROPERTY_METHODS(MovementComponent, gravity_scale, FLOAT);
		BIND_PROPERTY_METHODS(MovementComponent, walking_speed, FLOAT);
		BIND_PROPERTY_METHODS(MovementComponent, daelking_impulse_strength, FLOAT);
		using godot::Shape3D;
		BIND_RESOURCE_PROPERTY_METHODS(MovementComponent, daelking_collision_shape, Shape3D);
	}

	void init(godot::ShapeCast3D* t_shapecast) {
		LOG_TRACE("Init {}", get_class().utf8().get_data());
		// Init ShapeCast3D node
		m_daelking_shapecast = t_shapecast;
		m_daelking_shapecast->set_enabled(false);
		m_daelking_shapecast->set_collide_with_areas(true);
		m_daelking_shapecast->set_collision_mask(collisionflag::daelking_collision);
		m_daelking_shapecast->set_shape(get_daelking_collision_shape());
	}

	void uninit() {
		m_daelking_shapecast = nullptr;
	}

	void physics_process(godot::CharacterBody3D& character, double delta) {
		m_position = character.get_global_position();

		auto velocity = character.get_velocity();
		velocity.x = m_walking_speed * (float)m_movement_direction * delta;
		velocity.y += character.get_gravity().y * delta * get_gravity_scale();

		if (m_daelk_impulse) {
			velocity.y = get_daelking_impulse_strength();
			m_daelk_impulse = false;
		}

		character.set_velocity(velocity);
		character.move_and_slide();
	}

	void process_input(const InputState& input) {
		m_movement_direction = get_new_movement_direction(input);
		if (input.daelking.just_pressed()) {
			m_daelk_impulse = detect_daelking_collision();
		}
	}

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

	auto detect_daelking_collision() -> bool {
		if (get_daelking_collision_shape().is_null()) {
			LOG_ERROR("{} has no daelking collision shape", get_name().utf8().get_data());
			return false;
		}
		if (!m_daelking_shapecast) {
			LOG_ERROR("{} daelking shapecast3d ptr", get_name().utf8().get_data());
			return false;
		}

		m_daelking_shapecast->set_enabled(true);
		m_daelking_shapecast->set_target_position(m_position);
		m_daelking_shapecast->force_shapecast_update();

		if (m_daelking_shapecast->is_colliding()) {
			m_daelking_shapecast->set_enabled(false);
			return true;
		}
		return false;
	}
};
