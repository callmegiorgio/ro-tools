#include "Effect.hpp"

#include <iostream>
#include <cstring>
#include <string>
#include <glad/glad.h>
#include "../format/Image.hpp"
#include "../util/filehandler.hpp"

using namespace std;
using format::Image;

namespace gl {

static GLenum glValueFromBlendType(Str::Frame::BlendType type)
{
    switch (type)
    {
        case Str::Frame::Zero: return GL_ZERO;
        case Str::Frame::One: return GL_ONE;
        case Str::Frame::SrcColor: return GL_SRC_COLOR;
        case Str::Frame::InvSrcColor: return GL_ONE_MINUS_SRC_COLOR;
        case Str::Frame::SrcAlpha: return GL_SRC_ALPHA;
        case Str::Frame::InvSrcAlpha: return GL_ONE_MINUS_SRC_ALPHA;
        case Str::Frame::DestAlpha: return GL_DST_ALPHA;
        case Str::Frame::InvDestAlpha: return GL_ONE_MINUS_DST_ALPHA;
        case Str::Frame::DestColor: return GL_DST_COLOR;
        case Str::Frame::InvDestColor: return GL_ONE_MINUS_DST_COLOR;
        case Str::Frame::SrcAlphaSat: return GL_SRC_ALPHA_SATURATE;
        case Str::Frame::BothSrcAlpha: return GL_SRC_ALPHA;
        case Str::Frame::BothInvSrcAlpha: return GL_ZERO;
    }

    return GL_ZERO;
}

void Effect::load(const Str& str, const char* texture_path)
{
    str_ = &str;
    string texture_path_str(texture_path);

    // Append / if not found.
    if (!strchr("/\\", texture_path[strlen(texture_path) - 1]))
        texture_path_str += '/';

    for (int layer_index = 0; layer_index < str.layers.size(); layer_index++)
    {
        const Str::Layer& layer = str.layers[layer_index];

        for (const Str::Texture& texture : layer.textures)
        {
            string filepath = texture_path_str + texture.filename.data();
            
            // Create or retrieve texture from cache
            Texture& tex = texture_cache_[filepath];

            // If texture hasn't been previously loaded, it's a new texture, so load it
            if (!tex.width())
            {
                Image image(readFile(filepath.c_str()));
                tex.load(image.width, image.height, image.channels == 3 ? Texture::Rgb : Texture::Rgba, image.pixels.get());
            }

            // Add a ref so the layer can use it
            layer_textures_[layer_index].emplace_back(tex);
        }
    }

    current_layer_frames_.resize(str.layers.size());
    updateCurrentFrames();
}

void Effect::update(double dt)
{
    const double req_frame_ms = 1.0 / str_->fps;

    elapsed_time_ += dt;

    if (elapsed_time_ < req_frame_ms)
        return;

    advanceFrame();
    elapsed_time_ = 0;
}

void Effect::draw() const
{
    for (int i = 0; i < str_->layers.size(); i++)
        drawLayer(i);
}

void Effect::advanceFrame()
{
    if (++current_frame_ >= str_->frame_count)
        current_frame_ = 0;

    updateCurrentFrames();
}

void Effect::recedeFrame()
{
    if (--current_frame_ < 0)
        current_frame_ = str_->frame_count - 1;

    updateCurrentFrames();
}

vector<int> Effect::activeLayers() const
{
    vector<int> active_layers;

    for (int layer_idx = 0; layer_idx < str_->layers.size(); layer_idx++)
    {
        if (current_layer_frames_[layer_idx].base || current_layer_frames_[layer_idx].animation)
            active_layers.push_back(layer_idx);
    }

    return move(active_layers);
}

void Effect::updateCurrentFrames()
{
    // Update each layer
    for (int layer_idx = 0; layer_idx < str_->layers.size(); layer_idx++)
    {
        const Str::Layer& layer = str_->layers[layer_idx];

        if (layer.frames.empty())
            continue;

        bool found = false;

        // Each layer's frame
        for (const Str::Frame& frame : layer.frames)
        {
            if (frame.frame_number > current_frame_)
                break;

            if (frame.morph) {
                current_layer_frames_[layer_idx].animation = &frame;
            }
            else {
                current_layer_frames_[layer_idx].base = &frame;
                current_layer_frames_[layer_idx].animation = nullptr;
            }

            found = frame.frame_number == current_frame_;
        }

        if (!found && !current_layer_frames_[layer_idx].animation)
            current_layer_frames_[layer_idx].base = nullptr;
    }
}

void Effect::drawLayer(int layer_index) const
{
    // Draw layer only if there's a frame to draw
    if (!current_layer_frames_[layer_index].base)
        return;

    const Str::Frame& base_frame = *current_layer_frames_[layer_index].base;

    // Draw frame only if alpha is more than zero
    if (base_frame.color.a == 0)
        return;

    auto textures_iter = layer_textures_.find(layer_index);
    if (textures_iter == layer_textures_.end())
        return;

    auto& textures = textures_iter->second;

    // Ensure base frame's texture index is valid
    if (base_frame.texture_index < 0 || base_frame.texture_index >= textures.size())
        return;
    
    constexpr float str_angle_to_degrees = 2.8444f;

    Color color = base_frame.color;
    Point2D position = base_frame.position;
    Rect<Point2D> drawing_rect = base_frame.drawing_rect;
    Rect<Point2D> uv_mapping = base_frame.uv_mapping;
    float rotation = base_frame.rz / str_angle_to_degrees;

    // Translate by character size
    //position.x += 320;
    //position.y += 290;

    // Apply a few modifications if there's an animation frame along with base frame
    if (const Str::Frame* anim_frame = current_layer_frames_[layer_index].animation)
    {
        const int ani_factor = current_frame_ - anim_frame->frame_number;

        // Color
        color.r += anim_frame->color.r * ani_factor;
        color.g += anim_frame->color.g * ani_factor;
        color.b += anim_frame->color.b * ani_factor;
        color.a += anim_frame->color.a * ani_factor;

        // Position
        position.x += anim_frame->position.x * ani_factor;
        position.y += anim_frame->position.y * ani_factor;

        // Drawing rect
        drawing_rect.a.x += anim_frame->drawing_rect.a.x * ani_factor;
        drawing_rect.a.y += anim_frame->drawing_rect.a.y * ani_factor;
        drawing_rect.b.x += anim_frame->drawing_rect.b.x * ani_factor;
        drawing_rect.b.y += anim_frame->drawing_rect.b.y * ani_factor;
        drawing_rect.c.x += anim_frame->drawing_rect.c.x * ani_factor;
        drawing_rect.c.y += anim_frame->drawing_rect.c.y * ani_factor;
        drawing_rect.d.x += anim_frame->drawing_rect.d.x * ani_factor;
        drawing_rect.d.y += anim_frame->drawing_rect.d.y * ani_factor;

        // Texture mapping
        uv_mapping.a.x += anim_frame->uv_mapping.a.x * ani_factor;
        uv_mapping.a.y += anim_frame->uv_mapping.a.y * ani_factor;
        uv_mapping.b.x += anim_frame->uv_mapping.b.x * ani_factor;
        uv_mapping.b.y += anim_frame->uv_mapping.b.y * ani_factor;
        uv_mapping.c.x += anim_frame->uv_mapping.c.x * ani_factor;
        uv_mapping.c.y += anim_frame->uv_mapping.c.y * ani_factor;
        uv_mapping.d.x += anim_frame->uv_mapping.d.x * ani_factor;
        uv_mapping.d.y += anim_frame->uv_mapping.d.y * ani_factor;

        // Rotation
        rotation += (anim_frame->rz / str_angle_to_degrees) * ani_factor;
    }

    const Texture& texture = textures[base_frame.texture_index].get();

    glPushMatrix();

    GLfloat current_color[4];
    glGetFloatv(GL_CURRENT_COLOR, current_color);
    
    glColor4d(1, 1, 1, 1);
    //glColor4ub(color.r, color.g, color.b, color.a);
    glTranslatef(position.x, position.y, 0.f);
    glRotatef(rotation, 0.f, 0.f, 1.f);
    
    glBlendFunc(
        glValueFromBlendType(base_frame.src_blend_type),
        glValueFromBlendType(base_frame.dest_blend_type)
    );
    
    glEnable(GL_BLEND);
    texture.bind();
    glColorMask(true, true, true, false);

    glBegin(GL_QUADS);
    glTexCoord2f(uv_mapping.a.x, uv_mapping.a.y); glVertex3f(drawing_rect.a.x, drawing_rect.a.y, 0.f); // bottom left
    glTexCoord2f(uv_mapping.b.x, uv_mapping.b.y); glVertex3f(drawing_rect.b.x, drawing_rect.b.y, 0.f); // bottom right
    glTexCoord2f(uv_mapping.c.x, uv_mapping.c.y); glVertex3f(drawing_rect.c.x, drawing_rect.c.y, 0.f); // top right
    glTexCoord2f(uv_mapping.d.x, uv_mapping.d.y); glVertex3f(drawing_rect.d.x, drawing_rect.d.y, 0.f); // top left
    glEnd();

    Texture::unbind();

    glColorMask(true, true, true, true);
    glDisable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    if (show_border_)
    {
        glColor4d(1, 1, 1, 1);

        glBegin(GL_LINE_LOOP);
        glVertex3f(drawing_rect.a.x, drawing_rect.a.y, 0.f); // bottom left
        glVertex3f(drawing_rect.b.x, drawing_rect.b.y, 0.f); // bottom right
        glVertex3f(drawing_rect.c.x, drawing_rect.c.y, 0.f); // top right
        glVertex3f(drawing_rect.d.x, drawing_rect.d.y, 0.f); // top left
        glEnd();
    }

    glColor4f(
        current_color[0],
        current_color[1],
        current_color[2],
        current_color[3]
    );

    glPopMatrix();
}

} // namespace gl