#include "render/vertex_array.h"

namespace mixi
{

VertexArray::VertexArray(void* data, GLsizeiptr dataSize, std::vector<GLint>& attributeNumbers) :
    vao_(0),
    vertexBuffer_(data, dataSize),
    vertexCount_(0)
{
    glGenVertexArrays(1, &vao_);

    bind();
    GLuint relativeOffset = 0;
    for (int i = 0; i < attributeNumbers.size(); i++) {

        glEnableVertexAttribArray(i);

        GLint number = attributeNumbers[i];
        glVertexAttribFormat(i, number, GL_FLOAT, GL_FALSE, relativeOffset);
        relativeOffset += number * sizeof(float);

        glVertexAttribBinding(i, 0);

    }
    glBindVertexBuffer(0, vertexBuffer_.vbo(), 0, relativeOffset);
    vertexCount_ = dataSize / relativeOffset;
    unbind();
}

VertexArray::~VertexArray()
{
    glDeleteVertexArrays(1, &vao_);
}

void VertexArray::bind() const
{
    glBindVertexArray(vao_);
}

void VertexArray::unbind() const
{
    glBindVertexArray(0);
}

void VertexArray::draw(GLenum mode) const
{
    glDrawArrays(mode, 0, vertexCount_);
}

} // namespace mixi
