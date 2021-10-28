#pragma once

#include "pch.h"

namespace mixi
{

class ISaveable
{
public:

    using Ptr = std::shared_ptr<ISaveable>;

    ISaveable(fs::path filename);
    virtual ~ISaveable() = default;

    virtual void save(const fs::path& parentPath) const = 0;

    const fs::path& filename();

protected:

    fs::path filename_;

};

class MemoryFile : public ISaveable
{
public:

    using Ptr = std::shared_ptr<MemoryFile>;

    MemoryFile(fs::path filename);
    virtual ~MemoryFile() = default;

    // enum class Hold
    // {
    //     JUST_ASSIGN_POINTER,
    //     NEW_MEMCPY_DELETE,
    //     MALLOC_MEMCPY_FREE,
    //     JUST_ASSIGN_POINTER_DELETE,
    //     JUST_ASSIGN_POINTER_FREE,
    // };

    // MemoryFile(fs::path& filepath);
    // MemoryFile(void* data, int dataBytes, fs::path& filename, Hold hold);
    // virtual ~MemoryFile();

    // const void* data() const;
    // virtual void save(const fs::path& parentPath) const override;

protected:

    void saveBinary_(const fs::path& parentPath, void* data, int bytes);

    // void* data_;
    // int dataBytes_;

private:

    // Hold hold_;

};



class MemoryDirectory : public ISaveable
{
public:

    using Ptr = std::shared_ptr<MemoryDirectory>;
    
    MemoryDirectory(fs::path& filename);

    void save(const fs::path& parentPath) const override;

    std::vector<MemoryFile::Ptr> files;
    std::vector<MemoryDirectory::Ptr> dirs;

};

void MakeSureDirectoryExists(const fs::path& path);

void MakeSureDirectoryExistsRecursive_(const fs::path& absolutePath);

} // namespace mixi