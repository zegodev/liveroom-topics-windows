#include "stdafx.h"
#include "PublishDemo.h"

#include "AppSupport/ZGManager.h"
#include "AppSupport/ZGHelper.h"
#include "AppSupport/ZGLog.h"
#include "AppSupport/ZGUtilTools.h"

#include "mix_stream_config.hpp"

using std::string;

using namespace zg;

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
    MIXSTREAM::SetMixStreamExCallback(this);
    
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

    bool ret = LIVEROOM::RequireHardwareEncoder(true);

    PrintLogToView("开启硬编 %s", ret ?"成功":"失败");

    LIVEROOM::EnableSelectedAudioRecord(4);
    LIVEROOM::SetAudioRecordCallback(this);

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

void PublishDemo::StartMixStream()
{
    MixStreamLayout layout(90, 160, 270, 480);
//     MixStreamInput(std::string stream_id_,
//         MixStreamLayout layout_,
//         int32_t sound_level_id_,
//         int32_t content_control_)
    std::vector<MixStreamInput> input_stream_list;

    MixStreamInput item(live_config_.stream_id
    , layout
    , 0
    , 0);

    input_stream_list.push_back(item);


//     MixStreamOutput(std::string target_,
//         bool is_url_)

    std::vector<MixStreamOutput> output_stream_list;
    output_stream_list.push_back(MixStreamOutput("rtmp://publish.ws.zego.im/zegodemo/world-mix", true));


//     MixStreamOutputConfig(MixStreamOutputConfigVideo video_,
//         MixStreamOutputConfigAudio audio_)

//     MixStreamOutputConfigVideo(int32_t fps_,
//         int32_t bitrate_,
//         int32_t width_,
//         int32_t height_,
//         int64_t bg_color_,
//         std::string bg_image_,
//         int32_t rate_control_mode_,
//         int32_t quality_,
//        MixStreamOutputWatermark watermark_)

    MixStreamOutputConfigVideo v_config(15
        , 600000
        , 640
        , 360
        , 4288230144
        , ""
        , 0
        , 0
        , MixStreamOutputWatermark("", MixStreamLayout(0,0,0,0))
        );

//     MixStreamOutputConfigAudio(int32_t bitrate_,
//         int32_t config_,
//         int32_t channel_count_,
//         bool with_sound_level_)

    MixStreamOutputConfigAudio a_config(64000, 0, 1, 0);


    MixStreamOutputConfig output_config(v_config, a_config);


//     MixStreamConfig(int32_t request_seq_,
//         std::string mix_stream_id_,
//         std::vector<MixStreamInput> input_stream_list_,
//         std::vector<MixStreamOutput> output_stream_list_,
//         MixStreamOutputConfig output_config_,
//         std::vector<uint8_t> user_data_)

    MixStreamConfig config(2019
        ,"test-mix-stream"
        , input_stream_list
        , output_stream_list
        , output_config
        , std::vector<uint8_t>()
        );
    MIXSTREAM::ZegoMixStreamConfig c;

    // * user data
    c.pUserData = config.user_data.data();

    std::vector<MIXSTREAM::ZegoMixStreamInput> input_list;

    // * input stream
    for (int index = 0; index < config.input_stream_list.size(); ++index)
    {
        MIXSTREAM::ZegoMixStreamInput i;
        memcpy(i.szStreamID, config.input_stream_list[index].stream_id.data(), config.input_stream_list[index].stream_id.size());
        i.szStreamID[config.input_stream_list[index].stream_id.size()] = '\0';
        i.layout.top = config.input_stream_list[index].layout.top;
        i.layout.left = config.input_stream_list[index].layout.left;
        i.layout.bottom = config.input_stream_list[index].layout.bottom;
        i.layout.right = config.input_stream_list[index].layout.right;

        i.uSoundLevelID = config.input_stream_list[index].sound_level_id;
        i.nContentControl = config.input_stream_list[index].content_control;
        input_list.push_back(i);
    }

    c.pInputStreamList = input_list.data();
    c.nInputStreamCount = input_list.size();

    // * output stream
    std::vector<MIXSTREAM::ZegoMixStreamOutput> output_list;
    for (int index = 0; index < config.output_stream_list.size(); ++index)
    {
        MIXSTREAM::ZegoMixStreamOutput o;
        o.isUrl = config.output_stream_list[index].is_url;
        memcpy(o.target, config.output_stream_list[index].target.c_str(), config.output_stream_list[index].target.size());
        o.target[config.output_stream_list[index].target.size()] = '\0';

        output_list.push_back(o);
    }
    c.pOutputList = output_list.data();
    c.nOutputStreamCount = output_list.size();

    // * output config
    // * video
    c.nOutputFps = config.output_config.video.fps;
    c.nOutputBitrate = config.output_config.video.bitrate;
    c.nOutputRateControlMode = config.output_config.video.rate_control_mode;
    c.nOutputQuality = config.output_config.video.quality;
    c.nOutputWidth = config.output_config.video.width;
    c.nOutputHeight = config.output_config.video.height;
    // TODO
    c.nOutputBackgroundColor = config.output_config.video.bg_color;

    if (config.output_config.video.bg_image != "")
    {
        c.pOutputBackgroundImage = config.output_config.video.bg_image.data();
    }

    // * video - watermark
    if (config.output_config.video.watermark.image != "")
    {
        memcpy(c.pOutputWatermark->image, config.output_config.video.watermark.image.data(), config.output_config.video.watermark.image.size());
        c.pOutputWatermark->image[config.output_config.video.watermark.image.size()] = '\0';
    }

    c.nOutputAudioBitrate = config.output_config.audio.bitrate;
    c.nOutputAudioConfig = config.output_config.audio.config;
    c.nChannels = config.output_config.audio.channel_count;
    c.bWithSoundLevel = config.output_config.audio.with_sound_level;

    int ret_index = MIXSTREAM::MixStreamEx(config.mix_stream_id.c_str(), c);

    PrintLogToView("MixStreamEx ret_index = %d", ret_index);

}

