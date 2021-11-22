#pragma once

#include "pch.h"
#include "base.h"
#include "util/image.h"

namespace mixi
{

class ImageFile : public MemoryFile
{
public:

    ImageFile(const fs::path& filepath);
    ImageFile(const fs::path& filename, const Image::Ptr& image);

    ~ImageFile() = default;

    Image::Ptr image() const;

protected:

    Image::Ptr image_;

    virtual void save_(const fs::path& parentPath) override;


};


} // namespace mixi
