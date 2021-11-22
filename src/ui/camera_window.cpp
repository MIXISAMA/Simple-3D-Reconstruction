#include "ui/camera_window.h"

#include "ui/widget.h"

namespace mixi
{
namespace s3r
{

CameraWindow::CameraWindow(OutputFileWindow::Context* outputContext) :
    camera_(),
    cameraTexture_(),
    undistortTexture_(),
    frame_(nullptr),
    IContext(outputContext),
    imageOutputDir_(nullptr),
    calibratedImageOutputDir_(nullptr),
    cameraParameterOutputDir_(nullptr),
    cameraParameterFile_(nullptr),
    calibration_(),
    isUndistortOn_(false),
    isFindCornersOn_(false),
    isShootOnlyWhenFoundCornersOn_(true),
    isFrameFoundCorners_(false),
    calibrationImages_(),
    imageOutputCount_(0),
    cameraParameterOutputCount_(0)
{

}

void CameraWindow::render()
{
    ImGui::Begin("Camera");

    float aspectRatio = 0.57f;
    if (frame_ != nullptr) {
        aspectRatio = frame_->height / (float)frame_->width;
    }
    float width = ImGui::GetContentRegionAvail().x * 0.5f;
    float height = width * aspectRatio + 55.0f;
    ImGui::BeginChild("Original Camera", ImVec2(width, height));
    renderOriginalCamera_();
    ImGui::EndChild();

    ImGui::SameLine();

    ImGui::BeginChild("Dealt Camera", ImVec2(width, height));
    renderCalibratedCamera_();
    ImGui::EndChild();
    renderCalibratedCameraDnd_();

    ImGui::Separator();

    renderOperationOptions_();

    ImGui::Separator();

    renderCalibrationProcess_();

    ImGui::End();
}

void CameraWindow::renderOriginalCamera_()
{
    UsbCamera::Status status = camera_.getStatus();

    if (ImGui::Button("Find Camera")) {
        imageOutputDir_ = nullptr;
        camera_.refresh();
    }

    if (status != UsbCamera::Status::NOT_STARTED &&
        status != UsbCamera::Status::STARTING
    ) {
        ImGui::SameLine();
        ImGui::Text("no camera found");
        return;
    }

    UsbCamera::Info cameraInfo = camera_.getRecentInfo();
    int frameCount = camera_.getFrameCounter();
    ImGui::SameLine();
    ImGui::Text(
        "camera %d %dx%d fps:%d frame:%d",
        cameraInfo.terminalId,
        cameraInfo.width,
        cameraInfo.height,
        cameraInfo.fps,
        frameCount
    );
    
    ImGui::Separator();

    if (status == UsbCamera::Status::STARTING && frameCount > 2) {
        frame_ = camera_.fetchFrameRgb();
        renderOriginalCameraImage_();
    }

    if (status != UsbCamera::Status::NOT_STARTED) {
        ImGui::BeginDisabled();
    }
    if (ImGui::Button("Start")) {
        camera_.start();
    }
    if (status != UsbCamera::Status::NOT_STARTED) {
        ImGui::EndDisabled();
    }

    ImGui::SameLine();
    if (status != UsbCamera::Status::STARTING) {
        ImGui::BeginDisabled();
    }
    if (ImGui::Button("Stop")) {
        camera_.stop();
    }
    if (status != UsbCamera::Status::STARTING) {
        ImGui::EndDisabled();
    }

    ImGui::SameLine();
    bool isShootDisable = 
        status != UsbCamera::Status::STARTING ||
        frameCount <= 2 ||
        isFindCornersOn_ &&
        isShootOnlyWhenFoundCornersOn_ &&
        !isFrameFoundCorners_;

    if (isShootDisable) {
        ImGui::BeginDisabled();
    }
    if (ImGui::Button("Shoot")) {
        Image::Ptr shootImage(new Image(
            frame_->width,
            frame_->height,
            frame_->data_bytes, 3,
            frame_->data
        ));
        fs::path filename = std::to_string(++imageOutputCount_) + ".png";
        outputShootImage_(imageOutputDir_, "Camera-Shoots", shootImage, filename);
    }
    if (isShootDisable) {
        ImGui::EndDisabled();
    }
}

void CameraWindow::renderOriginalCameraImage_()
{

    if (isFindCornersOn_) {
        updateCameraTextureWithCorners_();
    }
    if (!isFindCornersOn_ || !isFrameFoundCorners_) {
        cameraTexture_.updateRgbImage2D(
            frame_->width,
            frame_->height,
            frame_->data
        );
    }

    float width = ImGui::GetContentRegionAvail().x;
    float height = width * frame_->height / frame_->width;
    ImGui::Image(
        (void*)(intptr_t)cameraTexture_.id(),
        ImVec2(width, height)
    );
    
}

void CameraWindow::updateCameraTextureWithCorners_()
{
    std::vector<cv::Point2f> corners;
    isFrameFoundCorners_ = calibration_.findCornersFast(
        frame_->data,
        frame_->data_bytes,
        frame_->width,
        frame_->height,
        corners
    );
    if (!isFrameFoundCorners_) {
        return;
    }
    
    std::byte* data = new std::byte[frame_->data_bytes];
    memcpy(data, frame_->data, frame_->data_bytes);

    calibration_.drawCornersFast(
        data,
        frame_->data_bytes,
        frame_->width,
        frame_->height,
        corners
    );
    cameraTexture_.updateRgbImage2D(
        frame_->width,
        frame_->height,
        data
    );

    delete[] data;
}

void CameraWindow::renderCalibratedCamera_()
{
    ImGui::Checkbox("Show Undistorted", &isUndistortOn_);
    if (cameraParameterFile_ != nullptr) {
        ImGui::SameLine();
        ImGui::Text("[%s]", cameraParameterFile_->filename().c_str());
    }

    if (!isUndistortOn_ ||
        cameraParameterFile_ == nullptr ||
        camera_.getStatus() != UsbCamera::Status::STARTING) {
        return;
    }

    ImGui::Separator();

    void* data = new char[frame_->data_bytes];

    const cv::Mat intrinsic = cameraParameterFile_->intrinsic();
    const cv::Mat distCoeffs = cameraParameterFile_->distCoeffs();

    calibration_.undistort(
        frame_->data,
        frame_->data_bytes,
        frame_->width,
        frame_->height,
        data,
        intrinsic,
        distCoeffs
    );

    undistortTexture_.updateRgbImage2D(
        frame_->width,
        frame_->height,
        data
    );

    float width = ImGui::GetContentRegionAvail().x;
    float height = width * frame_->height / frame_->width;
    ImGui::Image(
        (void*)(intptr_t)undistortTexture_.id(),
        ImVec2(width, height)
    );

    if (ImGui::Button("Shoot")) {
        Image::Ptr shootImage(new Image(
            frame_->width,
            frame_->height,
            frame_->data_bytes, 3,
            data
        ));
        fs::path filename = std::to_string(++imageOutputCount_) + ".png";
        outputShootImage_(imageOutputDir_, "Undistorted-Img", shootImage, filename);
    }

    delete[] (char*)data;
}

void CameraWindow::renderCalibratedCameraDnd_()
{
    if (!ImGui::BeginDragDropTarget()) {
        return;
    }
    if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("DND_SAVEABLE")) {
        IM_ASSERT(payload->DataSize == sizeof(ISaveable::Ptr));
        ISaveable::Ptr* saveable = (ISaveable::Ptr*)payload->Data;
        if (dynamic_cast<CameraParameterFile*>(saveable->get()) != nullptr) {
            cameraParameterFile_ = *(CameraParameterFile::Ptr*)saveable;
        }
    }
    if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("DND_PATH")) {
        IM_ASSERT(payload->DataSize == sizeof(fs::path));
        fs::path* path = (fs::path*)payload->Data;
        try {
            cameraParameterFile_ = CameraParameterFile::Ptr(
                new CameraParameterFile(*path)
            );
        }
        catch (cv::Exception e) {

        }
    }
    ImGui::EndDragDropTarget();
}



