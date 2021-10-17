#include "util/usb_camera.h"

namespace mixi
{

UsbCamera::UsbCamera() :
    context_(nullptr),
    device_(nullptr),
    handle_(nullptr),
    frameRgb_{nullptr, nullptr},
    currentFrameRgb_(0),
    frameFormat_(UVC_COLOR_FORMAT_MJPEG),
    width_(640),
    height_(480),
    fps_(30),
    terminalId_(0),
    status_(Status::UNINITIALIZED),
    frameCounter_(0)
{

}

UsbCamera::~UsbCamera()
{
    exit_();
}

void UsbCamera::init_()
{
    if (status_ != Status::UNINITIALIZED) {
        return;
    }

    uvc_error_t res;

    res = uvc_init(&context_, nullptr);
    if (res != UVC_SUCCESS) {
        throw std::runtime_error("uvc init error!");
    }

    res = uvc_find_device(context_, &device_, 0, 0, NULL);
    if (res != UVC_SUCCESS) {
        status_ = Status::NOT_FOUND;
        return;
    }

    res = uvc_open(device_, &handle_);
    if (res != UVC_SUCCESS) {
        throw std::runtime_error("uvc init error!");
    }

    const uvc_format_desc_t* format_desc = uvc_get_format_descs(handle_);
    switch (format_desc->bDescriptorSubtype) {
    case UVC_VS_FORMAT_MJPEG:
        frameFormat_ = UVC_COLOR_FORMAT_MJPEG;
        break;
    case UVC_VS_FORMAT_FRAME_BASED:
        frameFormat_ = UVC_FRAME_FORMAT_H264;
        break;
    default:
        frameFormat_ = UVC_FRAME_FORMAT_YUYV;
        break;
    }

    const uvc_frame_desc_t* frame_desc = format_desc->frame_descs;
    if (frame_desc) {
        width_ = frame_desc->wWidth;
        height_ = frame_desc->wHeight;
        fps_ = 10000000 / frame_desc->dwDefaultFrameInterval;
    }

    for (int i = 0; i < 2; i++) {
        frameRgb_[i] = uvc_allocate_frame(width_ * height_ * 3);
        if (frameRgb_[i] == nullptr) {
            throw std::runtime_error("uvc allocate frame error!");
        }
    }

    outputFrameRgb_ = uvc_allocate_frame(width_ * height_ * 3);

    const uvc_input_terminal_t* input_terminal = uvc_get_camera_terminal(handle_);
    terminalId_ = input_terminal->bTerminalID;

    res = uvc_get_stream_ctrl_format_size(
        handle_, &ctrl_, frameFormat_, width_, height_, fps_);
    if (res != UVC_SUCCESS) {
        throw std::runtime_error("device doesn't provide a matching stream");
    }

    status_ = Status::NOT_STARTED;
    frameCounter_ = 0;
}

void UsbCamera::exit_()
{
    if (status_ == Status::UNINITIALIZED) {
        return;
    }

    if (status_ == Status::STARTING) {
        stop();
    }
    if (status_ == Status::NOT_STARTED) {
        uvc_close(handle_);
        uvc_unref_device(device_);
        for (int i = 0; i < 2; i++) {
            uvc_free_frame(frameRgb_[i]);
            frameRgb_[i] = nullptr;
        }
        uvc_free_frame(outputFrameRgb_);
    }

    uvc_exit(context_);

    status_ = Status::UNINITIALIZED;
}

void UsbCamera::refresh()
{
    exit_();
    init_();
}

void UsbCamera::start()
{
    if (status_ == Status::STARTING) {
        return;
    }

    if (status_ != Status::NOT_STARTED) {
        throw std::runtime_error("camera has not initialized or found");
    }

    uvc_error_t res = uvc_start_streaming(handle_, &ctrl_, CameraFrameCallback_, this, 0);
    if (res != UVC_SUCCESS) {
        throw std::runtime_error("uvc can not start streaming");
    }

    // uvc_set_ae_mode(handle_, 1); /* e.g., turn on auto exposure */

    status_ = Status::STARTING;
}

void UsbCamera::stop()
{
    if (status_ != Status::STARTING) {
        return;
    }

    uvc_stop_streaming(handle_);
    status_ = Status::NOT_STARTED;
}

void UsbCamera::CameraFrameCallback_(uvc_frame* frame, void* camera)
{
    UsbCamera* cam = (UsbCamera*)camera;
    if (cam->frameCounter_++ == 0) {
        return;
    }
    int anotherFrameRgb = cam->currentFrameRgb_ ^ 1;
    uvc_error_t ret;
    if (frame->frame_format == UVC_FRAME_FORMAT_MJPEG) {
#ifdef LIBUVC_HAS_JPEG
        ret = uvc_mjpeg2rgb(frame, cam->frameRgb_[anotherFrameRgb]);
#else
        throw std::runtime_error("need libjpeg!");
#endif
    }
    else {
        ret = uvc_any2rgb(frame, cam->frameRgb_[anotherFrameRgb]);
    }

    if (ret != UVC_SUCCESS) {
        return;
    }

    cam->currentFrameRgb_mutex_.lock();
    cam->currentFrameRgb_ = anotherFrameRgb;
    cam->currentFrameRgb_mutex_.unlock();
}

const void* UsbCamera::fetchFrameRgbData()
{
    if (status_ != Status::STARTING) {
        throw std::runtime_error("fetch frame but camera is not starting");
    }

    currentFrameRgb_mutex_.lock_shared();
    uvc_duplicate_frame(frameRgb_[currentFrameRgb_], outputFrameRgb_);
    currentFrameRgb_mutex_.unlock_shared();
    return outputFrameRgb_;
}

long long UsbCamera::getFrameCounter()
{
    return frameCounter_;
}

UsbCamera::Info UsbCamera::getRecentInfo() const
{
    if (status_ == Status::UNINITIALIZED ||
        status_ == Status::NOT_FOUND) {
        throw std::runtime_error("get recent info but camera is not initialized or found");
    }
    return {
        width_,
        height_,
        fps_,
        terminalId_
    };
}

UsbCamera::Status UsbCamera::getStatus() const
{
    return status_;
}


} // namespace mixi

