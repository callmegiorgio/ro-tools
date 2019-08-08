#include "Act.hpp"

#include <cstring>
#include <cmath>
#include "../util/InvalidResource.hpp"

using namespace std;

namespace format {

void Act::load(const Buffer& buf)
try {
    const char act_magic[] = {'A', 'C'};
    char magic[sizeof(act_magic)];

    buf.read(magic, sizeof(act_magic));

    // Check magic
    if (strncmp(magic, act_magic, sizeof(act_magic)) != 0)
        throw InvalidResource("act: invalid magic, expected 'AC'");

    const int hex_version = buf.readUint16();
    version.major = (hex_version >> 8) & 0xFF;
    version.minor = hex_version & 0xFF;

    // Check version
    switch (hex_version)
    {
        case 0x200:
		case 0x201:
		case 0x202:
		case 0x203:
		case 0x204:
		case 0x205:
            break; // supported
        default:
            throw InvalidResource("act: unsupported version '" + to_string(version.major) + '.' + to_string(version.minor) + "'");
    }

    // Animation count
    animations.resize(buf.readUint16());
    
    // Unnused 10 bytes
    buf.skip(10);

    // Read animations
    for (Animation& anim : animations)
    {
        // Frame count
        anim.frames.resize(buf.readUint32());

        // Read frames
        for (Frame& frame : anim.frames)
        {
            // Unnused 32 bytes
            buf.skip(32);

            // Images count
            frame.images.resize(buf.readUint32());

            // Read images
            for (Image& image : frame.images)
            {
                image.x = buf.readUint32();
                image.y = buf.readUint32();
                image.index = buf.readUint32();
                image.mirror = (buf.readUint32() != 0);
                
                if (hex_version >= 0x200)
                {
                    image.color = Color(buf.readUint32());

                    if (hex_version >= 0x204) {
                        image.scale_x = round(buf.readFloat());
                        image.scale_y = round(buf.readFloat());
                    }
                    else {
                        image.scale_x = image.scale_y = round(buf.readFloat());
                    }

                    image.rotation = buf.readUint32();
                    image.is_rgba = (buf.readUint32() == 1); // 0 - palette, 1 - rgba

                    // dontjump?

                    if (hex_version >= 0x205) {
                        image.width = buf.readUint32();
                        image.height = buf.readUint32();
                    }
                }
            }

            // Sound index
            if (hex_version >= 0x200)
                frame.sound_index = buf.readUint32();

            // Anchors
            if (hex_version >= 0x203) {
                frame.anchors.resize(buf.readUint32());

                for (Anchor& anchor : frame.anchors)
                {
                    buf.skip(4); // unnused
                    anchor.x = buf.readUint32();
                    anchor.y = buf.readUint32();
                    anchor.attribute = buf.readUint32();
                }
            }
        }
    }

    if (hex_version >= 0x201)
    {
        // Sound count
        sounds.resize(buf.readUint32());

        // Read sound paths
        for (Sound& sound : sounds)
        {
            buf.read(sound.filename.data(), sound.filename.size());
            sound.filename[39] = '\0';
        }
    }

    // Read delays
    if (hex_version >= 0x202)
    {
        for (Animation& anim : animations)
            anim.delay = round(buf.readFloat());
    }
}
catch (const out_of_range&) {
    throw InvalidResource("act: missing data");
}

void Act::save(Buffer& buf) const
{

}

} // namespace format