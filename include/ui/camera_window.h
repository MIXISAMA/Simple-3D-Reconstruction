#pragma once

#include "pch.h"
#include "app/imgui_window.h"

#include "render/texture.h"
#include "util/usb_camera.h"
#include "util/image.h"
#include "util/camera_parameter.h"
#include "output_file_window.h"
#include "process/calibration.h"

namespace mixi
{
namespace s3r
{

class CameraWindow :
    public app::ImguiWindow,
    public OutputFileWindow::IContext
{
public:

    CameraWindow(OutputFileWindow::Context* outputContext);

    void render() override;

private:

    UsbCamera camera_;
    Texture cameraTexture_;
    Texture undistortTexture_;
    const uvc_frame_t* frame_;

    bool isUndistortOn_;
    CameraParameterFile::Ptr cameraParameterFile_;

    MemoryDirectory::Ptr imageOutputDir_;
    MemoryDirectory::Ptr calibratedImageOutputDir_;
    MemoryDirectory::Ptr cameraParameterOutputDir_;

    Calibration calibration_;

    bool isFindCornersOn_;
    bool isShootOnlyWhenFoundCornersOn_;
    bool isFrameFoundCorners_;

    std::map<fs::path, Image::Ptr> calibrationImages_;

    int imageOutputCount_;
    int cameraParameterOutputCount_;

    void renderOriginalCamera_();

    void renderOriginalCameraImage_();

    void updateCameraTextureWithCorners_();

    void renderCalibratedCamera_();

    void renderCalibratedCameraDnd_();

    void renderOperationOptions_();

    void renderCalibrationProcess_();

    void renderCalibrationProcessDnd_();

    void addCalibrationImagesMap_(const ISaveable::Ptr& saveable);

    void outputShootImage_(
        MemoryDirectory::Ptr& outputDir,
        const std::string& defaultDirName,
        const Image::Ptr& image,
        const fs::path& filename
    );

    void outputCameraParameter_(cv::Mat& intrinsic, cv::Mat& distCoeffs, fs::path& filename);

};

    
} // namespace s3r   
} // namespace mixi

