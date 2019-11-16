#include "stdafx.h"
#include "MixStreamDemo.h"

#include <algorithm>
#include <thread>

#include "AppSupport/ZGHelper.h"
#include "AppSupport/ZGManager.h"
#include "AppSupport/ZGConfigHelper.h"

MixStreamDemo::MixStreamDemo()
{
}

MixStreamDemo::~MixStreamDemo()
{
}

void MixStreamDemo::InitStreamId(std::string publish_stream_id, std::string mix_stream_id)
{
    publish_stream_id_ = publish_stream_id;
    mix_stream_id_ = mix_stream_id;
}

void MixStreamDemo::SetDemoCallBack(MixStreamDemoCallBack * cb)
{
    demo_event_cb_ = cb;
}

void MixStreamDemo::SetLocalVideoView(HWND view, LIVEROOM::ZegoVideoViewMode view_mode /*= LIVEROOM::ZegoVideoViewModeScaleAspectFit*/)
{
    local_video_view_ = view;
    local_view_mode = view_mode;
    
}

void MixStreamDemo::SetRemoteVideoView(HWND view, ZegoMixStreamUser for_user, LIVEROOM::ZegoVideoViewMode view_mode /*= LIVEROOM::ZegoVideoViewModeScaleAspectFit*/)
{
    PlayConfig config;
    if (GetPlayConfigByUser(for_user, config))
    {
        config.view_hwnd = view;
        config.view_mode = view_mode;
        UpdatePlayConfigByUser(for_user, config);

        LIVEROOM::StartPlayingStream(config.stream_id.c_str(), config.view_hwnd);
        LIVEROOM::SetViewMode(config.view_mode, config.stream_id.c_str());

    }
    else {
        PlayConfig play_config;
        play_config.view_hwnd = view;
        play_config.view_mode = view_mode;

        std::lock_guard<std::mutex> lock(play_config_map_mutex_);
        play_map_info_.insert(std::make_pair(for_user, play_config));
    }
}


void MixStreamDemo::PrintLogToView(const char * format, ...)
{
    if (view_log_ != nullptr)
    {
        char log_buf[1024] = { 0 };
        va_list la;
        va_start(la, format);
        vsprintf_s(log_buf, format, la);
        va_end(la);

        view_log_->PrintToView(log_buf);
    }
}


void MixStreamDemo::LoginRoom(string room_id, bool need_publish_after_logined)
{
    // 1.init sdk        初始化sdk
    // 2.login room      登录房间
    // 3.start publish   开始推流
    // 4.on stream added 收到流新增事件
    // 5.start playing   开始拉流

    room_id_ = room_id;
    logined_need_publish_ = need_publish_after_logined;

    bool is_test_env = true;
    PrintLogToView("使用%s环境", is_test_env ? "测试环境":"正式环境");

    LIVEROOM::SetUseTestEnv(is_test_env);

    ZGHelper::Instance()->GetCurTimeStampMs();

    sound_level_id_ = ZGHelper::Instance()->GetCurTimeStampMs()/1000;
    char szuser_id[100] = {0};
    sprintf(szuser_id, "u-%d", sound_level_id_);

    string user_id = szuser_id;
    string user_name = ZGHelper::Instance()->GetMachineName().c_str();

    PrintLogToView("设置userid = %s，username = %s", user_id.c_str(), user_name.c_str());
    LIVEROOM::SetUser(user_id.c_str(), user_name.c_str());

    // 配置观众可以创建房间，开启房间用户列表更新
    LIVEROOM::SetRoomConfig(true, true);

    LIVEROOM::SetRoomCallback(this);             // 房间回调注册
    LIVEROOM::SetLivePublisherCallback(this);    // 推流回调注册 
    LIVEROOM::SetLivePlayerCallback(this);       // 拉流回调注册
    LIVEROOM::SetDeviceStateCallback(this);      // 设备事件回调
    MIXSTREAM::SetMixStreamExCallback(this);     // 设置混流回调
    MIXSTREAM::SetSoundLevelInMixedStreamCallback(this); // 设置混流音浪回调

    PrintLogToView("执行初始化");
    if (!ZGManagerInstance()->InitSdk())
    {
        UpdateLoginEvent(kZGMixStreamLoginEventLoginFailed);
    }
}

