#ifndef GD_COREMACROES_PLUGIN_STEIKEMANNGAMEPLAY_H
#define GD_COREMACROES_PLUGIN_STEIKEMANNGAMEPLAY_H

#define GETNAME(class_name) String get_class_name() const { return #class_name; }
#define DEFAULT_PROPERTY(class_name)                                                                 \
    ClassDB::bind_method(D_METHOD("get_class_name"), &class_name::get_class_name);                   \
    ClassDB::add_property(                                                                           \
        #class_name,                                                                                 \
        PropertyInfo(Variant::STRING, "class_name", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_DEFAULT), \
        "",                                                                                          \
        "get_class_name");

#endif // GD_COREMACROES_PLUGIN_STEIKEMANNGAMEPLAY_H