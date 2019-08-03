#include <cstring>
#include <iostream>
#include <string>
#include "../format/Act.hpp"
#include "../format/Pal.hpp"
#include "../format/Spr.hpp"
#include "../format/Sprite.hpp"
#include "../util/filehandler.hpp"

using namespace std;
using namespace format;

Sprite toSprite(Act act, Spr spr)
{
    Sprite sprite;

    // Palette
    sprite.pal = *spr.pal;

    // Images
    sprite.images.resize(spr.palette_images.size());

    for (int i = 0; i < spr.palette_images.size(); i++)
    {
        sprite.images[i].width = spr.palette_images[i].width;
        sprite.images[i].height = spr.palette_images[i].height;
        sprite.images[i].indices = move(spr.palette_images[i].indices);
    }
    
    // Sounds
    sprite.sounds.resize(act.sounds.size());

    for (int i = 0; i < act.sounds.size(); i++)
        sprite.sounds[i].filename = move(act.sounds[i].filename);

    // Animations
    sprite.animations.resize(act.animations.size());

    for (int i = 0; i < act.animations.size(); i++)
    {
        Sprite::Animation& spr_anim = sprite.animations[i];
        Act::Animation& act_anim = act.animations[i];

        spr_anim.delay = static_cast<uint16_t>(act_anim.delay);

        // Frames
        spr_anim.frames.resize(act_anim.frames.size());

        for (int j = 0; j < act_anim.frames.size(); j++)
        {
            Sprite::Frame& spr_frame = spr_anim.frames[j];
            Act::Frame& act_frame = act_anim.frames[j];

            if (!act_frame.anchors.empty()) {
                spr_frame.anchor_x = act_frame.anchors[0].x;
                spr_frame.anchor_y = act_frame.anchors[0].y;
            }
            else 
                spr_frame.anchor_x = spr_frame.anchor_y = 0;

            spr_frame.sound_index = static_cast<int8_t>(act_frame.sound_index);

            // Images/layers
            spr_frame.layers.resize(act_frame.images.size());

            for (int k = 0; k < act_frame.images.size(); k++)
            {
                Sprite::Layer& layer = spr_frame.layers[k];
                Act::Image& image = act_frame.images[k];

                layer.image_index = static_cast<uint8_t>(image.index);
                layer.x = static_cast<int16_t>(image.x);
                layer.y = static_cast<int16_t>(image.y);
                layer.rotation = static_cast<uint16_t>(image.rotation);
                layer.color = image.color;
                layer.color.a = 255;
                layer.mirror = image.mirror;
            }
        }
    }

    return move(sprite);
}

int main(int argc, const char* argv[])
{
    if (argc < 3) {
        cout << "Usage: " << argv[0] << " <act file> <sprite file>" << endl;
        return 1;
    }

    const char* filename = argv[1];
    size_t filename_len = strlen(filename);

    if (filename_len < 3) {
        cout << "File name is too short" << endl;
        return 1;
    }

    try {
        Act act(readFile(filename));
        memcpy(const_cast<char*>(&filename[filename_len - 3]), "spr", 3);
        Spr spr(readFile(filename));

        if (!spr.pal) {
            cout << "File '" << filename << "' has no palette" << endl;
            return 1;
        }

        // Create sprite out of act and spr
        Sprite sprite = toSprite(move(act), move(spr));

        // Serialize sprite obj to buffer
        Buffer buf;
        sprite.save(buf);

        writeFile(argv[2], buf);
        cout << "Exported sprite: " << argv[2] << endl;
    }
    catch (const exception& e) {
        cout << "Exception: " << e.what() << endl;
        return 1;
    }    

    return 0;
}