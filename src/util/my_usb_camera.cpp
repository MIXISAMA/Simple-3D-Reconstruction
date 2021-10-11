#include "util/my_usb_camera.h"

MyUsbCamera::MyUsbCamera() :
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
    isStarting_(false),
    isFound_(false),
    isExited_(true),
    frameCounter_(0)
{

}

MyUsbCamera::~MyUsbCamera()
{
    exit();
}

MyUsbCamera::usbCam_ptr MyUsbCamera::Instance_ = nullptr;
std::mutex MyUsbCamera::Instance_mutex_;

MyUsbCamera::usbCam_ptr MyUsbCamera::Instance()
{
    if(Instance_ == nullptr){
        std::lock_guard<std::mutex> lk(Instance_mutex_);
        if(Instance_ == nullptr){
            Instance_ = std::shared_ptr<MyUsbCamera>(new MyUsbCamera());
        }
    }
    return Instance_;
}

void MyUsbCamera::init()
{
    stop();
    exit();

    uvc_error_t res;

    res = uvc_init(&context_, nullptr);
    if (res != UVC_SUCCESS) {
        throw std::runtime_error("uvc init error!");
    }

    res = uvc_find_device(context_, &device_, 0, 0, NULL);
    if (res != UVC_SUCCESS) {
        isFound_ = false;
        return;
    }
    isFound_ = true;

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

    const uvc_input_terminal_t* input_terminal = uvc_get_camera_terminal(handle_);
    terminalId_ = input_terminal->bTerminalID;

    isExited_ = false;
    frameCounter_ = 0;
}

void MyUsbCamera::exit()
{
    if (isExited_) {
        return;
    }

    uvc_close(handle_);
    uvc_unref_device(device_);
    uvc_exit(context_);
    for (int i = 0; i < 2; i++) {
        uvc_free_frame(frameRgb_[i]);
        frameRgb_[i] = nullptr;
    }

    isExited_ = true;
}

void MyUsbCamera::refresh()
{
    init();
    exit();
}

void MyUsbCamera::start()
{
    init();
    if (!isFound_) {
        return;
    }

    uvc_error_t res = uvc_get_stream_ctrl_format_size(
        handle_, &ctrl_, frameFormat_, width_, height_, fps_);
    if (res != UVC_SUCCESS) {
        throw std::runtime_error("device doesn't provide a matching stream");
    }

    res = uvc_start_streaming(handle_, &ctrl_, CameraFrameCallback, this, 0);
    if (res != UVC_SUCCESS) {
        throw std::runtime_error("uvc can not start streaming");
    }

    uvc_set_ae_mode(handle_, 1); /* e.g., turn on auto exposure */

    isStarting_ = true;
}

void MyUsbCamera::stop()
{
    if (!isStarting_) {
        return;
    }
    uvc_stop_streaming(handle_);
    isStarting_ = false;
    exit();
}

void MyUsbCamera::CameraFrameCallback(uvc_frame* frame, void* camera)
{
    MyUsbCamera* cam = (MyUsbCamera*)camera;
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

void MyUsbCamera::loadFrameRgb(GLuint* frameTexture)
{
    if (!isStarting_) {
        // Todo: default image
        return;
    }

    if (frameCounter_ <= 2) {
        return;
    }

    glBindTexture(GL_TEXTURE_2D, *frameTexture);

#if defined(GL_UNPACK_ROW_LENGTH) && !defined(__EMSCRIPTEN__)
    glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
#endif

    currentFrameRgb_mutex_.lock_shared();
    uvc_frame_t* frameRgb = frameRgb_[currentFrameRgb_];
    if (frameRgb != nullptr) {
        glTexImage2D(
            GL_TEXTURE_2D, 0, GL_RGB,
            frameRgb->width, frameRgb->height, 0,
            GL_RGB, GL_UNSIGNED_BYTE, frameRgb->data
        );
    }
    currentFrameRgb_mutex_.unlock_shared();

    glBindTexture(GL_TEXTURE_2D, 0);
}

MyUsbCamera::Info MyUsbCamera::getRecentInfo() const
{
    return {
        width_,
        height_,
        fps_,
        terminalId_
    };
}

bool MyUsbCamera::isStarting() const
{
    return isStarting_;
}

bool MyUsbCamera::isFound() const
{
    return isFound_;
}
