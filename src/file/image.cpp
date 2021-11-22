#include "file/image.h"


namespace mixi
{


ImageFile::ImageFile(const fs::path& filepath) :
    MemoryFile(filepath.filename()),
    image_(new Image(filepath))
{

}

ImageFile::ImageFile(const fs::path& filename, const Image::Ptr& image) :
    MemoryFile(filename),
    image_(image)
{

}

Image::Ptr ImageFile::image() const
{
    return image_;
}

void ImageFile::save_(const fs::path& parentPath)
{
    fs::path filepath(parentPath / filename_);
    image_->saveToPngFile(filepath);
}


} // namespace mixi
