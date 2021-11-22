#include "util/utils.h"


namespace mixi
{

unsigned int _incrementId = 0;

unsigned int GenId()
{
    return _incrementId++;
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
