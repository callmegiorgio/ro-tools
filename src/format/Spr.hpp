#ifndef RO_SPR_HPP
#define RO_SPR_HPP

#include <cstdint>
#include <memory>
#include <vector>
#include <string>
#include "Pal.hpp"
#include "../util/Buffer.hpp"
#include "../util/Color.hpp"

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
     * @throws InvalidFile on failure.
     */
    void load(const Buffer& buf);

    /// Saves to memory buffer.
    void save(Buffer& buf) const;

    struct { int major, minor; } version;
    std::vector<PaletteImage> palette_images;
    std::vector<RgbaImage> rgba_images;
    std::unique_ptr<Pal> pal;
};

#endif // RO_SPR_HPP