void MixStreamDemo::StartPublish()
{
    if (have_published_)
    {
        return;
    }

    have_published_ = true;

    PrintLogToView("开始预览");

    // 设置采集分辨率和编码分辨率
    PrintLogToView("设置采集分辨率和编码分辨率,%dx%d", MixStreamHelper::GetMixStreamConfig().output_w, MixStreamHelper::GetMixStreamConfig().output_h);
    ZGConfigHelperInstance()->SetPublishResolution(MixStreamHelper::GetMixStreamConfig().output_w, MixStreamHelper::GetMixStreamConfig().output_h);

    LIVEROOM::SetPreviewView(local_video_view_);
    LIVEROOM::StartPreview();

    string live_title = "";
    string publish_param = "";
    AV::ZegoPublishFlag publish_flag = AV::ZEGO_MIX_STREAM;// 推出的流要做混流操作，需要使用这个模式进行推流

    //string stream_id = ZGHelper::Instance()->GetRandomUUID().c_str();
    if (publish_stream_id_ == "")
    {
        publish_stream_id_ = ZGHelper::Instance()->GetRandomUUID().c_str();
    }

    PrintLogToView("推流 id = %s", publish_stream_id_.c_str());

    PrintLogToView("开始推流");

    ZGConfigHelperInstance()->SetVideoBitrate(MixStreamHelper::GetMixStreamConfig().bitrate);

    PrintLogToView("设置码率为:%d bps", MixStreamHelper::GetMixStreamConfig().bitrate);

    LIVEROOM::StartPublishing(live_title.c_str(), publish_stream_id_.c_str(), publish_flag, publish_param.c_str());
}

void MixStreamDemo::InitMainHwnd(HWND hwnd)
{
    main_hwnd_ = hwnd;
}

void MixStreamDemo::SetLogCallBack(ILogToView *log_cb)
{
    view_log_ = log_cb;
}

void MixStreamDemo::StartMixStream()
{
    have_mixed_stream_ = true;

    int output_video_w = MixStreamHelper::GetMixStreamConfig().output_w;
    int output_video_h = MixStreamHelper::GetMixStreamConfig().output_h;

    mix_stream_config_helper_.ClearInputStream();
    mix_stream_config_helper_.ClearOutputAddr();

    int sound_level_id = sound_level_id_;
    mix_stream_config_helper_.AddInputStream(publish_stream_id_, 0, 0, output_video_h, output_video_w, sound_level_id, 0);

    mix_stream_config_helper_.SetOutputVideoConfig(MixStreamHelper::GetMixStreamConfig().fps, output_video_w, output_video_h, MixStreamHelper::GetMixStreamConfig().bitrate);

    mix_stream_config_helper_.SetOutputAudioConfig(80, MixStreamHelper::GetMixStreamConfig().audio_channel_count, MixStreamHelper::GetMixStreamConfig().with_sound_level);

    bool output_addr_is_url = false;
    mix_stream_config_helper_.AddOutputAddr(mix_stream_id_, output_addr_is_url);

    MIXSTREAM::ZegoMixStreamConfig mix_config;
    mix_stream_config_helper_.GetMixStreamConfig(mix_config);

    PrintLogToView("准备混流，音浪ID=%d, 混流流id:%s", sound_level_id_, mix_stream_id_.c_str());

    string mixstream_task_id = mix_stream_id_;//ZGHelper::Instance()->GetRandomUUID();//
    mix_stream_task_seq_ = MIXSTREAM::MixStreamEx(mixstream_task_id.c_str(), mix_config);
    
    PrintLogToView("混流 返回seq %d", mix_stream_task_seq_);
}

