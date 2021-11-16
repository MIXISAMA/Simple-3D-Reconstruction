#include "render/vertex_buffer.h"

namespace mixi
{

VertexBuffer::VertexBuffer(GLenum mode, void* data, GLsizeiptr dataSize) :
    mode_(mode),
    vbo_(0)
{
    glGenBuffers(1, &vbo_);

    bind();
    glBufferData(mode_, dataSize, data, GL_STATIC_DRAW);
    unbind();
}

VertexBuffer::~VertexBuffer()
{
    glDeleteBuffers(1, &vbo_);
}

void VertexBuffer::bind() const
{
    glBindBuffer(mode_, vbo_);
}

void VertexBuffer::unbind() const
{
    glBindBuffer(mode_, 0);
}

GLuint VertexBuffer::vbo() const
{
    return vbo_;
}

} // namespace mixi
