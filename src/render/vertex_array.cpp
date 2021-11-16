#include "render/vertex_array.h"

namespace mixi
{

VertexArray::VertexArray(
    void* data, GLsizeiptr dataSize, const std::vector<GLint>& attributeNumbers
) :
    vao_(0),
    vertexBuffer_(GL_ARRAY_BUFFER, data, dataSize),
    elementBuffer_(nullptr),
    vertexCount_(0),
    elementCount_(0),
    mode_(GL_POINTS)
{
    glGenVertexArrays(1, &vao_);

    initVertexBuffer_(dataSize, attributeNumbers);
}

VertexArray::VertexArray(
    void* data, GLsizeiptr dataSize, const std::vector<GLint>& attributeNumbers,
    void* indicesData, GLsizeiptr indicesSize
) :
    vao_(0),
    vertexBuffer_(GL_ARRAY_BUFFER, data, dataSize),
    elementBuffer_(
        new VertexBuffer(GL_ELEMENT_ARRAY_BUFFER, indicesData, indicesSize)
    ),
    vertexCount_(0),
    elementCount_(indicesSize / sizeof(unsigned int)),
    mode_(GL_TRIANGLES)
{
    glGenVertexArrays(1, &vao_);

    initVertexBuffer_(dataSize, attributeNumbers);

    bind();
    elementBuffer_->bind();
    unbind();
}

VertexArray::~VertexArray()
{
    glDeleteVertexArrays(1, &vao_);
}

void VertexArray::initVertexBuffer_(
    GLsizeiptr dataSize,
    const std::vector<GLint>& attributeNumbers
)
{
    bind();
    vertexBuffer_.bind();

    GLsizei stride = 0;
    for (GLint number : attributeNumbers) {
        stride += number * sizeof(float);
    }

    int relativeOffset = 0;
    for (int i = 0; i < attributeNumbers.size(); i++) {
        GLint number = attributeNumbers[i];
        const void* pointer = (const void*)(size_t)relativeOffset;
        glVertexAttribPointer(i, number, GL_FLOAT, GL_FALSE, stride, pointer);
        glEnableVertexAttribArray(i);
        relativeOffset += number * sizeof(float);
    }

    vertexCount_ = dataSize / relativeOffset;

    vertexBuffer_.unbind();
    unbind();
}

void VertexArray::bind() const
{
    glBindVertexArray(vao_);
}

void VertexArray::unbind() const
{
    glBindVertexArray(0);
}

void VertexArray::setMode(GLenum mode)
{
    mode_ = mode;
}

void VertexArray::draw() const
{
    bind();
    if (elementBuffer_ == nullptr) {
        glDrawArrays(mode_, 0, vertexCount_);
    }
    else {
        glDrawElements(mode_, elementCount_, GL_UNSIGNED_INT, 0);
    }
    unbind();
}

} // namespace mixi
