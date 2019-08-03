#ifndef ROTOOLS_GL_SPRITE_HPP
#define ROTOOLS_GL_SPRITE_HPP

#include <vector>
#include "Texture.hpp"

namespace gl {

class Sprite {
public:
    explicit Sprite() = default;
    explicit Sprite(Sprite&&) = default;
    virtual ~Sprite() = default;

    virtual void load() = 0;

    virtual void update(double dt) = 0;
    virtual void draw() const = 0;
    virtual void draw(const Sprite& anchor_sprite) const = 0;

    virtual void advanceAnimation() = 0;
    virtual void recedeAnimation() = 0;
    virtual void advanceFrame() = 0;
    virtual void recedeFrame() = 0;

    int currentAnimationIndex() const { return anim_idx_; }
    int currentFrameIndex() const { return frame_idx_; }
    
    void setMagFilter(Texture::ResizeFilter filter)
    {
        if (filter == mag_filter_)
            return;

        mag_filter_ = filter;

        for (Texture& texture : textures_)
            texture.setMagFilter(filter);
    }

    void setMinFilter(Texture::ResizeFilter filter)
    {
        if (filter == min_filter_)
            return;

        min_filter_ = filter;

        for (Texture& texture : textures_)
            texture.setMinFilter(filter);
    }

    Texture::ResizeFilter magFilter() const { return mag_filter_; }
    Texture::ResizeFilter minFilter() const { return min_filter_; }

protected:
    std::vector<Texture> textures_;
    Texture::ResizeFilter mag_filter_ = Texture::Linear;
    Texture::ResizeFilter min_filter_ = Texture::LinearMipmapLinear;

    int anim_idx_ = 0;
    int frame_idx_ = 0;
    double elapsed_time_ = 0;
};

} // namespace gl

#endif // ROTOOLS_GL_SPRITE_HPP