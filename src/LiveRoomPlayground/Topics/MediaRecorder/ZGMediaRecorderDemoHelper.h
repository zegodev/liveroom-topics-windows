//
// ZGMediaRecorderDemoHelper.h
//
// Copyright 2018Äê Zego. All rights reserved.
// 

#ifndef ZGMediaRecorderDemoHelper_h__
#define ZGMediaRecorderDemoHelper_h__

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

enum ZGMediaRecord_PublishStatus
{
    kZGMediaRecord_PublishStatus_None = 0,
    kZGMediaRecord_PublishStatus_Starting_Login_Room,
    kZGMediaRecord_PublishStatus_Login_OK,
    kZGMediaRecord_PublishStatus_Starting_Publishing,
    kZGMediaRecord_PublishStatus_Publishing,
    kZGMediaRecord_PublishStatus_Stoped_Publishing
};

typedef std::function<void(string)> PublishStateObserverType;

class ZGMediaRecorderDemoHelper :
    public LIVEROOM::ILivePublisherCallback,
    public LIVEROOM::IRoomCallback
{
public:
    ZGMediaRecorderDemoHelper();
    ~ZGMediaRecorderDemoHelper();

    void LoginAndPreview(void * local_video_view);

    void StartingPublishing();

    void StopPublishing();

    void SetPublishStateObserver(PublishStateObserverType cb);

    void StartPreview();

protected:

    void LoginRoom();

    void InitVideoView(void * local_video_view);

    string DescOfStatus(ZGMediaRecord_PublishStatus status);

    virtual void OnLoginRoom(int errorCode, const char *pszRoomID, const ZegoStreamInfo *pStreamInfo, unsigned int streamCount) override;
    virtual void OnKickOut(int reason, const char *pszRoomID) override;
    virtual void OnDisconnect(int errorCode, const char *pszRoomID) override;
    virtual void OnStreamUpdated(ZegoStreamUpdateType type, ZegoStreamInfo *pStreamInfo, unsigned int streamCount, const char *pszRoomID) override;
    virtual void OnStreamExtraInfoUpdated(ZegoStreamInfo *pStreamInfo, unsigned int streamCount, const char *pszRoomID) override;
    virtual void OnCustomCommand(int errorCode, int requestSeq, const char *pszRoomID) override;
    virtual void OnRecvCustomCommand(const char *pszUserId, const char *pszUserName, const char *pszContent, const char *pszRoomID) override;
    virtual void OnPublishStateUpdate(int stateCode, const char* pszStreamID, const ZegoPublishingStreamInfo& oStreamInfo) override;
private:

    void SetCurStatus(ZGMediaRecord_PublishStatus s);

    string stream_id_ = "";

    void * local_video_view_ = nullptr;

    ZGMediaRecord_PublishStatus cur_status_;

    PublishStateObserverType publish_observer_;


};




#endif // ZGMediaRecorderDemoHelper_h__

