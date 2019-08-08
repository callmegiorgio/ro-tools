#include "Image.hpp"

#include <iostream>
#include <FreeImage.h>
#include "../util/InvalidResource.hpp"

using namespace std;

namespace format {

void Image::saveAsBmp(Buffer& buf, int width, int height, int channels, const void* pixels)
{
    const unsigned int bpp = channels * 8;

    switch (bpp)
    {
        // case 24: not working
        case 32:
            break; // supported
        
        default:
            throw InvalidResource(to_string(bpp) + "-bit images are not supported");
    }

    FIMEMORY* hmem = FreeImage_OpenMemory();
    FIBITMAP* dib = nullptr;
    
    try {
        dib = FreeImage_Allocate(width, height, bpp);

        if (!dib)
            throw InvalidResource("could not allocate memory to create image");

        BYTE* dest_bits = FreeImage_GetBits(dib);
        const BYTE* src_bits = reinterpret_cast<const BYTE*>(pixels);

        switch (bpp)
        {
            case 24:
                for (int i = 0; i < (width * height); i++)
                {
                    dest_bits[i*3 + FI_RGBA_RED]   = src_bits[i*3 + 0];
                    dest_bits[i*3 + FI_RGBA_GREEN] = src_bits[i*3 + 1];
                    dest_bits[i*3 + FI_RGBA_BLUE]  = src_bits[i*3 + 2];
                }
                break;

            case 32:
                for (int i = 0; i < (width * height); i++)
                {
                    dest_bits[i*4 + FI_RGBA_RED]   = src_bits[i*4 + 0];
                    dest_bits[i*4 + FI_RGBA_GREEN] = src_bits[i*4 + 1];
                    dest_bits[i*4 + FI_RGBA_BLUE]  = src_bits[i*4 + 2];
                    dest_bits[i*4 + FI_RGBA_ALPHA] = src_bits[i*4 + 3];
                }
                break;
        }

        FreeImage_FlipVertical(dib);
        FreeImage_SaveToMemory(FIF_BMP, dib, hmem, 0);
        
        FreeImage_SeekMemory(hmem, 0, SEEK_END);
        const size_t buf_size = FreeImage_TellMemory(hmem);
        buf.grow(buf_size);
        
        FreeImage_SeekMemory(hmem, 0, SEEK_SET);
        FreeImage_ReadMemory(buf.data(), buf_size, 1, hmem);
    }
    catch (const exception& e)
    {
        if (dib)
            FreeImage_Unload(dib);

        FreeImage_CloseMemory(hmem);
        throw;
    }

    if (dib)
        FreeImage_Unload(dib);

    FreeImage_CloseMemory(hmem);
}

void Image::load(const Buffer& buf)
{
    const BYTE* cdata = reinterpret_cast<const BYTE*>(buf.data());
    BYTE* data = const_cast<BYTE*>(cdata);
    DWORD size = static_cast<DWORD>(buf.size());

    FIMEMORY* hmem = FreeImage_OpenMemory(data, size);
    FIBITMAP* dib = nullptr;
    FIBITMAP* dib32 = nullptr;

    try {
        FREE_IMAGE_FORMAT fif = FreeImage_GetFileTypeFromMemory(hmem, 0);
        dib = FreeImage_LoadFromMemory(fif, hmem, 0);

        if (!dib || !FreeImage_HasPixels(dib))
            throw InvalidResource("image: could not load image");

        unsigned int bpp = FreeImage_GetBPP(dib);
        
        // 48- and 64-bit unsupported for now
        if (bpp > 32)
            throw InvalidResource("image: " + to_string(bpp) + "-bit images are not supported");

        // Try converting image to 32-bit
        dib32 = FreeImage_ConvertTo32Bits(dib);

        if (!dib32 || !FreeImage_HasPixels(dib32))
            throw InvalidResource("image: could not convert image to 32 bits");

        width = FreeImage_GetWidth(dib32);
        height = FreeImage_GetHeight(dib32);
        channels = 4; // FreeImage_GetBPP(dib32) / 8

        FreeImage_FlipVertical(dib32);

        BYTE* bits = FreeImage_GetBits(dib32);
        pixels.reset(new uint8_t[width * height * channels]);

        // Convert platform-dependent bits to RGBA
        for (int i = 0; i < (width * height); i++)
        {
            pixels[i*4 + 0] = bits[i*4 + FI_RGBA_RED];
            pixels[i*4 + 1] = bits[i*4 + FI_RGBA_GREEN];
            pixels[i*4 + 2] = bits[i*4 + FI_RGBA_BLUE];
            pixels[i*4 + 3] = bits[i*4 + FI_RGBA_ALPHA];
        }
    }
    catch (const exception& e)
    {
        if (dib)
            FreeImage_Unload(dib);
        
        if (dib32)
            FreeImage_Unload(dib32);
        
        FreeImage_CloseMemory(hmem);
        throw;
    }

    if (dib)
        FreeImage_Unload(dib);
    
    if (dib32)
        FreeImage_Unload(dib32);

    FreeImage_CloseMemory(hmem);
}

} // namespace format