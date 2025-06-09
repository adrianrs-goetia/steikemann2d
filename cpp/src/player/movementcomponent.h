#pragma once

#include <collisionmasks.h>
#include <input/inputmanager.h>
#include <input/typedef.h>
#include <log.h>
#include <macros.h>

#include <godot_cpp/classes/character_body3d.hpp>
#include <godot_cpp/classes/node3d.hpp>
#include <godot_cpp/classes/packed_scene.hpp>
#include <godot_cpp/classes/shape3d.hpp>
#include <godot_cpp/classes/shape_cast3d.hpp>

inline const char* g_shapecast_name = "ShapeCast3D";
inline const char* g_arrow_name = "Arrow3D";

class MovementComponent : public godot::Node3D {
	GDCLASS(MovementComponent, godot::Node3D)

	PROPERTY(int, movement_direction, 0);
	PROPERTY(float, walking_speed, 450.f);
	PROPERTY(float, gravity_scale, 2.f);

	PROPERTY(float, daelking_impulse_strength, 8.0f);
	bool m_daelk_impulse = false;
	PROPERTY(godot::Ref<godot::Shape3D>, daelking_collision_shape);

	PROPERTY(godot::Ref<godot::PackedScene>, arrow_scene);
	godot::Node3D* m_arrow = nullptr;

public:
	static void _bind_methods() {
		BIND_PROPERTY_METHODS(MovementComponent, gravity_scale, FLOAT);
		BIND_PROPERTY_METHODS(MovementComponent, walking_speed, FLOAT);
		BIND_PROPERTY_METHODS(MovementComponent, daelking_impulse_strength, FLOAT);

		using godot::Shape3D, godot::PackedScene;
		BIND_RESOURCE_PROPERTY_METHODS(MovementComponent, daelking_collision_shape, Shape3D);
		BIND_RESOURCE_PROPERTY_METHODS(MovementComponent, arrow_scene, PackedScene);
	}

	void _enter_tree() override {
		set_name(get_class());

		GAME_SCOPE {
			LOG_TRACE("Init {}", get_class().utf8().get_data());

			init_daelking_shapecast_node();
			m_arrow = init_daelking_arrow();

			if (auto* im = get_node<InputManager>(InputManager::get_path())) {
				im->register_input_callback(get_path(), [this](const InputState& i) { this->input_callback(i); });
			}
		}
	}

	void _physics_process(double delta) override {
		auto* parent = get_parent();
		if (auto* character = cast_to<godot::CharacterBody3D>(parent)) {
			physics_process(*character, delta);
		}
	}

	void physics_process(godot::CharacterBody3D& character, double delta) {
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

	void input_callback(const InputState& input) {
		m_movement_direction = get_new_movement_direction(input);
		if (input.daelking.just_pressed()) {
			m_daelk_impulse = detect_daelking_collision();
			if (m_daelk_impulse) {
				arrow_enable();
			}
		}
		else if (input.daelking.just_released()) {
			arrow_disable();
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

	auto init_daelking_shapecast_node() -> void {
		auto* daelking_shapecast = memnew(godot::ShapeCast3D);
		daelking_shapecast->set_name(g_shapecast_name);
		add_child(daelking_shapecast);
		daelking_shapecast->set_enabled(false);
		daelking_shapecast->set_collide_with_areas(true);
		daelking_shapecast->set_collision_mask(collisionflag::daelking_collision);

		if (get_daelking_collision_shape().is_null()) {
			LOG_ERROR(
				"{} is missing a daelking collision shape resource.", godot::String(get_path()).utf8().get_data());
			return;
		}
		daelking_shapecast->set_shape(get_daelking_collision_shape());
	}

	auto detect_daelking_collision() -> bool {
		auto* shapecast = get_node<godot::ShapeCast3D>(g_shapecast_name);
		if (!shapecast) {
			LOG_ERROR("{} missing daelking shapecast3d ptr", godot::String(get_name()).utf8().get_data());
			return false;
		}

		shapecast->set_enabled(true);
		shapecast->set_target_position(get_position());
		shapecast->force_shapecast_update();

		if (shapecast->is_colliding()) {
			shapecast->set_enabled(false);
			return true;
		}
		return false;
	}

	auto init_daelking_arrow() -> godot::Node3D* {
		if (get_arrow_scene().is_null()) {
			LOG_ERROR("{} arrow scene is_null", godot::String(get_path()).utf8().get_data());
			return nullptr;
		}

		if (!get_arrow_scene()->can_instantiate()) {
			LOG_ERROR("{} arrow scene !can_instantiate", godot::String(get_path()).utf8().get_data());
			return nullptr;
		}

		auto arrow = cast_to<godot::Node3D>(get_arrow_scene()->instantiate());
		if (!arrow) {
			LOG_ERROR("{} failed to instantiate arrow scene", godot::String(get_path()).utf8().get_data());
			return nullptr;
		}

		arrow->set_name(g_arrow_name);
		add_child(arrow);
		arrow->set_as_top_level(true); // Not inheriting parent transform, force global transform only
		arrow->hide();

		return arrow;
	}

	auto arrow_enable() -> void {
		if (!m_arrow) {
			return;
		}
		m_arrow->show();
		m_arrow->set_position(
			[gp = get_global_position()] -> godot::Vector3
			{
				auto pos = gp;
				pos.z = 1.f; // Tmp foreground placement
				return pos;
			}());
	}

	auto arrow_disable() -> void {
		if (m_arrow) {
			m_arrow->hide();
		}
	}
};
