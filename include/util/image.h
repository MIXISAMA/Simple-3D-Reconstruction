#pragma once

#include "pch.h"

namespace mixi
{

class Image
{
public:

    Image(const char* filePath);
    Image(int width, int height, int bytes, void* data);
    ~Image();

    int width();
    int height();
    const void* data();

    void invertedColor();
    
    void saveToPngFile(const char* filePath);

protected:

    Image();

    void loadFromFile(const char* filePath);
    void loadFromMemory(int width, int height, int bytes, void* data);

    int width_;
    int height_;
    int comp_;
    void* data_;

};

}