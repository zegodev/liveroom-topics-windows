//
// ZGManager.h
//
// Copyright 2018Äê Zego. All rights reserved.
// 

#ifndef ZGManager_h__
#define ZGManager_h__

#include <stdint.h>
#include <vector>

#include "ZegoLiveRoom/LiveRoom.h"
#include "ZegoLiveRoom/LiveRoom-Player.h"
#include "ZegoLiveRoom/LiveRoom-Publisher.h"
#include "ZegoLiveRoom/zego-api-external-video-capture.h"
#include "ZegoLiveRoom/video_capture.h"

using namespace ZEGO;

class ZGManager
{
public:
    static ZGManager* Instance()
    {
        static ZGManager z;
        return &z;
    }
    void EnableExternalVideoCapture(AVE::VideoCaptureFactory* factory, LIVEROOM::IZegoVideoRenderCallback* randerer);
    void EnableMic(bool enableMic);
    bool InitSdk(unsigned int uiAppID, unsigned char* sign, int len);
    bool InitSdk();
    void UninitSdk();

private:

    
    ZGManager();
    ~ZGManager();

    bool sdk_have_inited_;
};

#define ZGManagerInstance ZGManager::Instance

#endif // ZGManager_h__

