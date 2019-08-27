#include "stdafx.h"
#include "ZGScreenRecorderDemoHelper.h"

#include <assert.h>

#include "AppSupport/ZGManager.h"
#include "AppSupport/ZGHelper.h"
#include "AppSupport/ZGLog.h"
#include "AppSupport/ZGUtilTools.h"

ZGScreenRecorderDemoHelper::ZGScreenRecorderDemoHelper()
{
	stream_id_ = ZGHelperInstance()->GetDeviceUUID();
}


ZGScreenRecorderDemoHelper::~ZGScreenRecorderDemoHelper()
{
}

void ZGScreenRecorderDemoHelper::LoginAndPreview(void * local_video_view)
{
	InitVideoView(local_video_view);
	LoginRoom();
}

void ZGScreenRecorderDemoHelper::StartPublish()
{
	StartPreview();
	bool publish_result = LIVEROOM::StartPublishing(stream_id_.c_str(), stream_id_.c_str(), 0);
}

void ZGScreenRecorderDemoHelper::StopPublish()
{
	LIVEROOM::StopPreview();
	LIVEROOM::StopPublishing();
}

void ZGScreenRecorderDemoHelper::StartPreview()
{
	if (local_video_view_ != nullptr)
	{
		LIVEROOM::SetPreviewView(local_video_view_);
	}

	bool preview_result = LIVEROOM::StartPreview();
}

void ZGScreenRecorderDemoHelper::LoginRoom()
{
	// 使用测试环境，生产上线时，需要联系zego技术支持切换为正式环境，并修改为 SetUseTestEnv(FALSE); 表示启用正式环境
	LIVEROOM::SetUseTestEnv(TRUE);
	ZGManagerInstance()->InitSdk();
	LIVEROOM::SetUser(stream_id_.c_str(), stream_id_.c_str());
	LIVEROOM::SetLivePublisherCallback(this);
	LIVEROOM::SetRoomCallback(this);
	// 开启回音消除
	LIVEROOM::EnableAEC(true);
	// 开启噪音抑制
	LIVEROOM::EnableNoiseSuppress(true);

	LIVEROOM::LoginRoom(stream_id_.c_str(), ZEGO::LIVEROOM::Audience, stream_id_.c_str());
}

void ZGScreenRecorderDemoHelper::InitVideoView(void * local_video_view)
{
	local_video_view_ = local_video_view;
}

void ZGScreenRecorderDemoHelper::OnLoginRoom(int errorCode, const char *pszRoomID, const ZegoStreamInfo *pStreamInfo, unsigned int streamCount)
{

}

void ZGScreenRecorderDemoHelper::OnKickOut(int reason, const char *pszRoomID)
{

}

void ZGScreenRecorderDemoHelper::OnDisconnect(int errorCode, const char *pszRoomID)
{

}

void ZGScreenRecorderDemoHelper::OnStreamUpdated(ZegoStreamUpdateType type, ZegoStreamInfo *pStreamInfo, unsigned int streamCount, const char *pszRoomID)
{

}

void ZGScreenRecorderDemoHelper::OnStreamExtraInfoUpdated(ZegoStreamInfo *pStreamInfo, unsigned int streamCount, const char *pszRoomID)
{

}

void ZGScreenRecorderDemoHelper::OnCustomCommand(int errorCode, int requestSeq, const char *pszRoomID)
{

}

void ZGScreenRecorderDemoHelper::OnRecvCustomCommand(const char *pszUserId, const char *pszUserName, const char *pszContent, const char *pszRoomID)
{

}

void ZGScreenRecorderDemoHelper::OnPublishStateUpdate(int stateCode, const char* pszStreamID, const ZegoPublishingStreamInfo& oStreamInfo)
{
	ZGLog("OnPublishStateUpdate=> stateCode:%d; streamId:%s", stateCode, pszStreamID);
}
