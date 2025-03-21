#include "version.h"

#include <gdextension_interface.h>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/core/defs.hpp>
#include <godot_cpp/godot.hpp>

#include "character/player_character.h"

void init_parameter_plugin_module(godot::ModuleInitializationLevel t_level) {
	if (t_level != godot::ModuleInitializationLevel::MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}

	std::cout << "Init Steikemann plugin version: " << STEIKEMANN_VERSION_STR << std::endl;

	godot::ClassDB::register_class<player_character::PlayerCharacterBody>();
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