void PublishDemo::StopMixStream()
{
    //MIXSTREAM::MixStreamEx()
}

void PublishDemo::OnAudioRecordCallback(const unsigned char *pData, int data_len, int sample_rate, int num_channels, int bit_depth, unsigned int type)
{
    static bool have_invoke = false;
    if (!have_invoke)
    {
        have_invoke = true;
        PrintLogToView("OnAudioRecordCallback 1, %d", data_len);
    }
}

void PublishDemo::OnAudioRecordCallback(const unsigned char *pData, int data_len, int sample_rate, int num_channels, int bit_depth)
{
    static bool have_invoke = false;
    if (!have_invoke)
    {
        have_invoke = true;
        PrintLogToView("OnAudioRecordCallback 2, %d", data_len);
    }
}

void PublishDemo::OnCaptureAudioFirstFrame()
{
    PrintLogToView("音频第一帧采集到达");
}

void PublishDemo::OnMixStreamEx(const AV::ZegoMixStreamResultEx& result, const char* mixStreamID, int seq)
{
    PrintLogToView("OnMixStreamEx, uiErrorCode = %d, seq = %d, mixstreamid = %s", result.uiErrorCode, seq, mixStreamID);
}

void PublishDemo::OnMixStreamRelayCDNStateUpdate(const char *mixStreamID, AV::ZegoStreamRelayCDNInfo *statesInfo, unsigned int statesInfoCount)
{
    if (statesInfoCount > 0)
    {
        PrintLogToView("OnMixStreamRelayCDNStateUpdate, mixStreamID = %s , state : %d", mixStreamID, statesInfo[0].state);
    }
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
    PrintLogToView("是否硬件编码%d", publishQuality.isHardwareVenc);

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
    PrintLogToView("OnMixStream，seq = %d, pszMixStreamID = %s, uiErrorCode = %d", seq, pszMixStreamID, result.uiErrorCode);
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


