//
// ZGExternalVideoCaptureDemo.h
//
// Copyright 2018年 Zego. All rights reserved.
// 
#ifndef ZGExternalVideoCaptureDemo_h__
#define ZGExternalVideoCaptureDemo_h__

#include <mutex>

#include "ZegoLiveRoom/video_capture.h"

using namespace AVE;

class ZGExternalVideoCaptureDemo : 
    public VideoCaptureDevice, 
    public VideoCaptureFactory
{
public:

    ZGExternalVideoCaptureDemo();
    ~ZGExternalVideoCaptureDemo();

    // 开启外部采集
    void EnableExternalVideoCapture();

    // 把外部采集到的数据推给sdk
    void PushExternalVideoData(const char* data, int len, VideoCaptureFormat format, unsigned long long reference_time);

protected:
    virtual void AllocateAndStart(Client* client) override;
    virtual void StopAndDeAllocate() override;
    virtual int StartCapture() override;
    virtual int StopCapture() override;
    virtual int SetFrameRate(int framerate) override;
    virtual int SetResolution(int width, int height) override;
    virtual int SetFrontCam(int bFront) override;
    virtual int SetView(void *view) override;
    virtual int SetViewMode(int nMode) override;
    virtual int SetViewRotation(int nRotation) override;
    virtual int SetCaptureRotation(int nRotation) override;
    virtual int StartPreview() override;
    virtual int StopPreview() override;
    virtual int EnableTorch(bool bEnable) override;
    virtual int TakeSnapshot() override;
    virtual VideoCaptureDeviceBase* Create(const char* device_id) override;
    virtual void Destroy(VideoCaptureDeviceBase *vc) override;

protected:

    std::mutex capture_mutex_;
    bool capture_started_ = false;
    Client * client_ = nullptr;

    int width_ = 0;
    int height_ = 0;
};


#endif // ZGExternalVideoCaptureDemo_h__




