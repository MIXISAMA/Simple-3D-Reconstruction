#pragma once

#include "pch.h"

namespace mixi
{

namespace vfs
{

class Node
{
public:

    Node(const fs::path& nodeName, Node* parent, bool isFile);
    virtual ~Node();

    std::vector<Node*> dirs() const;
    std::vector<Node*> files() const;

    bool isFile() const;

    fs::path nodeName() const;
    virtual fs::path path() const;

    Node* newDir(const fs::path& dirName);

    void move(Node* dstDir);
    void rename(fs::path& newName);
    void removeChildFile(Node* childFile);
    void removeChildDir(Node* childDir);

    void open();
    void close();

    void update();

    bool isOpening();

protected:

    bool isFile_;

    fs::path nodeName_;

    Node* parent_;

    std::set<Node*> childDirs_;
    std::set<Node*> childFiles_;

    bool isOpening_;

private:

    void removeChildDirOrFile_(
        Node* child,
        std::set<Node*>& children
    );

    void updateDirs_(std::set<fs::path>& newDirNames);
    void updateFiles_(std::set<fs::path>& newFileNames);

    void assertNodeType_(bool isFile);

    void updateDirsOrFiles_(
        std::set<fs::path>& newNames,
        std::set<Node*>& child,
        bool isFile
    );

    static void ClearSet(std::set<Node*>& s);

};



class Workspace : public Node
{
public:

    using Ptr = std::shared_ptr<Workspace>;

    Workspace(const fs::path& rootDir);
    ~Workspace() = default;

    fs::path path() const;

protected:

    const fs::path rootDir_;

};


} // namespace vfs

} // namespace mixi
