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

    void save(const fs::path& parentPath);

    bool saving() const;
    float progress() const;

    const fs::path& filename();

    void clearSaveState();

protected:

    fs::path filename_;

    std::atomic<float> progress_;

    virtual void save_(const fs::path& parentPath) = 0;

private:

    std::atomic<bool> saving_;

};

class MemoryFile : public ISaveable
{
public:

    using Ptr = std::shared_ptr<MemoryFile>;

    MemoryFile(fs::path filename);
    virtual ~MemoryFile() = default;

protected:

    void saveBinary_(const fs::path& parentPath, void* data, int bytes);

private:

};



class MemoryDirectory : public ISaveable
{
public:

    // struct SaveProgressDescription
    // {
    //     int         savedFileNumber;
    //     MemoryFile* savingFile;
    // };

    using Ptr = std::shared_ptr<MemoryDirectory>;
    
    MemoryDirectory(const fs::path& filename);

    std::vector<MemoryFile::Ptr> files;
    std::vector<MemoryDirectory::Ptr> dirs;

    int fileNumber();

protected:

    virtual void save_(const fs::path& parentPath) override;
    // void saveWithDescription_(const fs::path& parentPath);

private:

    // void setDescription_(std::atomic<SaveProgressDescription>* description);

    // std::atomic<SaveProgressDescription>* description_;

};

class FakeMemoryFile : public MemoryFile
{
public:

    FakeMemoryFile(const fs::path& pkgname);
    virtual ~FakeMemoryFile();

    fs::path tmpDir();

protected:

    fs::path tmpDir_;

private:

    void makeSureTmpDirExistAndEmpty_();

    static fs::path GenTmpDir_();

};

} // namespace mixi