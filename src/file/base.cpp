#include "file/base.h"

#include "util/utils.h"

namespace mixi
{

ISaveable::ISaveable(fs::path filename) :
    filename_(filename),
    saving_(false),
    progress_(0.0f)
{
    
}

void ISaveable::save(const fs::path& parentPath)
{
    saving_ = true;
    progress_ = 0.0f;
    save_(parentPath);
    progress_ = 1.0f;
    saving_ = false;
}

bool ISaveable::saving() const
{
    return saving_;
}

float ISaveable::progress() const
{
    return progress_;
}

const fs::path& ISaveable::filename()
{
    return filename_;
}

MemoryFile::MemoryFile(fs::path filename) :
    ISaveable(filename)
{

}

void MemoryFile::saveBinary_(const fs::path& parentPath, void* data, int bytes)
{
    std::ofstream ofs(parentPath / filename_, std::ios::binary);
    ofs.write((char*)data, bytes);
    ofs.close();
}


MemoryDirectory::MemoryDirectory(const fs::path& filename) :
    ISaveable(filename),
    files(),
    dirs()
{
    
}

void MemoryDirectory::save_(const fs::path& parentPath)
{
    float stride = 1.0f / (files.size() + dirs.size());
    fs::path dirpath = parentPath / filename_;
    fs::create_directory(dirpath);
    for (const MemoryDirectory::Ptr& childDir : dirs) {
        childDir->save(dirpath);
        progress_ = progress_ + stride;
    }
    for (const MemoryFile::Ptr& file : files) {
        file->save(dirpath);
        progress_ = progress_ + stride;
    }
}

int MemoryDirectory::fileNumber()
{
    int count = files.size();
    for (const MemoryDirectory::Ptr& childDir : dirs) {
        count += childDir->fileNumber();
    }
    return count;
}

FakeMemoryFile::FakeMemoryFile(const fs::path& pkgname) :
    MemoryFile(pkgname),
    tmpDir_(GenTmpDir_())
{
    makeSureTmpDirExistAndEmpty_();
}

FakeMemoryFile::~FakeMemoryFile()
{
    fs::remove_all(tmpDir_);
}

fs::path FakeMemoryFile::tmpDir()
{
    return tmpDir_;
}

void FakeMemoryFile::makeSureTmpDirExistAndEmpty_()
{
    if (fs::exists(tmpDir_)) {
        fs::remove_all(tmpDir_);
    }
    fs::create_directories(tmpDir_);
}

fs::path FakeMemoryFile::GenTmpDir_()
{
    return fs::temp_directory_path() / PROJECT_NAME / std::to_string(GenId());
}

} // namespace mixi
