#pragma once

#include "pch.h"
#include <libuvc/libuvc.h>
#include "render/texture.h"
#include "util/singleton.h"

namespace mixi
{

class UsbCamera
{
public:
    
    using Ptr = std::shared_ptr<UsbCamera>;

    UsbCamera();
    ~UsbCamera();

    struct Info {
        int width;
        int height;
        int fps;
        int terminalId;
    };

    enum class Status
    {
        UNINITIALIZED,
        NOT_FOUND,
        NOT_STARTED,
        STARTING,
    };
    
    void refresh();
    void start();
    void stop();
    const uvc_frame_t* fetchFrameRgb();
    long long getFrameCounter();
    Info getRecentInfo() const;
    Status getStatus() const;

private:

    void init_();
    void exit_();

    uvc_context_t* context_;
    uvc_device_t* device_;
    uvc_device_handle_t* handle_;
    uvc_stream_ctrl_t ctrl_;
    uvc_frame_format frameFormat_;

    uvc_frame_t* outputFrameRgb_;
    uvc_frame_t* frameRgb_[2];
    int currentFrameRgb_;
    std::shared_mutex currentFrameRgb_mutex_;

    int width_;
    int height_;
    int fps_;
    int terminalId_;
    Status status_;
    std::atomic<long long> frameCounter_;

    static void CameraFrameCallback_(uvc_frame* frame, void* camera);

};

}

