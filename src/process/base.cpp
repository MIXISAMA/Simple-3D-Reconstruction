#include "process/base.h"

namespace mixi
{

IProgress::IProgress() :
    progress_(0)
{
    
}

float IProgress::progress()
{
    return progress_;
}

}