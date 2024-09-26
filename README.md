# godot_gv_texture_serializer

GVTextureSerializer addon for Godot 4.2.2 - 4.3

You can use this for Texture sharing as bytes, via ZeroMQ / Shared Memory etc.

## Install

use [godot_gv_texture_serializer_bin](https://github.com/funatsufumiya/godot_gv_texture_serializer_bin). see [instruction](https://github.com/funatsufumiya/godot_gv_texture_serializer_bin/blob/main/README.md)

## Usage

see [project/](project/), or [GDScript of it](project/test.gd)

```gdscript
var src_image: Image = sub_viewport.get_texture().get_image()
# print("compresse image")
# print("width: ", src_image.get_width())
# print("height: ", src_image.get_height())
# print("format: ", _image_format_to_string(src_image.get_format()))
# print("image_bytes: ", src_image.get_data().size())

var bytes:PackedByteArray = serializer.serializeImage(src_image)
# print("packed bytes: ", bytes.size())

var image: Image = serializer.deserialize(bytes)
# print("decompressed image")
# print("width: ", image.get_width())
# print("height: ", image.get_height())
# print("format: ", _image_format_to_string(image.get_format()))
# print("image_bytes: ", image.get_data().size())

texture_rect.texture = ImageTexture.create_from_image(image) # show decompressed image
```


## Binary File Format (GVTexture)

```txt
0: uint32_t width
4: uint32_t height
12: uint32_t format (DXT1 = 1, DXT3 = 3, DXT5 = 5, BC7 = 7)
16: uint32_t frame bytes
20: raw frame storage (lz4 compressed)
```

This format is nearly same as [ofxExtremeGpuVideo](https://github.com/Ushio/ofxExtremeGpuVideo), [godot_gv_video_extension](https://github.com/funatsufumiya/godot_gv_video_extension)

## Build and Run

(This process is needed only if you build this plugin by your own)

```bash
$ git submodule update --init --recursive --recommend-shallow --depth 1
$ scons
$ scons target=template_release
$ godot project/project.godot # (only first time)
$ godot --path project/ # run demo
```
