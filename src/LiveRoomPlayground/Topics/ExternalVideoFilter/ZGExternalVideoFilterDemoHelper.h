//
// ZGExternalVideoFilterDemoHelper.h
//
// Copyright 2018Äê Zego. All rights reserved.
// 

#ifndef ZGExternalVideoFilterDemoHelper_h__
#define ZGExternalVideoFilterDemoHelper_h__

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

enum ZGExternalVideoFilter_PublishStatus
{
    kZGExternalVideoFilter_PublishStatus_None = 0,
    kZGExternalVideoFilter_PublishStatus_Starting_Login_Room,
    kZGExternalVideoFilter_PublishStatus_Login_OK,
    kZGExternalVideoFilter_PublishStatus_Starting_Publishing,
    kZGExternalVideoFilter_PublishStatus_Publishing,
    kZGExternalVideoFilter_PublishStatus_Stoped_Publishing
};

typedef std::function<void(string)> PublishStateObserverType;

class ZGExternalVideoFilterDemoHelper :
    public LIVEROOM::ILivePublisherCallback,
    public LIVEROOM::IRoomCallback
{
public:
    ZGExternalVideoFilterDemoHelper();
    ~ZGExternalVideoFilterDemoHelper();

    //void Login();

    void LoginRoom();

    void StartPreview(HWND hwnd = nullptr);

    void StartPublishing();

    void StopPublishing();

    void SetPublishStateObserver(PublishStateObserverType cb);

    void StartPlaying(HWND hwnd = nullptr);

protected:

    string DescOfStatus(ZGExternalVideoFilter_PublishStatus status);

    virtual void OnLoginRoom(int errorCode, const char *pszRoomID, const ZegoStreamInfo *pStreamInfo, unsigned int streamCount) override;
    virtual void OnKickOut(int reason, const char *pszRoomID, const char* pszCustomReason="") override;
    virtual void OnDisconnect(int errorCode, const char *pszRoomID) override;
    virtual void OnStreamUpdated(ZegoStreamUpdateType type, ZegoStreamInfo *pStreamInfo, unsigned int streamCount, const char *pszRoomID) override;
    virtual void OnStreamExtraInfoUpdated(ZegoStreamInfo *pStreamInfo, unsigned int streamCount, const char *pszRoomID) override;
    virtual void OnCustomCommand(int errorCode, int requestSeq, const char *pszRoomID) override;
    virtual void OnRecvCustomCommand(const char *pszUserId, const char *pszUserName, const char *pszContent, const char *pszRoomID) override;
    virtual void OnPublishStateUpdate(int stateCode, const char* pszStreamID, const ZegoPublishingStreamInfo& oStreamInfo) override;
private:

    void SetCurStatus(ZGExternalVideoFilter_PublishStatus s);

    string stream_id_ = "";

    ZGExternalVideoFilter_PublishStatus cur_status_;

    PublishStateObserverType publish_observer_;


};




#endif // ZGExternalVideoFilterDemoHelper_h__

