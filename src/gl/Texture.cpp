#include "Texture.hpp"

static inline bool resizeFilterGlParam(Texture::ResizeFilter filter, GLint& param)
{
    switch (filter)
    {
        case Texture::Nearest:              param = GL_NEAREST; break;
        case Texture::Linear:               param = GL_LINEAR; break;
        case Texture::NearestMipmapNearest: param = GL_NEAREST_MIPMAP_NEAREST; break;
        case Texture::NearestMipmapLinear:  param = GL_NEAREST_MIPMAP_LINEAR; break;
        case Texture::LinearMipmapLinear:   param = GL_LINEAR_MIPMAP_LINEAR; break;
        case Texture::LinearMipmapNearest:  param = GL_LINEAR_MIPMAP_NEAREST; break;
        default: return false;
    }

    return true;
}

void Texture::load(unsigned int width, unsigned int height, Format format, const void* data) const
{
    GLenum gl_format;

    switch (format)
    {
        case Format::Red: gl_format = GL_RED; break;
        case Format::Rgb: gl_format = GL_RGB; break;
        case Format::Rgba: gl_format = GL_RGBA; break;
        default:
            return; // invalid format
    }

    bind();
    glTexImage2D(GL_TEXTURE_2D, 0, gl_format, width, height, 0, gl_format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void Texture::setMinFilter(ResizeFilter filter) const
{
    GLint param;

    if (resizeFilterGlParam(filter, param))
    {
        bind();
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, param);
    }
}

void Texture::setMagFilter(ResizeFilter filter) const
{
    GLint param;

    if (resizeFilterGlParam(filter, param))
    {
        bind();
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, param);
    }
}

void Texture::setResizeFilters(ResizeFilter mag_filter, ResizeFilter min_filter) const
{
    GLint mag_param, min_param;

    if (resizeFilterGlParam(mag_filter, mag_param) && resizeFilterGlParam(min_filter, min_param))
    {
        bind();
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min_param);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mag_filter);
    }
}