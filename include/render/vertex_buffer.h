#pragma once

#include "pch.h"

namespace mixi
{

class VertexBuffer
{
public:

    VertexBuffer(void* data, GLsizeiptr dataSize);
    ~VertexBuffer();

    void bind() const;
    void unbind() const;

    GLuint vbo() const;

protected:

    GLuint vbo_;

};

} // namespace mixi
