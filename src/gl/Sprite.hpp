#ifndef RO_SPRITE_HPP
#define RO_SPRITE_HPP

#include <vector>
#include "Texture.hpp"
#include "../format/Act.hpp"
#include "../format/Spr.hpp"

class Sprite final {
public:
    explicit Sprite(const Act& act, const Spr& spr)
        : act_{ act }
        , spr_{ spr } {}

    bool load();

    void update(double dt);
    void draw() const { draw(0, 0); }
    void draw(const Sprite& anchor_sprite) const;

    void advanceAnimation();
    void recedeAnimation();
    void advanceFrame();
    void recedeFrame();

    int currentAnimationIndex() const { return anim_idx_; }
    int currentFrameIndex() const { return frame_idx_; }
    
    const Act::Animation& currentAnimation() const { return act_.animations[anim_idx_]; }
    const Act::Frame& currentFrame() const { return currentAnimation().frames[frame_idx_]; }

    void setPal(const Pal& pal);
    void setMagFilter(Texture::ResizeFilter filter);
    void setMinFilter(Texture::ResizeFilter filter);

    Texture::ResizeFilter magFilter() const { return mag_filter_; }
    Texture::ResizeFilter minFilter() const { return min_filter_; }

private:
    void draw(int offset_x, int offset_y) const;

    const Act& act_;
    const Spr& spr_;
    const Pal* pal_ = nullptr;
    std::vector<Texture> textures_;
    Texture::ResizeFilter mag_filter_ = Texture::Linear;
    Texture::ResizeFilter min_filter_ = Texture::LinearMipmapLinear;

    int anim_idx_ = 0;
    int frame_idx_ = 0;
    double elapsed_time_ = 0;
};

#endif // RO_SPRITE_HPP