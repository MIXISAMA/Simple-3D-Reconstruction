#pragma once

#include "pch.h"

namespace mixi
{

class ChildProcess
{
public:

    template<typename... Args>
    ChildProcess(Args&&... args) :
        ips_(),
        eips_(),
        process_(std::forward<Args>(args)..., bp::std_out > ips_, bp::std_err > eips_)
    {

    }

    virtual ~ChildProcess();

    bp::ipstream& ips();
    bp::ipstream& eips();

    bool running();
    void wait();

protected:

    bp::ipstream ips_; // pipeline
    bp::ipstream eips_; // error pipeline
    bp::child process_;

};

} // namespace mixi
