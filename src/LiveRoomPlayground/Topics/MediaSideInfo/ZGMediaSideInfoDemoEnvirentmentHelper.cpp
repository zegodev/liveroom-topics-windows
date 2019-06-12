#include "stdafx.h"
#include "ZGMediaSideInfoDemoEnvirentmentHelper.h"

#include <assert.h>

#include "AppSupport/ZGManager.h"
#include "AppSupport/ZGHelper.h"
#include "AppSupport/ZGLog.h"
#include "AppSupport/ZGUtilTools.h"


ZGMediaSideInfoDemoEnvirentmentHelper::ZGMediaSideInfoDemoEnvirentmentHelper()
{
    device_uuid_ = ZGHelperInstance()->GetDeviceUUID();
}

ZGMediaSideInfoDemoEnvirentmentHelper::~ZGMediaSideInfoDemoEnvirentmentHelper()
{

}

void ZGMediaSideInfoDemoEnvirentmentHelper::InitSetLoginAndPlayConfig(HelperConfig config)
{
    login_and_play_config_ = config;
}

void ZGMediaSideInfoDemoEnvirentmentHelper::LoginByConfig()
{
    string user_id = device_uuid_ + "_user_id";
    string user_name = device_uuid_ + "_user_name";
    LIVEROOM::SetUser(user_id.c_str(), user_name.c_str());
    LIVEROOM::SetLivePlayerCallback(this);
    LIVEROOM::SetRoomCallback(this);
    ZGManagerInstance()->InitSdk(login_and_play_config_.app_id, login_and_play_config_.sign.data(), login_and_play_config_.sign.size());
    LIVEROOM::LoginRoom(login_and_play_config_.room_id.c_str(), ZEGO::LIVEROOM::Audience, login_and_play_config_.stream_id.c_str());
    SetCurStatus(kZGMediaSideTopicStatus_Starting_Login_Room);
}

void ZGMediaSideInfoDemoEnvirentmentHelper::StopPlay()
{
    LIVEROOM::StopPlayingStream(login_and_play_config_.stream_id.c_str());
}

void ZGMediaSideInfoDemoEnvirentmentHelper::PlayStreamByConfig()
{
    const char* play_params = login_and_play_config_.play_params == "" ? 0 : login_and_play_config_.play_params.c_str();
    bool play_result = LIVEROOM::StartPlayingStream(login_and_play_config_.stream_id.c_str(), login_and_play_config_.video_view, play_params);
    if (play_result)
    {
        SetCurStatus(kZGMediaSideTopicStatus_Starting_Playing);
    }
    else {
        SetCurStatus(kZGMediaSideTopicStatus_None);
    }
}

void ZGMediaSideInfoDemoEnvirentmentHelper::LoginRoom()
{
    ZGManagerInstance()->EnableExternalVideoCapture(nullptr, nullptr);
    LIVEROOM::SetUser(device_uuid_.c_str(), device_uuid_.c_str());
    LIVEROOM::SetLivePublisherCallback(this);
    LIVEROOM::SetLivePlayerCallback(this);
    LIVEROOM::SetRoomCallback(this);
    LIVEROOM::LoginRoom(device_uuid_.c_str(), ZEGO::LIVEROOM::Audience, device_uuid_.c_str());
    SetCurStatus(kZGMediaSideTopicStatus_Starting_Login_Room);
}

void ZGMediaSideInfoDemoEnvirentmentHelper::InitVideoView(void * local_video_view, void * remote_video_view)
{
    local_video_view_ = local_video_view;
    remote_video_view_ = remote_video_view;
}

void ZGMediaSideInfoDemoEnvirentmentHelper::PublishAndPlayWithConfig(ZGMediaSideInfoDemoConfig config)
{
    send_msg_list_.clear();
    recv_msg_list_.clear();

    config_ = config;
    if (config_.only_audio_publish)
    {
        LIVEROOM::EnableCamera(false);
    }
    else {
        if (local_video_view_ != nullptr)
        {
            LIVEROOM::SetPreviewView(local_video_view_);
        }
        else {
            ZGLog("local_video_view_ is null");
        }
        
        LIVEROOM::EnableCamera(true);
        LIVEROOM::StartPreview();
    }

    bool publish_result = LIVEROOM::StartPublishing(device_uuid_.c_str(), device_uuid_.c_str(), 0);
    if (publish_result)
    {
        SetCurStatus(kZGMediaSideTopicStatus_Starting_Publishing);
    }
    else {
        SetCurStatus(kZGMediaSideTopicStatus_None);
    }
}

void ZGMediaSideInfoDemoEnvirentmentHelper::AddSentMsg(string msg)
{
    send_msg_list_.push_back(msg);
}

void ZGMediaSideInfoDemoEnvirentmentHelper::AddRecvMsg(string msg)
{
    recv_msg_list_.push_back(msg);
}

