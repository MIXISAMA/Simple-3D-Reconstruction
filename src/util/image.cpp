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

Image::Image(const fs::path& filepath) :
    Image()
{
    data_ = stbi_load(filepath.c_str(), &width_, &height_, &comp_, 4);
}

Image::Image(int width, int height, int bytes, int comp, void* data) :
    width_(width),
    height_(height),
    comp_(comp),
    data_(nullptr)
{
    data_ = malloc(bytes);
    memcpy(data_, data, bytes);
}

Image::~Image()
{
    stbi_image_free(data_);
}


void Image::invertedColor()
{
    for (int i = height_ * width_ - 1; i >= 0 ; i--) {
        ((int32_t*)data_)[i] ^= 0x00ffffff;
    }
}

int Image::width() const
{
    return width_;
}

int Image::height() const
{
    return height_;
}

const void* Image::data() const
{
    return data_;
}

void Image::saveToPngFile(const fs::path& filepath)
{
    stbi_write_png(filepath.c_str(), width_, height_, comp_, data_, 0);
}


ImageFile::ImageFile(fs::path& filepath) :
    MemoryFile(filepath.filename()),
    image_(new Image(filepath))
{

}

ImageFile::ImageFile(fs::path& filename, Image::Ptr& image) :
    MemoryFile(filename),
    image_(image)
{

}

Image::Ptr ImageFile::image() const
{
    return image_;
}

void ImageFile::save(const fs::path& parentPath) const
{
    fs::path filepath(parentPath / filename_);
    image_->saveToPngFile(filepath);
}


} // namespace mixi
