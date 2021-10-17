#include "render/vertex_buffer.h"

namespace mixi
{

VertexBuffer::VertexBuffer(void* data, GLsizeiptr dataSize) :
    vbo_(0)
{
    glGenBuffers(1, &vbo_);

    bind();
    glBufferData(GL_ARRAY_BUFFER, dataSize, data, GL_STATIC_DRAW);
    unbind();
}

VertexBuffer::~VertexBuffer()
{
    glDeleteBuffers(1, &vbo_);
}

void VertexBuffer::bind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
}

void VertexBuffer::unbind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

GLuint VertexBuffer::vbo() const
{
    return vbo_;
}

} // namespace mixi
