#pragma once

#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/core/object.hpp>

#define TOSTRING(x) #x

#define BIND_METHOD(classname, method, ...)                                                                            \
	godot::ClassDB::bind_method(godot::D_METHOD(TOSTRING(method) __VA_OPT__(, ) __VA_ARGS__), &classname::method);

#define BIND_PROPERTY_METHODS(classname, method, type)                                                                 \
	::godot::ClassDB::bind_method(godot::D_METHOD(TOSTRING(get_##method)), &classname::get_##method);                  \
	::godot::ClassDB::bind_method(godot::D_METHOD(TOSTRING(set_##method), "value"), &classname::set_##method);         \
	::godot::ClassDB::add_property(get_class_static(),                                                                 \
		::godot::PropertyInfo(::godot::Variant::type, "m_##method"),                                                   \
		TOSTRING(set_##method),                                                                                        \
		TOSTRING(get_##method))