void MixStreamDemo::UpdateMixStream()
{
    if (!have_mixed_stream_)
    {
        return;
    }

    mix_stream_config_helper_.ClearInputStream();
    mix_stream_config_helper_.ClearOutputAddr();

    int output_video_w = MixStreamHelper::GetMixStreamConfig().output_w;
    int output_video_h = MixStreamHelper::GetMixStreamConfig().output_h;

    {
        if (play_map_info_.size() > 0)
        {
            std::lock_guard<std::mutex> lock(play_config_map_mutex_);

            string user_id = play_map_info_.begin()->first.user_id;
            string sound_level_id_str = user_id.substr(2, user_id.length() - 2);
            int sound_level_id = atoi(sound_level_id_str.c_str());

            mix_stream_config_helper_.AddInputStream(play_map_info_.begin()->second.stream_id, 0, 0, output_video_h/2, output_video_w, sound_level_id, 0);

            mix_stream_config_helper_.AddInputStream(publish_stream_id_, output_video_h / 2, 0 ,  output_video_h, output_video_w, sound_level_id_, 0);
        }
        else {
            mix_stream_config_helper_.AddInputStream(publish_stream_id_, 0, 0, output_video_h, output_video_w, sound_level_id_, 0);
        }
    }

    mix_stream_config_helper_.SetOutputVideoConfig(MixStreamHelper::GetMixStreamConfig().fps, output_video_w, output_video_h, MixStreamHelper::GetMixStreamConfig().bitrate);
    mix_stream_config_helper_.SetOutputAudioConfig(80, MixStreamHelper::GetMixStreamConfig().audio_channel_count, MixStreamHelper::GetMixStreamConfig().with_sound_level);

    bool output_addr_is_url = false;
    mix_stream_config_helper_.AddOutputAddr(mix_stream_id_, output_addr_is_url);

    MIXSTREAM::ZegoMixStreamConfig mix_config;
    mix_stream_config_helper_.GetMixStreamConfig(mix_config);

    PrintLogToView("准备更新混流配置，混流流id:%s", mix_stream_id_.c_str());

    string mixstream_task_id = mix_stream_id_;
    mix_stream_task_seq_ = MIXSTREAM::MixStreamEx(mixstream_task_id.c_str(), mix_config);

    PrintLogToView("更新混流 返回seq %d", mix_stream_task_seq_);
}

void MixStreamDemo::StopMixStream()
{
    if (!have_mixed_stream_)
    {
        return;
    }

    mix_stream_config_helper_.ClearInputStream();

    MIXSTREAM::ZegoMixStreamConfig mix_config;
    mix_stream_config_helper_.GetMixStreamConfig(mix_config);

    PrintLogToView("准备停止混流配置，混流流id:%s", mix_stream_id_.c_str());

    string mixstream_task_id = mix_stream_id_;
    mix_stream_task_seq_ = MIXSTREAM::MixStreamEx(mixstream_task_id.c_str(), mix_config);

    PrintLogToView("更新停止混流 返回seq %d", mix_stream_task_seq_);

    have_mixed_stream_ = false;
}

void MixStreamDemo::StartPlayingStream(string stream_id, HWND hwnd)
{
    LIVEROOM::StartPlayingStream(stream_id.c_str(), hwnd);
}

void MixStreamDemo::StopPlayingStream(string stream_id)
{
    LIVEROOM::StopPlayingStream(stream_id.c_str());
}

void MixStreamDemo::OnSoundLevelInMixedPlayStream(AV::SoundLevelInfo *volume_list, int list_size)
{
    //PrintLogToView("OnSoundLevelInMixedPlayStream , size = %d", list_size);

    std::vector<MixStreamSoundLevelInfo> sound_levels;
    for (int i = 0;i < list_size; ++i)
    {
        MixStreamSoundLevelInfo sound_level_item;
        sound_level_item.sound_level = volume_list[i].soundLevel;

        char sz_user_id_from_sound_id[64] = {0};
        sprintf(sz_user_id_from_sound_id, "u-%d", volume_list[i].soundLevelID);

        sound_level_item.user_id = sz_user_id_from_sound_id;

        //PrintLogToView("音浪ID:%d，音浪值：%d",volume_list[i].soundLevelID, volume_list[i].soundLevel);

        sound_levels.push_back(sound_level_item);
    }

    if (demo_event_cb_ != nullptr && sound_levels.size() > 0)
    {
        demo_event_cb_->OnSoundLevelUpdate(sound_levels);
    }
}

