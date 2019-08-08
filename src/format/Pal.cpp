#include "Pal.hpp"

#include <string>
#include "../util/InvalidResource.hpp"

using namespace std;

namespace format {

void Pal::load(const Buffer& buf)
{
    const size_t required_size = colors.size() * 4;

    if (buf.remaining() < required_size)
        throw InvalidResource("pal: invalid size " + to_string(buf.remaining()) + ", expected " + to_string(required_size));
    
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
    buf.grow(colors.size() * 4);

    for (const Color& color : colors)
    {
        buf.setUint8(color.r);
        buf.setUint8(color.g);
        buf.setUint8(color.b);
        buf.setUint8(color.a);
    }
}

} // namespace format