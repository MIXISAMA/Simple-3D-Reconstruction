#ifndef _MY_USB_CAMERA_H
#define _MY_USB_CAMERA_H

#include "pch.h"
#include <libuvc/libuvc.h>

class MyUsbCamera
{
public:
    struct Info {
        int width;
        int height;
        int fps;
        int terminalId;
    };
    using usbCam_ptr = std::shared_ptr<MyUsbCamera>;
    ~MyUsbCamera();
    MyUsbCamera(MyUsbCamera&)=delete;
    MyUsbCamera& operator=(const MyUsbCamera&)=delete;
    static usbCam_ptr Instance();
    void refresh();
    void start();
    void stop();
    void loadFrameRgb(GLuint* frameTexture);
    Info getRecentInfo() const;
    bool isStarting() const;
    bool isFound() const;
private:
    MyUsbCamera();
    void init();
    void exit();
    uvc_context_t* context_;
    uvc_device_t* device_;
    uvc_device_handle_t* handle_;
    uvc_stream_ctrl_t ctrl_;
    uvc_frame_t* frameRgb_[2];
    int currentFrameRgb_;
    std::shared_mutex currentFrameRgb_mutex_;
    enum uvc_frame_format frameFormat_;
    int width_;
    int height_;
    int fps_;
    int terminalId_;
    bool isStarting_;
    bool isFound_;
    bool isExited_;
    long long int frameCounter_;
    static usbCam_ptr Instance_;
    static std::mutex Instance_mutex_;
    static void CameraFrameCallback(uvc_frame* frame, void* camera);
};

#endif
