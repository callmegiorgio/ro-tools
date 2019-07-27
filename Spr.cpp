#include "Spr.hpp"

#include <iostream>
#include <cstring>
#include <fstream>
#include <vector>
#include "InvalidFile.hpp"

using namespace std;

Spr::Spr(Buffer& buf)
{
    try {
        parse(buf);
    }
    catch (const out_of_range&) {
        throw InvalidFile("spr: missing data");
    }
}

void Spr::parse(Buffer& buf)
{
    // 0x100, 0x101: <magic>.2B <version>.S <pal image count>.S <pal images>...
    // 0x200, 0x201: <magic>.2B <version>.S <pal image count>.S <rgba image count>.S <pal images>... <rgba images>...
    // <pal images>: <width>.S <height>.S <data>.<width * height>
    // <rgba images>: <width>.S <height>.S <rgba color>.4B

    const char spr_magic[] = {'S', 'P'};
    char magic[sizeof(spr_magic)];

    buf.read(magic, sizeof(spr_magic));

    // Check magic
    if (strncmp(magic, spr_magic, sizeof(spr_magic)) != 0)
        throw InvalidFile("spr: invalid magic, expected 'SP'");

    // Get version
    version_ = buf.readUint16();

    switch (version_)
    {
        case 0x100:
        case 0x101:
        case 0x200:
        case 0x201:
            break; // supported    
        default:
            throw InvalidFile("spr: unsupported version '" + to_string(versionMajor()) + '.' + to_string(versionMinor()) + "'");
    }

    // Get palette images count
    uint16_t pal_img_count = buf.readUint16();
    
    // Get rgba images count if any
    uint16_t rgba_img_count = (version_ >= 0x200 ? buf.readUint16() : 0);

    for (int i = 0; i < pal_img_count; i++)
    {
        PaletteImage img;
        img.width = buf.readUint16();
        img.height = buf.readUint16();
        const size_t pixel_count = img.width * img.height;

        //cout << "pal img [" << (i+1) << '/' << pal_img_count << "]: width: " << img.width << ", height: " << img.height << ", pixel count: " << pixel_count << endl;

        if (!pixel_count)
            continue; // empty image, skip it
        
        //cout << "size: " << size << ", read_index == " << read_index << endl;

        img.indices.resize(pixel_count);

        if (version_ < 0x201) {
            // Plain data, just copy
            buf.read(img.indices.data(), pixel_count);
        }
        else
        {
            // Decode data
            unsigned int i = 0;
            uint16_t encoded = buf.readUint16();

            while (i < pixel_count && encoded > 0)
            {
                uint8_t color = buf.readUint8();
                encoded--;

                //cout << "pixel color [" << (img_index+1) << '/' << pixel_count << "]: " << int(color) << endl;

                if (color == 0)
                {
                    // color 0 is rle-encoded (invisible/background palette index)
                    uint8_t len = buf.readUint8();
                    encoded--;

                    //cout << "pixel color 0, len: " << int(len) << endl;

                    if (len == 0)
                        len = 1;

                    if ((i + len) > pixel_count)
                        throw InvalidFile("spr: too much encoded data for pal image");

                    memset(&img.indices[i], 0, len);
                    i += len;
                }
                else
                    img.indices[i++] = color;
            }
        }

        pal_images_.emplace_back(move(img));
    }

    for (int i = 0; i < rgba_img_count; i++)
    {
        RgbaImage img;
        img.width = buf.readUint16();
        img.height = buf.readUint16();
        const size_t pixel_count = img.width * img.height;

        if (!pixel_count)
            continue; // empty image, skip it
        
        for (int j = 0; j < pixel_count; j++)
        {
            Color color;
            color.r = buf.readUint8();
            color.g = buf.readUint8();
            color.b = buf.readUint8();
            color.a = buf.readUint8();
            img.pixels.emplace_back(move(color));
        }

        rgba_images_.emplace_back(move(img));
    }

    if (version_ >= 0x101)
        pal_ = make_unique<Pal>(buf);
}