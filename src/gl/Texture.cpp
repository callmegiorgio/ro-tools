#include "Texture.hpp"

static inline GLint resizeFilterGlParam(Texture::ResizeFilter filter)
{
    switch (filter)
    {
        case Texture::Nearest:              return GL_NEAREST; break;
        case Texture::Linear:               return GL_LINEAR; break;
        case Texture::NearestMipmapNearest: return GL_NEAREST_MIPMAP_NEAREST; break;
        case Texture::NearestMipmapLinear:  return GL_NEAREST_MIPMAP_LINEAR; break;
        case Texture::LinearMipmapLinear:   return GL_LINEAR_MIPMAP_LINEAR; break;
        case Texture::LinearMipmapNearest:  return GL_LINEAR_MIPMAP_NEAREST; break;
        default: return 0;
    }
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
    if (GLint param = resizeFilterGlParam(filter))
    {
        bind();
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, param);
    }
}

void Texture::setMagFilter(ResizeFilter filter) const
{
    if (filter != Nearest && filter != Linear)
        return;

    bind();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, resizeFilterGlParam(filter));
}

void Texture::setResizeFilters(ResizeFilter mag_filter, ResizeFilter min_filter) const
{
    if (mag_filter != Nearest && mag_filter != Linear)
        return;

    if (GLint min_param = resizeFilterGlParam(min_filter))
    {
        bind();
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min_param);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, resizeFilterGlParam(mag_filter));
    }
}