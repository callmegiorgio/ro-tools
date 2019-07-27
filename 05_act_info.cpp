#include <iostream>
#include "filereader.hpp"
#include "Act.hpp"

using namespace std;

int main(int argc, const char* argv[])
{
    if (argc < 2) {
        cout << "Usage: " << argv[0] << " <act file>" << endl;
        return 1;
    }

    try {
        // Try opening the file and creating act object
        Act act(readFile(argv[1]));

        const vector<Act::Action>& actions = act.actions();
        const vector<Act::Event>& events = act.events();

        cout << "Act file '" << argv[1] << "' loaded:" << endl;
        cout << "  version: " << act.versionMajor() << '.' << act.versionMinor() << endl;
        cout << "  actions: " << actions.size() << endl;

        for (auto& action : actions)
        {
            cout << "  {" << endl;
            cout << "    delay: " << action.delay << endl;
            cout << "    motions: " << action.motions.size() << endl;

            for (auto& motion : action.motions)
            {
                cout << "    {" << endl;
                cout << "      event id: " << motion.event_id << endl;
                cout << "      sprite clips: " << motion.sprite_clips.size() << endl;

                for (auto& spr_clip : motion.sprite_clips)
                {
                    cout << "      {" << endl;
                    cout << "        x: " << spr_clip.x << endl;
                    cout << "        y: " << spr_clip.y << endl;
                    cout << "        width: " << spr_clip.width << endl;
                    cout << "        height: " << spr_clip.height << endl;
                    cout << "        rotation: " << spr_clip.rotation << endl;
                    cout << "        scale x: " << spr_clip.scale_x << endl;
                    cout << "        scale y: " << spr_clip.scale_y << endl;
                    cout << "        color: " << spr_clip.color << endl;
                    cout << "        sprite number: " << spr_clip.sprite_number << endl;
                    cout << "        mirror: " << std::boolalpha << spr_clip.mirror << endl;
                    cout << "        image type: " << (spr_clip.image_type == Act::SpriteClip::Palette ? "palette" : "rgba") << endl;
                    cout << "      }" << endl;
                }

                cout << "      attach points: " << motion.attach_points.size() << endl;

                for (auto& point : motion.attach_points)
                {
                    cout << "      {" << endl;
                    cout << "        x: " << point.x << endl;
                    cout << "        y: " << point.y << endl;
                    cout << "        attr: " << point.attr << endl;
                    cout << "      }" << endl;
                }

                cout << "    }" << endl;
            }

            cout << "  }" << endl;
        }

        cout << "  events: " << events.size() << endl;

        for (auto& event : events)
            cout << "    " << event.name() << endl;
    }
    catch (const exception& e) {
        cout << "Exception: " << e.what() << endl;
        return 1;
    }    

    return 0;
}