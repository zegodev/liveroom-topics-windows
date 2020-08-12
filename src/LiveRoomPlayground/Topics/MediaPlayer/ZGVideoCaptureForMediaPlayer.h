//
// ZGVideoCaptureForMediaPlayer.h
//
// Copyright 2018Äê Zego. All rights reserved.
//

#ifndef ZGVideoCaptureForMediaPlayer_h__
#define ZGVideoCaptureForMediaPlayer_h__

#include <mutex>

#include "ZegoLiveRoom/zego-api-mediaplayer.h"
#include "ZegoLiveRoom/video_capture.h"

using namespace ZEGO;

using AVE::VideoCaptureFactory;
using AVE::VideoCaptureDeviceBase;
using AVE::VideoCaptureDevice;
using AVE::IMediaPlayer;
using AVE::VideoDataFormat;
using AVE::VideoCaptureFormat;

using MEDIAPLAYER::ZegoMediaPlayerVideoDataFormat;
using MEDIAPLAYER::ZegoMediaPlayerIndex;

class ZGVideoCaptureForMediaPlayer :
    public VideoCaptureDevice,
    public VideoCaptureFactory,
    public MEDIAPLAYER::IZegoMediaPlayerVideoDataWithIndexCallback
{
public:
    ZGVideoCaptureForMediaPlayer();
    ~ZGVideoCaptureForMediaPlayer();

    virtual VideoCaptureDeviceBase* Create(const char* device_id) override;
    virtual void Destroy(VideoCaptureDeviceBase *vc) override;

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

    virtual void AllocateAndStart(Client* client) override;
    virtual void StopAndDeAllocate() override;
    virtual int StartCapture() override;
    virtual int StopCapture() override;

    virtual void OnPlayVideoData(const char* data, int len, ZegoMediaPlayerVideoDataFormat& format, ZegoMediaPlayerIndex index) override;

private:
    std::mutex capture_mutex_;
    bool capture_started_;
    Client * client_;

};


#endif // ZGVideoCaptureForMediaPlayer_h__

