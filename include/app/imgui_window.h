#pragma once

namespace mixi
{
namespace app
{

class ImguiWindow
{
public:

    using Ptr = std::shared_ptr<ImguiWindow>;

    virtual ~ImguiWindow() = default;

    virtual void render() = 0;

};



} // namespace app
} // namespace mixi

