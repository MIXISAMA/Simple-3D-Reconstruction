#pragma once

#include "pch.h"

namespace mixi
{

class VertexBuffer
{
public:

    VertexBuffer(GLenum mode, void* data, GLsizeiptr dataSize);
    ~VertexBuffer();

    void bind() const;
    void unbind() const;

    GLuint vbo() const;

protected:

    GLenum mode_;

    GLuint vbo_;

};

} // namespace mixi
