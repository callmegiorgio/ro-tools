#include "ROSprite.hpp"

#include <cmath>
#include <glad/glad.h>

using namespace std;

namespace gl {

void ROSprite::update(double dt)
{
    elapsed_time_ += dt;

    if (elapsed_time_ < 0.1)
        return;

    advanceFrame();
    elapsed_time_ = 0;
}

void ROSprite::draw(const Sprite& anchor_sprite) const
{
    if (const ROSprite* other = dynamic_cast<const ROSprite*>(&anchor_sprite))
    {
        // If there's not anchor for any of the sprites, do a simple draw
        if (currentFrame().anchors.empty() || other->currentFrame().anchors.empty()) {
            draw();
            return;
        }

        // Use the first anchor of both
        const Act::Anchor& this_anchor = currentFrame().anchors[0];
        const Act::Anchor& other_anchor = other->currentFrame().anchors[0];

        draw(other_anchor.x - this_anchor.x, other_anchor.y - this_anchor.y);
    }
}

void ROSprite::draw(int offset_x, int offset_y) const
{
    for (const Act::Image& image : currentFrame().images)
    {
        if (image.index < 0 || image.index >= spr_.palette_images.size())
            continue;

        const Color& c = image.color;
        const int w = spr_.palette_images[image.index].width;
        const int h = spr_.palette_images[image.index].height;
        const int x = image.x - static_cast<int>(round(w / 2.0)) + offset_x;
        const int y = image.y - static_cast<int>(round(h / 2.0)) + offset_y;

        textures_[image.index].bind();

        glBegin(GL_QUADS);
        glColor4f(c.r / 255, c.g / 255, c.b / 255, c.a / 255);

        // Flip texture vertically and maybe horizontally
        glTexCoord2d(image.mirror ? 1 : 0, 0); glVertex3d(x,   y,   0); // top left
        glTexCoord2d(image.mirror ? 1 : 0, 1); glVertex3d(x,   y+h, 0); // bottom left
        glTexCoord2d(image.mirror ? 0 : 1, 1); glVertex3d(x+w, y+h, 0); // bottom right
        glTexCoord2d(image.mirror ? 0 : 1, 0); glVertex3d(x+w, y,   0); // top right
        
        glEnd();
    }

    Texture::unbind();
}

void ROSprite::advanceAnimation()
{
    if (++anim_idx_ >= act_.animations.size())
        anim_idx_ = 0;

    frame_idx_ = 0;
}

void ROSprite::recedeAnimation()
{
    if (--anim_idx_ < 0)
        anim_idx_ = act_.animations.size() - 1;

    frame_idx_ = 0;
}

void ROSprite::advanceFrame()
{
    if (++frame_idx_ >= currentAnimation().frames.size())
        frame_idx_ = 0;
}

void ROSprite::recedeFrame()
{
    if (--frame_idx_ < 0)
        frame_idx_ = currentAnimation().frames.size() - 1;
}

void ROSprite::load()
{
    // Create textures for each palette image
    for (const Spr::PaletteImage& img : spr_.palette_images)
    {
        // First palette color is the transparency color.
        const Color& transp_color = pal_.colors[0];
        vector<uint8_t> pixels(img.indices.size() * 4);

        for (int i = 0; i < img.indices.size(); i++)
        {
            uint8_t index = img.indices[i];
            const Color& color = pal_.colors[index];

            pixels[i*4 + 0] = color.r;
            pixels[i*4 + 1] = color.g;
            pixels[i*4 + 2] = color.b;
            pixels[i*4 + 3] =
                color.r == transp_color.r &&
                color.g == transp_color.g &&
                color.b == transp_color.b ?
                0 : 255;
        }

        Texture texture(img.width, img.height, Texture::Rgba, pixels.data());
        texture.setMagFilter(mag_filter_);

        textures_.emplace_back(std::move(texture));
    }
}

} // namespace gl