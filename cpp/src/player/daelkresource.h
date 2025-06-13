#pragma once

#include "state.h"
#include <collisionmasks.h>
#include <input/typedef.h>
#include <log.h>
#include <macros.h>

#include <godot_cpp/classes/node3d.hpp>
#include <godot_cpp/classes/packed_scene.hpp>
#include <godot_cpp/classes/resource.hpp>
#include <godot_cpp/classes/shape3d.hpp>
#include <godot_cpp/classes/shape_cast3d.hpp>

/**
 * Tracking state of daelking arrow
 */
class DaelkResource : public godot::Resource {
	GDCLASS(DaelkResource, godot::Resource)

	// How to safely get a hold of owner? Owner passes itself into functions?
	// Not a standalone node, only a resource that is dependent on being invoked by the owner.
	//
	// Node: Self sufficient actor in the scene. Does not rely on outside initialization
	// Resource: Extension of Node functionality. Can hold pointers to nodes in scene, but
	//           does not own anything. Can instantiate nodes for owner to add as children.
	//
public:
	static inline const char* g_shapecast_name = "ShapeCast3D";
	static inline const char* g_arrow_name = "Arrow3D";

private:
	PROPERTY(float, daelking_impulse_strength, 8.0f);
	PROPERTY(bool, daelk_impulse, false);
	PROPERTY(godot::Ref<godot::Shape3D>, daelking_collision_shape);

	PROPERTY(godot::Ref<godot::PackedScene>, arrow_scene);

private:
	godot::Node3D* m_arrow = nullptr;
	godot::ShapeCast3D* m_shapecast = nullptr;

public:
	static void _bind_methods() {
		BIND_PROPERTY_METHODS(DaelkResource, daelking_impulse_strength, FLOAT);

		using godot::Shape3D, godot::PackedScene;
		BIND_RESOURCE_PROPERTY_METHODS(DaelkResource, daelking_collision_shape, Shape3D);
		BIND_RESOURCE_PROPERTY_METHODS(DaelkResource, arrow_scene, PackedScene);
	}

	auto physics_process(const Context& context, godot::Vector3 velocity, double delta)
		-> std::tuple<PlayerState, godot::Vector3> {
		auto state = PlayerState::DAELKING_STANDSTILL;
		if (m_daelk_impulse && context.state == PlayerState::DAELKING_LAUNCH) {
			velocity.y = get_daelking_impulse_strength();
			m_daelk_impulse = false;
			state = PlayerState::NONE;
		}
		return { state, velocity };
	}

	auto handle_input(const Context& context, const InputState& input) -> PlayerState {
		if (input.daelking.just_pressed()) {
			m_daelk_impulse = detect_daelking_collision(context);
			if (m_daelk_impulse) {
				arrow_enable(context);
				return PlayerState::DAELKING_STANDSTILL;
			}
		}
		else if (input.daelking.just_released()) {
			arrow_disable();
			if (context.state == PlayerState::DAELKING_STANDSTILL) {
				return PlayerState::DAELKING_LAUNCH;
			}
		}
		return context.state;
	}

	auto detect_daelking_collision(const Context& context) -> bool {
		if (!m_shapecast) {
			LOG_ERROR("{} missing daelking shapecast3d ptr", str(get_name()));
			return false;
		}

		m_shapecast->set_enabled(true);
		m_shapecast->set_target_position(context.owner.get_position());
		m_shapecast->force_shapecast_update();

		if (m_shapecast->is_colliding()) {
			m_shapecast->set_enabled(false);
			return true;
		}
		return false;
	}

	auto allocate_daelking_arrow() -> godot::Node3D* {
		if (get_arrow_scene().is_null()) {
			LOG_ERROR("{} arrow scene is_null", str(get_path()));
			return nullptr;
		}

		if (!get_arrow_scene()->can_instantiate()) {
			LOG_ERROR("{} arrow scene !can_instantiate", str(get_path()));
			return nullptr;
		}

		auto arrow = cast_to<godot::Node3D>(get_arrow_scene()->instantiate());
		if (!arrow) {
			LOG_ERROR("{} failed to instantiate arrow scene", str(get_path()));
			return nullptr;
		}

		arrow->set_name(g_arrow_name);
		arrow->set_as_top_level(true); // Not inheriting parent transform, force global transform only
		arrow->hide();

		m_arrow = arrow;
		return arrow;
	}

	auto arrow_enable(const Context& context) -> void {
		if (!m_arrow) {
			return;
		}

		m_arrow->show();
		m_arrow->set_position(
			[gp = context.owner.get_global_position()] -> godot::Vector3
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
