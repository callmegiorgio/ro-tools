#ifndef RO_PAL_HPP
#define RO_PAL_HPP

#include <array>
#include <cstdint>
#include "../util/Buffer.hpp"
#include "../util/Color.hpp"

struct Pal {
    /**
     * Constructs from memory buffer.
     *
     * @throws InvalidFile on failure.
     */
    explicit Pal(Buffer& buf) { load(buf); }
    explicit Pal(Buffer&& buf) : Pal(buf) {}

    /**
     * Loads from memory buffer.
     *
     * @throws InvalidFile on failure.
     */
    void load(Buffer& buf);

    /// Saves to memory buffer.
    void save(Buffer& buf) const;

    std::array<Color, 256> colors;
};

#endif // RO_PAL_HPP