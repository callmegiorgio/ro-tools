#ifndef RO_SPR_HPP
#define RO_SPR_HPP

#include <cstdint>
#include <memory>
#include <vector>
#include <string>
#include "Buffer.hpp"
#include "Color.hpp"
#include "Pal.hpp"

class Spr final {
public:
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
     * Construct from memory buffer.
     *
     * @throw InvalidFile on failure
     */
    explicit Spr(Buffer& buf);
    explicit Spr(Buffer&& buf) : Spr(buf) {}

    int versionMajor() const { return (version_ >> 8) & 0xFF; }
    int versionMinor() const { return version_ & 0xFF; }
    
    const std::vector<PaletteImage>& paletteImages() const { return pal_images_; }
    const std::vector<RgbaImage>& rgbaImages() const { return rgba_images_; }
    const Pal* pal() const { return pal_.get(); }

private:
    void parse(Buffer& buf);

    int version_;
    std::vector<PaletteImage> pal_images_;
    std::vector<RgbaImage> rgba_images_;
    std::unique_ptr<Pal> pal_;
};

#endif // RO_SPR_HPP