#pragma once

#include "pch.h"

namespace mixi
{

class RenderBuffer
{
public:
    RenderBuffer(int width, int height);
    ~RenderBuffer();

    void bind() const;
    void unbind() const;

    GLuint rbo() const;

protected:

    GLuint rbo_;

private:

};

} // namespace mixi
