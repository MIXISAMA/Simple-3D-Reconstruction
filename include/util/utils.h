#pragma once

#include "pch.h"

namespace mixi
{

template<class T, typename... Args>
void NewIfNullptr(T** obj, Args&&... args)
{
    if (*obj != nullptr) {
        return;
    }
    *obj = new T(std::forward<Args>(args)...);
}

template<class T, typename... Args>
void ResetIfNullptr(std::unique_ptr<T>& obj, Args&&... args)
{
    if (obj != nullptr) {
        return;
    }
    obj.reset(new T(std::forward<Args>(args)...));
}

template<class T1, class T2, typename... Args>
void ResetIfCanNotCast(std::unique_ptr<T2>& obj, Args&&... args)
{
    if (dynamic_cast<T1*>(obj.get()) == nullptr) {
        obj.reset(new T1(std::forward<Args>(args)...));
    }
}

template<typename T>
class OffsetRangeIter
{
public:

    using iterator_category = std::forward_iterator_tag;
    using difference_type   = std::ptrdiff_t;
    using value_type        = T;
    using pointer           = T*;
    using reference         = T&;

    OffsetRangeIter(pointer data, unsigned int stride) :
        data_(data),
        stride_(stride)
    {

    }

    OffsetRangeIter<value_type>& operator ++ ()  
    {
        data_ += stride_;
        return *this;
    }

    OffsetRangeIter<value_type> operator ++ (int)
    {
        OffsetRangeIter<T> tmp = *this;
        ++(*this);
        return tmp;
    }

    friend bool operator == (const OffsetRangeIter<T>& a, const OffsetRangeIter<T>& b)
    {
        return a.data_ == b.data_;
    }

    friend bool operator != (const OffsetRangeIter<T>& a, const OffsetRangeIter<T>& b)
    {
        return a.data_ != b.data_;
    }

    reference operator * ()
    {
        return *data_;
    }

    pointer operator -> ()
    {
        return data_;
    }

private:

    pointer data_;
    unsigned int stride_;

};

template<typename T>
class OffsetRange
{
public:
    OffsetRange(
        T* data,
        unsigned int len,
        unsigned int offset,
        unsigned int stride
    ) :
        data_(data),
        len_(len),
        offset_(offset),
        stride_(stride)
    {

    }

    OffsetRangeIter<T> begin() noexcept
    {
        return OffsetRangeIter<T>(data_ + offset_, stride_);
    }

    OffsetRangeIter<T> end() noexcept
    {
        return OffsetRangeIter<T>(data_ + stride_ * len_ + offset_, stride_);
    }

private:

    T* data_;
    unsigned int len_;
    unsigned int offset_;
    unsigned int stride_;

};

unsigned int GenId();

void MakeSureDirectoryExists(const fs::path& path);

void MakeSureDirectoryExistsRecursive_(const fs::path& absolutePath);


}