void CameraWindow::renderOperationOptions_()
{
    ImGui::Checkbox("Find the Pattern Corners", &isFindCornersOn_);

    if (!isFindCornersOn_) {
        ImGui::BeginDisabled();
    }
    ImGui::Checkbox("Shoot Only When Found Corners", &isShootOnlyWhenFoundCornersOn_);
    if (!isFindCornersOn_) {
        ImGui::EndDisabled();
    }

    ImGui::RadioButton<Calibration::Pattern>(
        "Chess Board",
        &calibration_.pattern,
        Calibration::Pattern::CHESSBOARD
    );
    ImGui::SameLine();
    ImGui::RadioButton(
        "Circles Grid",
        &calibration_.pattern,
        Calibration::Pattern::CIRCLES_GRID
    );
    ImGui::SameLine();
    ImGui::RadioButton(
        "Asymmetric Circles Grid",
        &calibration_.pattern,
        Calibration::Pattern::ASYMMETRIC_CIRCLES_GRID
    );

    ImGui::InputInt("Pattern Width", &calibration_.boardWidth);
    ImGui::InputInt("Pattern Height", &calibration_.boardHeight);
    ImGui::InputFloat("Square Size (mm)", &calibration_.squareSize);
}

void CameraWindow::renderCalibrationProcess_()
{
    int imagesCount = calibrationImages_.size();
    ImGui::Text("Calibration Process. images: %d", imagesCount);
    ImGui::SameLine();
    HelpMarker(
        "You can drag the photos you need to calibrate here, "
        "and then click the Calibrate button to get the camera parameters."
    );
    ImGui::BeginChild(
        "Calibration Process",
        ImVec2(0, 180),
        true,
        ImGuiWindowFlags_HorizontalScrollbar
    );

    ImVec2 button_sz(90, 30);
    
    ImGuiStyle& style = ImGui::GetStyle();
    int i = 0;
    for (std::pair<fs::path, Image::Ptr> path_image : calibrationImages_) {
        ImGui::Button(path_image.first.c_str(), button_sz);
        float last_button_x2 = ImGui::GetItemRectMax().x;
        float window_visible_x2 = ImGui::GetWindowPos().x + ImGui::GetWindowContentRegionMax().x;
        float next_button_x2 = last_button_x2 + style.ItemSpacing.x + button_sz.x; // Expected position if next button was on same line
        if (i++ + 1 < imagesCount && next_button_x2 < window_visible_x2) {
            ImGui::SameLine();
        }
    }

    ImGui::EndChild();
    renderCalibrationProcessDnd_();
    if (ImGui::Button("Calibrate")) {
        std::vector<Image::Ptr> images;
        for (auto name_image : calibrationImages_) {
            images.push_back(name_image.second);
        }
        cv::Mat intrinsic, distCoeffs;
        double rpe = calibration_.calibrate(images, intrinsic, distCoeffs);

        fs::path filename = 
            std::to_string(++cameraParameterOutputCount_) +
            "-" +
            std::to_string((int)(rpe * 1000)) +
            ".xml";
        outputCameraParameter_(intrinsic, distCoeffs, filename);
    }
}

