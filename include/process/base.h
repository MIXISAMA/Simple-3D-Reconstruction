#pragma once

#include "pch.h"

namespace mixi
{

class IProgress
{

public:

    IProgress();

    float progress();

protected:

    std::atomic<float> progress_;

};


} // namespace mixi