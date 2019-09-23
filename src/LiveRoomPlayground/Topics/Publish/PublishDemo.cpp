#include "stdafx.h"
#include "PublishDemo.h"

#include "AppSupport/ZGManager.h"
#include "AppSupport/ZGHelper.h"
#include "AppSupport/ZGLog.h"
#include "AppSupport/ZGUtilTools.h"

using std::string;

PublishDemo::PublishDemo()
{
}

PublishDemo::~PublishDemo()
{
}

void PublishDemo::SetViewLogCallBack(ILogToView * log_cb)
{
    view_log_ = log_cb;
}

void PublishDemo::SetStateUpdateObserver(ZGPublishStatusCallBackType cb)
{
    status_cb_ = cb;
}

void PublishDemo::SetRealDataRefreshObserver(ZGPublishRealDataRefreshCallBackType cb)
{
    real_data_refresh_cb_ = cb;
}

bool PublishDemo::InitSDK(InitSDKConfig c)
{
    UpdateStatus(kZGStatus_Starting_Init_SDK);

    PrintLogToView("设置SDK环境：%s", c.is_test_env ?  "使用测试环境" : "使用正式环境");
    LIVEROOM::SetUseTestEnv(c.is_test_env);

    PrintLogToView("设置userid = %s，username = %s", c.user_id.c_str(), c.user_name.c_str());
    LIVEROOM::SetUser(c.user_id.c_str(), c.user_name.c_str());
    LIVEROOM::SetLivePublisherCallback(this);-
    LIVEROOM::SetRoomCallback(this);
    LIVEROOM::SetDeviceStateCallback(this);
    
    init_config_ = c;
    PrintLogToView("执行初始化");
    return ZGManagerInstance()->InitSdk(c.app_id, c.app_sign.data(), c.app_sign.size());
}

void PublishDemo::StartPreview(HANDLE video_view_hwnd)
{
    PrintLogToView("设置预览视图");
    LIVEROOM::SetPreviewView(video_view_hwnd);
    PrintLogToView("开始预览");
    LIVEROOM::StartPreview();
}

bool PublishDemo::LoginRoom(LoginRomConfig c)
{
    UpdateStatus(kZGStatus_Starting_Login_Room);
    login_config_ = c;
    PrintLogToView("room id = %s", c.room_id.c_str());
    return LIVEROOM::LoginRoom(c.room_id.c_str(), c.room_role);
}

bool PublishDemo::StartPublish(PublishConfig c)
{
    live_config_ = c;
    
    PrintLogToView("推流 id = %s", c.stream_id.c_str());

    return LIVEROOM::StartPublishing(c.live_title.c_str(), c.stream_id.c_str(), c.publish_flag, c.publish_param.c_str());
}

string PublishDemo::DescOfStatus(ZGPublishStatus status)
{
    //     kZGStatus_None = 0,
    //     kZGStatus_Starting_Init_SDK,
    //     kZGStatus_Init_SDK_OK,
    //     kZGStatus_Starting_Login_Room,
    //     kZGStatus_Login_OK,
    //     kZGStatus_Starting_Publishing,
    //     kZGStatus_Publishing,
    //     kZGStatus_Stoped_Publishing

    static string status_des_array[] = {
        "None",
        "Starting_Init_SDK",
        "Init_SDK_OK",
        "Starting_Login_Room",
        "Login_OK",
        "Starting_Publishing",
        "Publishing",
        "Stoped_Publishing"
    };

    return status_des_array[status];
}

void PublishDemo::OnAudioDeviceStateChanged(ZEGO::AV::AudioDeviceType deviceType, ZEGO::AV::DeviceInfo *deviceInfo, ZEGO::AV::DeviceState state)
{
}

void PublishDemo::OnVideoDeviceStateChanged(ZEGO::AV::DeviceInfo *deviceInfo, ZEGO::AV::DeviceState state)
{
}

void PublishDemo::OnDeviceError(const char* deviceName, int errorCode)
{
    PrintLogToView("设备错误，错误码为：%d, 设备名称为：%s", errorCode, deviceName);
}

void PublishDemo::UpdateStatus(ZGPublishStatus s)
{
    cur_status_ = s;
    if (status_cb_ != nullptr)
    {
        status_cb_(cur_status_);
    }
}

void PublishDemo::OnInitSDK(int nError)
{
    if (nError == 0)
    {
        UpdateStatus(kZGStatus_Init_SDK_OK);
    }
    else {
        UpdateStatus(kZGStatus_None);
        PrintLogToView("初始化失败，错误码为：%d",nError);
    }
}

void PublishDemo::OnLogWillOverwrite()
{
	PrintLogToView("日志已满 OnLogWillOverwrite");
}

void PublishDemo::OnLogUploadResult(int errorCode)
{
	PrintLogToView("日志上传结果 OnLogUploadResul: %d", errorCode);
}

