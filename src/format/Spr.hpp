#ifndef ROTOOLS_FORMAT_SPR_HPP
#define ROTOOLS_FORMAT_SPR_HPP

#include <cstdint>
#include <memory>
#include <vector>
#include <string>
#include "Pal.hpp"
#include "../util/Buffer.hpp"
#include "../util/Color.hpp"

namespace format {

struct Spr {
    struct PaletteImage {
        unsigned short width;
        unsigned short height;
        std::vector<uint8_t> indices;
    };

    struct RgbaImage {
        unsigned short width;
        unsigned short height;
        std::vector<Color> pixels;
    };

    /// Constructs an empty Spr.
    explicit Spr() = default;

    /// Constructs and loads from memory buffer.
    explicit Spr(const Buffer& buf) { load(buf); }

    /**
     * Loads from memory buffer.
     *
     * @throws InvalidResource on failure.
     */
    void load(const Buffer& buf);

    /// Saves to memory buffer.
    void save(Buffer& buf) const;

    struct { uint8_t major, minor; } version;
    std::vector<PaletteImage> palette_images;
    std::vector<RgbaImage> rgba_images;
    std::unique_ptr<Pal> pal;
};

} // namespace format

#endif // ROTOOLS_FORMAT_SPR_HPP