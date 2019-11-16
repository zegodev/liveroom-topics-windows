#include "stdafx.h"
#include "PlayDemo.h"


#include "AppSupport/ZGManager.h"
#include "AppSupport/ZGHelper.h"
#include "AppSupport/ZGLog.h"
#include "AppSupport/ZGUtilTools.h"

using std::string;

PlayDemo::PlayDemo()
{
}


PlayDemo::~PlayDemo()
{
}


void PlayDemo::SetViewLogCallBack(ILogToView * log_cb)
{
    view_log_ = log_cb;
}

void PlayDemo::SetStateUpdateObserver(ZGPlayStatusCallBackType cb)
{
    status_cb_ = cb;
}

void PlayDemo::SetRealDataRefreshObserver(ZGPlayRealDataRefreshCallBackType cb)
{
    real_data_refresh_cb_ = cb;
}

bool PlayDemo::InitSDK(PlayInitSDKConfig c)
{
    UpdateStatus(kZGPlayStatus_Starting_Init_SDK);

    PrintLogToView("设置SDK环境：%s", c.is_test_env ? "使用测试环境" : "使用正式环境");
    LIVEROOM::SetUseTestEnv(c.is_test_env);

    PrintLogToView("设置userid = %s，username = %s", c.user_id.c_str(), c.user_name.c_str());
    LIVEROOM::SetUser(c.user_id.c_str(), c.user_name.c_str());
    LIVEROOM::SetLivePlayerCallback(this);
    LIVEROOM::SetRoomCallback(this);
    LIVEROOM::SetLiveEventCallback(this);

    cur_real_data_.bit_rate_bps_per_second = 0;
    cur_real_data_.frame_rate = 0;
    cur_real_data_.resolution_h = 0;
    cur_real_data_.resolution_w = 0;
    cur_real_data_.room_id = "";
    cur_real_data_.stream_id = "";

	/*
	PrintLogToView("强制从服务器拉流");
	LIVEROOM::SetConfig("prefer_play_ultra_source=1");
	*/

    PrintLogToView("执行初始化");
    return ZGManagerInstance()->InitSdk(c.app_id, c.app_sign.data(), c.app_sign.size());
}

bool PlayDemo::LoginRoom(PlayLoginRomConfig c)
{
    UpdateStatus(kZGPlayStatus_Starting_Login_Room);

    cur_real_data_.room_id = c.room_id;
    
    PrintLogToView("room id = %s", c.room_id.c_str());

    return LIVEROOM::LoginRoom(c.room_id.c_str(), c.room_role);
}

bool PlayDemo::StartPlay(PlayConfig c)
{
    cur_real_data_.stream_id = c.stream_id;
    PrintLogToView("拉流 id = %s", c.stream_id.c_str());
    return LIVEROOM::StartPlayingStream(c.stream_id.c_str(), c.view_hwnd, c.play_param.c_str());
}

void PlayDemo::OnRemoteCameraStatusUpdate(const char* pStreamID, int nStatus, int nReason)
{
    PrintLogToView("对端摄像头状态 stream id = %s, status = %d", pStreamID, nStatus);
}

void PlayDemo::OnRemoteMicStatusUpdate(const char* pStreamID, int nStatus, int nReason)
{
    PrintLogToView("对端麦克风状态 stream id = %s, status = %d", pStreamID, nStatus);
}

std::string PlayDemo::DescOfStatus(ZGPlayStatus status)
{
    static string status_des_array[] = {
    "None",
    "Starting_Init_SDK",
    "Init_SDK_OK",
    "Starting_Login_Room",
    "Login_OK",
    "Starting_Playing",
    "Playing",
    "Stoped_Playing"
    };

    return status_des_array[status];
}

void PlayDemo::OnAVKitEvent(int event, AV::EventInfo* pInfo)
{
    
}

