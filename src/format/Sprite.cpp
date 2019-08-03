#include "Sprite.hpp"

#include "../util/InvalidFile.hpp"

using namespace std;

namespace format {

void Sprite::load(const Buffer& buf)
try {
    // Palette
    pal.load(buf);

    // Images
    images.resize(buf.readUint8());

    for (Image& image : images)
    {
        image.width = buf.readUint16();
        image.height = buf.readUint16();
        const size_t index_count = image.width * image.height;

        if (!index_count)
            continue;
        
        image.indices.resize(index_count);
        buf.read(image.indices.data(), image.indices.size());
    }

    // Sounds
    sounds.resize(buf.readUint8());

    for (Sound& sound : sounds)
    {
        buf.read(sound.filename.data(), sound.filename.size());
        sound.filename[39] = '\0';
    }

    // Animations
    animations.resize(buf.readUint8());

    for (Animation& anim : animations)
    {
        anim.delay = buf.readUint16();
        
        // Frames
        anim.frames.resize(buf.readUint8());

        for (Frame& frame : anim.frames)
        {
            // Layers
            frame.layers.resize(buf.readUint8());

            for (Layer& layer : frame.layers)
            {
                layer.image_index = buf.readUint8();
                layer.x = buf.readInt16();
                layer.y = buf.readInt16();
                layer.rotation = buf.readUint16();
                // scale_x
                // scale_y
                layer.color.r = buf.readUint8();
                layer.color.g = buf.readUint8();
                layer.color.b = buf.readUint8();
                layer.color.a = buf.readUint8();
                layer.mirror = buf.readUint8();
            }

            frame.anchor_x = buf.readInt16();
            frame.anchor_y = buf.readInt16();
            frame.sound_index = buf.readInt8();
        }
    }
}
catch (const out_of_range&) {
    throw InvalidFile("sprite: missing data");
}

void Sprite::save(Buffer& buf) const
{
    // Palette
    pal.save(buf);

    // Images
    buf.writeUint8(static_cast<uint8_t>(images.size()));

    for (const Image& image : images)
    {
        buf.writeUint16(image.width);
        buf.writeUint16(image.height);
        const size_t index_count = image.width * image.height;

        // TODO: should throw or log it?
        if (!index_count)// || index_count != image.indices.size())
            continue;

        buf.write(image.indices.data(), image.indices.size());
    }

    // Sounds
    buf.writeUint8(static_cast<uint8_t>(sounds.size()));

    for (const Sound& sound : sounds)
        buf.write(sound.filename.data(), sound.filename.size());

    // Animations
    buf.writeUint8(static_cast<uint8_t>(animations.size()));

    for (const Animation& anim : animations)
    {
        buf.writeUint16(anim.delay);
        
        // Frames
        buf.writeUint8(static_cast<uint8_t>(anim.frames.size()));

        for (const Frame& frame : anim.frames)
        {
            // Layers
            buf.writeUint8(static_cast<uint8_t>(frame.layers.size()));

            for (const Layer& layer : frame.layers)
            {
                buf.writeUint8(layer.image_index);
                buf.writeInt16(layer.x);
                buf.writeInt16(layer.y);
                buf.writeUint16(layer.rotation);
                // scale_x
                // scale_y
                buf.writeUint8(layer.color.r);
                buf.writeUint8(layer.color.g);
                buf.writeUint8(layer.color.b);
                buf.writeUint8(layer.color.a);
                buf.writeUint8(layer.mirror);
            }

            buf.writeInt16(frame.anchor_x);
            buf.writeInt16(frame.anchor_y);
            buf.writeInt8(frame.sound_index);
        }
    }
}

} // namespace format