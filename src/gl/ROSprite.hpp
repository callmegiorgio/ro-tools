#ifndef ROTOOLS_GL_ROSPRITE_HPP
#define ROTOOLS_GL_ROSPRITE_HPP

#include "Sprite.hpp"
#include "../format/Act.hpp"
#include "../format/Pal.hpp"
#include "../format/Spr.hpp"

using format::Act;
using format::Spr;
using format::Pal;

namespace gl {

class ROSprite final : public Sprite {
public:
    explicit ROSprite(const Act& act, const Spr& spr, const Pal& pal)
        : act_{ act }
        , spr_{ spr }
        , pal_{ pal } {}

    void load() override;

    void update(double dt) override;
    void draw() const override { draw(0, 0); }
    void draw(const Sprite& anchor_sprite) const override;

    void advanceAnimation() override;
    void recedeAnimation() override;
    void advanceFrame() override;
    void recedeFrame() override;

    const Act::Animation& currentAnimation() const { return act_.animations[anim_idx_]; }
    const Act::Frame& currentFrame() const { return currentAnimation().frames[frame_idx_]; }

private:
    void draw(int offset_x, int offset_y) const;

    const Act& act_;
    const Spr& spr_;
    const Pal& pal_;
};

} // namespace gl

#endif // ROTOOLS_GL_ROSPRITE_HPP