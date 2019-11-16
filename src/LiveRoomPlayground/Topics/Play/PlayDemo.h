#pragma once

#include <vector>
#include <string>
#include <functional>

#include "ZegoLiveRoom/LiveRoom.h"
#include "ZegoLiveRoom/LiveRoom-Player.h"
#include "ZegoLiveRoom/LiveRoomCallback-Player.h"
#include "ZegoLiveRoom/LiveRoomCallBack.h"

#include "AppSupport/ZGLog.h"
#include "ZegoLiveRoom/AVDefines.h"

using namespace ZEGO;

using ZEGO::COMMON::ZegoStreamInfo;
using LIVEROOM::ZegoPublishQuality;
using LIVEROOM::ZegoPlayQuality;
using COMMON::ZegoStreamUpdateType;

typedef struct PlayInitSDKConfig
{
    bool is_test_env;

    std::string user_id;
    std::string user_name;
    unsigned int app_id;
    std::vector<unsigned char> app_sign;

}PlayInitSDKConfig;

typedef struct PlayLoginRomConfig
{
    std::string room_id;
    COMMON::ZegoRoomRole room_role;

}PlayLoginRomConfig;

typedef struct PlayConfig
{
    HWND view_hwnd;
    std::string stream_id;
    std::string play_param;

}PlayConfig;


enum ZGPlayStatus
{
    kZGPlayStatus_None = 0,
    kZGPlayStatus_Starting_Init_SDK,
    kZGPlayStatus_Init_SDK_OK,
    kZGPlayStatus_Starting_Login_Room,
    kZGPlayStatus_Login_OK,
    kZGPlayStatus_Starting_Playing,
    kZGPlayStatus_Playing,
    kZGPlayStatus_Stoped_Playing
};

typedef struct PlayRealDataInfo {
    std::string room_id;
    std::string stream_id;
    int resolution_w;
    int resolution_h;
    double bit_rate_bps_per_second;
    double frame_rate;

}PlayRealDataInfo;

typedef std::function<void(ZGPlayStatus status)> ZGPlayStatusCallBackType;
typedef std::function<void(const PlayRealDataInfo & status)> ZGPlayRealDataRefreshCallBackType;


class PlayDemo : 
    public LIVEROOM::IRoomCallback,
    public LIVEROOM::ILivePlayerCallback,
    public AV::IZegoLiveEventCallback
{
public:
    PlayDemo();
    ~PlayDemo();

    void SetViewLogCallBack(ILogToView * log_cb);

    void SetStateUpdateObserver(ZGPlayStatusCallBackType cb);

    void SetRealDataRefreshObserver(ZGPlayRealDataRefreshCallBackType cb);

    bool InitSDK(PlayInitSDKConfig c);

    bool LoginRoom(PlayLoginRomConfig c);

    bool StartPlay(PlayConfig c);

    std::string DescOfStatus(ZGPlayStatus status);


    virtual void OnAVKitEvent(int event, ZEGO::AV::EventInfo* pInfo) override;

protected:

    virtual void OnRemoteCameraStatusUpdate(const char* pStreamID, int nStatus, int nReason) override;
 
 
    virtual void OnRemoteMicStatusUpdate(const char* pStreamID, int nStatus, int nReason) override;


    virtual void OnInitSDK(int nError) override;


    virtual void OnLogoutRoom(int errorCode, const char *pszRoomID) override;


    virtual void OnReconnect(int errorCode, const char *pszRoomID) override;


    virtual void OnTempBroken(int errorCode, const char *pszRoomID) override;


    virtual void OnLoginRoom(int errorCode, const char *pszRoomID, const ZegoStreamInfo *pStreamInfo, unsigned int streamCount) override;


    virtual void OnKickOut(int reason, const char *pszRoomID, const char* pszCustomReason="") override;


    virtual void OnDisconnect(int errorCode, const char *pszRoomID) override;


    virtual void OnStreamUpdated(ZegoStreamUpdateType type, ZegoStreamInfo *pStreamInfo, unsigned int streamCount, const char *pszRoomID) override;


    virtual void OnStreamExtraInfoUpdated(ZegoStreamInfo *pStreamInfo, unsigned int streamCount, const char *pszRoomID) override;


    virtual void OnCustomCommand(int errorCode, int requestSeq, const char *pszRoomID) override;


    virtual void OnRecvCustomCommand(const char *pszUserId, const char *pszUserName, const char *pszContent, const char *pszRoomID) override;


    virtual void OnPlayStateUpdate(int stateCode, const char* pszStreamID) override;


    virtual void OnPlayQualityUpdate(const char* pszStreamID, ZegoPlayQuality playQuality) override;


    virtual void OnVideoSizeChanged(const char* pStreamID, int nWidth, int nHeight) override;

	virtual void OnRecvRemoteAudioFirstFrame(const char* pStreamID) override;


private:
    void UpdateStatus(ZGPlayStatus s);

    ZGPlayStatus cur_status_ = kZGPlayStatus_None;

    ZGPlayStatusCallBackType status_cb_ = nullptr;

    ZGPlayRealDataRefreshCallBackType real_data_refresh_cb_ = nullptr;

    PlayRealDataInfo cur_real_data_;

    void PrintLogToView(const char * format, ...);

    ILogToView * view_log_ = nullptr;
};

