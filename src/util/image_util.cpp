#include "util/image_util.h"
#include "pch.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Texture LoadTextureFromFile(const char* filename, bool invertColor)
{
    // Load from file
    int image_width = 0;
    int image_height = 0;
    unsigned char* image_data = stbi_load(filename, &image_width, &image_height, NULL, 4);
    unsigned int* edit_data = (unsigned int*)image_data;

    if (image_data == NULL) {
        throw std::runtime_error("stbi load image data error!");
    }

    if (invertColor) {
        for (int i = image_height * image_width - 1; i >= 0 ; i--) {
            edit_data[i] ^= 0x00ffffff;
        }
    }

    // Create a OpenGL texture identifier
    GLuint image_texture;
    glGenTextures(1, &image_texture);
    glBindTexture(GL_TEXTURE_2D, image_texture);

    // Setup filtering parameters for display
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // This is required on WebGL for non power-of-two textures
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); // Same

    // Upload pixels into texture
#if defined(GL_UNPACK_ROW_LENGTH) && !defined(__EMSCRIPTEN__)
    glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
#endif
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_width, image_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
    stbi_image_free(image_data);

    glBindTexture(GL_TEXTURE_2D, 0);

    return {image_texture, image_width, image_height};
}
