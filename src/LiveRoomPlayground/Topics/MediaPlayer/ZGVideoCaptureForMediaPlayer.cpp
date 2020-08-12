#include "stdafx.h"
#include "ZGVideoCaptureForMediaPlayer.h"
#include "AppSupport/ZGLog.h"

ZGVideoCaptureForMediaPlayer::ZGVideoCaptureForMediaPlayer()
    :client_(nullptr),
     capture_started_(false)
{
}

ZGVideoCaptureForMediaPlayer::~ZGVideoCaptureForMediaPlayer()
{
}

void ZGVideoCaptureForMediaPlayer::AllocateAndStart(Client* client)
{
    ZGENTER_FUN_LOG;
    client_ = client;
}

void ZGVideoCaptureForMediaPlayer::StopAndDeAllocate()
{
    ZGENTER_FUN_LOG;
    client_->Destroy();
    client_ = nullptr;
}

AVE::VideoCaptureDeviceBase* ZGVideoCaptureForMediaPlayer::Create(const char* device_id)
{
    ZGENTER_FUN_LOG;
    return this;
}


int ZGVideoCaptureForMediaPlayer::StartCapture()
{
    ZGENTER_FUN_LOG;
    std::lock_guard<std::mutex> lock(capture_mutex_);
    capture_started_ = true;
    return 0;
}

int ZGVideoCaptureForMediaPlayer::StopCapture()
{
    ZGENTER_FUN_LOG;
    std::lock_guard<std::mutex> lock(capture_mutex_);
    capture_started_ = false;
    return 0;
}


void ZGVideoCaptureForMediaPlayer::OnPlayVideoData(const char* data, int len, ZegoMediaPlayerVideoDataFormat& format, ZegoMediaPlayerIndex index)
{
    //ZGENTER_FUN_LOG;
    {
        std::lock_guard<std::mutex> lock(capture_mutex_);
        if (capture_started_)
        {
            VideoCaptureFormat capture_fmt;
            capture_fmt.width = format.width;
            capture_fmt.height = format.height;
            capture_fmt.pixel_format = AVE::PIXEL_FORMAT_RGBA32;
            capture_fmt.strides[0] = format.width * 4;
            capture_fmt.rotation = 0;

            client_->OnIncomingCapturedData(data, len, capture_fmt, GetTickCount(), 1000);
        }
    }
}

void ZGVideoCaptureForMediaPlayer::Destroy(VideoCaptureDeviceBase *vc)
{
    ZGENTER_FUN_LOG;
}


int ZGVideoCaptureForMediaPlayer::SetFrameRate(int framerate)
{
    ZGENTER_FUN_LOG;
    return 0;
}

int ZGVideoCaptureForMediaPlayer::SetResolution(int width, int height)
{
    ZGENTER_FUN_LOG;
    return 0;
}

int ZGVideoCaptureForMediaPlayer::SetFrontCam(int bFront)
{
    ZGENTER_FUN_LOG;
    return 0;
}

int ZGVideoCaptureForMediaPlayer::SetView(void *view)
{
    ZGENTER_FUN_LOG;
    return 0;
}

int ZGVideoCaptureForMediaPlayer::SetViewMode(int nMode)
{
    ZGENTER_FUN_LOG;
    return 0;
}

int ZGVideoCaptureForMediaPlayer::SetViewRotation(int nRotation)
{
    ZGENTER_FUN_LOG;
    return 0;
}

int ZGVideoCaptureForMediaPlayer::SetCaptureRotation(int nRotation)
{
    ZGENTER_FUN_LOG;
    return 0;
}

int ZGVideoCaptureForMediaPlayer::StartPreview()
{
    ZGENTER_FUN_LOG;
    return 0;
}

int ZGVideoCaptureForMediaPlayer::StopPreview()
{
    ZGENTER_FUN_LOG;
    return 0;
}

int ZGVideoCaptureForMediaPlayer::EnableTorch(bool bEnable)
{
    ZGENTER_FUN_LOG;
    return 0;
}

int ZGVideoCaptureForMediaPlayer::TakeSnapshot()
{
    ZGENTER_FUN_LOG;
    return 0;
}

