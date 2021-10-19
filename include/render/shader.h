#pragma once

#include "pch.h"

namespace mixi
{
    
class Shader
{
public:

    using Ptr = std::shared_ptr<Shader>;

    Shader(fs::path& shaderPath, GLenum shaderType);
    ~Shader();

    Shader(const Shader&) = delete;
    Shader& operator=(const Shader&) = delete;

    GLuint id() const;

protected:

    GLuint id_;

private:

    void compile(fs::path& shaderPath);
    void checkCompileErrors();

};



} // namespace mixi
