#ifndef ROTOOLS_GL_EFFECT_HPP
#define ROTOOLS_GL_EFFECT_HPP

#include <functional>
#include <string>
#include <unordered_map>
#include <vector>
#include "Texture.hpp"
#include "../format/Str.hpp"

using format::Str;

namespace gl {

class Effect final {
public:
    /**
     * Loads from str and loads textures.
     *
     * @throws FileNotOpen if it fails to open a str texture.
     */
    void load(const Str& str, const char* texture_path);

    void update(double dt);
    void draw() const;

    void advanceFrame();
    void recedeFrame();

    int currentFrame() const { return current_frame_ + 1; }
    size_t frameCount() const { return str_->frame_count; }

    void showBorder(bool show = true) { show_border_ = show; }
    bool showingBorder() const { return show_border_; }

    std::vector<int> activeLayers() const;

    /*void setMagFilter(Texture::ResizeFilter filter)
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
    }*/

    Texture::ResizeFilter magFilter() const { return mag_filter_; }
    Texture::ResizeFilter minFilter() const { return min_filter_; }

private:
    struct LayerFrames {
        const Str::Frame* base = nullptr;
        const Str::Frame* animation = nullptr;
    };

    void updateCurrentFrames();
    void drawLayer(int layer_index) const;

    std::unordered_map<std::string, Texture> texture_cache_;
    std::unordered_map<int, std::vector<std::reference_wrapper<Texture>>> layer_textures_;
    Texture::ResizeFilter mag_filter_ = Texture::Linear;
    Texture::ResizeFilter min_filter_ = Texture::LinearMipmapLinear;

    const Str* str_ = nullptr;

    std::vector<LayerFrames> current_layer_frames_;
    int current_frame_ = 0;
    double elapsed_time_ = 0;

    bool show_border_ = true;
};

} // namespace gl

#endif // ROTOOLS_GL_EFFECT_HPP