void MixStreamDemo::OnMixStreamEx(const AV::ZegoMixStreamResultEx& result, const char* mixStreamID, int seq)
{
    PrintLogToView("OnMixStreamEx, error code = %d, seq = %d 混流输出个数:%d, 不存在的输入流个数:%d", result.uiErrorCode, seq, result.nStreamInfoCount, result.nNonExistsStreamCount);
    if (mix_stream_task_seq_ == seq )
    {
        if (result.uiErrorCode == 0)
        {
            if (result.nStreamInfoCount > 0)
            {
                PrintLogToView("混流任务id = %s, 混流成功", mixStreamID);
            }
            else {
                PrintLogToView("混流任务id = %s，没有混流输出，可能混流输入参数有问题。", mixStreamID);
            }
        }
        else {
            PrintLogToView("混流失败，错误码为%d", result.uiErrorCode);
        }
    }
}

void MixStreamDemo::OnMixStreamRelayCDNStateUpdate(const char *mixStreamID, AV::ZegoStreamRelayCDNInfo *statesInfo, unsigned int statesInfoCount)
{
    for (int i = 0;i < statesInfoCount; ++i)
    {
        PrintLogToView("OnMixStreamRelayCDNStateUpdate, state = %d", statesInfo[i].state);
        
    }
}

void MixStreamDemo::OnPlayStateUpdate(int stateCode, const char* pszStreamID)
{
    if (stateCode == 0)
    {
        PrintLogToView("拉流成功");
    }
    else {
        PrintLogToView("拉流状态更新，拉流失败，状态码 = %d",stateCode);
    }
}

void MixStreamDemo::OnPlayQualityUpdate(const char* pszStreamID, ZegoPlayQuality playQuality)
{
    PrintLogToView("OnPlayQualityUpdate, stream id = %s, w = %d, h = %d, fps = %.2f, bitrate = %.2f", pszStreamID, playQuality.width, playQuality.height, playQuality.fps, playQuality.kbps);
}

void MixStreamDemo::OnVideoSizeChanged(const char* pStreamID, int nWidth, int nHeight)
{
    PrintLogToView("OnVideoSizeChanged, stream id = %s, w = %d, h = %d", pStreamID, nWidth, nHeight);
}

void MixStreamDemo::OnAudioDeviceStateChanged(AV::AudioDeviceType deviceType, AV::DeviceInfo *deviceInfo, AV::DeviceState state)
{
    
}

void MixStreamDemo::OnDeviceError(const char* deviceName, int errorCode)
{
    if (errorCode != 0)
    {
        string name = deviceName;
        if (name == "camera")
        {
            PrintLogToView("打开摄像头发生错误，请检查摄像头是否被占用");
        }else if (name == "microphone")
        {
            PrintLogToView("打开麦克风失败");
        }else if (name == "speaker")
        {
            PrintLogToView("打开扬声器失败");
        }
    }   
}

void MixStreamDemo::LogoutRoom()
{
    have_published_ = false;
    LIVEROOM::StopPublishing();
    LIVEROOM::LogoutRoom();
    UpdateLoginEvent(kZGMixStreamLoginEventLogout);
}

void MixStreamDemo::OnInitSDK(int nError)
{
    if (nError == 0)
    {
        PrintLogToView("初始化sdk成功");

        // 初始化成功，切线程然后在开始登录房间
        PostUIData * pdata = CreateUIData();
        pdata->cb_in_ui_fun = [pdata, this]()->void
        {
            PrintLogToView("开始登录，room id = %s", room_id_.c_str());

            UpdateLoginEvent(kZGMixStreamLoginEventLogin);
            start_retry_login_room_timestamp_ = -1;

            COMMON::ZegoRoomRole role = logined_need_publish_ ? COMMON::Anchor: COMMON::Audience;

            is_anchor_ = role == COMMON::Anchor ? true : false;

            PrintLogToView("以%s身份登录", role == COMMON::Anchor ? "主播":"观众");
            if (!LIVEROOM::LoginRoom(room_id_.c_str(), role))
            {
                UpdateLoginEvent(kZGMixStreamLoginEventLoginFailed);
            }

            DestroyUIData(pdata);
        };

        GlobalPostMsgDataToUI(main_hwnd_, pdata);
    }
    else 
    {
        PrintLogToView("初始化失败，错误码为：%d", nError);
        UpdateLoginEvent(kZGMixStreamLoginEventLoginFailed);
    }
}

