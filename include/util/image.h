#pragma once

#include "pch.h"
#include "file_util.h"

namespace mixi
{

class Image
{
public:

    using Ptr = std::shared_ptr<Image>;

    Image(const fs::path& filepath);
    Image(int width, int height, int bytes, int comp, void* data);
    ~Image();

    int width() const;
    int height() const;
    const void* data() const;

    void invertedColor();
    
    void saveToPngFile(const fs::path& filepath);

protected:

    Image();

    int width_;
    int height_;
    int comp_;
    void* data_;

};

class ImageFile : public MemoryFile
{
public:

    ImageFile(const fs::path& filepath);
    ImageFile(const fs::path& filename, const Image::Ptr& image);

    ~ImageFile() = default;

    void save(const fs::path& parentPath) const override;
    Image::Ptr image() const;

protected:

    Image::Ptr image_;

};


}