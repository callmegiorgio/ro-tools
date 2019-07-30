#include "Pal.hpp"

#include <string>
#include "../util/InvalidFile.hpp"

using namespace std;

void Pal::load(Buffer& buf)
{
    const size_t required_size = colors.size() * 4;

    if (buf.remaining() < required_size)
        throw InvalidFile("pal: invalid size " + to_string(buf.remaining()) + ", expected " + to_string(required_size));
    
    for (Color& color : colors)
    {
        color.r = buf.getUint8();
        color.g = buf.getUint8();
        color.b = buf.getUint8();
        color.a = buf.getUint8();
    }
}

void Pal::save(Buffer& buf) const
{
    for (const Color& color : colors)
        ; //buf << color.r << color.g << color.b << color.a;
}