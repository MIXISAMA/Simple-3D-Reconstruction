#include "render/render_buffer.h"

namespace mixi
{

RenderBuffer::RenderBuffer(int width, int height) :
    rbo_(0)
{
    glGenRenderbuffers(1, &rbo_);
    
    bind();
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
    unbind();
}

void RenderBuffer::bind() const
{
    glBindRenderbuffer(GL_RENDERBUFFER, rbo_); 
}

void RenderBuffer::unbind() const
{
    glBindRenderbuffer(GL_RENDERBUFFER, 0); 
}

GLuint RenderBuffer::rbo() const
{
    return rbo_;
}

} // namespace mixi
