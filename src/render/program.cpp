#include "render/program.h"


namespace mixi
{

Program::Program(const std::vector<Shader*>& shaders)
{
    id_ = glCreateProgram();
    for (Shader* shader : shaders) {
        glAttachShader(id_, shader->id());
    }
    glLinkProgram(id_);
    checkLinkErrors();
}

Program::~Program()
{
    glDeleteProgram(id_);
}

void Program::checkLinkErrors()
{
    GLint success;
    GLchar infoLog[1024];

    glGetProgramiv(id_, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(id_, 1024, NULL, infoLog);
        std::runtime_error("program link error!");
    }
}

void Program::use() const
{
    glUseProgram(id_);
}

GLuint Program::id() const
{
    return id_;
}

void Program::setInt(int i, const char* name) const
{
    GLint myLoc = glGetUniformLocation(id_, name);
    glProgramUniform1i(id_, myLoc, i);
}

void Program::setFloat(float f, const char* name) const
{
    GLint myLoc = glGetUniformLocation(id_, name);
    glProgramUniform1f(id_, myLoc, f);
}

void Program::setVec3(float* vec3, const char* name) const
{
    GLint myLoc = glGetUniformLocation(id_, name);
    glProgramUniform3fv(id_, myLoc, 1, vec3);
}

void Program::setMat4(float* mat4, const char* name) const
{
    GLint myLoc = glGetUniformLocation(id_, name);
	glProgramUniformMatrix4fv(id_, myLoc, 1, GL_FALSE, mat4);
}

}

