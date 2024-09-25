# godot_gv_texture_serializer

GVTextureSerializer addon for Godot 4.2.2 - 4.3

Can be used for Texture sharing as bytes, with ZeroMQ etc.

## Usage

***WIP***

## Install

***WIP***

use [godot_gv_texture_serializer_bin](https://github.com/funatsufumiya/godot_gv_texture_serializer_bin). see [instruction](https://github.com/funatsufumiya/godot_gv_texture_serializer_bin/blob/main/README.md)

## Binary File Format (GVTexture)

```txt
0: uint32_t width
4: uint32_t height
8: uint32_t frame count
12: float fps
16: uint32_t format (DXT1 = 1, DXT3 = 3, DXT5 = 5, BC7 = 7)
20: uint32_t frame bytes
24: raw frame storage (lz4 compressed)
eof - (frame count) * 16: [(uint64_t, uint64_t)..<frame count] (address, size) of lz4, address is zero based from file head
```

## Build and Run

(This process is needed only if you build this plugin by your own)

```bash
$ git submodule update --init --recursive --recommend-shallow --depth 1
$ scons
$ scons target=template_release
$ godot project/project.godot # (only first time)
$ godot --path project/ # run demo
```