void MixStreamDemo::OnLoginRoom(int errorCode, const char *pszRoomID, const  COMMON::ZegoStreamInfo *pStreamInfo, unsigned int streamCount)
{
    if (errorCode == 0)
    {
        start_retry_login_room_timestamp_ = -1;

        PrintLogToView("登录成功");
        UpdateLoginEvent(kZGMixStreamLoginEventLoginSuccess);

        if (logined_need_publish_)
        {
            // 登录房间成功，需要推流，切线程在开始推流
            PostUIData * pdata = CreateUIData();
            pdata->cb_in_ui_fun = [pdata, this]()->void
            {
                StartPublish();

                DestroyUIData(pdata);
            };

            GlobalPostMsgDataToUI(main_hwnd_, pdata);
        }

        for (int i = 0; i < streamCount; ++i)
        {
            PrintLogToView("index:%d, szStreamId=%s, szUserId = %s, szExtraInfo = %s", i, pStreamInfo[i].szStreamId, pStreamInfo[i].szUserId, pStreamInfo[i].szExtraInfo);

            ProcessStreamAdded(pStreamInfo[i].szStreamId, pStreamInfo[i].szUserId, pStreamInfo[i].szUserName, pszRoomID);

            string mix_stream_id = "";
            if (mix_stream_config_helper_.ParseExtraInfoForMixStreamId(pStreamInfo[i].szExtraInfo, mix_stream_id))
            {
                if (demo_event_cb_ != nullptr && mix_stream_id != "")
                {
                    ZegoMixStreamUser anchor;
                    anchor.user_id = pStreamInfo[i].szUserId;
                    anchor.user_name = pStreamInfo[i].szUserName;
                    demo_event_cb_->OnMixStreamUpdate(anchor, mix_stream_id);
                }
            }
        }
    }
    else {
        PrintLogToView("登录房间失败，错误码为：%d", errorCode);
        UpdateLoginEvent(kZGMixStreamLoginEventLoginFailed);
        if (auto_retry_login_room_)
        {
            std::thread([this]() {
                std::this_thread::sleep_for(std::chrono::milliseconds(retry_interval_ms_));
                RetryLoginRoom();
            }).detach();
        }
    }
}

void MixStreamDemo::OnLogoutRoom(int errorCode, const char *pszRoomID)
{
    PrintLogToView("已退出登录");
}

void MixStreamDemo::OnKickOut(int reason, const char *pszRoomID, const char* pszCustomReason/*=""*/)
{
    PrintLogToView("被踢掉线，reason = %d", reason);
    UpdateLoginEvent(kZGMixStreamLoginEventKickOut);
}

void MixStreamDemo::OnDisconnect(int errorCode, const char *pszRoomID)
{
    PrintLogToView("已掉线");
    UpdateLoginEvent(kZGMixStreamLoginEventDisconnect);
}

void MixStreamDemo::OnReconnect(int errorCode, const char *pszRoomID)
{
    PrintLogToView("已重连上服务器");
    UpdateLoginEvent(kZGMixStreamLoginEventReconnect);
}

void MixStreamDemo::OnTempBroken(int errorCode, const char *pszRoomID)
{
    PrintLogToView("OnTempBroken，临时掉线，SDK内部正在进行重连，错误码 = %d", errorCode);
    UpdateLoginEvent(kZGMixStreamLoginEventTempBroke);
}