void PublishDemo::OnLoginRoom(int errorCode, const char *pszRoomID, const COMMON::ZegoStreamInfo *pStreamInfo, unsigned int streamCount)
{
    if (errorCode == 0)
    {
        UpdateStatus(kZGStatus_Login_OK);
    }
    else {
        UpdateStatus(kZGStatus_None);
        PrintLogToView("登录房间失败，错误码为：%d", errorCode);
    }
}

void PublishDemo::OnLogoutRoom(int errorCode, const char *pszRoomID)
{
    UpdateStatus(kZGStatus_None);
}

void PublishDemo::OnKickOut(int reason, const char *pszRoomID)
{
    UpdateStatus(kZGStatus_None);
    PrintLogToView("被踢掉线，reason = %d", reason);
}

void PublishDemo::OnDisconnect(int errorCode, const char *pszRoomID)
{
    UpdateStatus(kZGStatus_None);
    PrintLogToView("已掉线");
}

void PublishDemo::OnReconnect(int errorCode, const char *pszRoomID)
{
    PrintLogToView("已重连上服务器");
}

void PublishDemo::OnTempBroken(int errorCode, const char *pszRoomID)
{
    PrintLogToView("OnTempBroken，临时掉线通知，错误码 = %d", errorCode);
}

void PublishDemo::OnStreamUpdated(ZegoStreamUpdateType type, COMMON::ZegoStreamInfo *pStreamInfo, unsigned int streamCount, const char *pszRoomID)
{
    
}

void PublishDemo::OnStreamExtraInfoUpdated(COMMON::ZegoStreamInfo *pStreamInfo, unsigned int streamCount, const char *pszRoomID)
{
    
}

void PublishDemo::OnCustomCommand(int errorCode, int requestSeq, const char *pszRoomID)
{
    
}

void PublishDemo::OnRecvCustomCommand(const char *pszUserId, const char *pszUserName, const char *pszContent, const char *pszRoomID)
{
    
}

void PublishDemo::OnPublishStateUpdate(int stateCode, const char* pszStreamID, const ZegoPublishingStreamInfo& oStreamInfo)
{
    if (stateCode == 0)
    {
        UpdateStatus(kZGStatus_Publishing);
    }
    else {
        UpdateStatus(kZGStatus_None);
        PrintLogToView("推流状态码更新，状态码 = %d", stateCode);
    }
}

void PublishDemo::OnJoinLiveRequest(int seq, const char *pszFromUserId, const char *pszFromUserName, const char *pszRoomID)
{
    
}

void PublishDemo::OnInviteJoinLiveResponse(int result, const char *pszFromUserId, const char *pszFromUserName, int seq)
{
    
}

void PublishDemo::OnEndJoinLive(int result, int seq, const char *pszRoomID)
{
    
}

void PublishDemo::OnPublishQulityUpdate(const char* pszStreamID, int quality, double videoFPS, double videoKBS)
{
    
}

void PublishDemo::OnPublishQualityUpdate(const char* pszStreamID, ZegoPublishQuality publishQuality)
{
    if (real_data_refresh_cb_ != nullptr)
    {
        cur_real_data_.room_id = login_config_.room_id;
        cur_real_data_.stream_id = live_config_.stream_id;

        cur_real_data_.bit_rate_bps_per_second = publishQuality.kbps;
        cur_real_data_.frame_rate = publishQuality.fps;

        cur_real_data_.resolution_w = cur_resolution_w_;
        cur_real_data_.resolution_h = cur_resolution_h_;

        real_data_refresh_cb_(cur_real_data_);
    }
}

void PublishDemo::OnCaptureVideoSizeChanged(int nWidth, int nHeight)
{
    cur_resolution_w_ = nWidth;
    cur_resolution_h_ = nHeight;

    PrintLogToView("采集视频帧大小变化，width = %d, height = %d", nWidth, nHeight);
}

void PublishDemo::OnCaptureVideoSizeChanged(AV::PublishChannelIndex index, int nWidth, int nHegith)
{
    
}

void PublishDemo::OnPreviewSnapshot(void *pImage)
{
    
}

void PublishDemo::OnPreviewSnapshot(AV::PublishChannelIndex index, void *pImage)
{
    
}

void PublishDemo::OnAuxCallback(unsigned char *pData, int *pDataLen, int *pSampleRate, int *pNumChannels)
{
    
}

void PublishDemo::OnMixStream(const AV::ZegoMixStreamResult& result, const char* pszMixStreamID, int seq)
{
    
}

void PublishDemo::OnRelayCDNStateUpdate(const char *streamID, AV::ZegoStreamRelayCDNInfo* statesInfo, unsigned int statesInfoCount)
{

}

void PublishDemo::OnCaptureVideoFirstFrame()
{
    PrintLogToView("采集视频首帧到达");
}

void PublishDemo::OnCaptureVideoFirstFrame(AV::PublishChannelIndex idx)
{
    
}

void PublishDemo::OnUpdateMixStreamConfig(unsigned int uiErrorCode, const char* pszMixStreamID, const ZegoPublishingStreamInfo& oStreamInfo)
{
    
}

void PublishDemo::PrintLogToView(const char * format, ...)
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


