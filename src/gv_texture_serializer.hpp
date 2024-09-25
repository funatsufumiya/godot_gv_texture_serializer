#pragma once

#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/classes/ref.hpp>
#include <godot_cpp/classes/image.hpp>
#include <godot_cpp/classes/texture2d.hpp>
#include <godot_cpp/classes/compressed_texture2d.hpp>
#include "lz4.h"

namespace godot {

class GVTextureSerializer : public Node
{
    GDCLASS(GVTextureSerializer, Node);

protected:
	static void _bind_methods();

public:

    GVTextureSerializer();
    ~GVTextureSerializer();

    void _ready() override;
    void _process(double delta) override;
    PackedByteArray serializeImage(const Ref<Image> &p_image);
    PackedByteArray serializeTexture2D(const Ref<Texture2D> &p_texture);
    PackedByteArray serializeCompressedTexture2D(const Ref<CompressedTexture2D> &p_texture);
};

}