string ZGMediaSideInfoDemoEnvirentmentHelper::CheckSentRecvMsgs()
{
    if (send_msg_list_.size() != recv_msg_list_.size())
    {
        return "COUNT not EQUAL";
    }
    for (size_t i = 0; i < send_msg_list_.size(); i++)
    {
        if (send_msg_list_[i] != recv_msg_list_[i])
        {
            string str_ret = StringFormat("%d, recv:%s - sent:%s", i, recv_msg_list_[i].c_str(), send_msg_list_[i].c_str());
            return str_ret;
        }
    }

    send_msg_list_.clear();
    recv_msg_list_.clear();

    return "ALL the same";
}

string ZGMediaSideInfoDemoEnvirentmentHelper::DescOfStatus(MediaSideInfoStatus status)
{
    string status_str = "";
    switch (status)
    {
    case kZGMediaSideTopicStatus_None:
        status_str = "NONE";
    	break;
    case kZGMediaSideTopicStatus_Starting_Login_Room:
        status_str = "Logining";
        break;
    case kZGMediaSideTopicStatus_Login_OK:
        status_str = "LoginOk";
        break;
    case kZGMediaSideTopicStatus_Starting_Publishing:
        status_str = "Starting Publishing";
        break;
    case kZGMediaSideTopicStatus_Starting_Playing:
        status_str = "Starting Playing";
        break;
    case kZGMediaSideTopicStatus_Ready_For_Messaging:
        status_str = "Ready";
        break;
    default:
        break;
    }

    return status_str;
}

void ZGMediaSideInfoDemoEnvirentmentHelper::OnLoginRoom(int errorCode, const char *pszRoomID, const ZegoStreamInfo *pStreamInfo, unsigned int streamCount)
{
    if (errorCode != 0)
    {
        SetCurStatus(kZGMediaSideTopicStatus_None);
    }
    else {
        SetCurStatus(kZGMediaSideTopicStatus_Login_OK);
    }
}

void ZGMediaSideInfoDemoEnvirentmentHelper::OnKickOut(int reason, const char *pszRoomID)
{
    SetCurStatus(kZGMediaSideTopicStatus_None);
}

void ZGMediaSideInfoDemoEnvirentmentHelper::OnDisconnect(int errorCode, const char *pszRoomID)
{
    SetCurStatus(kZGMediaSideTopicStatus_None);
}

void ZGMediaSideInfoDemoEnvirentmentHelper::OnStreamUpdated(ZegoStreamUpdateType type, ZegoStreamInfo *pStreamInfo, unsigned int streamCount, const char *pszRoomID)
{
}

void ZGMediaSideInfoDemoEnvirentmentHelper::OnStreamExtraInfoUpdated(ZegoStreamInfo *pStreamInfo, unsigned int streamCount, const char *pszRoomID)
{
}

void ZGMediaSideInfoDemoEnvirentmentHelper::OnCustomCommand(int errorCode, int requestSeq, const char *pszRoomID)
{
}

void ZGMediaSideInfoDemoEnvirentmentHelper::OnRecvCustomCommand(const char *pszUserId, const char *pszUserName, const char *pszContent, const char *pszRoomID)
{
}

void ZGMediaSideInfoDemoEnvirentmentHelper::OnPublishStateUpdate(int stateCode, const char* pszStreamID, const ZegoPublishingStreamInfo& oStreamInfo)
{
    if (stateCode == 0)
    {
        void * remote_view = remote_video_view_;
        if (config_.only_audio_publish)
        {
            remote_view = nullptr;
        }

        bool play_result = LIVEROOM::StartPlayingStream(device_uuid_.c_str(), remote_view);
        if (play_result)
        {
            SetCurStatus(kZGMediaSideTopicStatus_Starting_Playing);
        }
        else {
            SetCurStatus(kZGMediaSideTopicStatus_None);
        }
    }
}

void ZGMediaSideInfoDemoEnvirentmentHelper::OnPlayStateUpdate(int stateCode, const char* pszStreamID)
{
    if (stateCode == 0)
    {
        SetCurStatus(kZGMediaSideTopicStatus_Ready_For_Messaging);
    }
    else {
        SetCurStatus(kZGMediaSideTopicStatus_None);
    }
}

void ZGMediaSideInfoDemoEnvirentmentHelper::SetCurStatus(MediaSideInfoStatus s)
{
    ZGENTER_FUN_LOG;

    ZGLog("cur status %d ", s);
    cur_status_ = s;
    if (cur_status_ == kZGMediaSideTopicStatus_None)
    {
        LIVEROOM::LogoutRoom();
    }

    if (status_cb_)
    {
        status_cb_(cur_status_);
    }
    ZGLEAVE_FUN_LOG;
}
