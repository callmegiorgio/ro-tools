#include "Pal.hpp"

#include <string>
#include "InvalidFile.hpp"

using namespace std;

Pal::Pal(Buffer& buf)
{
    if (buf.remaining() < (colors_.size() * 4))
        throw InvalidFile("pal: invalid size " + to_string(buf.remaining()) + ", expected " + to_string(colors_.size() * 4));
    
    for (Color& color : colors_)
    {
        color.r = buf.getUint8();
        color.g = buf.getUint8();
        color.b = buf.getUint8();
        color.a = buf.getUint8();
    }
}