#include <cstdlib>
#include <cstring>
#include <iostream>
#include "../resource/Act.hpp"
#include "../util/filehandler.hpp"

using namespace std;

int main(int argc, const char* argv[])
{
    if (argc < 2) {
        cout << "Usage: " << argv[0] << " [-<verbosity>, -h] <act file>" << endl;
        return 1;
    }

    // Show help
    if (strcmp(argv[1], "-h") == 0)
    {
        cout << "-<verbosity>:" << endl;
        cout << "  0 - default" << endl;
        cout << "  1 - animation and sound details" << endl;
        cout << "  2 - frame details" << endl;
        cout << "  3 - image and anchor details" << endl;
        cout << "-h: Shows this message" << endl;
        return 0;
    }

    const char* filename = argv[argc - 1];
    const int verbosity = (argc > 2 && argv[1][0] == '-') ? atoi(&argv[1][1]) : 0;

    try {
        // Try opening the file and creating act object
        Act act(readFile(filename));

        cout << "Act file '" << filename << "' loaded with verbosity " << verbosity << ":" << endl;
        cout << "  version: " << act.version.major << '.' << act.version.minor << endl;
        cout << "  animations: " << act.animations.size() << endl;

        if (verbosity >= 1)
        {
            for (const Act::Animation& anim : act.animations)
            {
                cout << "  {" << endl;
                cout << "    delay: " << anim.delay << endl;
                cout << "    frames: " << anim.frames.size() << endl;

                if (verbosity >= 2) 
                {
                    for (const Act::Frame& frame : anim.frames)
                    {
                        cout << "    {" << endl;
                        cout << "      sound index: " << frame.sound_index << endl;
                        cout << "      images: " << frame.images.size() << endl;

                        if (verbosity >= 3)
                        {
                            for (const Act::Image& image : frame.images)
                            {
                                cout << "      {" << endl;
                                cout << "        x: " << image.x << endl;
                                cout << "        y: " << image.y << endl;
                                cout << "        width: " << image.width << endl;
                                cout << "        height: " << image.height << endl;
                                cout << "        rotation: " << image.rotation << endl;
                                cout << "        scale x: " << image.scale_x << endl;
                                cout << "        scale y: " << image.scale_y << endl;
                                cout << "        color: " << image.color << endl;
                                cout << "        index: " << image.index << endl;
                                cout << "        mirror: " << std::boolalpha << image.mirror << endl;
                                cout << "        is rgba: " << std::boolalpha << image.is_rgba << endl;
                                cout << "      }" << endl;
                            }
                        }

                        cout << "      anchors: " << frame.anchors.size() << endl;

                        if (verbosity >= 3)
                        {
                            for (const Act::Anchor& anchor : frame.anchors)
                            {
                                cout << "      {" << endl;
                                cout << "        x: " << anchor.x << endl;
                                cout << "        y: " << anchor.y << endl;
                                cout << "        attribute: " << anchor.attribute << endl;
                                cout << "      }" << endl;
                            }
                        }

                        cout << "    }" << endl;
                    }
                }

                cout << "  }" << endl;
            }
        }

        cout << "  sounds: " << act.sounds.size() << endl;

        if (verbosity >= 1) {
            for (const Act::Sound& sound : act.sounds)
                cout << "    " << sound.filepath.data() << endl;
        }
    }
    catch (const exception& e) {
        cout << "Exception: " << e.what() << endl;
        return 1;
    }    

    return 0;
}