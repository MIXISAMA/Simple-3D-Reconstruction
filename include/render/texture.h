#ifndef _TEXTURE_H
#define _TEXTURE_H

#include "pch.h"

namespace mixi
{

class Texture
{
public:

    using Ptr = std::shared_ptr<Texture>;

    Texture();
    ~Texture();

    void bind() const;
    void unbind() const;

    void updateRgbaImage2D(GLsizei width, GLsizei height, const void* data);
    void updateRgbImage2D(GLsizei width, GLsizei height, const void* data);

    GLuint id() const;

protected:

    GLuint to_;

private:

    void updateImage2D(GLsizei width, GLsizei height, const void* data, GLenum format);

};

}

#endif
