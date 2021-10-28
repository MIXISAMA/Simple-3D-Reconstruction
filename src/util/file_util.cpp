#include "util/file_util.h"

#include "util/image.h"

namespace mixi
{

ISaveable::ISaveable(fs::path filename) :
    filename_(filename)
{
    
}

const fs::path& ISaveable::filename()
{
    return filename_;
}

// MemoryFile::MemoryFile(fs::path& filepath) :
//     data_(nullptr),
//     dataBytes_(0),
//     ISaveable(filepath.filename()),
//     hold_(Hold::NEW_MEMCPY_DELETE)
// {
//     std::ifstream ifs(filepath, std::ios::binary);
//     ifs.seekg(0, ifs.end);
//     dataBytes_ = ifs.tellg();
//     data_ = new char[dataBytes_];

//     ifs.seekg(0, ifs.beg);
//     ifs.read((char*)data_, dataBytes_);
//     ifs.close();
// }

// MemoryFile::MemoryFile(void* data, int dataBytes, fs::path& filename, Hold hold) :
//     data_(data),
//     dataBytes_(dataBytes),
//     ISaveable(filename),
//     hold_(hold)
// {
//     switch (hold) {
//     case Hold::MALLOC_MEMCPY_FREE:
//         data_ = malloc(dataBytes);
//         memcpy(data_, data, dataBytes);
//         break;
//     case Hold::NEW_MEMCPY_DELETE:
//         data_ = new char[dataBytes];
//         memcpy(data_, data, dataBytes);
//         break;
//     default:
//         break;
//     }
// }

// MemoryFile::~MemoryFile()
// {
//     switch (hold_) {
//     case Hold::MALLOC_MEMCPY_FREE:
//     case Hold::JUST_ASSIGN_POINTER_FREE:
//         free(data_);
//         break;
//     case Hold::NEW_MEMCPY_DELETE:
//     case Hold::JUST_ASSIGN_POINTER_DELETE:
//         delete[] (char*)data_;
//         break;
//     default:
//         break;
//     }
// }

// void MemoryFile::save(const fs::path& parentPath) const
// {
//     std::ofstream ofs(parentPath / filename_, std::ios::binary);
//     ofs.write((char*)data_, dataBytes_);
//     ofs.close();
// }

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

// const void* MemoryFile::data() const
// {
//     return data_;
// }






MemoryDirectory::MemoryDirectory(fs::path& filename) :
    files(),
    dirs(),
    ISaveable(filename)
{
    
}

void MemoryDirectory::save(const fs::path& parentPath) const
{
    fs::path dirpath = parentPath / filename_;
    fs::create_directory(dirpath);
    for (const MemoryDirectory::Ptr& childDir : dirs) {
        childDir->save(dirpath);
    }
    for (const MemoryFile::Ptr& file : files) {
        file->save(dirpath);
    }
}





void MakeSureDirectoryExists(const fs::path& path)
{
    if (path.is_relative()) {
        throw std::runtime_error("path can not be relative");
    }
    MakeSureDirectoryExistsRecursive_(path);
}

void MakeSureDirectoryExistsRecursive_(const fs::path& path)
{
    if (fs::exists(path) && fs::is_directory(path)) {
        return;
    }
    MakeSureDirectoryExistsRecursive_(path.parent_path());
    fs::create_directory(path);
}

} // namespace mixi
