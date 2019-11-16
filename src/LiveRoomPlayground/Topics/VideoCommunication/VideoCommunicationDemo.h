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

using std::string;
using std::map;

using namespace ZEGO;
using ZEGO::COMMON::ZegoPublishingStreamInfo;
using LIVEROOM::ZegoPublishQuality;
using COMMON::ZegoStreamUpdateType;
using LIVEROOM::ZegoPlayQuality;

typedef struct ZegoUser
{
    string user_id;
    string user_name;
    bool operator < (ZegoUser const& z) const
    {
        if (user_id < z.user_id) {
            return true;
        }
        return false;
    }
    ZegoUser()
    {
        user_id = "";
        user_name = "";
    }

}ZegoUser;

enum ZGVideoCommunicationRoomStatus
{
    kZGVideoCommunicationStatus_None = 0,             // 未开始初始状态
    kZGVideoCommunicationStatus_Starting_Init_SDK,    // 开始初始化sdk
    kZGVideoCommunicationStatus_Init_SDK_OK,          // sdk初始化成功
    kZGVideoCommunicationStatus_Starting_Login_Room,  // 开始登陆房间
    kZGVideoCommunicationStatus_Login_OK,             // 登陆房间成功
    kZGVideoCommunicationStatus_Starting_Publishing,  // 开始推流 
    kZGVideoCommunicationStatus_Publishing,           // 推流成功
    kZGVideoCommunicationStatus_Stoped_Publishing     // 停止推流
};

// 房间登录事件
enum ZGVideoCommunicationLoginEvent {
    kZGVideoCommunicationLoginEventLogin,           // 开始登录请求
    kZGVideoCommunicationLoginEventLoginSuccess,    // 登录请求成功
    kZGVideoCommunicationLoginEventLoginFailed,     // 登录请求失败
    kZGVideoCommunicationLoginEventLogout,          // 退出登录
    kZGVideoCommunicationLoginEventTempBroke,       // 房间连接暂时断开
    kZGVideoCommunicationLoginEventReconnect,       // 房间连接恢复
    kZGVideoCommunicationLoginEventDisconnect,      // 重连超时，停止重连断开房间连接
    kZGVideoCommunicationLoginEventKickOut,         // 被踢出房间，断开房间连接
};

// 房间登录状态
enum ZGVideoCommunicationStatus
{
    kZGVideoCommunicationLoginStatusLogout,     // 未登录
    kZGVideoCommunicationLoginStatusStartLogin, // 开始登录请求
    kZGVideoCommunicationLoginStatusLogin,      // 已登录
    kZGVideoCommunicationLoginStatusTempBroken, // 暂时断开房间连接
};

// 房间重连终止原因
enum ZGVideoCommunicationReconnectStopReason {
    kZGVideoCommunicationReconnectStopReasonLogout, // 手动调用leaveRoom终止重连
    kZGVideoCommunicationReconnectStopReasonKickout,// 被服务器踢出终止重连
    kZGVideoCommunicationReconnectStopReasonTimeout,// 重连timeout
};

class VideoCommunicationCallBack
{
public:
    virtual void OnLoginEvent(ZGVideoCommunicationLoginEvent event, ZGVideoCommunicationStatus status) = 0;
    virtual void OnUserJoin(ZegoUser user) {}
    virtual void OnUserLeave(ZegoUser user) {}
};


class VideoCommunicationDemo :
    public LIVEROOM::ILivePublisherCallback,
    public LIVEROOM::ILivePlayerCallback,
    public LIVEROOM::IRoomCallback,
    public AV::IZegoDeviceStateCallback
{
public:

    VideoCommunicationDemo();
    ~VideoCommunicationDemo();

    void SetDemoCallBack(VideoCommunicationCallBack * cb);

    void SetLocalVideoView(HWND view, LIVEROOM::ZegoVideoViewMode view_mode = LIVEROOM::ZegoVideoViewModeScaleAspectFit);

    void SetRemoteVideoView(HWND view, ZegoUser for_user, LIVEROOM::ZegoVideoViewMode view_mode = LIVEROOM::ZegoVideoViewModeScaleAspectFit);

    void LoginRoom(string room_id);

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


    virtual void OnKickOut(int reason, const char *pszRoomID, const char* pszCustomReason="") override;


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

    // 拉流配置
    typedef struct PlayConfig
    {
        HWND view_hwnd = nullptr;
        std::string stream_id;
        std::string play_param;

        LIVEROOM::ZegoVideoViewMode view_mode = LIVEROOM::ZegoVideoViewModeScaleAspectFit;

    }PlayConfig;

    // 所有拉流的列表
    // ZegoUser - PlayConfig
    std::map<ZegoUser, PlayConfig> play_map_info_;

    std::mutex play_config_map_mutex_;

    ZGVideoCommunicationRoomStatus cur_status_ = kZGVideoCommunicationStatus_None;

    ZGVideoCommunicationStatus login_status_ = kZGVideoCommunicationLoginStatusLogout;

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

    void UpdatePlayConfigByUser(ZegoUser user, const PlayConfig& config);
    void RemovePlayConfigByStreamId(string stream_id);
    bool GetPlayConfigByUser(ZegoUser user, PlayConfig& config);

    void UpdateLoginEvent(ZGVideoCommunicationLoginEvent event);
    bool RetryLoginRoom();
    void StopRetryRoomReconnect(ZGVideoCommunicationReconnectStopReason reason);

    VideoCommunicationCallBack * video_communication_cb_ = nullptr;

    ILogToView * view_log_ = nullptr;
};

