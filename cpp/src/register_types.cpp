#include "gdextension_interface.h"
#include "godot_cpp/core/class_db.hpp"
#include "godot_cpp/core/defs.hpp"
#include "godot_cpp/godot.hpp"

#include "log.h"

#include "camera/playercamera.h"
#include "gameplay_events/gameplay_node.h"
#include "input/input_manager.h"
#include "input/input_parser.h"
#include "player/fsm/state_daelking_launch.h"
#include "player/fsm/state_daelking_pre_launch.h"
#include "player/fsm/state_walking.h"
#include "player/fsm/typedef.h"
#include "player/movementcomponent.h"
#include "player/playercharacterbody.h"
#include "terrain/terrainobjectbase.h"

#ifdef DEBUG_ENABLED
#define TESTS_ENABLED
#include "tests/tests.h"
#include <godot_cpp/classes/os.hpp>
#endif

void init_parameter_plugin_module(godot::ModuleInitializationLevel t_level) {
	if (t_level != godot::ModuleInitializationLevel::MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}

	LOG_TRACE("!! Hello again steikemann cpp !!");

	// camera/
	godot::ClassDB::register_class<PlayerCamera>();

	// gameplay_events/
	godot::ClassDB::register_class<GameplayNode3D>();

	// input/
	godot::ClassDB::register_class<InputManager>();
	godot::ClassDB::register_internal_class<InputParser>();

	// player/
	godot::ClassDB::register_class<MovementComponent>();
	godot::ClassDB::register_class<PlayerCharacterBody>();
	// player/fsm
	godot::ClassDB::register_abstract_class<PlayerStateBase>();
	godot::ClassDB::register_class<WalkingState>();
	godot::ClassDB::register_class<DaelkingPreLaunchState>();
	godot::ClassDB::register_class<DaelkingLaunchState>();

	// terrain
	godot::ClassDB::register_class<TerrainObjectBase>();

#ifdef TESTS_ENABLED
	const auto cmd_line_user_args = godot::OS::get_singleton()->get_cmdline_user_args();
	if (tests::should_run(cmd_line_user_args)) {
		LOG_INFO("\n-----------------------------------------------");
		LOG_INFO("\t\tStart tests");

		auto test_suites = tests::register_classes();
		tests::run(test_suites);

		LOG_INFO("\n\t\tEnd tests");
		LOG_INFO("-----------------------------------------------");
	}
#endif
}

void uninit_parameter_plugin_module(godot::ModuleInitializationLevel t_level) {
	if (t_level != godot::ModuleInitializationLevel::MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}
}

extern "C" {
GDExtensionBool GDE_EXPORT steikemann_plugin_init(GDExtensionInterfaceGetProcAddress t_get_proc_address,
	const GDExtensionClassLibraryPtr t_library, GDExtensionInitialization* t_initialization) {
	godot::GDExtensionBinding::InitObject init_obj(t_get_proc_address, t_library, t_initialization);

	init_obj.register_initializer(init_parameter_plugin_module);
	init_obj.register_terminator(uninit_parameter_plugin_module);
	init_obj.set_minimum_library_initialization_level(
		godot::ModuleInitializationLevel::MODULE_INITIALIZATION_LEVEL_SCENE);

	return init_obj.init();
}
}
