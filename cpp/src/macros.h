#pragma once

#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/core/object.hpp>

/**
 * In Editor or in Game
 */
#define EDITOR_SCOPE if (godot::Engine::get_singleton()->is_editor_hint())
#define GAME_SCOPE if (!godot::Engine::get_singleton()->is_editor_hint())

/**
 * Property get-set helper macros
 */
#define PROPERTY(type, propertyname, ...)                                                                              \
public:                                                                                                                \
	auto get_##propertyname()->type {                                                                                  \
		return m_##propertyname;                                                                                       \
	}                                                                                                                  \
	auto set_##propertyname(type value)->void {                                                                        \
		m_##propertyname = value;                                                                                      \
	}                                                                                                                  \
                                                                                                                       \
private:                                                                                                               \
	type m_##propertyname __VA_OPT__(=) __VA_ARGS__;

/**
 * Macros for static void _bind_methods() {...}
 * BIND_METHOD
 *      Allow compile time error when function name changes. Since godot expects String name...
 * BIND_PROPERTY_METHODS
 *      Property 'get..' and 'set..' methods. Required to let editor read and set property value
 * BIND_RESOURCE_PROPERTY_METHODS
 *      'get' & 'set' methods
 *      filling property info, so editor can filter resource by class
 */
#define TOSTRING(x) #x

#define BIND_METHOD(classname, method, ...)                                                                            \
	godot::ClassDB::bind_method(godot::D_METHOD(TOSTRING(method) __VA_OPT__(, ) __VA_ARGS__), &classname::method);

#define BIND_PROPERTY_METHODS(classname, propertyname, type, ...)                                                      \
	godot::ClassDB::bind_method(godot::D_METHOD(TOSTRING(get_##propertyname)), &classname::get_##propertyname);        \
	godot::ClassDB::bind_method(                                                                                       \
		godot::D_METHOD(TOSTRING(set_##propertyname), "value"), &classname::set_##propertyname);                       \
	godot::ClassDB::add_property(get_class_static(),                                                                   \
		godot::PropertyInfo(::godot::Variant::type, #propertyname __VA_OPT__(, ) __VA_ARGS__),                         \
		TOSTRING(set_##propertyname),                                                                                  \
		TOSTRING(get_##propertyname))

#define BIND_RESOURCE_PROPERTY_METHODS(classname, propertyname, propertyclassname)                                     \
	godot::ClassDB::bind_method(godot::D_METHOD(TOSTRING(get_##propertyname)), &classname::get_##propertyname);        \
	godot::ClassDB::bind_method(                                                                                       \
		godot::D_METHOD(TOSTRING(set_##propertyname), "value"), &classname::set_##propertyname);                       \
	static_assert(std::is_base_of_v<godot::Resource, propertyclassname>);                                              \
	godot::ClassDB::add_property(get_class_static(),                                                                   \
		godot::PropertyInfo(::godot::Variant::OBJECT,                                                                  \
			#propertyname,                                                                                             \
			godot::PropertyHint::PROPERTY_HINT_RESOURCE_TYPE,                                                          \
			#propertyclassname),                                                                                       \
		TOSTRING(set_##propertyname),                                                                                  \
		TOSTRING(get_##propertyname))
