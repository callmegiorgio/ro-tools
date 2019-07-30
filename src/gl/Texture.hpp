#ifndef RO_TEXTURE_HPP
#define RO_TEXTURE_HPP

#include <utility>
#include <glad/glad.h>

class Texture final {
public:
    enum Format {
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
    explicit Texture(Texture&& other) : id_{ std::exchange(other.id_, 0) } {}

    /// no copy contructor
    explicit Texture(const Texture&) = delete;

    /// Destructor.
    ~Texture()
    {
        if (id_) glDeleteTextures(1, &id_);
    }
    
    /// Loads image data into the texture buffer.
    void load(unsigned int width, unsigned int height, Format format, const void* data) const;

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

    // Move assignment.
    Texture& operator=(Texture&& other)
    {
        id_ = std::exchange(other.id_, 0);
        return *this;
    }

    /// No copy assignment.
    void operator=(const Texture&) = delete; 

private:
    GLuint id_ = 0;
};

#endif // RO_TEXTURE_HPP