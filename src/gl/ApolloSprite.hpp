#ifndef ROTOOLS_GL_APOLLOSPRITE_HPP
#define ROTOOLS_GL_APOLLOSPRITE_HPP

#include "Sprite.hpp"
#include "../format/Palette.hpp"
#include "../format/Sprite.hpp"

namespace gl {

class ApolloSprite final : public gl::Sprite {
public:
    explicit ApolloSprite(const format::Sprite& sprite, const format::Palette& palette)
        : sprite_{ sprite }
        , palette_{ palette } {}

    void load() override;

    void update(double dt) override;
    void draw() const override { draw(0, 0); }
    void draw(const Sprite& anchor_sprite) const override;

    void advanceAnimation() override;
    void recedeAnimation() override;
    void advanceFrame() override;
    void recedeFrame() override;

    const format::Sprite::Animation& currentAnimation() const { return sprite_.animations[anim_idx_]; }
    const format::Sprite::Frame& currentFrame() const { return currentAnimation().frames[frame_idx_]; }

private:
    void draw(int offset_x, int offset_y) const;

    const format::Sprite& sprite_;
    const format::Palette& palette_;

};

} // namespace gl

#endif // ROTOOLS_GL_APOLLOSPRITE_HPP