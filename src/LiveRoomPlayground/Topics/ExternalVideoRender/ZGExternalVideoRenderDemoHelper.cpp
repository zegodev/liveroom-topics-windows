#include "stdafx.h"
#include "ZGExternalVideoRenderDemoHelper.h"

#include <assert.h>

#include "AppSupport/ZGManager.h"
#include "AppSupport/ZGHelper.h"
#include "AppSupport/ZGLog.h"
#include "AppSupport/ZGUtilTools.h"


ZGExternalVideoRenderDemoHelper::ZGExternalVideoRenderDemoHelper()
{
    stream_id_ = ZGHelperInstance()->GetDeviceUUID();
}

ZGExternalVideoRenderDemoHelper::~ZGExternalVideoRenderDemoHelper()
{

}

// void ZGExternalVideoRenderDemoHelper::Login(void * local_video_view, void * remote_video_view)
// {
//     local_video_view_ = local_video_view;
//     remote_video_view_ = remote_video_view;
// 
//     LoginRoom();
// }

void ZGExternalVideoRenderDemoHelper::LoginRoom()
{
    LIVEROOM::SetUser(stream_id_.c_str(), stream_id_.c_str());
    LIVEROOM::SetLivePublisherCallback(this);
    LIVEROOM::SetRoomCallback(this);
    LIVEROOM::LoginRoom(stream_id_.c_str(), ZEGO::LIVEROOM::Audience, stream_id_.c_str());
    SetCurStatus(kZGExternalVideoRender_PublishStatus_Starting_Login_Room);
}

void ZGExternalVideoRenderDemoHelper::StartPreview(HWND hwnd)
{
    if (hwnd != nullptr)
    {
        LIVEROOM::SetPreviewView(hwnd);
    }

    LIVEROOM::StartPreview();
}

void ZGExternalVideoRenderDemoHelper::StartPublishing()
{
    bool publish_result = LIVEROOM::StartPublishing(stream_id_.c_str(), stream_id_.c_str(), 0);
    if (publish_result)
    {
        SetCurStatus(kZGExternalVideoRender_PublishStatus_Starting_Publishing);
    }
    else {
        SetCurStatus(kZGExternalVideoRender_PublishStatus_None);
    }
}

void ZGExternalVideoRenderDemoHelper::StopPublishing()
{
    LIVEROOM::StopPublishing();
    SetCurStatus(kZGExternalVideoRender_PublishStatus_Stoped_Publishing);
}

void ZGExternalVideoRenderDemoHelper::SetPublishStateObserver(PublishStateObserverType cb)
{
    publish_observer_ = cb;
}

void ZGExternalVideoRenderDemoHelper::StartPlaying(HWND hwnd)
{
    LIVEROOM::StartPlayingStream(stream_id_.c_str(), hwnd);
}

string ZGExternalVideoRenderDemoHelper::DescOfStatus(ZGExternalVideoRender_PublishStatus status)
{
    string status_str = "";
    switch (status)
    {
    case kZGExternalVideoRender_PublishStatus_None:
        status_str = "NONE";
        break;
    case kZGExternalVideoRender_PublishStatus_Starting_Login_Room:
        status_str = "Logining";
        break;
    case kZGExternalVideoRender_PublishStatus_Login_OK:
        status_str = "LoginOk";
        break;
    case kZGExternalVideoRender_PublishStatus_Starting_Publishing:
        status_str = "Starting Publishing";
        break;
    case kZGExternalVideoRender_PublishStatus_Publishing:
        status_str = "Publishing";
        break;
    case kZGExternalVideoRender_PublishStatus_Stoped_Publishing:
        status_str = "Stoped Publishing";
        break;
    default:
        break;
    }

    return status_str;
}

void ZGExternalVideoRenderDemoHelper::OnLoginRoom(int errorCode, const char *pszRoomID, const ZegoStreamInfo *pStreamInfo, unsigned int streamCount)
{
    if (errorCode != 0)
    {
        SetCurStatus(kZGExternalVideoRender_PublishStatus_None);
    }
    else {
        SetCurStatus(kZGExternalVideoRender_PublishStatus_Login_OK);
    }
}

void ZGExternalVideoRenderDemoHelper::OnKickOut(int reason, const char *pszRoomID)
{
    SetCurStatus(kZGExternalVideoRender_PublishStatus_None);
}

void ZGExternalVideoRenderDemoHelper::OnDisconnect(int errorCode, const char *pszRoomID)
{
    SetCurStatus(kZGExternalVideoRender_PublishStatus_None);
}

void ZGExternalVideoRenderDemoHelper::OnStreamUpdated(ZegoStreamUpdateType type, ZegoStreamInfo *pStreamInfo, unsigned int streamCount, const char *pszRoomID)
{
}

void ZGExternalVideoRenderDemoHelper::OnStreamExtraInfoUpdated(ZegoStreamInfo *pStreamInfo, unsigned int streamCount, const char *pszRoomID)
{
}

void ZGExternalVideoRenderDemoHelper::OnCustomCommand(int errorCode, int requestSeq, const char *pszRoomID)
{
}

void ZGExternalVideoRenderDemoHelper::OnRecvCustomCommand(const char *pszUserId, const char *pszUserName, const char *pszContent, const char *pszRoomID)
{
}

void ZGExternalVideoRenderDemoHelper::OnPublishStateUpdate(int stateCode, const char* pszStreamID, const ZegoPublishingStreamInfo& oStreamInfo)
{
    if (stateCode == 0)
    {
        assert(cur_status_ == kZGExternalVideoRender_PublishStatus_Starting_Publishing);
        SetCurStatus(kZGExternalVideoRender_PublishStatus_Publishing);
    }
}

void ZGExternalVideoRenderDemoHelper::SetCurStatus(ZGExternalVideoRender_PublishStatus s)
{
    ZGENTER_FUN_LOG;

    ZGLog("publish status  %s -> %s", DescOfStatus(cur_status_).c_str(), DescOfStatus(s).c_str());
    cur_status_ = s;
    if (cur_status_ == kZGExternalVideoRender_PublishStatus_None)
    {
        LIVEROOM::LogoutRoom();
    }

    if (publish_observer_)
    {
        publish_observer_(DescOfStatus(s));
    }

    ZGLEAVE_FUN_LOG;
}