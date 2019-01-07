//
// ZegoCamera.h
//
// Copyright 2018�� Zego. All rights reserved.
// 

#ifndef ZegoCamera_h__
#define ZegoCamera_h__

#include <string>
#include <functional>

#include "ZegoLiveRoom/zego-api-defines.h"
#include "ZegoLiveRoom/video_capture.h"

namespace ZEGO
{
    namespace CAMERA
    {
        ZEGOAVKIT_API AVE::Camera* CreateCamera();

        ZEGOAVKIT_API void DestroyCamera(AVE::Camera *pCamera);
    }
}

using OnCaptureDeviceErrorCallBackType = std::function<void(const char *deviceId, const char *reason)>;
using OnVideoFrameDataCallBackType = std::function< void (const char* ,int ,const AVE::VideoCaptureFormat& ,unsigned long long , unsigned int)>;

/*

���ò���˵��

1. control camera
    - select device
    - set fps
    - set resolution
    - start cap
    - stop cap

2. callback
    - video frame data
    - device error
*/

class ZegoVideoCapExternal
{
public:
    ZegoVideoCapExternal() {}
    ~ZegoVideoCapExternal() {}

public:
    void SetVideoFrameCallBack(OnVideoFrameDataCallBackType d) {
        on_video_frame_delegate_ = d;
    }

    void SetCaptureDeviceErrorCallBack(OnCaptureDeviceErrorCallBackType d) {
        on_device_error_delegate_ = d;
    }

    std::string GetDeviceId();

    int SetCamera(std::string deviceId);
    int RemoveCamera();

    int StartCapture();
    int StopCapture();

    int SetFrameRate(int framerate);

    int SetCameraResolution(int width, int height);

public:
    void OnCaptureDeviceError(const std::string& deviceId, const char* reason);
    void OnIncommingCaptureData(const std::string& deviceId,
        const char* data,
        int length,
        const AVE::VideoCaptureFormat& frame_format,
        unsigned long long reference_time,
        unsigned int reference_time_scale);
    void OnTakeSnapshot(void* image);

private:
    struct CameraDeviceInfo
    {
        CameraDeviceInfo()
        {
            Reset();
        }

        ~CameraDeviceInfo()
        {
            Reset();
        }

        void Reset()
        {
            cameraDevice = nullptr;
            isCapture = false;
        }

        AVE::Camera *cameraDevice;
        bool isCapture;
    };

private:
    OnVideoFrameDataCallBackType on_video_frame_delegate_;
    OnCaptureDeviceErrorCallBackType on_device_error_delegate_;
    std::string m_deviceId;
    CameraDeviceInfo m_deviceInfo;
};


#endif // ZegoCamera_h__
