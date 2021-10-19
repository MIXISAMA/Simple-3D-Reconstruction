#include "render/frame_buffer.h"

namespace mixi
{

FrameBuffer::FrameBuffer(int width, int height) :
    fbo_(0),
    texture_(),
    renderBuffer_(width, height)
{
    texture_.updateRgbImage2D(width, height, nullptr);

    glGenFramebuffers(1, &fbo_);

    bind();
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture_.id(), 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, renderBuffer_.rbo());
    unbind();

    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        throw std::runtime_error("frame buffer create error!");
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

const Texture* FrameBuffer::texture() const
{
    return &texture_;
}

} // namespace mixi