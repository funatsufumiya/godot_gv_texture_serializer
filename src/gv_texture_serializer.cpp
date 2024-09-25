#include "gv_texture_serializer.hpp"

#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/core/class_db.hpp>

#include <memory>

#include "stb_dxt.h"
#include "lz4.h"

using namespace godot;

// NOTE
// ## Binary File Format (GVTexture)

// ```txt
// 0: uint32_t width
// 4: uint32_t height
// 12: uint32_t format (DXT1 = 1, DXT3 = 3, DXT5 = 5, BC7 = 7)
// 16: uint32_t frame bytes
// 20: uint32_t compressed bytes
// 24: raw frame storage (lz4 compressed)
// ```

#define GVT_DXT1 1
#define GVT_DXT3 3
#define GVT_DXT5 5
#define GVT_BC7 7

void GVTextureSerializer::_bind_methods() {
    // ClassDB::bind_method(D_METHOD("_init", "inPort", "outPort", "outIP"), &OSC::_init);
    // ClassDB::bind_static_method("GVTextureSerializer", D_METHOD("create", "inPort", "outPort", "outIP"), &OSC::create);
    ClassDB::bind_method(D_METHOD("serializeImage", "image"), &GVTextureSerializer::serializeImage);
    ClassDB::bind_method(D_METHOD("serializeTexture2D", "texture"), &GVTextureSerializer::serializeTexture2D);
    ClassDB::bind_method(D_METHOD("serializeCompressedTexture2D", "texture"), &GVTextureSerializer::serializeCompressedTexture2D);
    ClassDB::bind_method(D_METHOD("deserialize", "data"), &GVTextureSerializer::deserialize);
}


