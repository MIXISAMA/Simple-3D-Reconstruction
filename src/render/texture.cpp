#include "render/texture.h"

namespace mixi
{

Texture::Texture() :
    to_(0)
{
    glGenTextures(1, &to_);
    bind();

    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // This is required on WebGL for non power-of-two textures
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

    unbind();
}

Texture::~Texture()
{
    glDeleteTextures(1, &to_);
}

void Texture::updateImage2D(GLsizei width, GLsizei height, const void* data, GLenum format)
{
    bind();
#if defined(GL_UNPACK_ROW_LENGTH) && !defined(__EMSCRIPTEN__)
    glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
#endif
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    unbind();
}

void Texture::updateRgbaImage2D(GLsizei width, GLsizei height, const void* data)
{
    updateImage2D(width, height, data, GL_RGBA);
}

void Texture::updateRgbImage2D(GLsizei width, GLsizei height, const void* data)
{
    updateImage2D(width, height, data, GL_RGB);
}

void Texture::bind() const
{
    glBindTexture(GL_TEXTURE_2D, to_);
}

void Texture::unbind() const
{
    glBindTexture(GL_TEXTURE_2D, 0);
}

GLuint Texture::id() const
{
    return to_;
}

} // namespace mixi



