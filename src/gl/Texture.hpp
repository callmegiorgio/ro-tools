#ifndef ROTOOLS_GL_TEXTURE_HPP
#define ROTOOLS_GL_TEXTURE_HPP

#include <utility>
#include <glad/glad.h>

namespace gl {

class Texture final {
public:
    enum Format {
        Gray,
        Red,
        Rgb,
        Rgba
    };

    enum ResizeFilter {
        Nearest,
        Linear,
        NearestMipmapNearest,
        NearestMipmapLinear,
        LinearMipmapLinear,
        LinearMipmapNearest
    };
    
    /// Unbind any texture.
    static void unbind() { glBindTexture(GL_TEXTURE_2D, 0); }

    /// Constructs an empty texture.
    explicit Texture() { glGenTextures(1, &id_); }

    /// Contructs and loads.
    explicit Texture(unsigned int width, unsigned int height, Format format, const void* data)
        : Texture()
    {
        load(width, height, format, data);
    }

    /// Move constructor.
    explicit Texture(Texture&& other)
        : id_{ std::exchange(other.id_, 0) }
        , width_{ std::exchange(other.width_, 0) }
        , height_{ std::exchange(other.height_, 0) } {}

    /// no copy contructor
    explicit Texture(const Texture&) = delete;

    /// Destructor.
    ~Texture()
    {
        if (id_) glDeleteTextures(1, &id_);
    }
    
    /// Loads image data into the texture buffer.
    void load(unsigned int width, unsigned int height, Format format, const void* data);

    /// Sets texture minifying filter.
    void setMinFilter(ResizeFilter filter) const;

    /// Sets texture magnification filter.
    void setMagFilter(ResizeFilter filter) const;

    /// Sets textures magnification and minifying filter.
    void setResizeFilters(ResizeFilter mag_filter, ResizeFilter min_filter) const;

    /// Bind texture.
    void bind() const { glBindTexture(GL_TEXTURE_2D, id_); }

    /// Texture id.
    GLuint id() const { return id_; }

    int width() const { return width_; }
    int height() const { return height_; }

    // Move assignment.
    Texture& operator=(Texture&& other)
    {
        id_ = std::exchange(other.id_, 0);
        width_ = std::exchange(other.width_, 0);
        height_ = std::exchange(other.height_, 0);
        return *this;
    }

    /// No copy assignment.
    void operator=(const Texture&) = delete; 

private:
    GLuint id_ = 0;
    int width_ = 0;
    int height_ = 0;
};

} // namespace gl

#endif // ROTOOLS_GL_TEXTURE_HPP