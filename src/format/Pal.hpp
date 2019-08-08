#ifndef ROTOOLS_FORMAT_PAL_HPP
#define ROTOOLS_FORMAT_PAL_HPP

#include <array>
#include <cstdint>
#include "../util/Buffer.hpp"
#include "../util/Color.hpp"

namespace format {

struct Pal {
    
    /// Constructs an empty Pal.
    explicit Pal() = default;

    /// Constructs and loads from memory buffer.
    explicit Pal(const Buffer& buf) { load(buf); }

    /**
     * Loads from memory buffer.
     *
     * @throws InvalidResource on failure.
     */
    void load(const Buffer& buf);

    /// Saves to memory buffer.
    void save(Buffer& buf) const;

    std::array<Color, 256> colors;
};

} // namespace format

#endif // ROTOOLS_FORMAT_PAL_HPP