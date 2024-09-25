#pragma once

#include <godot_cpp/classes/node.hpp>
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
};

}