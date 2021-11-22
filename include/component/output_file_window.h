#pragma once

#include "pch.h"
#include "app/imgui_window.h"
#include "file/base.h"

namespace mixi
{
namespace s3r
{




class OutputFileWindow : app::ImguiWindow
{
public:

    class Context
    {
    friend class OutputFileWindow;
    public:

        Context();
        ~Context();

        void add(MemoryDirectory::Ptr& memoryDir);
        void add(MemoryDirectory::Ptr& memoryDir, const std::string& name);

    protected:

        std::vector<MemoryDirectory::Ptr> dirs_;

    };

    class IContext
    {
    public:

        IContext(Context* outputContext);
        ~IContext();

    protected:

        Context* outputContext_;

    };

    OutputFileWindow();
    ~OutputFileWindow();

    void render() override;

    Context* context();

protected:

    Context context_;

private:

    void renderFileTreeRecursive_(MemoryDirectory::Ptr& dir);

    void renderSaveableDnd_(ISaveable::Ptr* dir);


};



} // namespace s3r
} // namespace mixi
