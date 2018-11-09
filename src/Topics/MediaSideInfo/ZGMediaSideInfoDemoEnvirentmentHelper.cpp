#include "stdafx.h"
#include "ZGMediaSideInfoDemoEnvirentmentHelper.h"

#include <assert.h>

#include "AppSupport/ZGManager.h"
#include "AppSupport/ZGHelper.h"
#include "AppSupport/ZGLog.h"
#include "AppSupport/ZGUtilTools.h"


ZGMediaSideInfoDemoEnvirentmentHelper::ZGMediaSideInfoDemoEnvirentmentHelper()
{
    stream_id_ = ZGHelperInstance()->GetDeviceUUID();
}

ZGMediaSideInfoDemoEnvirentmentHelper::~ZGMediaSideInfoDemoEnvirentmentHelper()
{

}

void ZGMediaSideInfoDemoEnvirentmentHelper::LoginRoom()
{
    ZGManagerInstance()->EnableExternalVideoCapture(nullptr, nullptr);
    LIVEROOM::SetUser(stream_id_.c_str(), stream_id_.c_str());
    LIVEROOM::SetLivePublisherCallback(this);
    LIVEROOM::SetLivePlayerCallback(this);
    LIVEROOM::SetRoomCallback(this);
    LIVEROOM::LoginRoom(stream_id_.c_str(), ZEGO::LIVEROOM::Audience, stream_id_.c_str());
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

    bool publish_result = LIVEROOM::StartPublishing(stream_id_.c_str(), stream_id_.c_str(), 0);
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
        assert(cur_status_ == kZGMediaSideTopicStatus_Starting_Publishing);
        void * remote_view = remote_video_view_;
        if (config_.only_audio_publish)
        {
            remote_view = nullptr;
        }

        bool play_result = LIVEROOM::StartPlayingStream(stream_id_.c_str(), remote_view);
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
        assert(cur_status_ == kZGMediaSideTopicStatus_Starting_Playing);

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
