//
// ZGExternalVideoCatpureDemoHelper.h
//
// Copyright 2018年 Zego. All rights reserved.
// 

#ifndef ZGExternalVideoCatpureDemoHelper_h__
#define ZGExternalVideoCatpureDemoHelper_h__

#include <string>
#include <vector>
#include <functional>

#include "ZegoLiveRoom/LiveRoomCallback-Publisher.h"
#include "ZegoLiveRoom/LiveRoom.h"
#include "ZegoLiveRoom/LiveRoom-Player.h"
#include "ZegoLiveRoom/LiveRoomCallback-Player.h"

using namespace ZEGO;
using std::string;
using std::vector;

using COMMON::ZegoPublishingStreamInfo;
using COMMON::ZegoStreamInfo;
using COMMON::ZegoStreamUpdateType;

enum ZGExternalVideoCaptureStatus
{
    kZGExternalVideoCaptureStatus_None = 0,
    kZGExternalVideoCaptureStatus_Starting_Login_Room,
    kZGExternalVideoCaptureStatus_Login_OK,
    kZGExternalVideoCaptureStatus_Starting_Publishing,
    kZGExternalVideoCaptureStatus_Starting_Playing,
    kZGExternalVideoCaptureStatus_Playing

};

// 登录推拉流的状态回调函数类型声明
typedef std::function<void(string status)> ZGExternalVideoCatpureDemoHelperStatusCallBackType;
// 初始化回调函数类型声明
typedef std::function<void(int)> ZGInitSdkCallBackType;

class ZGExternalVideoCatpureDemoHelper : 
    public LIVEROOM::ILivePublisherCallback,
    public LIVEROOM::IRoomCallback,
    public LIVEROOM::ILivePlayerCallback
{
public:
    ZGExternalVideoCatpureDemoHelper();
    ~ZGExternalVideoCatpureDemoHelper();

    ZGExternalVideoCatpureDemoHelperStatusCallBackType status_cb_;

    // 登录并推流
    void LoginRoomAndPublishing();

    // 停止推流，并退出登录，反初始化sdk
    void Stop();

    // 初始化远端播放的视图
    void InitRemoteVideoView(void * remote_video_view);

    // 状态字符串
    string DescOfStatus(ZGExternalVideoCaptureStatus status);
    
    // 摄像头id列表
    vector<string> GetCameraDeviceIDList();
    
    // 摄像头name列表
    vector<string> GetCameraDeviceNameList();
    
    // 设置sdk初始化回调通知
    void SetInitSdkCallBack(ZGInitSdkCallBackType cb);

    // 设置码率
    void SetBitrate(int bitrate);

    void InitMainHwnd(HWND hwnd);

protected:
    virtual void OnPublishQualityUpdate(const char* pszStreamID, LIVEROOM::ZegoPublishQuality publishQuality) override;
    virtual void OnPlayQualityUpdate(const char* pszStreamID, LIVEROOM::ZegoPlayQuality playQuality) override;
    virtual void OnInitSDK(int nError) override;
    virtual void OnLoginRoom(int errorCode, const char *pszRoomID, const ZegoStreamInfo *pStreamInfo, unsigned int streamCount) override;
    virtual void OnKickOut(int reason, const char *pszRoomID) override;
    virtual void OnDisconnect(int errorCode, const char *pszRoomID) override;
    virtual void OnStreamUpdated(ZegoStreamUpdateType type, ZegoStreamInfo *pStreamInfo, unsigned int streamCount, const char *pszRoomID) override;
    virtual void OnStreamExtraInfoUpdated(ZegoStreamInfo *pStreamInfo, unsigned int streamCount, const char *pszRoomID) override;
    virtual void OnCustomCommand(int errorCode, int requestSeq, const char *pszRoomID) override;
    virtual void OnRecvCustomCommand(const char *pszUserId, const char *pszUserName, const char *pszContent, const char *pszRoomID) override;
    virtual void OnPublishStateUpdate(int stateCode, const char* pszStreamID, const ZegoPublishingStreamInfo& oStreamInfo) override;
    virtual void OnPlayStateUpdate(int stateCode, const char* pszStreamID) override;
private:

    void SetCurStatus(ZGExternalVideoCaptureStatus s);

    // 设备唯一id，用于用户名登录、房间id、房间名字、推流名称
    string device_uuid_ = "";
    
    // 远端拉流播放的视图
    void * remote_video_view_ = nullptr;

    // 当前状态
    ZGExternalVideoCaptureStatus cur_status_;

    // 初始化sdk回调
    ZGInitSdkCallBackType init_sdk_cb_;

    HWND main_hwnd_ = nullptr;

};




#endif // ZGExternalVideoCatpureDemoHelper_h__

