#include <iostream>
#include "../resource/Spr.hpp"
#include "../util/filehandler.hpp"

using namespace std;

int main(int argc, const char* argv[])
{
    if (argc < 2) {
        cout << "Usage: " << argv[0] << " <spr file>" << endl;
        return 1;
    }

    try {
        // Try opening the file and creating spr object
        Spr spr(readFile(argv[1]));

        cout << "Spr file '" << argv[1] << "' loaded:" << endl;
        cout << "  version: " << spr.version.major << '.' << spr.version.minor << endl;
        cout << "  palette images: " << spr.palette_images.size() << endl;

        for (const Spr::PaletteImage& img : spr.palette_images)
            cout << "   - " << img.width << 'x' << img.height << " pixels" << endl;

        cout << "  rgba images: " << spr.rgba_images.size() << endl;

        for (const Spr::RgbaImage& img : spr.rgba_images)
            cout << "    - " << img.width << 'x' << img.height << " pixels" << endl;

        cout << "  has palette? " << std::boolalpha << (spr.pal != nullptr) << endl;
    }
    catch (const exception& e) {
        cout << "Exception: " << e.what() << endl;
        return 1;
    }    

    return 0;
}