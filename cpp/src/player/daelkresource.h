#pragma once

#include "godot_cpp/variant/vector3.hpp"
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
	struct Context {
		godot::Node3D& owner;
	};
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

	auto physics_process(const Context& context, godot::Vector3 velocity, double delta) -> godot::Vector3 {
		if (get_daelk_impulse()) {
			velocity.y = get_daelking_impulse_strength();
			m_daelk_impulse = false;
		}
		return velocity;
	}

	auto handle_input(const Context& context, const InputState& input) -> void {
		if (input.daelking.just_pressed()) {
			m_daelk_impulse = detect_daelking_collision(context);
			if (m_daelk_impulse) {
				arrow_enable(context);
			}
		}
		else if (input.daelking.just_released()) {
			arrow_disable();
		}
	}

	auto init_daelking_shapecast_node() -> godot::ShapeCast3D* {
		auto* shapecast = memnew(godot::ShapeCast3D);
		shapecast->set_name(g_shapecast_name);
		shapecast->set_enabled(false);
		shapecast->set_collide_with_areas(true);
		shapecast->set_collision_mask(collisionflag::daelking_collision);

		if (get_daelking_collision_shape().is_null()) {
			LOG_ERROR(
				"{} is missing a daelking collision shape resource.", godot::String(get_path()).utf8().get_data());
			return nullptr;
		}
		shapecast->set_shape(get_daelking_collision_shape());
		m_shapecast = shapecast;
		return shapecast;
	}

	auto detect_daelking_collision(const Context& context) -> bool {
		if (!m_shapecast) {
			LOG_ERROR("{} missing daelking shapecast3d ptr", godot::String(get_name()).utf8().get_data());
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
