#ifndef RO_COLOR_HPP
#define RO_COLOR_HPP

#include <ostream>

struct Color {
    Color(uint8_t red = 0, uint8_t green = 0, uint8_t blue = 0, uint8_t alpha = 255)
        : r{ red }, g{ green }, b{ blue }, a{ alpha } {}

    Color(uint32_t rgba) : Color((rgba >> 24) & 0xFF, (rgba >> 16) & 0xFF, (rgba >> 8) & 0xFF, rgba & 0xFF) {}

    uint8_t r, g, b, a;
};

inline std::ostream& operator<<(std::ostream& out, const Color& color)
{
    return out << "rgba(" 
        << static_cast<int>(color.r) << ", "
        << static_cast<int>(color.g) << ", "
        << static_cast<int>(color.b) << ", "
        << static_cast<int>(color.a) << ')';
}

#endif // RO_COLOR_HPP