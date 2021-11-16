#pragma once

#include "pch.h"
#include "shader.h"

namespace mixi
{

class Program
{
public:

    Program(const std::vector<Shader*>& shaders);
    ~Program();

    Program(const Program&) = delete;
    Program& operator=(const Program&) = delete;

    void use() const;

    GLuint id() const;

    void setInt(int i, const char* name) const;
    void setFloat(float f, const char* name) const;
    void setVec3(float* vec3, const char* name) const;
    void setMat4(float* mat4, const char* name) const;

protected:

    GLuint id_;

private:

    void checkLinkErrors();

};


} // namespace mixi
