//
// ZGMediaSideInfoDemoEnvirentmentHelper.h
//
// Copyright 2018Äê Zego. All rights reserved.
// 

#ifndef ZGMediaSideInfoDemoEnvirentmentHelper_h__
#define ZGMediaSideInfoDemoEnvirentmentHelper_h__

#include <string>
#include <vector>
#include <functional>

#include "MediaSideInfo/ZGMediaSideInfoDemo.h"
#include "ZegoLiveRoom/LiveRoomCallback-Publisher.h"
#include "ZegoLiveRoom/LiveRoom.h"
#include "ZegoLiveRoom/LiveRoom-Player.h"
#include "ZegoLiveRoom/LiveRoomCallback-Player.h"

using namespace ZEGO;
using std::string;
using std::vector;

using COMMON::ZegoPublishingStreamInfo;
using COMMON::ZegoStreamInfo;
using COMMON::ZegoStreamUpdateType;


enum MediaSideInfoStatus
{
    kZGMediaSideTopicStatus_None = 0,
    kZGMediaSideTopicStatus_Starting_Login_Room,
    kZGMediaSideTopicStatus_Login_OK,
    kZGMediaSideTopicStatus_Starting_Publishing,
    kZGMediaSideTopicStatus_Starting_Playing,
    kZGMediaSideTopicStatus_Ready_For_Messaging,

};

typedef struct HelperConfig
{
    unsigned int app_id;
    vector<unsigned char> sign;

    string room_id;
    string room_name;
    string stream_id;
    string play_params;

    void * video_view;

}HelperConfig;

typedef std::function<void(MediaSideInfoStatus status)> ZGMediaSideInfoDemoEnvirentmentHelperStatusCallBackType;

class ZGMediaSideInfoDemoEnvirentmentHelper : 
    public LIVEROOM::ILivePublisherCallback,
    public LIVEROOM::IRoomCallback,
    public LIVEROOM::ILivePlayerCallback
{
public:
    ZGMediaSideInfoDemoEnvirentmentHelper();
    ~ZGMediaSideInfoDemoEnvirentmentHelper();

    ZGMediaSideInfoDemoEnvirentmentHelperStatusCallBackType status_cb_;

    void InitSetLoginAndPlayConfig(HelperConfig config);

    void LoginByConfig();

    void StopPlay();

    void PlayStreamByConfig();

    void LoginRoom();

    void InitVideoView(void * local_video_view, void * remote_video_view);

    void PublishAndPlayWithConfig(ZGMediaSideInfoDemoConfig config);

    void AddSentMsg(string msg);

    void AddRecvMsg(string msg);

    string CheckSentRecvMsgs();

    string DescOfStatus(MediaSideInfoStatus status);

protected:
    virtual void OnLoginRoom(int errorCode, const char *pszRoomID, const ZegoStreamInfo *pStreamInfo, unsigned int streamCount) override;
    virtual void OnKickOut(int reason, const char *pszRoomID) override;
    virtual void OnDisconnect(int errorCode, const char *pszRoomID) override;
    virtual void OnStreamUpdated(ZegoStreamUpdateType type, ZegoStreamInfo *pStreamInfo, unsigned int streamCount, const char *pszRoomID) override;
    virtual void OnStreamExtraInfoUpdated(ZegoStreamInfo *pStreamInfo, unsigned int streamCount, const char *pszRoomID) override;
    virtual void OnCustomCommand(int errorCode, int requestSeq, const char *pszRoomID) override;
    virtual void OnRecvCustomCommand(const char *pszUserId, const char *pszUserName, const char *pszContent, const char *pszRoomID) override;
    virtual void OnPublishStateUpdate(int stateCode, const char* pszStreamID, const ZegoPublishingStreamInfo& oStreamInfo) override;
    virtual void OnPlayStateUpdate(int stateCode, const char* pszStreamID) override;
private:

    void SetCurStatus(MediaSideInfoStatus s);

    string device_uuid_ = "";

    ZGMediaSideInfoDemoConfig config_;

    void * local_video_view_ = nullptr;
    void * remote_video_view_ = nullptr;

    MediaSideInfoStatus cur_status_;

    vector<string> send_msg_list_;
    vector<string> recv_msg_list_;


    HelperConfig login_and_play_config_;

};




#endif // ZGMediaSideInfoDemoEnvirentmentHelper_h__

