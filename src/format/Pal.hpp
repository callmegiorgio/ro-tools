#ifndef RO_PAL_HPP
#define RO_PAL_HPP

#include <array>
#include <cstdint>
#include "../util/Buffer.hpp"
#include "../util/Color.hpp"

struct Pal {
    
    /// Constructs an empty Pal.
    explicit Pal() = default;

    /// Constructs and loads from memory buffer.
    explicit Pal(const Buffer& buf) { load(buf); }

    /**
     * Loads from memory buffer.
     *
     * @throws InvalidFile on failure.
     */
    void load(const Buffer& buf);

    /// Saves to memory buffer.
    void save(Buffer& buf) const;

    std::array<Color, 256> colors;
};

#endif // RO_PAL_HPP