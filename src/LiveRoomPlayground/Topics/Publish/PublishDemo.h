//
// PublishDemo.h
//
// Copyright 2019Äê Zego. All rights reserved.
// 

#ifndef PublishDemo_h__
#define PublishDemo_h__

#include <vector>
#include <string>
#include <functional>

#include "ZegoLiveRoom/LiveRoom.h"
#include "ZegoLiveRoom/LiveRoomCallback-Publisher.h"
#include "ZegoLiveRoom/LiveRoomCallBack.h"

#include "AppSupport/ZGLog.h"

using namespace ZEGO;

using ZEGO::COMMON::ZegoPublishingStreamInfo;
using LIVEROOM::ZegoPublishQuality;
using COMMON::ZegoStreamUpdateType;

typedef struct InitSDKConfig
{
    bool is_test_env;

    std::string user_id;
    std::string user_name;
    unsigned int app_id;
    std::vector<unsigned char> app_sign;

}InitSDKConfig;

typedef struct LoginRomConfig
{
    std::string room_id;
    COMMON::ZegoRoomRole room_role;

}LoginRomConfig;

typedef struct PublishConfig
{
    std::string live_title;
    std::string stream_id;
    std::string publish_param;
    AV::ZegoPublishFlag publish_flag;

}PublishConfig;


enum ZGPublishStatus
{
    kZGStatus_None = 0,
    kZGStatus_Starting_Init_SDK,
    kZGStatus_Init_SDK_OK,
    kZGStatus_Starting_Login_Room,
    kZGStatus_Login_OK,
    kZGStatus_Starting_Publishing,
    kZGStatus_Publishing,
    kZGStatus_Stoped_Publishing
};

typedef struct RealDataInfo {
    std::string room_id;
    std::string stream_id;
    int resolution_w;
    int resolution_h;
    double bit_rate_bps_per_second;
    double frame_rate;

}RealDataInfo;

typedef std::function<void(ZGPublishStatus status)> ZGPublishStatusCallBackType;
typedef std::function<void(const RealDataInfo & status)> ZGPublishRealDataRefreshCallBackType;

class PublishDemo :
    public LIVEROOM::ILivePublisherCallback,
    public LIVEROOM::IRoomCallback,
    public AV::IZegoDeviceStateCallback
{

public:
    PublishDemo();
    ~PublishDemo();

    void SetViewLogCallBack(ILogToView * log_cb);

    void SetStateUpdateObserver(ZGPublishStatusCallBackType cb);

    void SetRealDataRefreshObserver(ZGPublishRealDataRefreshCallBackType cb);

    bool InitSDK(InitSDKConfig c);

    void StartPreview(HANDLE video_view_hwnd);

    bool LoginRoom(LoginRomConfig c);

    bool StartPublish(PublishConfig c);

    std::string DescOfStatus(ZGPublishStatus status);


protected:

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


    virtual void OnJoinLiveRequest(int seq, const char *pszFromUserId, const char *pszFromUserName, const char *pszRoomID) override;


    virtual void OnInviteJoinLiveResponse(int result, const char *pszFromUserId, const char *pszFromUserName, int seq) override;


    virtual void OnEndJoinLive(int result, int seq, const char *pszRoomID) override;


    virtual void OnPublishQulityUpdate(const char* pszStreamID, int quality, double videoFPS, double videoKBS) override;


    virtual void OnPublishQualityUpdate(const char* pszStreamID, ZegoPublishQuality publishQuality) override;


    virtual void OnCaptureVideoSizeChanged(int nWidth, int nHeight) override;


    virtual void OnCaptureVideoSizeChanged(AV::PublishChannelIndex index, int nWidth, int nHegith) override;


    virtual void OnPreviewSnapshot(void *pImage) override;


    virtual void OnPreviewSnapshot(AV::PublishChannelIndex index, void *pImage) override;


    virtual void OnAuxCallback(unsigned char *pData, int *pDataLen, int *pSampleRate, int *pNumChannels) override;


    virtual void OnMixStream(const AV::ZegoMixStreamResult& result, const char* pszMixStreamID, int seq) override;


    virtual void OnRelayCDNStateUpdate(const char *streamID, AV::ZegoStreamRelayCDNInfo* statesInfo, unsigned int statesInfoCount) override;


    virtual void OnCaptureVideoFirstFrame() override;


    virtual void OnCaptureVideoFirstFrame(AV::PublishChannelIndex idx) override;


    virtual void OnUpdateMixStreamConfig(unsigned int uiErrorCode, const char* pszMixStreamID, const ZegoPublishingStreamInfo& oStreamInfo) override;

    virtual void OnAudioDeviceStateChanged(ZEGO::AV::AudioDeviceType deviceType, ZEGO::AV::DeviceInfo *deviceInfo, ZEGO::AV::DeviceState state) override;

    virtual void OnVideoDeviceStateChanged(ZEGO::AV::DeviceInfo *deviceInfo, ZEGO::AV::DeviceState state) override;

    virtual void OnDeviceError(const char* deviceName, int errorCode) override;

private:

    void UpdateStatus(ZGPublishStatus s);
    ZGPublishStatus cur_status_ = kZGStatus_None;

    ZGPublishStatusCallBackType status_cb_ = nullptr;
    ZGPublishRealDataRefreshCallBackType real_data_refresh_cb_ = nullptr;

    RealDataInfo cur_real_data_;

    InitSDKConfig init_config_;
    LoginRomConfig login_config_;
    PublishConfig live_config_;

    int cur_resolution_w_ = 0;
    int cur_resolution_h_ = 0;

    void PrintLogToView(const char * format, ...);

    ILogToView * view_log_ = nullptr;

};


#endif // PublishDemo_h__


