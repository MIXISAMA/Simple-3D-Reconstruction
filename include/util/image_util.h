#ifndef _IMAGE_UTIL_H
#define _IMAGE_UTIL_H

#include "glad/gl.h"

struct Texture
{
    GLuint texture;
    int width;
    int height;
};

Texture LoadTextureFromFile(const char* filename, bool invertColor=false);

#endif
