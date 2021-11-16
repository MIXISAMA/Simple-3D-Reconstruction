#include "util/ply.h"

#include "util/utils.h"

namespace mixi
{

PlyFile::PlyFile(const fs::path& filepath) :
    MemoryFile(filepath.filename()),
    importer_(),
    scene_(
        importer_.ReadFile(
            filepath,
            aiProcess_GenSmoothNormals      |
            aiProcess_CalcTangentSpace      |
            aiProcess_Triangulate           |
            aiProcess_JoinIdenticalVertices |
            aiProcess_SortByPType           |
            aiProcess_FlipUVs
        )
    )
{
    if (scene_ == nullptr) {
        // std::cout << "ERROR::ASSIMP:: " << importer_.GetErrorString() << std::endl;
        throw std::runtime_error("load ply failed!");
    }

    if (!scene_->HasMeshes()) {
        throw std::runtime_error("PLY file has no meshes!");
    }

    if (scene_->mMeshes[0]->mNumVertices == 0) {
        throw std::runtime_error("mesh[0] has no vertices!");
    }

}

void PlyFile::save(const fs::path& parentPath) const
{
    // Todo: save
}

void PlyFile::parseVertexArray(VertexArray** vertexArray) const
{
    aiMesh* mesh = scene_->mMeshes[0];

    if (hasTexcoord()) {
        char *pointData, *elementData;
        int pointDataBytes, elementDataBytes;
        newDataPointNormalTexcoord_(mesh, &pointData, &pointDataBytes);
        newDataIndices_(mesh, &elementData, &elementDataBytes);

        *vertexArray = new VertexArray(
            pointData, pointDataBytes, std::vector<int>{3, 3, 2},
            elementData, elementDataBytes
        );
        delete[] pointData;
        delete[] elementData;
    }
    else if (hasIndices()) {
        char *pointData, *elementData;
        int pointDataBytes, elementDataBytes;
        newDataPointNormal_(mesh, &pointData, &pointDataBytes);
        newDataIndices_(mesh, &elementData, &elementDataBytes);
        
        *vertexArray = new VertexArray(
            pointData, pointDataBytes, std::vector<int>{3, 3},
            elementData, elementDataBytes
        );
        delete[] pointData;
        delete[] elementData;
    }
    else if (hasColors()) {
        char *pointColorData;
        int pointColorDataBytes;
        newDataPointColor_(mesh, &pointColorData, &pointColorDataBytes);
        
        *vertexArray = new VertexArray(
            pointColorData, pointColorDataBytes, std::vector<int>{3, 3}
        );
        delete[] pointColorData;
    }
    else {
        char *pointData;
        int pointDataBytes;
        newDataPoint_(mesh, &pointData, &pointDataBytes);
        
        *vertexArray = new VertexArray(
            pointData, pointDataBytes, std::vector<int>{3}
        );
        delete[] pointData;
    }
}

void PlyFile::newDataPoint_(const aiMesh* mesh, char** data, int* dataBytes) const
{
    int vec3Bytes = 3 * sizeof(float);

    *dataBytes = mesh->mNumVertices * vec3Bytes;
    *data = new char[*dataBytes];

    for (int i = 0; i < mesh->mNumVertices; i++) {
        memcpy(&(*data)[i * vec3Bytes], &mesh->mVertices[i], vec3Bytes);
    }
}

void PlyFile::newDataPointColor_(const aiMesh* mesh, char** data, int* dataBytes) const
{
    int vec3Bytes = 3 * sizeof(float);
    int lineBytes = vec3Bytes * 2;

    *dataBytes = mesh->mNumVertices * lineBytes;
    *data = new char[*dataBytes];

    for (int i = 0; i < mesh->mNumVertices; i++) {
        memcpy(&(*data)[i * lineBytes], &mesh->mVertices[i], vec3Bytes);
        memcpy(&(*data)[i * lineBytes + vec3Bytes], &mesh->mColors[0][i], vec3Bytes);
    }
}

void PlyFile::newDataIndices_(const aiMesh* mesh, char** data, int* dataBytes) const
{
    int vec3Bytes = 3 * sizeof(unsigned int);
    
    *dataBytes = mesh->mNumFaces * vec3Bytes;
    *data = new char[*dataBytes];

    for (int i = 0; i < mesh->mNumFaces; i++) {
        if (mesh->mFaces[i].mNumIndices != 3) {
            continue;
        }
        memcpy(&(*data)[i * vec3Bytes], mesh->mFaces[i].mIndices, vec3Bytes);
    }
}

void PlyFile::newDataPointNormal_(const aiMesh* mesh, char** data, int* dataBytes) const
{
    int vec3Bytes = 3 * sizeof(float);
    int lineBytes = vec3Bytes * 2;

    *dataBytes = mesh->mNumVertices * lineBytes;
    *data = new char[*dataBytes];

    for (int i = 0; i < mesh->mNumVertices; i++) {
        memcpy(&(*data)[i * lineBytes], &mesh->mVertices[i], vec3Bytes);
        memcpy(&(*data)[i * lineBytes + vec3Bytes], &mesh->mNormals[i], vec3Bytes);
    }
}

void PlyFile::newDataPointNormalTexcoord_(const aiMesh* mesh, char** data, int* dataBytes) const
{
    int vec3Bytes = 3 * sizeof(float);
    int vec2Bytes = 2 * sizeof(float);
    int lineBytes = vec3Bytes + vec3Bytes + vec2Bytes;
    
    *dataBytes = mesh->mNumVertices * lineBytes;
    *data = new char[*dataBytes];

    for (int i = 0; i < mesh->mNumVertices; i++) {
        memcpy(&(*data)[i * lineBytes], &mesh->mVertices[i], vec3Bytes);
        memcpy(&(*data)[i * lineBytes + vec3Bytes], &mesh->mNormals[i], vec3Bytes);
        memcpy(&(*data)[i * lineBytes + vec3Bytes + vec3Bytes], &mesh->mTextureCoords[0][i], vec2Bytes);
    }
}

std::pair<float, float> PlyFile::minmaxX() const
{
    return minmaxOffset_(0);
}

std::pair<float, float> PlyFile::minmaxY() const
{
    return minmaxOffset_(1);
}

std::pair<float, float> PlyFile::minmaxZ() const
{
    return minmaxOffset_(2);
}

std::pair<float, float> PlyFile::minmaxOffset_(unsigned int offset) const
{
    aiMesh* mesh = scene_->mMeshes[0];
    OffsetRange<float> offsetRange((float*)mesh->mVertices, mesh->mNumVertices, offset, 3);
    auto [min, max] = std::minmax_element(offsetRange.begin(), offsetRange.end());
    return {*min, *max};
}

bool PlyFile::hasColors() const
{
    return scene_->mMeshes[0]->HasVertexColors(0);
}

bool PlyFile::hasIndices() const
{
    return scene_->mMeshes[0]->HasFaces();
}

bool PlyFile::hasTexcoord() const
{
    return 
        scene_->mMeshes[0]->HasTextureCoords(0) &&
        scene_->HasMaterials();
}

void PlyFile::getTextureName(aiString* name) const
{
    scene_->mMaterials[0]->GetTexture(aiTextureType_DIFFUSE, 0, name);
}

} // namespace mixi