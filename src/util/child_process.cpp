#include "util/child_process.h"

namespace mixi
{

ChildProcess::~ChildProcess()
{
    if (process_.running()) {
        process_.terminate();
    }
}

bp::ipstream& ChildProcess::ips()
{
    return ips_;
}

bp::ipstream& ChildProcess::eips()
{
    return eips_;
}

bool ChildProcess::running()
{
    return process_.running();
}

void ChildProcess::wait()
{
    process_.wait();
}

} // namespace mixi