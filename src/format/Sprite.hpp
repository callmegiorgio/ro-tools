#ifndef ROTOOLS_FORMAT_SPRITE_HPP
#define ROTOOLS_FORMAT_SPRITE_HPP

#include <array>
#include <vector>
#include "Pal.hpp"
#include "../util/Buffer.hpp"
#include "../util/Color.hpp"

namespace format {

struct Sprite {
    struct Image {
        uint16_t width;
        uint16_t height;
        std::vector<uint8_t> indices;
    };

    struct Layer {
        uint8_t image_index;
        int16_t x;
        int16_t y;
        uint16_t rotation;
        // scale_x
        // scale_y
        Color color;
        bool mirror;
    };

    struct Frame {
        std::vector<Layer> layers;
        int16_t anchor_x;
        int16_t anchor_y;
        int8_t sound_index;
    };

    struct Animation {
        uint16_t delay;
        std::vector<Frame> frames;
    };

    struct Sound {
        std::array<char, 40> filename;
    };

    explicit Sprite() = default;
    explicit Sprite(const Buffer& buf) { load(buf); }

    void load(const Buffer& buf);
    void save(Buffer& buf) const;

    Pal pal;
    std::vector<Image> images;
    std::vector<Sound> sounds;
    std::vector<Animation> animations;
};

} // namespace format

#endif // ROTOOLS_FORMAT_SPRITE_HPP