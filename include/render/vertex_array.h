#pragma once

#include "pch.h"
#include "vertex_buffer.h"

namespace mixi
{

class VertexArray
{
public:

    using Ptr = std::shared_ptr<VertexArray>;

    VertexArray(
        void* data, GLsizeiptr dataSize, const std::vector<GLint>& attributeNumbers
    );
    VertexArray(
        void* data, GLsizeiptr dataSize, const std::vector<GLint>& attributeNumbers,
        void* indicesData, GLsizeiptr indicesSize
    );
    ~VertexArray();

    void bind() const;
    void unbind() const;

    void setMode(GLenum mode);

    void draw() const;

protected:

    GLuint vao_;

    VertexBuffer vertexBuffer_;
    GLsizei vertexCount_;

    std::unique_ptr<VertexBuffer> elementBuffer_;
    GLsizei elementCount_;

    GLenum mode_;

private:

    void initVertexBuffer_(
        GLsizeiptr dataSize,
        const std::vector<GLint>& attributeNumbers
    );

};

} // namespace mixi