void CameraWindow::renderCalibrationProcessDnd_()
{
    if (!ImGui::BeginDragDropTarget()) {
        return;
    }
    if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("DND_SAVEABLE")) {
        IM_ASSERT(payload->DataSize == sizeof(ISaveable::Ptr));
        ISaveable::Ptr& saveable = *(ISaveable::Ptr*)payload->Data;
        addCalibrationImagesMap_(saveable);
    }
    ImGui::EndDragDropTarget();
}

void CameraWindow::addCalibrationImagesMap_(const ISaveable::Ptr& saveable)
{
    ImageFile* imageFile = dynamic_cast<ImageFile*>(saveable.get());
    if (imageFile != nullptr) {
        calibrationImages_[imageFile->filename()] = imageFile->image();
        return;
    }
    MemoryDirectory* dir = dynamic_cast<MemoryDirectory*>(saveable.get());
    if (dir != nullptr) {
        for (const MemoryDirectory::Ptr& childDir : dir->dirs) {
            addCalibrationImagesMap_(childDir);
        }
        for (const MemoryFile::Ptr& file : dir->files) {
            addCalibrationImagesMap_(file);
        }
    }
}

void CameraWindow::outputCameraParameter_(
    cv::Mat& intrinsic,
    cv::Mat& distCoeffs,
    fs::path& filename
)
{
    if (cameraParameterOutputDir_ == nullptr) {
        outputContext_->add(cameraParameterOutputDir_, "Camera-Parameter");
    }

    CameraParameterFile::Ptr file(
        new CameraParameterFile(filename, intrinsic, distCoeffs)
    );
    cameraParameterOutputDir_->files.push_back(file);
}

void CameraWindow::outputShootImage_(
    MemoryDirectory::Ptr& outputDir,
    const std::string& defaultDirName,
    const Image::Ptr& image,
    const fs::path& filename
)
{
    if (outputDir == nullptr) {
        outputContext_->add(outputDir, defaultDirName);
    }

    ImageFile::Ptr file(new ImageFile(filename, image));
    outputDir->files.push_back(file);
}

} // namespace s3r
} // namespace mixi

