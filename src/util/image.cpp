#include "util/image.h"

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image.h"
#include "stb_image_write.h"

namespace mixi
{


Image::Image() :
    width_(0),
    height_(0),
    comp_(0),
    data_(nullptr)
{

}

Image::Image(const char* filePath) :
    Image()
{
    loadFromFile(filePath);
}

Image::Image(int width, int height, int bytes, void* data) :
    Image()
{
    loadFromMemory(width, height, bytes, data);
}

Image::~Image()
{
    stbi_image_free(data_);
}

void Image::loadFromFile(const char* filePath)
{
    data_ = stbi_load(filePath, &width_, &height_, NULL, 4);
}

void Image::loadFromMemory(int width, int height, int bytes, void* data)
{
    stbi_load_from_memory((const stbi_uc *)data_, bytes, &width, &height, nullptr, 4);
}

void Image::invertedColor()
{
    for (int i = height_ * width_ - 1; i >= 0 ; i--) {
        ((int32_t*)data_)[i] ^= 0x00ffffff;
    }
}

int Image::width()
{
    return width_;
}

int Image::height()
{
    return height_;
}

const void* Image::data()
{
    return data_;
}

void Image::saveToPngFile(const char* filePath)
{
    stbi_write_png(filePath, width_, height_, comp_, data_, 0);
}


} // namespace mixi
