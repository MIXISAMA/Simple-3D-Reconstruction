#pragma once

#include "pch.h"
#include "base.h"

#include "render/vertex_array.h"

namespace mixi
{

class PlyFile : public MemoryFile
{
public:

    using Ptr = std::shared_ptr<PlyFile>;

    PlyFile(const fs::path& filepath);
    
    ~PlyFile() = default;

    void parseVertexArray(VertexArray** vertexArray) const;

    bool hasColors() const;
    bool hasIndices() const;
    bool hasTexcoord() const;

    void getTextureName(aiString* name) const;

    std::pair<float, float> minmaxX() const;
    std::pair<float, float> minmaxY() const;
    std::pair<float, float> minmaxZ() const;

private:

    Assimp::Importer importer_;
    const aiScene* scene_;

    virtual void save_(const fs::path& parentPath) override;

    void newDataPoint_(const aiMesh* mesh, char** data, int* dataBytes) const;
    void newDataPointColor_(const aiMesh* mesh, char** data, int* dataBytes) const;
    void newDataIndices_(const aiMesh* mesh, char** data, int* dataBytes) const;
    void newDataPointNormal_(const aiMesh* mesh, char** data, int* dataBytes) const;
    void newDataPointNormalTexcoord_(const aiMesh* mesh, char** data, int* dataBytes) const;

    std::pair<float, float> minmaxOffset_(unsigned int offset) const;

};


} // namespace mixi
