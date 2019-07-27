#include "Act.hpp"

#include <cmath>
#include "InvalidFile.hpp"

using namespace std;

Act::Act(Buffer& buf)
{
    try {
        parse(buf);
    }
    catch (const out_of_range&) {
        throw InvalidFile("act: missing data");
    }
}

void Act::parse(Buffer& buf)
{
    // <magic>.2B <version>.S

    const char act_magic[] = {'A', 'C'};
    char magic[sizeof(act_magic)];

    buf.read(magic, sizeof(act_magic));

    // Check magic
    if (strncmp(magic, act_magic, sizeof(act_magic)) != 0)
        throw InvalidFile("act: invalid magic, expected 'AC'");

    version_ = buf.readUint16();

    // Check version
    switch (version_)
    {
        case 0x200:
		case 0x201:
		case 0x202:
		case 0x203:
		case 0x204:
		case 0x205:
            break; // supported
        default:
            throw InvalidFile("act: unsupported version '" + to_string(versionMajor()) + '.' + to_string(versionMinor()) + "'");
    }

    // Action count
    actions_.resize(buf.readUint16());
    
    // Unnused 10 bytes
    buf.skip(10);

    // Read actions
    for (Action& action : actions_)
    {
        // Motions
        action.motions.resize(buf.readUint32());

        for (Motion& motion : action.motions)
        {
            // ranges (yet to figure this out)
            buf.skip(8 * 4);

            // Sprite clips
            motion.sprite_clips.resize(buf.readUint32());

            for (SpriteClip& spr_clip : motion.sprite_clips)
            {
                spr_clip.x = buf.readUint32();
                spr_clip.y = buf.readUint32();
                spr_clip.sprite_number = buf.readUint32();
                spr_clip.mirror = (buf.readUint32() != 0);
                
                if (version_ >= 0x200)
                {
                    spr_clip.color = Color(buf.readUint32());

                    if (version_ >= 0x204) {
                        buf.read(&spr_clip.scale_x, sizeof(float));
                        buf.read(&spr_clip.scale_y, sizeof(float));
                        spr_clip.scale_x = round(spr_clip.scale_x);
                        spr_clip.scale_y = round(spr_clip.scale_y);
                    }
                    else {
                        buf.read(&spr_clip.scale_x, sizeof(float));
                        spr_clip.scale_x = round(spr_clip.scale_x);
                        spr_clip.scale_y = spr_clip.scale_x;
                    }

                    spr_clip.rotation = buf.readUint32();
                    spr_clip.image_type = static_cast<SpriteClip::ImageType>(buf.readUint32());

                    if (version_ >= 0x205) {
                        spr_clip.width = buf.readUint32();
                        spr_clip.height = buf.readUint32();
                    }
                }
            }

            // Event id
            if (version_ >= 0x200)
                motion.event_id = buf.readUint32();

            // Attach points
            if (version_ >= 0x203) {
                motion.attach_points.resize(buf.readUint32());

                for (AttachPoint& point : motion.attach_points)
                {
                    buf.skip(4); // unnused
                    point.x = buf.readUint32();
                    point.y = buf.readUint32();
                    point.attr = buf.readUint32();
                }
            }
        }
    }

    if (version_ >= 0x201)
    {
        size_t event_count = buf.readUint32();

        // Read events
        for (size_t i = 0; i < event_count; i++)
        {
            std::array<char, 40> name;
            buf.read(name.data(), name.size());
            name[39] = '\0';

            events_.emplace_back(Event(std::move(name)));
        }
    }

    // Read delays
    if (version_ >= 0x202)
    {
        for (Action& action : actions_) {
            buf.read(&action.delay, sizeof(float));
            action.delay = round(action.delay);
        }
    }
}

static_assert(sizeof(float) == 4, "float size is not 4 bytes");