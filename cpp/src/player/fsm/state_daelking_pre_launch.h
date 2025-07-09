#pragma once

#include "gameplay_events/daelk_event.h"
#include "gameplay_events/gameplay_node.h"
#include "input/typedef.h"
#include "log.h"
#include "macros.h"
#include "mathstatics.h"
#include "timestamp.h"
#include "typedef.h"
#include "utils.h"

#include "godot_cpp/classes/packed_scene.hpp"
#include "godot_cpp/variant/basis.hpp"

class DaelkingPreLaunchState : public PlayerStateBase {
	GDCLASS(DaelkingPreLaunchState, PlayerStateBase)

public:
	static inline const char* p_arrow_name = "Arrow3D";

private:
	PROPERTY(godot::Ref<godot::PackedScene>, arrow_scene);

	godot::Node3D* m_arrow = nullptr;

	TimeStamp m_cooldown_timestamp;

public:
	static void _bind_methods() {
		using godot::PackedScene;
		BIND_RESOURCE_PROPERTY_METHODS(DaelkingPreLaunchState, arrow_scene, PackedScene);
	}

	virtual auto enter(Context& c) -> std::optional<TransitionContext> override {
		m_arrow = allocate_visual_arrow(c);
		arrow_enable(c);

		if (c.daelked_node_path.has_value()) {
			send_daelk_pre_launch_event(c, c.daelked_node_path.value());
		}

		return {};
	}

	virtual auto exit(Context& c) -> void override {
		m_cooldown_timestamp.stamp();
		deallocate_nodes(c);
		return;
	}

	virtual auto process(Context&, double) -> std::optional<TransitionContext> override {
		return {};
	}

	virtual auto physics_process(Context&, double) -> std::optional<TransitionContext> override {
		return {};
	}

	virtual auto input_callback(Context& c) -> std::optional<TransitionContext> override {
		if (auto direction_opt = get_daelking_direction(c.input)) {
			c.daelk_launch_direction = direction_opt.value();
		}

		set_arrow_rotation(c.daelk_launch_direction, c.input);

		if (c.input.daelking.just_released()) {
			return TransitionContext{ .state = EState::DAELKING_LAUNCH };
		}
		return {};
	}

private:
	auto send_daelk_pre_launch_event(const Context& c, const godot::NodePath node_path) -> void {
		if (auto* gameplay_node = c.owner.get_node<GameplayNode3D>(node_path)) {
			gameplay_node->handle_daelk_pre_launch_event(DaelkPreLaunchEvent{ .direction = c.daelk_launch_direction });
		}
	}

	auto arrow_enable(const Context& c) -> void {
		if (!m_arrow) {
			return;
		}

		m_arrow->show();
		m_arrow->set_position(
			[gp = c.owner.get_global_position()] -> godot::Vector3
			{
				auto pos = gp;
				pos.z = 1.f; // Tmp foreground placement
				return pos;
			}());
	}

	auto set_arrow_rotation(const godot::Vector3& direction, const InputState& input) -> void {
		if (!m_arrow) {
			return;
		}

		const auto angle_offset = 90.f; // Arrow is by default pointing up.
		const auto angle = get_direction_angle(direction, angle_offset);
		m_arrow->set_basis(godot::Basis(mathstatics::depth, -angle));
	}

	auto allocate_visual_arrow(Context& c) -> godot::Node3D* {
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

		arrow->set_name(p_arrow_name);
		arrow->set_as_top_level(true); // Not inheriting parent transform, force global transform only
		arrow->hide();

		c.owner.add_child(arrow);
		return arrow;
	}

	auto deallocate_nodes(Context& c) -> void {
		if (m_arrow) {
			c.owner.remove_child(m_arrow);
			m_arrow->queue_free();
			m_arrow = nullptr;
		}
	}
};
