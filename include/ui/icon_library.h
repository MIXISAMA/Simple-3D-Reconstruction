#ifndef _ICON_LIBRARY_H
#define _ICON_LIBRARY_H

#include "pch.h"
#include "util/image_util.h"

class IconLibrary
{
public:
    static Texture loadTexture(const std::string& iconPath);
    static Texture freeTexture(const std::string& iconPath);
private:
    static std::set<std::string> texture_set_;
};

#endif
