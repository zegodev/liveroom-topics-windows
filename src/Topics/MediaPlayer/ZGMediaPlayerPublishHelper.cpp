#include "stdafx.h"
#include "ZGMediaPlayerPublishHelper.h"

#include "ZegoLiveRoom/LiveRoom.h"
#include "ZegoLiveRoom/LiveRoom-Publisher.h"
#include "AppSupport/ZGHelper.h"
#include "AppSupport/ZGLog.h"
#include "AppSupport/ZGUtilTools.h"

using namespace ZEGO;

ZGMediaPlayerPublishHelper::ZGMediaPlayerPublishHelper()
{
}

ZGMediaPlayerPublishHelper::~ZGMediaPlayerPublishHelper()
{
}

void ZGMediaPlayerPublishHelper::StartPublishing()
{
    LIVEROOM::SetUser(ZGHelperInstance()->GetDeviceUUID().c_str(), ZGHelperInstance()->GetDeviceUUID().c_str());
    LIVEROOM::SetLivePublisherCallback(this);
    LIVEROOM::SetRoomCallback(this);
    LIVEROOM::LoginRoom(ZGHelperInstance()->GetDeviceUUID().c_str(), ZEGO::LIVEROOM::Audience, ZGHelperInstance()->GetDeviceUUID().c_str());
}


void ZGMediaPlayerPublishHelper::SetPublishStateObserver(PublishStateObserverType cb)
{
    publish_state_observer_ = cb;
}

void ZGMediaPlayerPublishHelper::OnLoginRoom(int errorCode, const char *pszRoomID, const ZegoStreamInfo *pStreamInfo, unsigned int streamCount)
{
    ZGENTER_FUN_LOG;
    if (errorCode == 0)
    {
        LIVEROOM::StartPublishing(ZGHelperInstance()->GetDeviceUUID().c_str(), ZGHelperInstance()->GetDeviceUUID().c_str(), 0);
    }
}


void ZGMediaPlayerPublishHelper::OnPublishStateUpdate(int stateCode, const char* pszStreamID, const ZegoPublishingStreamInfo& oStreamInfo)
{
    ZGENTER_FUN_LOG;
    //ZGLog("%s %s %s %s", pszStreamID, oStreamInfo.arrRtmpURLs[0], oStreamInfo.arrFlvURLs[0], oStreamInfo.arrHlsURLs[0]);
    if (publish_state_observer_)
    {
        string str;
        if (oStreamInfo.uiRtmpURLCount > 0)
        {
            str += oStreamInfo.arrRtmpURLs[0];
            str += "\r\n";
        }
        if (oStreamInfo.uiFlvURLCount > 0)
        {
            str += oStreamInfo.arrFlvURLs[0];
            str += "\r\n";
        }
        if (oStreamInfo.uiHlsURLCount > 0)
        {
            str += oStreamInfo.arrHlsURLs[0];
            str += "\r\n";
        }
        publish_state_observer_(str);
    }
}


void ZGMediaPlayerPublishHelper::OnKickOut(int reason, const char *pszRoomID)
{
    ZGENTER_FUN_LOG;
}

void ZGMediaPlayerPublishHelper::OnDisconnect(int errorCode, const char *pszRoomID)
{
    ZGENTER_FUN_LOG;
}

void ZGMediaPlayerPublishHelper::OnStreamUpdated(ZegoStreamUpdateType type, ZegoStreamInfo *pStreamInfo, unsigned int streamCount, const char *pszRoomID)
{
    ZGENTER_FUN_LOG;
}

void ZGMediaPlayerPublishHelper::OnStreamExtraInfoUpdated(ZegoStreamInfo *pStreamInfo, unsigned int streamCount, const char *pszRoomID)
{
    ZGENTER_FUN_LOG;
}

void ZGMediaPlayerPublishHelper::OnCustomCommand(int errorCode, int requestSeq, const char *pszRoomID)
{
    ZGENTER_FUN_LOG;

}

void ZGMediaPlayerPublishHelper::OnRecvCustomCommand(const char *pszUserId, const char *pszUserName, const char *pszContent, const char *pszRoomID)
{
    ZGENTER_FUN_LOG;
}
