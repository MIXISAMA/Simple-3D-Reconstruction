#pragma once

#include "pch.h"

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


} // namespace mixi
