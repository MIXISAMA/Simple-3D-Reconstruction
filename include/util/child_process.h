#pragma once

#include "pch.h"

namespace mixi
{

class ChildProcess
{
public:

    template<typename... Args>
    ChildProcess(const std::atomic<bool>& terminate, Args&&... args) :
        terminate_(terminate)
    {
        if (terminate_) {
            return;
        }

        bp::child c(std::forward<Args>(args)...);

        while (c.running()) {
            if (terminate_) {
                c.terminate();
            }
        }
        c.wait();
    }

    ~ChildProcess() = default;

private:

    const std::atomic<bool>& terminate_;

};

} // namespace mixi
