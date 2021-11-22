#pragma once

#include "pch.h"
#include "base.h"

namespace mixi
{

class MvsSparsePointFile : public FakeMemoryFile
{
public:

    using Ptr = std::shared_ptr<MvsSparsePointFile>;

    MvsSparsePointFile(const std::string& pkgName);

    ~MvsSparsePointFile() = default;

private:

    std::future<void> saveFuture_;

    const std::string pkgName_;

    virtual void save_(const fs::path& parentPath) override;

    static float ParseProgress(const std::string& line, float defaultProgress);

};

} // namespace mixi