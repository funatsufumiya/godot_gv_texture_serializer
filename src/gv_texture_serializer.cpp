#include "gv_texture_serializer.hpp"

#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/core/class_db.hpp>

#include <memory>

using namespace godot;

// NOTE
// ## Binary File Format (GVTexture)

// ```txt
// 0: uint32_t width
// 4: uint32_t height
// 8: uint32_t frame count
// 12: float fps
// 16: uint32_t format (DXT1 = 1, DXT3 = 3, DXT5 = 5, BC7 = 7)
// 20: uint32_t frame bytes
// 24: raw frame storage (lz4 compressed)
// eof - (frame count) * 16: [(uint64_t, uint64_t)..<frame count] (address, size) of lz4, address is zero based from file head
// ```

#define DXT1 1
#define DXT3 3
#define DXT5 5
#define BC7 7

void GVTextureSerializer::_bind_methods() {
    // ClassDB::bind_method(D_METHOD("_init", "inPort", "outPort", "outIP"), &OSC::_init);
    // ClassDB::bind_static_method("GVTextureSerializer", D_METHOD("create", "inPort", "outPort", "outIP"), &OSC::create);
    ClassDB::bind_method(D_METHOD("serializeImage", "image"), &GVTextureSerializer::serializeImage);
    ClassDB::bind_method(D_METHOD("serializeTexture2D", "texture"), &GVTextureSerializer::serializeTexture2D);
    ClassDB::bind_method(D_METHOD("serializeCompressedTexture2D", "texture"), &GVTextureSerializer::serializeCompressedTexture2D);
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

PackedByteArray GVTextureSerializer::serializeImage(const Ref<Image> &p_image)
{
    UtilityFunctions::print("GVTextureSerializer::serializeImage, but not implemented");
    return PackedByteArray();
}

PackedByteArray GVTextureSerializer::serializeTexture2D(const Ref<Texture2D> &p_texture)
{
    // UtilityFunctions::print("GVTextureSerializer::serializeTexture2D");
    return serializeImage(p_texture->get_image());
}

PackedByteArray GVTextureSerializer::serializeCompressedTexture2D(const Ref<CompressedTexture2D> &p_texture)
{
    // UtilityFunctions::print("GVTextureSerializer::serializeCompressedTexture2D");
    return serializeImage(p_texture->get_image());
}