#ifndef ROTOOLS_FORMAT_ACT_HPP
#define ROTOOLS_FORMAT_ACT_HPP

#include <array>
#include <vector>
#include "../util/Buffer.hpp"
#include "../util/Color.hpp"

namespace format {

struct Act {
    struct Image {
        int x;                        // x position relative to frame's center
        int y;                        // y position relative to frame's center
        int width = 0;                // 0 for image's width in spr file
        int height = 0;               // 0 for image's height in spr file
        int rotation = 0;             // rotation angle in degrees
        float scale_x = 1.0f;         // scale of x axis
        float scale_y = 1.0f;         // scale of y axis
        bool mirror;                  // whether to mirror image along the vertical axis or not
        bool is_rgba;                 // whether the image uses rgba or palette colors
        int index;                    // index of the image in the spr file; -1 for none
        Color color{ 255, 255, 255 };
    };

    struct Anchor {
        int x;
        int y;
        int attribute;
    };

    /// A frame is a mix of images, anchors, and a sound.
    struct Frame {
        int sound_index = -1;
        std::vector<Image> images;
        std::vector<Anchor> anchors;
    };

    /// An animation is a collection of delayed frames.
    struct Animation {
        float delay = 4.f;
        std::vector<Frame> frames;
    };

    struct Sound {
        std::array<char, 40> filename; // C-string filename in relative sound path
    };

    /// Constructs an empty Act.
    explicit Act() = default;
    
    /// Construct and loads from memory buffer.
    explicit Act(const Buffer& buf) { load(buf); }

    /**
     * Loads from memory buffer.
     *
     * @throws InvalidResource on failure.
     */
    void load(const Buffer& buf);

    /// Saves to memory buffer.
    void save(Buffer& buf) const;

    struct { uint8_t major, minor; } version;
    std::vector<Animation> animations;
    std::vector<Sound> sounds;
};

} // namespace format

#endif // ROTOOLS_FORMAT_ACT_HPP