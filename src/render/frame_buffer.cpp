#include "render/frame_buffer.h"

namespace mixi
{

FrameBuffer::FrameBuffer(GLuint to, GLuint rbo) :
    fbo_(0)
{
    glGenFramebuffers(1, &fbo_);

    bind();
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, to, 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);
    unbind();

    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE) {
        // "good boy";
    }
}

FrameBuffer::~FrameBuffer()
{
    glDeleteFramebuffers(1, &fbo_);
}

void FrameBuffer::bind() const
{
    glBindFramebuffer(GL_FRAMEBUFFER, fbo_);
}

void FrameBuffer::unbind() const
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

} // namespace mixi