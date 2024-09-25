#pragma once

#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/classes/ref.hpp>
#include <godot_cpp/classes/image.hpp>
#include <godot_cpp/classes/texture2d.hpp>
#include <godot_cpp/classes/compressed_texture2d.hpp>
#include "lz4.h"

namespace godot {

enum ImageFormat {
    FORMAT_L8 = 0,
    FORMAT_LA8 = 1,
    FORMAT_R8 = 2,
    FORMAT_RG8 = 3,
    FORMAT_RGB8 = 4,
    FORMAT_RGBA8 = 5,
    FORMAT_RGBA4444 = 6,
    FORMAT_RGB565 = 7,
    FORMAT_RF = 8,
    FORMAT_RGF = 9,
    FORMAT_RGBF = 10,
    FORMAT_RGBAF = 11,
    FORMAT_RH = 12,
    FORMAT_RGH = 13,
    FORMAT_RGBH = 14,
    FORMAT_RGBAH = 15,
    FORMAT_RGBE9995 = 16,
    FORMAT_DXT1 = 17,
    FORMAT_DXT3 = 18,
    FORMAT_DXT5 = 19,
    FORMAT_RGTC_R = 20,
    FORMAT_RGTC_RG = 21,
    FORMAT_BPTC_RGBA = 22,
    FORMAT_BPTC_RGBF = 23,
    FORMAT_BPTC_RGBFU = 24,
    FORMAT_ETC = 25,
    FORMAT_ETC2_R11 = 26,
    FORMAT_ETC2_R11S = 27,
    FORMAT_ETC2_RG11 = 28,
    FORMAT_ETC2_RG11S = 29,
    FORMAT_ETC2_RGB8 = 30,
    FORMAT_ETC2_RGBA8 = 31,
    FORMAT_ETC2_RGB8A1 = 32,
    FORMAT_ETC2_RA_AS_RG = 33,
    FORMAT_DXT5_RA_AS_RG = 34,
    FORMAT_ASTC_4x4 = 35,
    FORMAT_ASTC_4x4_HDR = 36,
    FORMAT_ASTC_8x8 = 37,
    FORMAT_ASTC_8x8_HDR = 38,
    FORMAT_MAX = 39
};

inline std::string imageFormatToString(int format) {
    switch (format) {
        case FORMAT_L8: return "L8";
        case FORMAT_LA8: return "LA8";
        case FORMAT_R8: return "R8";
        case FORMAT_RG8: return "RG8";
        case FORMAT_RGB8: return "RGB8";
        case FORMAT_RGBA8: return "RGBA8";
        case FORMAT_RGBA4444: return "RGBA4444";
        case FORMAT_RGB565: return "RGB565";
        case FORMAT_RF: return "RF";
        case FORMAT_RGF: return "RGF";
        case FORMAT_RGBF: return "RGBF";
        case FORMAT_RGBAF: return "RGBAF";
        case FORMAT_RH: return "RH";
        case FORMAT_RGH: return "RGH";
        case FORMAT_RGBH: return "RGBH";
        case FORMAT_RGBAH: return "RGBAH";
        case FORMAT_RGBE9995: return "RGBE9995";
        case FORMAT_DXT1: return "DXT1";
        case FORMAT_DXT3: return "DXT3";
        case FORMAT_DXT5: return "DXT5";
        case FORMAT_RGTC_R: return "RGTC_R";
        case FORMAT_RGTC_RG: return "RGTC_RG";
        case FORMAT_BPTC_RGBA: return "BPTC_RGBA";
        case FORMAT_BPTC_RGBF: return "BPTC_RGBF";
        case FORMAT_BPTC_RGBFU: return "BPTC_RGBFU";
        case FORMAT_ETC: return "ETC";
        case FORMAT_ETC2_R11: return "ETC2_R11";
        case FORMAT_ETC2_R11S: return "ETC2_R11S";
        case FORMAT_ETC2_RG11: return "ETC2_RG11";
        case FORMAT_ETC2_RG11S: return "ETC2_RG11S";
        case FORMAT_ETC2_RGB8: return "ETC2_RGB8";
        case FORMAT_ETC2_RGBA8: return "ETC2_RGBA8";
        case FORMAT_ETC2_RGB8A1: return "ETC2_RGB8A1";
        case FORMAT_ETC2_RA_AS_RG: return "ETC2_RA_AS_RG";
        case FORMAT_DXT5_RA_AS_RG: return "DXT5_RA_AS_RG";
        case FORMAT_ASTC_4x4: return "ASTC_4x4";
        case FORMAT_ASTC_4x4_HDR: return "ASTC_4x4_HDR";
        case FORMAT_ASTC_8x8: return "ASTC_8x8";
        case FORMAT_ASTC_8x8_HDR: return "ASTC_8x8_HDR";
        case FORMAT_MAX: return "MAX";
        default: return "UNKNOWN";
    }
}

struct LZ4Data {
    PackedByteArray lz4_compressed_bytes;
    size_t frame_size;
    int width;
    int height;
    int format;
};

class GVTextureSerializer : public Node
{
    GDCLASS(GVTextureSerializer, Node);

protected:
	static void _bind_methods();

    int compressedImageFormat = -1;

    PackedByteArray serializeImageWithoutLZ4(const Ref<Image> &p_image);
    PackedByteArray compressLZ4(const PackedByteArray &p_data);
    PackedByteArray decompressLZ4(const PackedByteArray &p_data, size_t frameSize);
    PackedByteArray createGVTextureByteArray(LZ4Data lz4Data);
    LZ4Data getLZ4DataFromGVTextureByteArray(const PackedByteArray &p_data);
    int imageFormatToGVTextureFormat(int image_format);
    int gvTextureFormatToImageFormat(int gv_texture_format);

public:

    GVTextureSerializer();
    ~GVTextureSerializer();

    void _ready() override;
    void _process(double delta) override;
    PackedByteArray serializeImage(const Ref<Image> &p_image);
    PackedByteArray serializeTexture2D(const Ref<Texture2D> &p_texture);
    PackedByteArray serializeCompressedTexture2D(const Ref<CompressedTexture2D> &p_texture);
    Ref<Image> deserialize(const PackedByteArray &p_data);

    int getCompressedImageFormat() { return compressedImageFormat; }
};

}