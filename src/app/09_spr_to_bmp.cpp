#include <cstring>
#include <iostream>
#include <vector>
#include <string>
#include <string_view>
#include "../format/Image.hpp"
#include "../format/Spr.hpp"
#include "../util/Buffer.hpp"
#include "../util/filehandler.hpp"

using namespace std;
using namespace format;

void exportBmpFile(const string& path, const string& filename, int width, int height, int channels, const void* pixels)
{
    string bmp_fn(path + filename);
    
    Buffer buffer;

    try {
        Image::saveAsBmp(buffer, width, height, channels, pixels);

        // Try writing it to disk
        writeFile(bmp_fn.c_str(), buffer);
        cout << "Exported bmp: " << bmp_fn << " (" << width << 'x' << height << 'x' << channels << ')' << endl;
    }
    catch (const exception& e) {
        cout << "Error saving bmp: " << e.what() << endl;
    }
}

int main(int argc, const char* argv[])
{
    if (argc < 2) {
        cout << "Usage: " << argv[0] << " <spr file> [<path>]" << endl;
        return 1;
    }

    const char* spr_fn = argv[1];
    
    try {
        // Try opening the file and creating spr object
        Spr spr(readFile(spr_fn));

        // Remove the path from spr filename if any.
        if (const char* p = strrchr(argv[1], '/'))
            spr_fn = p+1;
        else if (const char* p = strrchr(argv[1], '\\'))
            spr_fn = p+1;

        // Get the file extension by looking for the last '.'.
        const char* spr_ext = strrchr(spr_fn, '.');
        
        // Cut off the extension part if any.
        const size_t spr_fn_len = spr_ext ? (spr_ext - spr_fn) : strlen(spr_fn);

        string bmp_path;

        if (argc > 2)
            bmp_path.assign(strchr("/\\", argv[2][strlen(argv[2]) - 1]) ? argv[2] : string(argv[2]) + '/');

        string spr_name = string(spr_fn, spr_fn_len);

        if (spr.pal)
        {
            for (int i = 0; i < spr.palette_images.size(); i++)
            {
                const Spr::PaletteImage& image = spr.palette_images[i];
                
                std::vector<uint8_t> pixels(image.indices.size() * 4);

                // Generate the pixels from the combination of indices and palette colors.
                for (int j = 0; j < image.indices.size(); j++)
                {
                    uint8_t index = image.indices[j];
                    const Color& color = spr.pal->colors[index];
                    
                    pixels[j*4 + 0] = color.r;
                    pixels[j*4 + 1] = color.g;
                    pixels[j*4 + 2] = color.b;
                    pixels[j*4 + 3] = 255;
                }
                
                // filename.spr -> path/filename_i.bmp
                exportBmpFile(bmp_path, spr_name + '_' + to_string(i+1) + ".bmp", image.width, image.height, 4, pixels.data());
            }
        }

        
        for (int i = 0; i < spr.rgba_images.size(); i++)
        {
            const Spr::RgbaImage& image = spr.rgba_images[i];
            
            // filename.spr -> path/filename_rgba_i.bmp
            exportBmpFile(bmp_path, spr_name + "_rgba_" + to_string(i+1) + ".bmp", image.width, image.height, 4, image.pixels.data());
        }
    }
    catch (const exception& e) {
        cout << "Exception: " << e.what() << endl;
        return 1;
    }    

    return 0;
}