void MixStreamDemo::OnStreamUpdated(ZegoStreamUpdateType type, COMMON::ZegoStreamInfo *pStreamInfo, unsigned int streamCount, const char *pszRoomID)
{
    PrintLogToView("OnStreamUpdated, %s, streamCount = %d, pszRoomID = %s", type == COMMON::StreamAdded ? "流新增" : "流删除", streamCount, pszRoomID);

    for (int i = 0; i < streamCount; ++i)
    {
        PrintLogToView("index:%d , szStreamId = %s, ", i, pStreamInfo[i].szStreamId, pStreamInfo[i].szUserId);
        // 流新增加
        if (type == COMMON::StreamAdded)
        {
            ProcessStreamAdded(pStreamInfo[i].szStreamId, pStreamInfo[i].szUserId, pStreamInfo[i].szUserName, pszRoomID);
        }
        // 流删除的通知
        else if (type == COMMON::StreamDeleted) {

            ProcessStreamDeleted(pStreamInfo[i].szStreamId, pStreamInfo[i].szUserId, pStreamInfo[i].szUserName, pszRoomID);
        }
    }
}

void MixStreamDemo::OnStreamExtraInfoUpdated(COMMON::ZegoStreamInfo *pStreamInfo, unsigned int streamCount, const char *pszRoomID)
{
    PrintLogToView("OnStreamExtraInfoUpdated");
    for (int i = 0;i < streamCount; ++i)
    {
        PrintLogToView("流附加信息更新：%s, %s", pStreamInfo[i].szStreamId, pStreamInfo[i].szExtraInfo);
    }    
}

void MixStreamDemo::OnCustomCommand(int errorCode, int requestSeq, const char *pszRoomID)
{
    
}

void MixStreamDemo::OnRecvCustomCommand(const char *pszUserId, const char *pszUserName, const char *pszContent, const char *pszRoomID)
{
    
}

void MixStreamDemo::OnPublishStateUpdate(int stateCode, const char* pszStreamID, const ZegoPublishingStreamInfo& oStreamInfo)
{
    if (stateCode == 0)
    {
        PrintLogToView("推流成功");
        if (is_anchor_)
        {
            //{"first":true,"rtmp":"%s","Hls":"%s","mixStreamID":""}
            string extra_info = R"({"first":true, "rtmp":)";
            if (oStreamInfo.uiRtmpURLCount > 0)
            {
                extra_info += R"(")";
                extra_info += oStreamInfo.arrRtmpURLs[0];
                extra_info += R"(",)";
            }
            else {
                extra_info += R"(")";
                extra_info += R"(",)";
            }

            extra_info += R"(")";
            extra_info += "Hls";
            extra_info += R"(":)";

            if (oStreamInfo.uiHlsURLCount > 0)
            {
                extra_info += R"(")";
                extra_info += oStreamInfo.arrHlsURLs[0];
                extra_info += R"(",)";
            }
            else {
                extra_info += R"(")";
                extra_info += R"(",)";
            }

            extra_info += R"(")";
            extra_info += "mixStreamID";
            extra_info += R"(":)";
            
            extra_info += R"(")";
            extra_info += mix_stream_id_;
            extra_info += R"(")";
            
            extra_info += "}";

            // 切线程然后在开始发起混流
            PostUIData * pdata = CreateUIData();
            pdata->cb_in_ui_fun = [pdata, extra_info, this]()->void
            {
                StartMixStream();

                PrintLogToView("设置流附加信息: %s", extra_info.c_str());

                bool ret = LIVEROOM::SetPublishStreamExtraInfo(extra_info.c_str());

                DestroyUIData(pdata);
            };

            GlobalPostMsgDataToUI(main_hwnd_, pdata);
        }
    }
    else {
        PrintLogToView("推流状态码更新，推流失败，状态码 = %d", stateCode);
        have_published_ = false;
    }
}

void MixStreamDemo::OnPublishQualityUpdate(const char* pszStreamID, ZegoPublishQuality publishQuality)
{
    
}

void MixStreamDemo::OnCaptureVideoSizeChanged(int nWidth, int nHeight)
{
    PrintLogToView("采集视频帧大小变化，width = %d, height = %d", nWidth, nHeight);
}

void MixStreamDemo::OnCaptureVideoFirstFrame()
{
    PrintLogToView("采集视频首帧到达");
}


