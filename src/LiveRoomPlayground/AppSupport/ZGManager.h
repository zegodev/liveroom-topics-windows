//
// ZGManager.h
//
// Copyright 2018年 Zego. All rights reserved.
// 

#ifndef ZGManager_h__
#define ZGManager_h__

#include <stdint.h>
#include <vector>
#include <string>
#include <functional>

#include "ZegoLiveRoom/LiveRoom.h"
#include "ZegoLiveRoom/LiveRoom-Player.h"
#include "ZegoLiveRoom/LiveRoom-Publisher.h"
#include "ZegoLiveRoom/zego-api-external-video-capture.h"
#include "ZegoLiveRoom/video_capture.h"

#include "ZGTaskQueue.h"

using namespace ZEGO;

typedef struct RoomInfo
{
    std::string room_id;
    std::string room_name;
    std::string anchor_id;
    std::string anchor_name;

}RoomInfo;

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
    bool SdkIsInited();

    void GetRoomList(bool is_test_env, std::function<void(std::vector<RoomInfo> room_list)> room_list_cb);
    std::vector<RoomInfo> GetRoomList(unsigned int app_id, bool test_env);
private:

    
    ZGManager();
    ~ZGManager();

    bool sdk_have_inited_;

    // 异步线程执行任务队列
    APPSUPPORT::TaskQueue async_task_queue_;
};

#define ZGManagerInstance ZGManager::Instance

#endif // ZGManager_h__

