#ifndef RO_TEXTURE_HPP
#define RO_TEXTURE_HPP

#include <glad/glad.h>

class Texture {
public:
    explicit Texture(unsigned int width, unsigned int height, int channels, const void* data)
        : Texture()
    {
        load(width, height, channels, data);
    }

    explicit Texture() { glGenTextures(1, &id_); }
    explicit Texture(const Texture&) = delete; // no copy
    ~Texture() { glDeleteTextures(1, &id_); }

    void load(unsigned int width, unsigned int height, int channels, const void* data);
    void bind() { glBindTexture(GL_TEXTURE_2D, id_); }

    void operator=(const Texture&) = delete;

private:
    GLuint id_;
};

void Texture::load(unsigned int width, unsigned int height, int channels, const void* data)
{
    GLenum format;

    switch (channels)
    {
        case 1: format = GL_RED; break;
        case 3: format = GL_RGB; break;
        case 4: format = GL_RGBA; break;
        default:
            return; // invalid channels
    }

    glBindTexture(GL_TEXTURE_2D, id_);
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

#endif // RO_TEXTURE_HPP