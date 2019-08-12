#pragma once

#include <vector>
#include <string>
#include <functional>
#include <map>
#include <mutex>

#include "ZegoLiveRoom/LiveRoom.h"
#include "ZegoLiveRoom/LiveRoomCallback-Publisher.h"
#include "ZegoLiveRoom/LiveRoomCallBack.h"
#include "ZegoLiveRoom/LiveRoom-Player.h"
#include "ZegoLiveRoom/LiveRoomCallback-Player.h"

#include "AppSupport/ZGLog.h"
#include "AppSupport/ZGUIOperator.h"

#include <windows.h>

using std::string;
using std::map;

using namespace ZEGO;
using ZEGO::COMMON::ZegoPublishingStreamInfo;
using LIVEROOM::ZegoPublishQuality;
using COMMON::ZegoStreamUpdateType;
using LIVEROOM::ZegoPlayQuality;

typedef struct ZegoJoinLiveUser
{
    string user_id;
    string user_name;
    bool operator < (ZegoJoinLiveUser const& z) const
    {
        if (user_id < z.user_id) {
            return true;
        }
        return false;
    }
    ZegoJoinLiveUser()
    {
        user_id = "";
        user_name = "";
    }

}ZegoJoinLiveUser;

enum ZGJoinLiveRoomStatus
{
    kZGJoinLiveStatus_None = 0,             // 未开始初始状态
    kZGJoinLiveStatus_Starting_Init_SDK,    // 开始初始化sdk
    kZGJoinLiveStatus_Init_SDK_OK,          // sdk初始化成功
    kZGJoinLiveStatus_Starting_Login_Room,  // 开始登陆房间
    kZGJoinLiveStatus_Login_OK,             // 登陆房间成功
    kZGJoinLiveStatus_Starting_Publishing,  // 开始推流 
    kZGJoinLiveStatus_Publishing,           // 推流成功
    kZGJoinLiveStatus_Stoped_Publishing     // 停止推流
};

// 房间登录事件
enum ZGJoinLiveLoginEvent {
    kZGJoinLiveLoginEventLogin,           // 开始登录请求
    kZGJoinLiveLoginEventLoginSuccess,    // 登录请求成功
    kZGJoinLiveLoginEventLoginFailed,     // 登录请求失败
    kZGJoinLiveLoginEventLogout,          // 退出登录
    kZGJoinLiveLoginEventTempBroke,       // 房间连接暂时断开
    kZGJoinLiveLoginEventReconnect,       // 房间连接恢复
    kZGJoinLiveLoginEventDisconnect,      // 重连超时，停止重连断开房间连接
    kZGJoinLiveLoginEventKickOut,         // 被踢出房间，断开房间连接
};

// 房间登录状态
enum ZGJoinLiveStatus
{
    kZGJoinLiveLoginStatusLogout,     // 未登录
    kZGJoinLiveLoginStatusStartLogin, // 开始登录请求
    kZGJoinLiveLoginStatusLogin,      // 已登录
    kZGJoinLiveLoginStatusTempBroken, // 暂时断开房间连接
};

// 房间重连终止原因
enum ZGJoinLiveReconnectStopReason {
    kZGJoinLiveReconnectStopReasonLogout, // 手动调用leaveRoom终止重连
    kZGJoinLiveReconnectStopReasonKickout,// 被服务器踢出终止重连
    kZGJoinLiveReconnectStopReasonTimeout,// 重连timeout
};

class JoinLiveCallBack
{
public:
    virtual void OnLoginEvent(ZGJoinLiveLoginEvent event, ZGJoinLiveStatus status) = 0;
    virtual void OnUserJoin(ZegoJoinLiveUser user) {}
    virtual void OnUserLeave(ZegoJoinLiveUser user) {}
};


