#pragma once

#include "pch.h"
#include "base.h"

namespace mixi
{

class MvsDensePointCloud : public IProgress
{
public:

    MvsDensePointCloud();
    ~MvsDensePointCloud() = default;

    void run(
        const std::string& outputName,
        const fs::path& mvsInputFilePath
    );

};

class MvsRoughMesh : public IProgress
{
public:

    MvsRoughMesh();
    ~MvsRoughMesh() = default;

    void run(
        const std::string& outputName,
        const fs::path& mvsInputFilePath
    );

};

class MvsRefineMesh : public IProgress
{
public:

    MvsRefineMesh();
    ~MvsRefineMesh() = default;

    void run(
        const std::string& outputName,
        const fs::path& mvsInputFilePath
    );

};

class MvsTextureMesh : public IProgress
{
public:

    MvsTextureMesh();
    ~MvsTextureMesh() = default;

    void run(
        const std::string& outputName,
        const fs::path& mvsInputFilePath
    );

};

}