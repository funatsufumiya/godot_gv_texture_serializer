#include "gv_texture_serializer.hpp"

#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/core/class_db.hpp>

#include <memory>

using namespace godot;

void GVTextureSerializer::_bind_methods() {
    // ClassDB::bind_method(D_METHOD("_init", "inPort", "outPort", "outIP"), &OSC::_init);
    // ClassDB::bind_static_method("GVTextureSerializer", D_METHOD("create", "inPort", "outPort", "outIP"), &OSC::create);
}


GVTextureSerializer::GVTextureSerializer()
{
    UtilityFunctions::print("GVTextureSerializer::constructor");
}

GVTextureSerializer::~GVTextureSerializer()
{
    // UtilityFunctions::print("GVTextureSerializer::destructor");
}

void GVTextureSerializer::_ready()
{
    // UtilityFunctions::print("GVTextureSerializer::_ready");
}

void GVTextureSerializer::_process(double delta)
{
    // UtilityFunctions::print("GVTextureSerializer::_process");
}