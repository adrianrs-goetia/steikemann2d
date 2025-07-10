#pragma once

#include <optional>

#include "groups.h"
#include "log.h"
#include "notifications.h"

#include "godot_cpp/classes/node3d.hpp"
#include "godot_cpp/classes/wrapped.hpp"
#include "godot_cpp/variant/node_path.hpp"
#include "godot_cpp/variant/typed_array.hpp"

class PlayerSpawnPoint;
class TemporaryPlayerSpawnPoint;

/**
 * Abstraction which spawns player at correct location.
 * On in-game spawnpoints or on temporary spawn points.
 */
class PlayerSpawner : public godot::Node3D {
	GDCLASS(PlayerSpawner, godot::Node3D)

	godot::TypedArray<godot::NodePath> m_spawnpoints;
	std::optional<godot::Vector3> m_temporaryspawnpoint;

public:
	static void _bind_methods() {}

	static auto get_path() -> godot::NodePath {
		return godot::NodePath("/root/" + get_class_static());
	}

	void _enter_tree() override {
		set_name(get_class_static());
		add_to_group(group::playerspawn::name);
	}

	void _notification(int what) {
		if (what == SteikeNotification::SPAWN_PLAYER) {
			LOG_INFO("Got spawn player notification");
		}
	}

	void register_spawn_point(const godot::NodePath& path) {
		if (get_node<PlayerSpawnPoint>(path)) {
			m_spawnpoints.append(path);
		}
	}
	void unregister_spawn_point(const godot::NodePath& path) {
		if (get_node<PlayerSpawnPoint>(path)) {
			m_spawnpoints.erase(path);
		}
	}

	void register_temporary_spawn_point(const godot::Vector3& globalspawnposition) {
		m_temporaryspawnpoint = globalspawnposition;
	}
	void unregister_temporary_spawn_point() {
		if (m_temporaryspawnpoint.has_value()) {
			LOG_NEW_INFO("Unregistering temporary spawn point: ", m_temporaryspawnpoint.value());
		}
		m_temporaryspawnpoint = std::nullopt;
	}

	auto get_current_spawn_location() const -> std::optional<godot::Vector3> {
		if (m_temporaryspawnpoint.has_value()) {
			return m_temporaryspawnpoint.value();
			// if (const auto* spawnpoint = get_node<godot::Node3D>(m_temporaryspawnpoint.value())) { }
		}
		LOG_WARN("No spawnpoint available, returning nullopt.");
		return {};
	}
};