GVTextureSerializer::GVTextureSerializer()
{
    // UtilityFunctions::print("GVTextureSerializer::constructor");
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

int GVTextureSerializer::imageFormatToGVTextureFormat(int image_format)
{
    switch (image_format) {
        case ImageFormat::FORMAT_RGBA8: return GVT_DXT5;
        case ImageFormat::FORMAT_DXT5: return GVT_DXT5;
        case ImageFormat::FORMAT_DXT3: return GVT_DXT3;
        case ImageFormat::FORMAT_DXT1: return GVT_DXT1;
        case ImageFormat::FORMAT_BPTC_RGBA: return GVT_BC7;
        default: {
            UtilityFunctions::push_error("Unsupported image format");
            return -1;
        }
    }
}

int GVTextureSerializer::gvTextureFormatToImageFormat(int gv_texture_format)
{
    switch (gv_texture_format) {
        case GVT_DXT5: return ImageFormat::FORMAT_DXT5;
        case GVT_DXT3: return ImageFormat::FORMAT_DXT3;
        case GVT_DXT1: return ImageFormat::FORMAT_DXT1;
        case GVT_BC7: return ImageFormat::FORMAT_BPTC_RGBA;
        default: {
            UtilityFunctions::push_error("Unsupported GVTexture format");
            return -1;
        }
    }
}

PackedByteArray GVTextureSerializer::serializeImageWithoutLZ4(const Ref<Image> &p_image)
{
    // UtilityFunctions::print("GVTextureSerializer::serializeImageWithoutLZ4");

    int image_format = p_image->get_format();
    String format_name = imageFormatToString(image_format).c_str();
    // UtilityFunctions::print("Image format: " + format_name);

    if (image_format == ImageFormat::FORMAT_RGBA8 ) {
        PackedByteArray bytes_rgba8 = p_image->get_data();
        // void rygCompress( unsigned char *dst, unsigned char *src, int w, int h, int isDxt5 );
        PackedByteArray bytes_dxt5 = PackedByteArray();
        size_t rgba_size = p_image->get_width() * p_image->get_height() * 4;
        // NOTE: dxt5 size is 1/4 of rgba8 size, dxt1 size is 1/6 of rgba8 size
        bytes_dxt5.resize( rgba_size / 4 );

        rygCompress( (unsigned char *)bytes_dxt5.ptrw(), (unsigned char *)bytes_rgba8.ptrw(), p_image->get_width(), p_image->get_height(), 1 );

        // UtilityFunctions::print("bytes_rgba8.size(): " + String::num_int64(bytes_rgba8.size()));
        // UtilityFunctions::print("bytes_dxt5.size(): " + String::num_int64(bytes_dxt5.size()));

        this->compressedImageFormat = FORMAT_DXT5;

        return bytes_dxt5;
    } else if (image_format == ImageFormat::FORMAT_DXT5) {
        PackedByteArray bytes_dxt5 = p_image->get_data();
        this->compressedImageFormat = FORMAT_DXT5;
        return bytes_dxt5;
    } else if (image_format == ImageFormat::FORMAT_DXT3) {
        PackedByteArray bytes_dxt3 = p_image->get_data();
        this->compressedImageFormat = FORMAT_DXT3;
        return bytes_dxt3;
    } else if (image_format == ImageFormat::FORMAT_DXT1) {
        PackedByteArray bytes_dxt1 = p_image->get_data();
        this->compressedImageFormat = FORMAT_DXT1;
        return bytes_dxt1;
    } else if (image_format == ImageFormat::FORMAT_BPTC_RGBA ) { // BC7
        PackedByteArray bytes_bc7 = p_image->get_data();
        this->compressedImageFormat = FORMAT_BPTC_RGBA;
        return bytes_bc7;
    } else {
        UtilityFunctions::push_error("Unsupported image format: " + format_name);
        return PackedByteArray();
    }
}

PackedByteArray GVTextureSerializer::compressLZ4(const PackedByteArray &p_data)
{
    PackedByteArray compressed_data = PackedByteArray();
    size_t compressed_bound = LZ4_compressBound(p_data.size());
    compressed_data.resize( compressed_bound );
    // return PackedByteArray();
    int real_compressed_size = LZ4_compress_default(
        (const char *)p_data.ptr(),
        (char *)compressed_data.ptrw(),
        p_data.size(),
        compressed_bound
    );
    if (real_compressed_size < 0) {
        UtilityFunctions::push_error("LZ4 compression failed");
        abort();
    }
    compressed_data.resize(real_compressed_size);

    return compressed_data;
}

PackedByteArray GVTextureSerializer::decompressLZ4(const PackedByteArray &p_data, size_t frameSize)
{
    PackedByteArray decompressed_data = PackedByteArray();
    size_t decompressed_bound = frameSize;
    decompressed_data.resize( decompressed_bound );
    int real_decompressed_size = LZ4_decompress_safe(
        (const char *)p_data.ptr(),
        (char *)decompressed_data.ptrw(),
        p_data.size(),
        decompressed_bound
    );
    if (real_decompressed_size < 0) {
        UtilityFunctions::push_error("LZ4 decompression failed");
        abort();
    }
    decompressed_data.resize(real_decompressed_size);

    return decompressed_data;
}

PackedByteArray GVTextureSerializer::createGVTextureByteArray(LZ4Data lz4Data)
{
    PackedByteArray gv_texture_bytes = PackedByteArray();
    gv_texture_bytes.resize(20 + lz4Data.lz4_compressed_bytes.size());
    uint32_t *gv_texture_bytes_ptr = (uint32_t *)gv_texture_bytes.ptrw();
    gv_texture_bytes_ptr[0] = lz4Data.width;
    gv_texture_bytes_ptr[1] = lz4Data.height;
    gv_texture_bytes_ptr[2] = lz4Data.format;
    gv_texture_bytes_ptr[3] = lz4Data.frame_size;
    gv_texture_bytes_ptr[4] = lz4Data.lz4_compressed_bytes.size();
    memcpy(gv_texture_bytes_ptr + 5, lz4Data.lz4_compressed_bytes.ptr(), lz4Data.lz4_compressed_bytes.size());

    return gv_texture_bytes;
}

LZ4Data GVTextureSerializer::getLZ4DataFromGVTextureByteArray(const PackedByteArray &p_data)
{
    LZ4Data lz4Data;
    uint32_t *gv_texture_bytes_ptr = (uint32_t *)p_data.ptr();
    lz4Data.width = gv_texture_bytes_ptr[0];
    lz4Data.height = gv_texture_bytes_ptr[1];
    lz4Data.format = gv_texture_bytes_ptr[2];
    lz4Data.frame_size = gv_texture_bytes_ptr[3];
    lz4Data.lz4_compressed_bytes.resize(gv_texture_bytes_ptr[4]);
    memcpy(lz4Data.lz4_compressed_bytes.ptrw(), gv_texture_bytes_ptr + 5, gv_texture_bytes_ptr[4]);

    return lz4Data;
}

PackedByteArray GVTextureSerializer::serializeImage(const Ref<Image> &p_image)
{
    // UtilityFunctions::print("GVTextureSerializer::serializeImage");

    PackedByteArray bytes_dxt5 = serializeImageWithoutLZ4(p_image);
    PackedByteArray compressed_bytes = compressLZ4(bytes_dxt5);

    LZ4Data lz4Data;
    lz4Data.lz4_compressed_bytes = compressed_bytes;
    lz4Data.width = p_image->get_width();
    lz4Data.height = p_image->get_height();
    lz4Data.format = imageFormatToGVTextureFormat(this->compressedImageFormat);
    lz4Data.frame_size = bytes_dxt5.size();

    // UtilityFunctions::print("serialized info");
    // UtilityFunctions::print("lz4Data.width: " + String::num_int64(lz4Data.width));
    // UtilityFunctions::print("lz4Data.height: " + String::num_int64(lz4Data.height));
    // UtilityFunctions::print("lz4Data.format: " + String::num_int64(lz4Data.format));
    // UtilityFunctions::print("lz4Data.frame_size: " + String::num_int64(lz4Data.frame_size));
    // UtilityFunctions::print("lz4Data.compressed_bytes.size(): " + String::num_int64(lz4Data.lz4_compressed_bytes.size()));

    return createGVTextureByteArray(lz4Data);
}

Ref<Image> GVTextureSerializer::deserialize(const PackedByteArray &p_data)
{
    // UtilityFunctions::print("GVTextureSerializer::deserialize");

    LZ4Data lz4Data = getLZ4DataFromGVTextureByteArray(p_data);

    // UtilityFunctions::print("deserialized info");
    // UtilityFunctions::print("lz4Data.width: " + String::num_int64(lz4Data.width));
    // UtilityFunctions::print("lz4Data.height: " + String::num_int64(lz4Data.height));
    // UtilityFunctions::print("lz4Data.format: " + String::num_int64(lz4Data.format));
    // UtilityFunctions::print("lz4Data.frame_size: " + String::num_int64(lz4Data.frame_size));
    // UtilityFunctions::print("lz4Data.compressed_bytes.size(): " + String::num_int64(lz4Data.lz4_compressed_bytes.size()));

    PackedByteArray decompressed_bytes = decompressLZ4(lz4Data.lz4_compressed_bytes, lz4Data.frame_size);

    // UtilityFunctions::print("decompressed_bytes.size(): " + String::num_int64(decompressed_bytes.size()));

    int image_format = gvTextureFormatToImageFormat(lz4Data.format);
    Image::Format format = (Image::Format)image_format;

    // UtilityFunctions::print("image_format: " + String(imageFormatToString(image_format).c_str()));

    Ref<Image> image = Image::create_from_data(lz4Data.width, lz4Data.height, false, format, decompressed_bytes);

    // UtilityFunctions::print("image->get_data().size(): " + String::num_int64(image->get_data().size()));

    return image;
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