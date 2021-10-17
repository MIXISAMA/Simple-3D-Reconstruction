#pragma once

#include "pch.h"

namespace mixi
{

class FrameBuffer
{
public:
    FrameBuffer(GLuint to, GLuint rbo);
    ~FrameBuffer();

    void bind() const;
    void unbind() const;
protected:
    GLuint fbo_;
private:

};

} // namespace mixi
