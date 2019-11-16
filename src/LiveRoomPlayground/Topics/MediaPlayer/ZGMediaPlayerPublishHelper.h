//
// ZGMediaPlayerPublishHelper.h
//
// Copyright 2018Äê Zego. All rights reserved.
//

#ifndef ZGMediaPlayerPublishHelper_h__
#define ZGMediaPlayerPublishHelper_h__

#include <functional>
#include <string>

#include "ZegoLiveRoom/LiveRoomCallback-Publisher.h"
#include "ZegoLiveRoom/LiveRoomCallBack.h"

#include "AppSupport/ZGUIOperator.h"
using std::string;

using namespace ZEGO;

using COMMON::ZegoPublishingStreamInfo;
using COMMON::ZegoStreamInfo;
using COMMON::ZegoStreamUpdateType;

typedef std::function<void(string)> PublishStateObserverType;

class ZGMediaPlayerPublishHelper :
    public LIVEROOM::ILivePublisherCallback,
    public LIVEROOM::IRoomCallback
{
public:
    ZGMediaPlayerPublishHelper();
    ~ZGMediaPlayerPublishHelper();

    void StartPublishing();
    void SetPublishStateObserver(PublishStateObserverType cb);

    void InitMainHwnd(HWND hwnd);
protected:
    virtual void OnLoginRoom(int errorCode, const char *pszRoomID, const ZegoStreamInfo *pStreamInfo, unsigned int streamCount) override;
    virtual void OnKickOut(int reason, const char *pszRoomID, const char* pszCustomReason="") override;
    virtual void OnDisconnect(int errorCode, const char *pszRoomID) override;
    virtual void OnStreamUpdated(ZegoStreamUpdateType type, ZegoStreamInfo *pStreamInfo, unsigned int streamCount, const char *pszRoomID) override;
    virtual void OnStreamExtraInfoUpdated(ZegoStreamInfo *pStreamInfo, unsigned int streamCount, const char *pszRoomID) override;
    virtual void OnCustomCommand(int errorCode, int requestSeq, const char *pszRoomID) override;
    virtual void OnRecvCustomCommand(const char *pszUserId, const char *pszUserName, const char *pszContent, const char *pszRoomID) override;
    virtual void OnPublishStateUpdate(int stateCode, const char* pszStreamID, const ZegoPublishingStreamInfo& oStreamInfo) override;

private:
    PublishStateObserverType publish_state_observer_;
    HWND main_hwnd_ = nullptr;
};


#endif // ZGMediaPlayerPublishHelper_h__


