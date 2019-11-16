//
// ZGExternalVideoRenderDemoHelper.h
//
// Copyright 2018Äê Zego. All rights reserved.
// 

#ifndef ZGExternalVideoRenderDemoHelper_h__
#define ZGExternalVideoRenderDemoHelper_h__

#include <string>
#include <vector>
#include <functional>

#include "ZegoLiveRoom/LiveRoomCallback-Publisher.h"
#include "ZegoLiveRoom/LiveRoom.h"

using namespace ZEGO;
using std::string;
using std::vector;

using COMMON::ZegoPublishingStreamInfo;
using COMMON::ZegoStreamInfo;
using COMMON::ZegoStreamUpdateType;

enum ZGExternalVideoRender_PublishStatus
{
    kZGExternalVideoRender_PublishStatus_None = 0,
    kZGExternalVideoRender_PublishStatus_Starting_Login_Room,
    kZGExternalVideoRender_PublishStatus_Login_OK,
    kZGExternalVideoRender_PublishStatus_Starting_Publishing,
    kZGExternalVideoRender_PublishStatus_Publishing,
    kZGExternalVideoRender_PublishStatus_Stoped_Publishing
};

typedef std::function<void(string)> PublishStateObserverType;

class ZGExternalVideoRenderDemoHelper :
    public LIVEROOM::ILivePublisherCallback,
    public LIVEROOM::IRoomCallback
{
public:
    ZGExternalVideoRenderDemoHelper();
    ~ZGExternalVideoRenderDemoHelper();

    //void Login();

    void LoginRoom();

    void StartPreview(HWND hwnd = nullptr);

    void StartPublishing();

    void StopPublishing();

    void SetPublishStateObserver(PublishStateObserverType cb);

    void StartPlaying(HWND hwnd = nullptr);

protected:

    string DescOfStatus(ZGExternalVideoRender_PublishStatus status);

    virtual void OnLoginRoom(int errorCode, const char *pszRoomID, const ZegoStreamInfo *pStreamInfo, unsigned int streamCount) override;
    virtual void OnKickOut(int reason, const char *pszRoomID, const char* pszCustomReason="") override;
    virtual void OnDisconnect(int errorCode, const char *pszRoomID) override;
    virtual void OnStreamUpdated(ZegoStreamUpdateType type, ZegoStreamInfo *pStreamInfo, unsigned int streamCount, const char *pszRoomID) override;
    virtual void OnStreamExtraInfoUpdated(ZegoStreamInfo *pStreamInfo, unsigned int streamCount, const char *pszRoomID) override;
    virtual void OnCustomCommand(int errorCode, int requestSeq, const char *pszRoomID) override;
    virtual void OnRecvCustomCommand(const char *pszUserId, const char *pszUserName, const char *pszContent, const char *pszRoomID) override;
    virtual void OnPublishStateUpdate(int stateCode, const char* pszStreamID, const ZegoPublishingStreamInfo& oStreamInfo) override;
private:

    void SetCurStatus(ZGExternalVideoRender_PublishStatus s);

    string stream_id_ = "";

    ZGExternalVideoRender_PublishStatus cur_status_;

    PublishStateObserverType publish_observer_;


};




#endif // ZGExternalVideoRenderDemoHelper_h__

