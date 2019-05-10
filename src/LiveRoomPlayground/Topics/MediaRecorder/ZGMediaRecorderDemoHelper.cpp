#include "stdafx.h"
#include "ZGMediaRecorderDemoHelper.h"

#include <assert.h>

#include "AppSupport/ZGManager.h"
#include "AppSupport/ZGHelper.h"
#include "AppSupport/ZGLog.h"
#include "AppSupport/ZGUtilTools.h"


ZGMediaRecorderDemoHelper::ZGMediaRecorderDemoHelper()
{
    stream_id_ = ZGHelperInstance()->GetDeviceUUID();
}

ZGMediaRecorderDemoHelper::~ZGMediaRecorderDemoHelper()
{

}

void ZGMediaRecorderDemoHelper::LoginAndPreview(void * local_video_view)
{
    InitVideoView(local_video_view);
    LoginRoom();
}

void ZGMediaRecorderDemoHelper::LoginRoom()
{
    ZGManagerInstance()->EnableExternalVideoCapture(nullptr, nullptr);
    LIVEROOM::SetUser(stream_id_.c_str(), stream_id_.c_str());
    LIVEROOM::SetLivePublisherCallback(this);
    LIVEROOM::SetRoomCallback(this);
    // 开启回音消除
    LIVEROOM::EnableAEC(true);
    // 开启噪音抑制
    LIVEROOM::EnableNoiseSuppress(true);

    LIVEROOM::LoginRoom(stream_id_.c_str(), ZEGO::LIVEROOM::Audience, stream_id_.c_str());
    SetCurStatus(kZGMediaRecord_PublishStatus_Starting_Login_Room);
}

void ZGMediaRecorderDemoHelper::InitVideoView(void * local_video_view)
{
    local_video_view_ = local_video_view;
}

void ZGMediaRecorderDemoHelper::StartPreview()
{
    if (local_video_view_ != nullptr)
    {
        LIVEROOM::SetPreviewView(local_video_view_);
    }

    LIVEROOM::StartPreview();
}

void ZGMediaRecorderDemoHelper::StartingPublishing()
{
    bool publish_result = LIVEROOM::StartPublishing(stream_id_.c_str(), stream_id_.c_str(), 0);
    if (publish_result)
    {
        SetCurStatus(kZGMediaRecord_PublishStatus_Starting_Publishing);
    }
    else {
        SetCurStatus(kZGMediaRecord_PublishStatus_None);
    }
}

void ZGMediaRecorderDemoHelper::StopPublishing()
{
    LIVEROOM::StopPublishing();
    SetCurStatus(kZGMediaRecord_PublishStatus_Stoped_Publishing);
}

void ZGMediaRecorderDemoHelper::SetPublishStateObserver(PublishStateObserverType cb)
{
    publish_observer_ = cb;
}

string ZGMediaRecorderDemoHelper::DescOfStatus(ZGMediaRecord_PublishStatus status)
{
    string status_str = "";
    switch (status)
    {
    case kZGMediaRecord_PublishStatus_None:
        status_str = "NONE";
        break;
    case kZGMediaRecord_PublishStatus_Starting_Login_Room:
        status_str = "Logining";
        break;
    case kZGMediaRecord_PublishStatus_Login_OK:
        status_str = "LoginOk";
        break;
    case kZGMediaRecord_PublishStatus_Starting_Publishing:
        status_str = "Starting Publishing";
        break;
    case kZGMediaRecord_PublishStatus_Publishing:
        status_str = "Publishing";
        break;
    case kZGMediaRecord_PublishStatus_Stoped_Publishing:
        status_str = "Stoped Publishing";
        break;
    default:
        break;
    }

    return status_str;
}

void ZGMediaRecorderDemoHelper::OnLoginRoom(int errorCode, const char *pszRoomID, const ZegoStreamInfo *pStreamInfo, unsigned int streamCount)
{
    if (errorCode != 0)
    {
        SetCurStatus(kZGMediaRecord_PublishStatus_None);
    }
    else {
        SetCurStatus(kZGMediaRecord_PublishStatus_Login_OK);
        
    }
}

void ZGMediaRecorderDemoHelper::OnKickOut(int reason, const char *pszRoomID)
{
    SetCurStatus(kZGMediaRecord_PublishStatus_None);
}

void ZGMediaRecorderDemoHelper::OnDisconnect(int errorCode, const char *pszRoomID)
{
    SetCurStatus(kZGMediaRecord_PublishStatus_None);
}

void ZGMediaRecorderDemoHelper::OnStreamUpdated(ZegoStreamUpdateType type, ZegoStreamInfo *pStreamInfo, unsigned int streamCount, const char *pszRoomID)
{
}

void ZGMediaRecorderDemoHelper::OnStreamExtraInfoUpdated(ZegoStreamInfo *pStreamInfo, unsigned int streamCount, const char *pszRoomID)
{
}

void ZGMediaRecorderDemoHelper::OnCustomCommand(int errorCode, int requestSeq, const char *pszRoomID)
{
}

void ZGMediaRecorderDemoHelper::OnRecvCustomCommand(const char *pszUserId, const char *pszUserName, const char *pszContent, const char *pszRoomID)
{
}

void ZGMediaRecorderDemoHelper::OnPublishStateUpdate(int stateCode, const char* pszStreamID, const ZegoPublishingStreamInfo& oStreamInfo)
{
    if (stateCode == 0)
    {
        assert(cur_status_ == kZGMediaRecord_PublishStatus_Starting_Publishing);
        SetCurStatus(kZGMediaRecord_PublishStatus_Publishing);
    }
}

void ZGMediaRecorderDemoHelper::SetCurStatus(ZGMediaRecord_PublishStatus s)
{
    ZGENTER_FUN_LOG;

    ZGLog("cur status %d ", s);
    cur_status_ = s;
    if (cur_status_ == kZGMediaRecord_PublishStatus_None)
    {
        LIVEROOM::LogoutRoom();
    }

    if (publish_observer_)
    {
        publish_observer_(DescOfStatus(s));
    }

    ZGLEAVE_FUN_LOG;
}