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
#include "ZegoLiveRoom/zego-api-mix-stream.h"

#include "AppSupport/ZGLog.h"
#include "AppSupport/ZGUIOperator.h"

#include "MixStreamHelper.h"

#include <windows.h>

using std::string;
using std::map;

using namespace ZEGO;
using ZEGO::COMMON::ZegoPublishingStreamInfo;
using LIVEROOM::ZegoPublishQuality;
using COMMON::ZegoStreamUpdateType;
using LIVEROOM::ZegoPlayQuality;



typedef struct ZegoMixStreamUser
{
    string user_id;
    string user_name;
    bool operator < (ZegoMixStreamUser const& z) const
    {
        if (user_id < z.user_id) {
            return true;
        }
        return false;
    }
    ZegoMixStreamUser()
    {
        user_id = "";
        user_name = "";
    }

}ZegoMixStreamUser;

enum ZGMixStreamRoomStatus
{
    kZGMixStreamRoomStatus_None = 0,             // 未开始初始状态
    kZGMixStreamRoomStatus_Starting_Init_SDK,    // 开始初始化sdk
    kZGMixStreamRoomStatus_Init_SDK_OK,          // sdk初始化成功
    kZGMixStreamRoomStatus_Starting_Login_Room,  // 开始登陆房间
    kZGMixStreamRoomStatus_Login_OK,             // 登陆房间成功
    kZGMixStreamRoomStatus_Starting_Publishing,  // 开始推流 
    kZGMixStreamRoomStatus_Publishing,           // 推流成功
    kZGMixStreamRoomStatus_Stoped_Publishing     // 停止推流
};

// 房间登录事件
enum ZGMixStreamLoginEvent {
    kZGMixStreamLoginEventLogin,           // 开始登录请求
    kZGMixStreamLoginEventLoginSuccess,    // 登录请求成功
    kZGMixStreamLoginEventLoginFailed,     // 登录请求失败
    kZGMixStreamLoginEventLogout,          // 退出登录
    kZGMixStreamLoginEventTempBroke,       // 房间连接暂时断开
    kZGMixStreamLoginEventReconnect,       // 房间连接恢复
    kZGMixStreamLoginEventDisconnect,      // 重连超时，停止重连断开房间连接
    kZGMixStreamLoginEventKickOut,         // 被踢出房间，断开房间连接
};

// 房间登录状态
enum ZGMixStreamJoinLiveStatus
{
    kZGMixStreamLoginStatusLogout,     // 未登录
    kZGMixStreamLoginStatusStartLogin, // 开始登录请求
    kZGMixStreamLoginStatusLogin,      // 已登录
    kZGMixStreamLoginStatusTempBroken, // 暂时断开房间连接
};

// 房间重连终止原因
enum ZGMixStreamReconnectStopReason {
    kZGMixStreamReconnectStopReasonLogout, // 手动调用leaveRoom终止重连
    kZGMixStreamReconnectStopReasonKickout,// 被服务器踢出终止重连
    kZGMixStreamReconnectStopReasonTimeout,// 重连timeout
};

typedef struct MixStreamSoundLevelInfo
{
    string user_id;
    int sound_level;
}MixStreamSoundLevelInfo;

class MixStreamDemoCallBack
{
public:
    virtual void OnLoginEvent(ZGMixStreamLoginEvent event, ZGMixStreamJoinLiveStatus status) = 0;
    virtual void OnUserJoin(ZegoMixStreamUser user) {}
    virtual void OnUserLeave(ZegoMixStreamUser user) {}
    virtual void OnMixStreamUpdate(ZegoMixStreamUser anchor, string mix_stream_id){};
    virtual void OnSoundLevelUpdate(const std::vector<MixStreamSoundLevelInfo> & sound_levels){};
};

class MixStreamDemo :
    public LIVEROOM::ILivePublisherCallback,
    public LIVEROOM::ILivePlayerCallback,
    public LIVEROOM::IRoomCallback,
    public AV::IZegoDeviceStateCallback,
    public MIXSTREAM::IZegoMixStreamExCallback,
    public MIXSTREAM::IZegoSoundLevelInMixedStreamCallback

{
public:

    MixStreamDemo();
    ~MixStreamDemo();

    void InitStreamId(std::string publish_stream_id, std::string mix_stream_id);

    void SetDemoCallBack(MixStreamDemoCallBack * cb);

    void SetLocalVideoView(HWND view, LIVEROOM::ZegoVideoViewMode view_mode = LIVEROOM::ZegoVideoViewModeScaleAspectFit);

    void SetRemoteVideoView(HWND view, ZegoMixStreamUser for_user, LIVEROOM::ZegoVideoViewMode view_mode = LIVEROOM::ZegoVideoViewModeScaleAspectFit);

    void LoginRoom(string room_id, bool need_publish_after_logined = false);

    void StartPublish();

    void LogoutRoom();

    // 保存主窗口的句柄，为了切线程使用
    void InitMainHwnd(HWND hwnd);

    void SetLogCallBack(ILogToView *log_cb);

    void StartMixStream();

    void UpdateMixStream();
    void StopMixStream();

    void StartPlayingStream(string stream_id, HWND hwnd);

    void StopPlayingStream(string stream_id);


protected:

    virtual void OnSoundLevelInMixedPlayStream(AV::SoundLevelInfo *volume_list, int list_size) override;

    virtual void OnMixStreamEx(const AV::ZegoMixStreamResultEx& result, const char* mixStreamID, int seq) override;

    virtual void OnMixStreamRelayCDNStateUpdate(const char *mixStreamID, AV::ZegoStreamRelayCDNInfo *statesInfo, unsigned int statesInfoCount) override;
    
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

    bool have_mixed_stream_ = false;

    int sound_level_id_ = 0;

    int mix_stream_task_seq_ = 0;

    bool is_anchor_ = false;

    std::string publish_stream_id_;
    std::string mix_stream_id_;

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
    std::map<ZegoMixStreamUser, PlayConfig> play_map_info_;

    std::mutex play_config_map_mutex_;

    ZGMixStreamRoomStatus cur_status_ = kZGMixStreamRoomStatus_None;

    ZGMixStreamJoinLiveStatus login_status_ = kZGMixStreamLoginStatusLogout;

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

    void UpdatePlayConfigByUser(ZegoMixStreamUser user, const PlayConfig& config);
    void RemovePlayConfigByStreamId(string stream_id);
    bool GetPlayConfigByUser(ZegoMixStreamUser user, PlayConfig& config);

    void UpdateLoginEvent(ZGMixStreamLoginEvent event);
    bool RetryLoginRoom();
    void StopRetryRoomReconnect(ZGMixStreamReconnectStopReason reason);

    MixStreamDemoCallBack * demo_event_cb_ = nullptr;

    ILogToView * view_log_ = nullptr;

    bool logined_need_publish_ = false;

    MixStreamHelper mix_stream_config_helper_;
};

