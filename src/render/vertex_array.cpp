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

    GLsizei stride = 0;
    for (GLint number : attributeNumbers) {
        stride += number * sizeof(float);
    }

    vertexBuffer_.bind();
    int relativeOffset = 0;
    for (int i = 0; i < attributeNumbers.size(); i++) {
        GLint number = attributeNumbers[i];
        const void* pointer = (const void*)(size_t)relativeOffset;
        glVertexAttribPointer(i, number, GL_FLOAT, GL_FALSE, stride, pointer);
        glEnableVertexAttribArray(i);
        relativeOffset += number * sizeof(float);
    }
    vertexBuffer_.unbind();

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
    bind();
    glDrawArrays(mode, 0, vertexCount_);
    unbind();
}

} // namespace mixi
