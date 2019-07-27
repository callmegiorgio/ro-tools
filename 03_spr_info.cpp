#include <iostream>
#include "filereader.hpp"
#include "Spr.hpp"

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

        const vector<Spr::PaletteImage>& pal_images = spr.paletteImages();
        const vector<Spr::RgbaImage>& rgba_images = spr.rgbaImages();

        cout << "Spr file " << argv[1] << " loaded:" << endl;
        cout << "  version: " << spr.versionMajor() << '.' << spr.versionMinor() << endl;
        cout << "  palette images: " << pal_images.size() << endl;

        for (auto& img : pal_images)
            cout << "   - " << img.width << 'x' << img.height << " pixels" << endl;

        cout << "  rgba images: " << rgba_images.size() << endl;

        for (auto& img : rgba_images)
            cout << "    - " << img.width << 'x' << img.height << " pixels" << endl;

        cout << "  has palette? " << boolalpha << (spr.pal() != nullptr) << endl;
    }
    catch (const exception& e) {
        cout << "Exception: " << e.what() << endl;
        return 1;
    }    

    return 0;
}