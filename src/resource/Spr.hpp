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

    /**
     * Constructs from memory buffer.
     *
     * @throws InvalidFile on failure.
     */
    explicit Spr(Buffer& buf) { load(buf); }
    explicit Spr(Buffer&& buf) : Spr(buf) {}

    /**
     * Loads from memory buffer.
     *
     * @throws InvalidFile on failure.
     */
    void load(Buffer& buf);

    /// Saves to memory buffer.
    void save(Buffer& buf) const;

    struct { int major, minor; } version;
    std::vector<PaletteImage> palette_images;
    std::vector<RgbaImage> rgba_images;
    std::unique_ptr<Pal> pal;
};

#endif // RO_SPR_HPP