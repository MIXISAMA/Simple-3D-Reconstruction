#include "util/workspace_file_system.h"

namespace mixi
{

namespace vfs
{


Node::Node(const fs::path& nodeName, Node* parent, bool isFile) :
    nodeName_(nodeName),
    parent_(parent),
    isFile_(isFile),
    childDirs_(),
    childFiles_(),
    isOpening_(false)
{

}

Node::~Node()
{
    close();
}

std::vector<Node*> Node::dirs() const
{
    return std::vector<Node*>(childDirs_.begin(), childDirs_.end());
}

std::vector<Node*> Node::files() const
{
    return std::vector<Node*>(childFiles_.begin(), childFiles_.end());
}

bool Node::isFile() const
{
    return isFile_;
}


fs::path Node::nodeName() const
{
    return nodeName_;
}

fs::path Node::path() const
{
    return parent_->path() / nodeName_;
}

Node* Node::newDir(const fs::path& dirName)
{
    assertNodeType_(true);
    fs::create_directory(path() / dirName);
    Node* childDir = new Node(dirName, this, false);
    childDirs_.insert(childDir);
    return childDir;
}

void Node::move(Node* dstDir)
{
    dstDir->assertNodeType_(false);
    fs::rename(path(), dstDir->path());
    parent_->childDirs_.erase(this);
    dstDir->childDirs_.insert(this);
}

void Node::rename(fs::path& newName)
{
    const fs::path& parentPath = parent_->path();
    fs::rename(parentPath / nodeName_, parentPath / newName);
    nodeName_ = newName;
}

void Node::removeChildFile(Node* childFile)
{
    removeChildDirOrFile_(childFile, childFiles_);
}

void Node::removeChildDir(Node* childDir)
{
    removeChildDirOrFile_(childDir, childDirs_);
}

void Node::removeChildDirOrFile_(
    Node* child,
    std::set<Node*>& children)
{
    auto iter = children.find(child);
    if (iter == children.end()) {
        throw std::runtime_error("Not found child node!");
    }
    Node* delChild = *iter;
    fs::remove(delChild->path());
    delete delChild;
    children.erase(iter);
}

void Node::open()
{
    update();
    isOpening_ = true;
}

void Node::close()
{
    ClearSet(childDirs_);
    ClearSet(childFiles_);
    isOpening_ = false;
}

bool Node::isOpening()
{
    return isOpening_;
}

void Node::updateDirs_(std::set<fs::path>& newDirNames)
{
    updateDirsOrFiles_(newDirNames, childDirs_, false);
}

void Node::updateFiles_(std::set<fs::path>& newFileNames)
{
    updateDirsOrFiles_(newFileNames, childFiles_, true);
}

void Node::updateDirsOrFiles_(
        std::set<fs::path>& newNames,
        std::set<Node*>& children,
        bool isFile)
{
    auto oldIter = children.begin();

    while (oldIter != children.end()) {
        auto currentOldIter = oldIter++;
        const fs::path& oldName = (*currentOldIter)->nodeName();
        auto newIter = newNames.find(oldName);
        if (newIter == newNames.end()) {
            delete *currentOldIter;
            children.erase(currentOldIter);
        }
        else {
            newNames.erase(newIter);
        }
    }

    for (const fs::path& newName : newNames) {
        Node* newChild = new Node(newName, this, isFile);
        children.insert(newChild);
    }
}

void Node::update()
{
    assertNodeType_(false);

    std::set<fs::path> newFileNames;
    std::set<fs::path> newDirNames;

    for (auto& iter : fs::directory_iterator(path())) {
        fs::path childPath = iter.path();
        if (fs::is_directory(childPath)) {
            newDirNames.insert(childPath.filename());
        }
        else {
            newFileNames.insert(childPath.filename());
        }
    }

    updateFiles_(newFileNames);
    updateDirs_(newDirNames);

}

void Node::assertNodeType_(bool isFile)
{
    if (isFile_ != isFile) {
        throw std::runtime_error("This node is file node!");
    }
}

void Node::ClearSet(std::set<Node*>& s)
{
    for (Node* node : s) {
        delete node;
    }
    s.clear();
}


Workspace::Workspace(const fs::path& rootDir) :
    Node(rootDir.filename(), nullptr, false),
    rootDir_(rootDir)
{
    if (!fs::is_directory(rootDir)) {
        throw std::runtime_error("Workspace need a directory path!");
    }
    open();
}

fs::path Workspace::path() const
{
    return rootDir_;
}


} // namespace vfs
} // namespace mixi
