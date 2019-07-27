#ifndef RO_PAL_HPP
#define RO_PAL_HPP

#include <array>
#include <cstdint>
#include "Buffer.hpp"
#include "Color.hpp"

class Pal final {
public:
    /**
     * Construct from memory buffer.
     *
     * @throw InvalidFile on failure
     */
    explicit Pal(Buffer& buf);
    explicit Pal(Buffer&& buf) : Pal(buf) {}

    const std::array<Color, 256>& colors() const { return colors_; }

    Color& at(size_t index) { return colors_.at(index); }
    const Color& at(size_t index) const { return colors_.at(index); }

    Color& operator[](size_t index) { return at(index); }

private:
    std::array<Color, 256> colors_;
};

#endif // RO_PAL_HPP