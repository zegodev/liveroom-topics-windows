#include "stdafx.h"
#include "ZGExternalVideoCaptureDemo.h"

#include "ZegoLiveRoom/LiveRoom-Publisher.h"
#include "ZegoLiveRoom/zego-api-external-video-capture.h"
#include "ZegoLiveRoom/zego-api-external-audio-device.h"
#include "ZegoLiveRoom/zego-api-audio-frame.h"

#include "AppSupport/ZGManager.h"
#include "AppSupport/ZGLog.h"

using namespace ZEGO;

ZGExternalVideoCaptureDemo::ZGExternalVideoCaptureDemo()
{
    ZGENTER_FUN_LOG;
}

ZGExternalVideoCaptureDemo::~ZGExternalVideoCaptureDemo()
{
    ZGENTER_FUN_LOG;
}

void ZGExternalVideoCaptureDemo::EnableExternalVideoCapture()
{
    ZGENTER_FUN_LOG;

    // 在InitSDK 之前调用
    LIVEROOM::SetVideoCaptureFactory(this);
    LIVEROOM::SetUseTestEnv(TRUE);
    ZGManagerInstance()->InitSdk();
    // 开麦克风
    //LIVEROOM::EnableMic(true);

    ZGLEAVE_FUN_LOG;
}

void ZGExternalVideoCaptureDemo::PushExternalVideoData(const char* data, int len, VideoCaptureFormat format, unsigned long long reference_time)
{
    //ZGENTER_FUN_LOG;
    {
        std::lock_guard<std::mutex> lock(capture_mutex_);
        if (capture_started_)
        {
            if (width_ != format.width || height_ != format.height)
            {
                width_ = format.width;
                height_ = format.height;

                ZGLog("SetVideoCaptureResolution and SetVideoEncodeResolution,w=%d h=%d",width_,height_);
                // 设置采集分辨率
                LIVEROOM::SetVideoCaptureResolution(width_, height_);
                // 设置编码分辨率
                LIVEROOM::SetVideoEncodeResolution(width_, height_);
            }

            //ZGLog("push external video data to sdk , data len = %d", len);
            if (client_ != nullptr)
            {
                client_->OnIncomingCapturedData(data, len, format, reference_time, 1000);
            }            
        }
    }
    //ZGLEAVE_FUN_LOG;
}

void ZGExternalVideoCaptureDemo::AllocateAndStart(Client* client)
{
    ZGENTER_FUN_LOG;
    client_ = client;
    ZGLEAVE_FUN_LOG;
}

void ZGExternalVideoCaptureDemo::StopAndDeAllocate()
{
    ZGENTER_FUN_LOG;
    client_->Destroy();
    client_ = nullptr;
    ZGLEAVE_FUN_LOG;
}

int ZGExternalVideoCaptureDemo::StartCapture()
{
    ZGENTER_FUN_LOG;

    std::lock_guard<std::mutex> lock(capture_mutex_);
    capture_started_ = true;
    width_ = 0;
    height_ = 0;
    
    ZGLEAVE_FUN_LOG;
    return 0;
}

int ZGExternalVideoCaptureDemo::StopCapture()
{
    ZGENTER_FUN_LOG;

    std::lock_guard<std::mutex> lock(capture_mutex_);
    capture_started_ = false;
    ZGLEAVE_FUN_LOG;
    return 0;
}

int ZGExternalVideoCaptureDemo::SetFrameRate(int framerate)
{
    ZGENTER_FUN_LOG;
    return 0;
}

int ZGExternalVideoCaptureDemo::SetResolution(int width, int height)
{
    ZGENTER_FUN_LOG;
    

    return 0;
}

int ZGExternalVideoCaptureDemo::SetFrontCam(int bFront)
{
    ZGENTER_FUN_LOG;
    return 0;
}

int ZGExternalVideoCaptureDemo::SetView(void *view)
{
    ZGENTER_FUN_LOG;
    return 0;
}

int ZGExternalVideoCaptureDemo::SetViewMode(int nMode)
{
    ZGENTER_FUN_LOG;
    return 0;
}

int ZGExternalVideoCaptureDemo::SetViewRotation(int nRotation)
{
    ZGENTER_FUN_LOG;
    return 0;
}

int ZGExternalVideoCaptureDemo::SetCaptureRotation(int nRotation)
{
    ZGENTER_FUN_LOG;
    return 0;
}

int ZGExternalVideoCaptureDemo::StartPreview()
{
    ZGENTER_FUN_LOG;
    return 0;
}

int ZGExternalVideoCaptureDemo::StopPreview()
{
    ZGENTER_FUN_LOG;
    return 0;
}

int ZGExternalVideoCaptureDemo::EnableTorch(bool bEnable)
{
    ZGENTER_FUN_LOG;
    return 0;
}

int ZGExternalVideoCaptureDemo::TakeSnapshot()
{
    ZGENTER_FUN_LOG;
    return 0;
}

AVE::VideoCaptureDeviceBase* ZGExternalVideoCaptureDemo::Create(const char* device_id)
{
    ZGENTER_FUN_LOG;

    return this;
}

void ZGExternalVideoCaptureDemo::Destroy(VideoCaptureDeviceBase *vc)
{
    ZGENTER_FUN_LOG;
     
}

