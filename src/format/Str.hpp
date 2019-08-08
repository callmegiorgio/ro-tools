#ifndef ROTOOLS_FORMAT_STR_HPP
#define ROTOOLS_FORMAT_STR_HPP

#include <array>
#include <vector>
#include "../util/Buffer.hpp"
#include "../util/Color.hpp"
#include "../util/Point2D.hpp"
#include "../util/Rect.hpp"

namespace format {

struct Str {
    /**
     * All rects below follow the following convention: 
     *   d -- c
     *   |    |
     *   a -- b
     */
    struct Frame {
        enum BlendType {
            Zero,
            One,
            SrcColor,
            InvSrcColor,
            SrcAlpha,
            InvSrcAlpha,
            DestAlpha,
            InvDestAlpha,
            DestColor,
            InvDestColor,
            SrcAlphaSat,
            BothSrcAlpha,
            BothInvSrcAlpha
        };

        uint32_t frame_number;
        bool morph; // whether it's a morph frame or a normal frame
        Point2D position;
        Rect<Point2D> uv_mapping;
        Rect<Point2D> uv_mapping2;
        Rect<Point2D> drawing_rect;
        uint32_t texture_index;
        uint32_t anitype;
        float anidelta; ///< Texture animation delta.
        float rz; //< Rotation [0,1024[ is equivalent to [0,360[ degrees
        Color color;
        BlendType src_blend_type;
        BlendType dest_blend_type;
        uint32_t mtpreset;
    };

    struct Texture {
        std::array<char, 128> filename;
    };

    struct Layer {
        std::vector<Texture> textures;
        std::vector<Frame> frames;
    };

    explicit Str() = default;
    explicit Str(const Buffer& buf) { load(buf); }

    void load(const Buffer& buf);
    void save(Buffer& buf) const { /* TODO */ }

    uint32_t version;
    uint32_t fps;
    uint32_t frame_count;
    std::vector<Layer> layers;
};

} // namespace format

#endif // ROTOOLS_FORMAT_STR_HPP