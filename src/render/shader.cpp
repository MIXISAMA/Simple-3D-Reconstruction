#include "render/shader.h"

namespace mixi
{

Shader::Shader(const fs::path& shaderPath, GLenum shaderType) :
    id_(0)
{
    id_ = glCreateShader(shaderType);
    compile(shaderPath);
    checkCompileErrors();
}

Shader::~Shader()
{
    glDeleteShader(id_);
}

void Shader::compile(const fs::path& shaderPath)
{
    std::string codeString;
    std::ifstream shaderFile;
    shaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    try 
    {
        std::stringstream shaderStream;

        shaderFile.open(shaderPath);
        shaderStream << shaderFile.rdbuf();
        shaderFile.close();

        codeString = shaderStream.str();
    }
    catch (std::ifstream::failure& e)
    {
        throw std::runtime_error("file read error!");
    }
    
    const char* code = codeString.c_str();

    glShaderSource(id_, 1, &code, NULL);
    glCompileShader(id_);
}

void Shader::checkCompileErrors()
{
    GLint success = 0;
    GLchar infoLog[1024];

    glGetShaderiv(id_, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(id_, 1024, NULL, infoLog);
        LOG(ERROR) << infoLog;
        throw std::runtime_error("shader compile error!");
    }
}

GLuint Shader::id() const
{
    return id_;
}

} // namespace mixi
