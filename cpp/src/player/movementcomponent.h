#pragma once

#include <memory>

#include "daelkresource.h"
#include "state.h"
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

/**
 * Movement:
 *      - Walking
 *      - Sick Heelies
 *      - Daelking
 *      - Landing
 */
class MovementComponent : public godot::Node3D {
	GDCLASS(MovementComponent, godot::Node3D)

	PROPERTY(int, movement_direction, 0);
	PROPERTY(float, walking_speed, 450.f);
	PROPERTY(float, gravity_scale, 2.f);

	PROPERTY(godot::Ref<DaelkResource>, daelk_res)

	// PlayerState m_state = PlayerState::WALKING;
	std::unique_ptr<Fsm> m_fsm;

public:
	static void _bind_methods() {
		BIND_PROPERTY_METHODS(MovementComponent, gravity_scale, FLOAT);
		BIND_PROPERTY_METHODS(MovementComponent, walking_speed, FLOAT);
		BIND_RESOURCE_PROPERTY_METHODS(MovementComponent, daelk_res, DaelkResource);
	}

	void _enter_tree() override {
		set_name(get_class());

		GAME_SCOPE {
			LOG_INFO("Init {}", str(get_class()));

			// Init daelking resource
			if (m_daelk_res.is_null()) {
				LOG_WARN("{} Daelk resource is_null. Setting default.", str(get_path()));
				m_daelk_res.instantiate();
			}
			add_child(m_daelk_res->allocate_daelking_arrow());
			add_child(m_daelk_res->allocate_daelking_shapecast_node());

			m_fsm = std::make_unique<Fsm>(Context{ .owner = *this }, EState::IDLE);

			if (auto* im = get_node<InputManager>(InputManager::get_path())) {
				im->register_input_callback(get_path(), [this](const InputState& i) { this->input_callback(i); });
			}
		}
	}

	void _exit_tree() override {
		m_daelk_res.unref(); // Unref before deleting children. Resource holds raw ptr to them.
	}

	void _physics_process(double delta) override {
		auto* parent = get_parent();
		if (auto* character = cast_to<godot::CharacterBody3D>(parent)) {
			physics_process(*character, delta);
		}
	}

	void physics_process(godot::CharacterBody3D& character, double delta) {
		auto velocity = character.get_velocity();
		if (m_state == DAELKING_STANDSTILL) {
			character.set_velocity(godot::Vector3());
			if (m_daelk_res.is_valid()) {
				auto [t_state, t_velocity] = m_daelk_res->physics_process(get_context(), velocity, delta);
				m_state = t_state character.move_and_slide();
			}
			else {
				return;
			}
		}
		velocity.x = m_walking_speed * (float)m_movement_direction * delta;
		velocity.y += character.get_gravity().y * delta * get_gravity_scale();

		character.set_velocity(velocity);
		character.move_and_slide();
	}

	void input_callback(const InputState& input) {
		m_movement_direction = get_new_movement_direction(input);
		m_state = m_daelk_res->handle_input(get_context(), input);
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

	auto get_context() -> Context {
		return Context{
			.owner = *this,
			.state = m_state,
		};
	}
};
