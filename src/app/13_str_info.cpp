#include <iostream>
#include "../format/Str.hpp"
#include "../util/filehandler.hpp"

using namespace std;
using namespace format;

int main(int argc, const char* argv[])
{
    if (argc < 2) {
        cout << "Usage: " << argv[0] << " <str file>" << endl;
        return 1;
    }

    try {
        Str str(readFile(argv[1]));
        
        cout << "Str file '" << argv[1] << "' loaded:" << endl;
        cout << "  version: " << str.version << endl;
        cout << "  fps: " << str.fps << endl;
        cout << "  frame count: " << str.frame_count << endl;
        cout << "  layers: " << str.layers.size() << endl;

        for (const Str::Layer& layer : str.layers)
        {
            cout << "  {" << endl;
            cout << "    textures: " << layer.textures.size() << endl;

            for (const Str::Texture& texture : layer.textures)
                cout << "     - " << texture.filename.data() << endl;

            cout << "    frames: " << layer.frames.size() << endl;

            for (const Str::Frame& frame : layer.frames)
            {
                cout << "    {" << endl;
                cout << "      frame number: " << frame.frame_number << endl;
                cout << "      morph: " << std::boolalpha << frame.morph << endl;
                cout << "      position: " << frame.position << endl;
                cout << "      uv mapping: " << frame.uv_mapping << endl;
                cout << "      uv mapping 2: " << frame.uv_mapping2 << endl;
                cout << "      drawing rect: " << frame.drawing_rect << endl;
                cout << "      texture index: " << frame.texture_index << endl;
                cout << "      anitype: " << frame.anitype << endl;
                cout << "      anidelta: " << frame.anidelta << endl;
                cout << "      rz: " << frame.rz << endl;
                cout << "      color: " << frame.color << endl;
                cout << "      src blend type: " << frame.src_blend_type << endl;
                cout << "      dest blend type: " << frame.dest_blend_type << endl;
                cout << "      mtpreset: " << frame.mtpreset << endl;
                cout << "    }" << endl;
            }

            cout << "  }" << endl;
        }
    }
    catch (const exception& e) {
        cout << "Exception: " << e.what() << endl;
        return 1;
    }

    return 0;
}