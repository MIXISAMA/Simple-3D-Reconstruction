#pragma once

namespace mixi
{

template<typename T>
class Singleton
{
public:
    static T& Instance()
    {
        static T t;
        return t;
    }
    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;
    void operator= (const T&) = delete;
protected:
    Singleton() = default;
    virtual ~Singleton() = default;
};

}