class JoinLiveDemo :
    public LIVEROOM::ILivePublisherCallback,
    public LIVEROOM::ILivePlayerCallback,
    public LIVEROOM::IRoomCallback,
    public AV::IZegoDeviceStateCallback
{
public:

    JoinLiveDemo();
    ~JoinLiveDemo();

    void SetDemoCallBack(JoinLiveCallBack * cb);

    void SetLocalVideoView(HWND view, LIVEROOM::ZegoVideoViewMode view_mode = LIVEROOM::ZegoVideoViewModeScaleAspectFit);

    void SetRemoteVideoView(HWND view, ZegoJoinLiveUser for_user, LIVEROOM::ZegoVideoViewMode view_mode = LIVEROOM::ZegoVideoViewModeScaleAspectFit);

    void LoginRoom(string room_id, bool need_publish_after_logined = false);

    void StartPublish();

    void LogoutRoom();

    // 保存主窗口的句柄，为了切线程使用
    void InitMainHwnd(HWND hwnd);

    void SetLogCallBack(ILogToView *log_cb);

protected:
    
    void PrintLogToView(const char * format, ...);

    virtual void OnPlayStateUpdate(int stateCode, const char* pszStreamID) override;


    virtual void OnPlayQualityUpdate(const char* pszStreamID, ZegoPlayQuality playQuality) override;


    virtual void OnVideoSizeChanged(const char* pStreamID, int nWidth, int nHeight) override;


    virtual void OnAudioDeviceStateChanged(AV::AudioDeviceType deviceType, AV::DeviceInfo *deviceInfo, AV::DeviceState state) override;


    virtual void OnDeviceError(const char* deviceName, int errorCode) override;


    virtual void OnInitSDK(int nError) override;


    virtual void OnLoginRoom(int errorCode, const char *pszRoomID, const COMMON::ZegoStreamInfo *pStreamInfo, unsigned int streamCount) override;


    virtual void OnLogoutRoom(int errorCode, const char *pszRoomID) override;


    virtual void OnKickOut(int reason, const char *pszRoomID) override;


    virtual void OnDisconnect(int errorCode, const char *pszRoomID) override;


    virtual void OnReconnect(int errorCode, const char *pszRoomID) override;


    virtual void OnTempBroken(int errorCode, const char *pszRoomID) override;


    virtual void OnStreamUpdated(ZegoStreamUpdateType type, COMMON::ZegoStreamInfo *pStreamInfo, unsigned int streamCount, const char *pszRoomID) override;


    virtual void OnStreamExtraInfoUpdated(COMMON::ZegoStreamInfo *pStreamInfo, unsigned int streamCount, const char *pszRoomID) override;


    virtual void OnCustomCommand(int errorCode, int requestSeq, const char *pszRoomID) override;


    virtual void OnRecvCustomCommand(const char *pszUserId, const char *pszUserName, const char *pszContent, const char *pszRoomID) override;


    virtual void OnPublishStateUpdate(int stateCode, const char* pszStreamID, const ZegoPublishingStreamInfo& oStreamInfo) override;


    virtual void OnPublishQualityUpdate(const char* pszStreamID, ZegoPublishQuality publishQuality) override;


    virtual void OnCaptureVideoSizeChanged(int nWidth, int nHeight) override;


    virtual void OnCaptureVideoFirstFrame() override;


protected:

    HWND main_hwnd_ = nullptr;
    HWND local_video_view_ = nullptr;
    LIVEROOM::ZegoVideoViewMode local_view_mode = LIVEROOM::ZegoVideoViewModeScaleAspectFit;

    string room_id_ = "";

    bool have_published_ = false;

    // 拉流配置
    typedef struct PlayConfig
    {
        HWND view_hwnd = nullptr;
        std::string stream_id;
        std::string play_param;

        LIVEROOM::ZegoVideoViewMode view_mode = LIVEROOM::ZegoVideoViewModeScaleAspectFit;

    }PlayConfig;

    // 所有拉流的列表
    std::map<ZegoJoinLiveUser, PlayConfig> play_map_info_;

    std::mutex play_config_map_mutex_;

    ZGJoinLiveRoomStatus cur_status_ = kZGJoinLiveStatus_None;

    ZGJoinLiveStatus login_status_ = kZGJoinLiveLoginStatusLogout;

    // 记录开始重连房间的时间戳
    int64_t start_retry_login_room_timestamp_ = -1;

    // 自动重试登陆
    bool auto_retry_login_room_ = true;

    // 房间重连超时设置，单位毫秒，0为不超时
    int retry_login_room_timeout_ms_ = 1200000;

    // 失败重试时间间隔，单位毫秒
    int retry_interval_ms_ = 1000;

    void ProcessStreamAdded(string stream_id, string user_id, string user_name, string room_id);
    void ProcessStreamDeleted(string stream_id, string user_id, string user_name, string room_id);

    void UpdatePlayConfigByUser(ZegoJoinLiveUser user, const PlayConfig& config);
    void RemovePlayConfigByStreamId(string stream_id);
    bool GetPlayConfigByUser(ZegoJoinLiveUser user, PlayConfig& config);

    void UpdateLoginEvent(ZGJoinLiveLoginEvent event);
    bool RetryLoginRoom();
    void StopRetryRoomReconnect(ZGJoinLiveReconnectStopReason reason);

    JoinLiveCallBack * join_live_cb_ = nullptr;

    ILogToView * view_log_ = nullptr;

    bool logined_need_publish_ = false;
};

