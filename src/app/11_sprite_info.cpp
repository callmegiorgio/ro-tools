#include <cstdlib>
#include <cstring>
#include <iostream>
#include "../format/Sprite.hpp"
#include "../util/filehandler.hpp"

using namespace std;
using namespace format;

int main(int argc, const char* argv[])
{
    if (argc < 2) {
        cout << "Usage: " << argv[0] << " [-<verbosity>, -h] <sprite file>" << endl;
        return 1;
    }

    // Show help
    if (strcmp(argv[1], "-h") == 0)
    {
        cout << "-<verbosity>:" << endl;
        cout << "  0 - default" << endl;
        cout << "  1 - image, sound, and animation details" << endl;
        cout << "  2 - frame details" << endl;
        cout << "  3 - layer details" << endl;
        cout << "-h: Shows this message" << endl;
        return 0;
    }

    const char* filename = argv[argc - 1];
    const int verbosity = (argc > 2 && argv[1][0] == '-') ? atoi(&argv[1][1]) : 0;

    try {
        // Try opening the file and creating sprite object
        Sprite sprite(readFile(filename));

        cout << "Sprite file '" << filename << "' loaded with verbosity " << verbosity << ":" << endl;

        cout << "  images: " << sprite.images.size() << endl;

        if (verbosity >= 1) {
            for (const Sprite::Image& image : sprite.images)
                cout << "   - " << image.width << 'x' << image.height << " pixels" << endl;
        }

        cout << "  sounds: " << sprite.sounds.size() << endl;

        if (verbosity >= 1) {
            for (const Sprite::Sound& sound : sprite.sounds)
                cout << "    " << sound.filename.data() << endl;
        }

        cout << "  animations: " << sprite.animations.size() << endl;

        if (verbosity >= 1)
        {
            for (const Sprite::Animation& anim : sprite.animations)
            {
                cout << "  {" << endl;
                cout << "    delay: " << anim.delay << endl;
                cout << "    frames: " << anim.frames.size() << endl;

                if (verbosity >= 2) 
                {
                    for (const Sprite::Frame& frame : anim.frames)
                    {
                        cout << "    {" << endl;
                        cout << "      layers: " << frame.layers.size() << endl;

                        if (verbosity >= 3)
                        {
                            for (const Sprite::Layer& layer : frame.layers)
                            {
                                const Sprite::Image* image = layer.image_index < sprite.images.size() ?
                                    &sprite.images[layer.image_index] : nullptr;

                                cout << "      {" << endl;
                                cout << "        image index: " << int(layer.image_index) << endl;
                                cout << "        x: " << int(layer.x) << endl;
                                cout << "        y: " << int(layer.y) << endl;
                                cout << "        width: " << (image ? image->width : 0) << endl;
                                cout << "        height: " << (image ? image->height : 0) << endl;
                                cout << "        rotation: " << layer.rotation << endl;
                                //cout << "        scale x: " << image.scale_x << endl;
                                //cout << "        scale y: " << image.scale_y << endl;
                                cout << "        color: " << layer.color << endl;
                                cout << "        mirror: " << std::boolalpha << layer.mirror << endl;
                                cout << "      }" << endl;
                            }
                        }

                        cout << "      anchor x: " << frame.anchor_x << endl;
                        cout << "      anchor y: " << frame.anchor_y << endl;
                        cout << "      sound index: " << int(frame.sound_index) << endl;
                        cout << "    }" << endl;
                    }
                }

                cout << "  }" << endl;
            }
        }
    }
    catch (const exception& e) {
        cout << "Exception: " << e.what() << endl;
        return 1;
    }    

    return 0;
}