#pragma once

#include <string>

#include "ZegoLiveRoom/LiveRoomCallback-Publisher.h"
#include "ZegoLiveRoom/LiveRoom.h"

using namespace ZEGO;
using std::string;
using COMMON::ZegoPublishingStreamInfo;
using COMMON::ZegoStreamInfo;
using COMMON::ZegoStreamUpdateType;

class ZGScreenRecorderDemoHelper:
	public LIVEROOM::ILivePublisherCallback,
	public LIVEROOM::IRoomCallback
{
public:
	ZGScreenRecorderDemoHelper();
	~ZGScreenRecorderDemoHelper();

	void LoginAndPreview(void * local_video_view);
	void StartPublish();
	void StopPublish();
	void StartPreview();

protected:

	void LoginRoom();

	void InitVideoView(void * local_video_view);

	virtual void OnLoginRoom(int errorCode, const char *pszRoomID, const ZegoStreamInfo *pStreamInfo, unsigned int streamCount) override;
	virtual void OnKickOut(int reason, const char *pszRoomID, const char* pszCustomReason="") override;
	virtual void OnDisconnect(int errorCode, const char *pszRoomID) override;
	virtual void OnStreamUpdated(ZegoStreamUpdateType type, ZegoStreamInfo *pStreamInfo, unsigned int streamCount, const char *pszRoomID) override;
	virtual void OnStreamExtraInfoUpdated(ZegoStreamInfo *pStreamInfo, unsigned int streamCount, const char *pszRoomID) override;
	virtual void OnCustomCommand(int errorCode, int requestSeq, const char *pszRoomID) override;
	virtual void OnRecvCustomCommand(const char *pszUserId, const char *pszUserName, const char *pszContent, const char *pszRoomID) override;
	virtual void OnPublishStateUpdate(int stateCode, const char* pszStreamID, const ZegoPublishingStreamInfo& oStreamInfo) override;


private:
	void * local_video_view_ = nullptr;

	string stream_id_ = "";

};

