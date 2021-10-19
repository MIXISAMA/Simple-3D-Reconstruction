#pragma

#include "pch.h"
#include "vertex_buffer.h"

namespace mixi
{

class VertexArray
{
public:

    using Ptr = std::shared_ptr<VertexArray>;

    VertexArray(void* data, GLsizeiptr dataSize, std::vector<GLint>& attributeNumbers);
    ~VertexArray();

    void bind() const;
    void unbind() const;

    void draw(GLenum mode) const;

protected:

    GLuint vao_;
    VertexBuffer vertexBuffer_;
    GLsizei vertexCount_;

};

} // namespace mixi