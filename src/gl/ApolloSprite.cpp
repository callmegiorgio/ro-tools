#include "ApolloSprite.hpp"

#include <cmath>
#include <glad/glad.h>

using namespace std;

namespace gl {

void ApolloSprite::update(double dt)
{
    elapsed_time_ += dt;

    if (elapsed_time_ < 0.1)
        return;

    advanceFrame();
    elapsed_time_ = 0;
}

void ApolloSprite::draw(const Sprite& anchor_sprite) const
{
    if (const ApolloSprite* other = dynamic_cast<const ApolloSprite*>(&anchor_sprite))
    {
        int this_anchor_x = currentFrame().anchor_x;
        int this_anchor_y = currentFrame().anchor_y;
        int other_anchor_x = other->currentFrame().anchor_x;
        int other_anchor_y = other->currentFrame().anchor_y;

        draw(other_anchor_x - this_anchor_x, other_anchor_y - this_anchor_y);
    }
}

void ApolloSprite::draw(int offset_x, int offset_y) const
{
    for (const format::Sprite::Layer& layer : currentFrame().layers)
    {
        auto image_index = layer.image_index;

        if (image_index >= sprite_.images.size())
            continue;

        const Color& c = layer.color;
        const int w = sprite_.images[image_index].width;
        const int h = sprite_.images[image_index].height;
        const int x = layer.x - static_cast<int>(round(w / 2.0)) + offset_x;
        const int y = layer.y - static_cast<int>(round(h / 2.0)) + offset_y;

        textures_[image_index].bind();

        glBegin(GL_QUADS);
        glColor4f(c.r / 255, c.g / 255, c.b / 255, c.a / 255);

        // Flip texture vertically and maybe horizontally
        glTexCoord2d(layer.mirror ? 1 : 0, 0); glVertex3d(x,   y,   0); // top left
        glTexCoord2d(layer.mirror ? 1 : 0, 1); glVertex3d(x,   y+h, 0); // bottom left
        glTexCoord2d(layer.mirror ? 0 : 1, 1); glVertex3d(x+w, y+h, 0); // bottom right
        glTexCoord2d(layer.mirror ? 0 : 1, 0); glVertex3d(x+w, y,   0); // top right
        
        glEnd();
    }

    Texture::unbind();
}

void ApolloSprite::advanceAnimation()
{
    if (++anim_idx_ >= sprite_.animations.size())
        anim_idx_ = 0;

    frame_idx_ = 0;
}

void ApolloSprite::recedeAnimation()
{
    if (--anim_idx_ < 0)
        anim_idx_ = sprite_.animations.size() - 1;

    frame_idx_ = 0;
}

void ApolloSprite::advanceFrame()
{
    if (++frame_idx_ >= currentAnimation().frames.size())
        frame_idx_ = 0;
}

void ApolloSprite::recedeFrame()
{
    if (--frame_idx_ < 0)
        frame_idx_ = currentAnimation().frames.size() - 1;
}

void ApolloSprite::load()
{
    // Create textures for each palette image
    for (const format::Sprite::Image& img : sprite_.images)
    {
        // First palette color is the transparency color.
        const Color& transp_color = palette_.colors[0];
        vector<uint8_t> pixels(img.indices.size() * 4);

        for (int i = 0; i < img.indices.size(); i++)
        {
            uint8_t index = img.indices[i];
            const Color& color = palette_.colors[index];

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