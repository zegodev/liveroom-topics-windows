#include "stdafx.h"
#include "ZGExternalVideoFilterDemoHelper.h"

#include <assert.h>

#include "AppSupport/ZGManager.h"
#include "AppSupport/ZGHelper.h"
#include "AppSupport/ZGLog.h"
#include "AppSupport/ZGUtilTools.h"


ZGExternalVideoFilterDemoHelper::ZGExternalVideoFilterDemoHelper()
{
    stream_id_ = ZGHelperInstance()->GetDeviceUUID();
}

ZGExternalVideoFilterDemoHelper::~ZGExternalVideoFilterDemoHelper()
{

}

void ZGExternalVideoFilterDemoHelper::LoginRoom()
{
    LIVEROOM::SetUser(stream_id_.c_str(), stream_id_.c_str());
    LIVEROOM::SetLivePublisherCallback(this);
    LIVEROOM::SetRoomCallback(this);
    LIVEROOM::LoginRoom(stream_id_.c_str(), ZEGO::LIVEROOM::Audience, stream_id_.c_str());
    SetCurStatus(kZGExternalVideoFilter_PublishStatus_Starting_Login_Room);
}

void ZGExternalVideoFilterDemoHelper::StartPreview(HWND hwnd)
{
    if (hwnd != nullptr)
    {
        LIVEROOM::SetPreviewView(hwnd);
    }

    LIVEROOM::StartPreview();
}

void ZGExternalVideoFilterDemoHelper::StartPublishing()
{
    bool publish_result = LIVEROOM::StartPublishing(stream_id_.c_str(), stream_id_.c_str(), 0);
    if (publish_result)
    {
        SetCurStatus(kZGExternalVideoFilter_PublishStatus_Starting_Publishing);
    }
    else {
        SetCurStatus(kZGExternalVideoFilter_PublishStatus_None);
    }
}

void ZGExternalVideoFilterDemoHelper::StopPublishing()
{
    LIVEROOM::StopPublishing();
    SetCurStatus(kZGExternalVideoFilter_PublishStatus_Stoped_Publishing);
}

void ZGExternalVideoFilterDemoHelper::SetPublishStateObserver(PublishStateObserverType cb)
{
    publish_observer_ = cb;
}

void ZGExternalVideoFilterDemoHelper::StartPlaying(HWND hwnd)
{
    LIVEROOM::StartPlayingStream(stream_id_.c_str(), hwnd);
}

string ZGExternalVideoFilterDemoHelper::DescOfStatus(ZGExternalVideoFilter_PublishStatus status)
{
    string status_str = "";
    switch (status)
    {
    case kZGExternalVideoFilter_PublishStatus_None:
        status_str = "NONE";
        break;
    case kZGExternalVideoFilter_PublishStatus_Starting_Login_Room:
        status_str = "Logining";
        break;
    case kZGExternalVideoFilter_PublishStatus_Login_OK:
        status_str = "LoginOk";
        break;
    case kZGExternalVideoFilter_PublishStatus_Starting_Publishing:
        status_str = "Starting Publishing";
        break;
    case kZGExternalVideoFilter_PublishStatus_Publishing:
        status_str = "Publishing";
        break;
    case kZGExternalVideoFilter_PublishStatus_Stoped_Publishing:
        status_str = "Stoped Publishing";
        break;
    default:
        break;
    }

    return status_str;
}

void ZGExternalVideoFilterDemoHelper::OnLoginRoom(int errorCode, const char *pszRoomID, const ZegoStreamInfo *pStreamInfo, unsigned int streamCount)
{
    if (errorCode != 0)
    {
        SetCurStatus(kZGExternalVideoFilter_PublishStatus_None);
    }
    else {
        SetCurStatus(kZGExternalVideoFilter_PublishStatus_Login_OK);
    }
}

void ZGExternalVideoFilterDemoHelper::OnKickOut(int reason, const char *pszRoomID)
{
    SetCurStatus(kZGExternalVideoFilter_PublishStatus_None);
}

void ZGExternalVideoFilterDemoHelper::OnDisconnect(int errorCode, const char *pszRoomID)
{
    SetCurStatus(kZGExternalVideoFilter_PublishStatus_None);
}

void ZGExternalVideoFilterDemoHelper::OnStreamUpdated(ZegoStreamUpdateType type, ZegoStreamInfo *pStreamInfo, unsigned int streamCount, const char *pszRoomID)
{
}

void ZGExternalVideoFilterDemoHelper::OnStreamExtraInfoUpdated(ZegoStreamInfo *pStreamInfo, unsigned int streamCount, const char *pszRoomID)
{
}

void ZGExternalVideoFilterDemoHelper::OnCustomCommand(int errorCode, int requestSeq, const char *pszRoomID)
{
}

void ZGExternalVideoFilterDemoHelper::OnRecvCustomCommand(const char *pszUserId, const char *pszUserName, const char *pszContent, const char *pszRoomID)
{
}

void ZGExternalVideoFilterDemoHelper::OnPublishStateUpdate(int stateCode, const char* pszStreamID, const ZegoPublishingStreamInfo& oStreamInfo)
{
    if (stateCode == 0)
    {
        SetCurStatus(kZGExternalVideoFilter_PublishStatus_Publishing);
    }
}

void ZGExternalVideoFilterDemoHelper::SetCurStatus(ZGExternalVideoFilter_PublishStatus s)
{
    ZGENTER_FUN_LOG;

    ZGLog("publish status  %s -> %s", DescOfStatus(cur_status_).c_str(), DescOfStatus(s).c_str());
    cur_status_ = s;
    if (cur_status_ == kZGExternalVideoFilter_PublishStatus_None)
    {
        LIVEROOM::LogoutRoom();
    }

    if (publish_observer_)
    {
        publish_observer_(DescOfStatus(s));
    }

    ZGLEAVE_FUN_LOG;
}