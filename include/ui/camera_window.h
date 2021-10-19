#pragma once

#include "pch.h"
#include "app/imgui_window.h"

#include "render/texture.h"

namespace mixi
{
namespace s3r
{

class CameraWindow : public app::ImguiWindow
{
public:

    CameraWindow();

    void render() override;

private:

    Texture cameraTexture_;

    void renderOriginalCamera_();

};

    
} // namespace s3r   
} // namespace mixi