void MixStreamDemo::UpdateLoginEvent(ZGMixStreamLoginEvent event)
{
    ZGMixStreamJoinLiveStatus status = login_status_;

    switch (status) {
    case kZGMixStreamLoginStatusLogout: {
        switch (event) {
        case kZGMixStreamLoginEventLogin:
            status = kZGMixStreamLoginStatusStartLogin;
            break;
        case kZGMixStreamLoginEventLogout:
            break;
        default:
            PrintLogToView("ZegoRoomManager error event with current status");
            break;
        }
        break;
    }
    case kZGMixStreamLoginStatusStartLogin: {
        switch (event) {
        case kZGMixStreamLoginEventLoginSuccess:
            status = kZGMixStreamLoginStatusLogin;
            break;
        case kZGMixStreamLoginEventLogout:
        case kZGMixStreamLoginEventLoginFailed:
            status = kZGMixStreamLoginStatusLogout;
            break;
        default:
            PrintLogToView("ZegoRoomManager error event with current status");
            break;
        }
        break;
    }
    case kZGMixStreamLoginStatusLogin: {
        switch (event) {
        case kZGMixStreamLoginEventTempBroke:
            status = kZGMixStreamLoginStatusTempBroken;
            break;
        case kZGMixStreamLoginEventLogout:
        case kZGMixStreamLoginEventKickOut:
        case kZGMixStreamLoginEventDisconnect:
            status = kZGMixStreamLoginStatusLogout;
            break;
        default:
            PrintLogToView("ZegoRoomManager error event with current status");
            break;
        }
        break;
    }
    case kZGMixStreamLoginStatusTempBroken: {
        switch (event) {
        case kZGMixStreamLoginEventTempBroke:
            break;
        case kZGMixStreamLoginEventReconnect:
            status = kZGMixStreamLoginStatusLogin;
            break;
        case kZGMixStreamLoginEventLogout:
        case kZGMixStreamLoginEventKickOut:
        case kZGMixStreamLoginEventDisconnect:
            status = kZGMixStreamLoginStatusLogout;
            break;
        default:
            PrintLogToView("ZegoRoomManager error event with current status");
            break;
        }
        break;
    }
    }

    login_status_ = status;

    if (demo_event_cb_ != nullptr)
    {
        demo_event_cb_->OnLoginEvent(event, login_status_);
    }
}

bool MixStreamDemo::RetryLoginRoom()
{
    if (start_retry_login_room_timestamp_ == -1)
    {
        start_retry_login_room_timestamp_ = ZGHelperInstance()->GetCurTimeStampMs();
    }
    else if (retry_login_room_timeout_ms_ != 0)
    {
        if (ZGHelperInstance()->GetCurTimeStampMs() - start_retry_login_room_timestamp_ > retry_login_room_timeout_ms_)
        {
            PrintLogToView("重连超时,不再重连");
            StopRetryRoomReconnect(kZGMixStreamReconnectStopReasonTimeout);
            return false;
        }
    }
    PrintLogToView("业务层重连中...");

    PrintLogToView("room id = %s", room_id_.c_str());
    UpdateLoginEvent(kZGMixStreamLoginEventLogin);
    BOOL ret = LIVEROOM::LoginRoom(room_id_.c_str(), COMMON::Audience);
    if (!ret)
    {
        UpdateLoginEvent(kZGMixStreamLoginEventLoginFailed);
    }
    return ret;
}

void MixStreamDemo::StopRetryRoomReconnect(ZGMixStreamReconnectStopReason reason)
{
    if (auto_retry_login_room_)
    {
//         if (on_stop_room_reconnect_ != nullptr)
//         {
//             on_stop_room_reconnect_(reason);
//         }
    }
    auto_retry_login_room_ = false;
}

