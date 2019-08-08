#include "Str.hpp"

#include <cstring>
#include <iostream>
#include "../util/InvalidResource.hpp"

using namespace std;

namespace format {

static Str::Frame::BlendType blendTypeFromUint(unsigned int value)
{
    switch (value)
    {
        case 0:
		case 1: return Str::Frame::Zero;
		case 2: return Str::Frame::One;
		case 3: return Str::Frame::SrcColor;
		case 4: return Str::Frame::InvSrcColor;
		case 5: return Str::Frame::SrcAlpha;
		case 6: return Str::Frame::InvSrcAlpha;
		case 7: return Str::Frame::DestAlpha;
		case 8: return Str::Frame::InvDestAlpha;
		case 9: return Str::Frame::DestColor;
		case 10: return Str::Frame::InvDestColor;
		case 11: return Str::Frame::SrcAlphaSat;
		case 12: return Str::Frame::BothSrcAlpha;
        case 13: return Str::Frame::BothInvSrcAlpha;
    }

    return Str::Frame::Zero;
}

void Str::load(const Buffer& buf)
try {
    const char str_magic[] = {'S', 'T', 'R', 'M'};
    constexpr size_t magic_size = sizeof(str_magic);
    char magic[magic_size];

    buf.read(magic, magic_size);

    // Check magic
    if (strncmp(magic, str_magic, magic_size) != 0)
        throw InvalidResource("str: invalid magic, expected 'STRM'");

    version = buf.readUint32();
    
    if (version != 148)
        throw InvalidResource("str: unknown version '" + to_string(version) + '\'');

    fps = buf.readUint32();
    frame_count = buf.readUint32();
    
    // Layer count
    layers.resize(buf.readUint32());
    
    buf.skip(16);

    for (Layer& layer : layers)
    {
        // Textures
        layer.textures.resize(buf.readUint32());

        for (Texture& texture : layer.textures)
        {
            buf.read(texture.filename.data(), texture.filename.size());
            texture.filename[127] = '\0';
        }

        // Frames
        layer.frames.resize(buf.readUint32());

        for (Frame& frame : layer.frames)
        {
            frame.frame_number = buf.readUint32();
            frame.morph = buf.readUint32() == 1;
            frame.position.x = buf.readFloat();
            frame.position.y = buf.readFloat();
            
            // First texture uv mapping coordinates
            float u = buf.readFloat();
            float v = buf.readFloat();
            float us = buf.readFloat();
            float vs = buf.readFloat();

            frame.uv_mapping.a = Point2D(u, v);
            frame.uv_mapping.b = Point2D(us, v);
            frame.uv_mapping.c = Point2D(us, vs);
            frame.uv_mapping.d = Point2D(u, vs);

            // Second texture uv mapping coordinates
            u = buf.readFloat();
            v = buf.readFloat();
            us = buf.readFloat();
            vs = buf.readFloat();

            frame.uv_mapping2.a = Point2D(u, v);
            frame.uv_mapping2.b = Point2D(us, v);
            frame.uv_mapping2.c = Point2D(us, vs);
            frame.uv_mapping2.d = Point2D(u, vs);

            // Drawing rect positions
            frame.drawing_rect.a.x = buf.readFloat();
            frame.drawing_rect.b.x = buf.readFloat();
            frame.drawing_rect.c.x = buf.readFloat();
            frame.drawing_rect.d.x = buf.readFloat();
            frame.drawing_rect.a.y = buf.readFloat();
            frame.drawing_rect.b.y = buf.readFloat();
            frame.drawing_rect.c.y = buf.readFloat();
            frame.drawing_rect.d.y = buf.readFloat();

            frame.texture_index = buf.readUint32(); // float?
            frame.anitype = buf.readUint32();
            frame.anidelta = buf.readFloat();
            frame.rz = buf.readFloat();

            frame.color = Color(
                static_cast<uint8_t>(buf.readFloat()),
                static_cast<uint8_t>(buf.readFloat()),
                static_cast<uint8_t>(buf.readFloat()),
                static_cast<uint8_t>(buf.readFloat())
            );

            frame.src_blend_type = blendTypeFromUint(buf.readUint32());
            frame.dest_blend_type = blendTypeFromUint(buf.readUint32());
            frame.mtpreset = buf.readUint32();
        }
    }
}
catch (const out_of_range&) {
    throw InvalidResource("str: missing data");
}

} // namespace format