#pragma once

#include "pch.h"
#include "render_buffer.h"
#include "texture.h"

namespace mixi
{

class FrameBuffer
{
public:

    using Ptr = std::shared_ptr<FrameBuffer>;

    FrameBuffer(int width, int height);
    ~FrameBuffer();

    void bind() const;
    void unbind() const;

    const Texture* texture() const;

protected:

    GLuint fbo_;

private:

    Texture texture_;
    RenderBuffer renderBuffer_;

};

} // namespace mixi