void MixStreamDemo::ProcessStreamAdded(string stream_id, string user_id, string user_name, string room_id)
{
    PlayConfig config;
    ZegoMixStreamUser for_user;

    for_user.user_id = user_id;
    for_user.user_name = user_name;

    if (GetPlayConfigByUser(for_user, config))
    {
        config.stream_id = stream_id;

        UpdatePlayConfigByUser(for_user, config);

        // 已经为该用户配置视图时直接拉该用户的流
        if (config.view_hwnd != nullptr)
        {
            // 切线程然后在开始拉流
            PostUIData * pdata = CreateUIData();
            pdata->cb_in_ui_fun = [pdata, config, this]()->void
            {
                LIVEROOM::StartPlayingStream(config.stream_id.c_str(), config.view_hwnd);
                LIVEROOM::SetViewMode(config.view_mode, config.stream_id.c_str());

                DestroyUIData(pdata);
            };

            GlobalPostMsgDataToUI(main_hwnd_, pdata);

        }
    }
    else {
        PlayConfig play_config;
        ZegoMixStreamUser user;

        user.user_id = user_id;
        user.user_name = user_name;
        play_config.stream_id = stream_id;

        std::lock_guard<std::mutex> lock(play_config_map_mutex_);
        play_map_info_.insert(std::make_pair(user, play_config));
    }


    // 切线程然后更新混流配置
    PostUIData * pdata = CreateUIData();
    pdata->cb_in_ui_fun = [pdata, this]()->void
    {
        UpdateMixStream();

        DestroyUIData(pdata);
    };

    GlobalPostMsgDataToUI(main_hwnd_, pdata);

    if (demo_event_cb_ != nullptr)
    {
        demo_event_cb_->OnUserJoin(for_user);
    }
}

void MixStreamDemo::ProcessStreamDeleted(string stream_id, string user_id, string user_name, string room_id)
{
    RemovePlayConfigByStreamId(stream_id);

    ZegoMixStreamUser for_user;
    for_user.user_id = user_id;
    for_user.user_name = user_name;

    if (demo_event_cb_ != nullptr)
    {
        demo_event_cb_->OnUserLeave(for_user);
    }
}

void MixStreamDemo::UpdatePlayConfigByUser(ZegoMixStreamUser user, const PlayConfig& config)
{
    std::lock_guard<std::mutex> lock(play_config_map_mutex_);

    std::map<ZegoMixStreamUser, PlayConfig>::iterator find_iter = std::find_if(play_map_info_.begin(), play_map_info_.end(), [&](const std::pair<ZegoMixStreamUser, PlayConfig> &item) {
        if (item.first.user_id == user.user_id && item.first.user_name == user.user_name)
        {
            return true;
        }
        else {
            return false;
        }
    });
    if (find_iter != play_map_info_.end())
    {
        find_iter->second = config;
    }
}

void MixStreamDemo::RemovePlayConfigByStreamId(string stream_id)
{
    std::lock_guard<std::mutex> lock(play_config_map_mutex_);

    std::map<ZegoMixStreamUser, PlayConfig>::iterator find_iter = std::find_if(play_map_info_.begin(), play_map_info_.end(), [&](const std::pair<ZegoMixStreamUser, PlayConfig> &item) {
        if (item.second.stream_id == stream_id)
        {
            return true;
        }
        else {
            return false;
        }
    });
    if (find_iter != play_map_info_.end())
    {
        PlayConfig need_stop_play_config;
        need_stop_play_config = find_iter->second;

        // 切线程然后在停止拉流
        PostUIData * pdata = CreateUIData();
        pdata->cb_in_ui_fun = [pdata, need_stop_play_config, this]()->void
        {
            PrintLogToView("停止拉流, stream id = %s", need_stop_play_config.stream_id.c_str());
            
            LIVEROOM::StopPlayingStream(need_stop_play_config.stream_id.c_str());

            DestroyUIData(pdata);
        };

        GlobalPostMsgDataToUI(main_hwnd_, pdata);

        play_map_info_.erase(find_iter);
    }
}


bool MixStreamDemo::GetPlayConfigByUser(ZegoMixStreamUser user, PlayConfig& config)
{
    std::lock_guard<std::mutex> lock(play_config_map_mutex_);

    std::map<ZegoMixStreamUser, PlayConfig>::iterator find_iter = std::find_if(play_map_info_.begin(), play_map_info_.end(), [&](const std::pair<ZegoMixStreamUser, PlayConfig> &item) {
        if (item.first.user_id == user.user_id && item.first.user_name == user.user_name)
        {
            return true;
        }
        else {
            return false;
        }
    });
    if (find_iter != play_map_info_.end())
    {
        config = find_iter->second;
        return true;
    }
    else {
        return false;
    }
}