void PlayDemo::OnInitSDK(int nError)
{
    if (nError == 0)
    {
        UpdateStatus(kZGPlayStatus_Init_SDK_OK);
    }
    else {
        UpdateStatus(kZGPlayStatus_None);
        PrintLogToView("初始化失败，错误码为：%d", nError);
    }
}

void PlayDemo::OnLogoutRoom(int errorCode, const char *pszRoomID)
{
    UpdateStatus(kZGPlayStatus_None);
}

void PlayDemo::OnReconnect(int errorCode, const char *pszRoomID)
{
    PrintLogToView("已重连上服务器");
}

void PlayDemo::OnTempBroken(int errorCode, const char *pszRoomID)
{
    PrintLogToView("OnTempBroken，临时掉线通知，错误码 = %d", errorCode);
}

void PlayDemo::OnLoginRoom(int errorCode, const char *pszRoomID, const ZegoStreamInfo *pStreamInfo, unsigned int streamCount)
{
    if (errorCode == 0)
    {
        UpdateStatus(kZGPlayStatus_Login_OK);
    }
    else {
        UpdateStatus(kZGPlayStatus_None);
        PrintLogToView("登录房间失败，错误码为：%d", errorCode);
    }
}

void PlayDemo::OnKickOut(int reason, const char *pszRoomID, const char* pszCustomReason/*=""*/)
{
    PrintLogToView("被踢掉线，reason = %d", reason);
}

void PlayDemo::OnDisconnect(int errorCode, const char *pszRoomID)
{
    PrintLogToView("已掉线");
}

void PlayDemo::OnStreamUpdated(ZegoStreamUpdateType type, ZegoStreamInfo *pStreamInfo, unsigned int streamCount, const char *pszRoomID)
{
    PrintLogToView("流更新通知，%s", type == COMMON::StreamAdded ? "流新增":"流删除");
}

void PlayDemo::OnStreamExtraInfoUpdated(ZegoStreamInfo *pStreamInfo, unsigned int streamCount, const char *pszRoomID)
{
    
}

void PlayDemo::OnCustomCommand(int errorCode, int requestSeq, const char *pszRoomID)
{
    
}

void PlayDemo::OnRecvCustomCommand(const char *pszUserId, const char *pszUserName, const char *pszContent, const char *pszRoomID)
{
    
}

void PlayDemo::OnPlayStateUpdate(int stateCode, const char* pszStreamID)
{
    if (stateCode == 0)
    {
        UpdateStatus(kZGPlayStatus_Playing);
    }
    else {
        UpdateStatus(kZGPlayStatus_None);
        PrintLogToView("拉流状态更新，状态码为:%d",stateCode);
    }
}

void PlayDemo::OnPlayQualityUpdate(const char* pszStreamID, ZegoPlayQuality playQuality)
{
    if (real_data_refresh_cb_ != nullptr)
    {
        cur_real_data_.bit_rate_bps_per_second = playQuality.kbps;
        cur_real_data_.frame_rate = playQuality.fps;

        real_data_refresh_cb_(cur_real_data_);
    }
}

void PlayDemo::OnVideoSizeChanged(const char* pStreamID, int nWidth, int nHeight)
{
    cur_real_data_.resolution_w = nWidth;
    cur_real_data_.resolution_h = nHeight;

    PrintLogToView("视频帧大小变化: width = %d, height = %d", nWidth, nHeight);
}

void PlayDemo::OnRecvRemoteAudioFirstFrame(const char* pStreamID)
{
	PrintLogToView("收到音频首帧: streamId= %s", pStreamID);
}

void PlayDemo::UpdateStatus(ZGPlayStatus s)
{
    cur_status_ = s;

    if (status_cb_ != nullptr)
    {
        status_cb_(cur_status_);
    }
}

void PlayDemo::PrintLogToView(const char * format, ...)
{
    char log_buf[1024] = { 0 };
    va_list la;
    va_start(la, format);
    vsprintf_s(log_buf, format, la);
    va_end(la);

    if (view_log_ != nullptr)
    {
        view_log_->PrintToView(log_buf);
    }
}
