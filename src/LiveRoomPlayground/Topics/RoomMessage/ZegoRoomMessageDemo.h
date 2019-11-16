#pragma once
#include "ZegoLiveRoom/LiveRoom.h"
#include "ZegoLiveRoom/LiveRoomCallback-Publisher.h"
#include "ZegoLiveRoom/LiveRoomCallBack.h"
#include "ZegoLiveRoom/LiveRoom-Player.h"
#include "ZegoLiveRoom/LiveRoom-IM.h"
#include "ZegoLiveRoom/LiveRoomCallback-Player.h"

#include "AppSupport/ZGLog.h"
#include "AppSupport/ZGUIOperator.h"
#include "AppSupport/ZGHelper.h"
#include "AppSupport/ZGManager.h"
#include "AppSupport/ZGConfigHelper.h"

#include <string>

using std::string;
using namespace ZEGO;

class ZegoRoomMessageDemo :
	public LIVEROOM::IRoomCallback,
	public LIVEROOM::IIMCallback
{
public:
	ZegoRoomMessageDemo();
	~ZegoRoomMessageDemo();

	void LoginRoom(string user_id);
	void SendCustomMessage(string peer_id, string content);
	void SendRoomMessage(string content);
	void SendBigRoomMessage(string content);

	// 保存主窗口的句柄，为了切线程使用
	void InitMainHwnd(HWND hwnd);

protected:
	virtual void OnLoginRoom(int errorCode, const char *pszRoomID, const COMMON::ZegoStreamInfo *pStreamInfo, unsigned int streamCount) override;
	virtual void OnInitSDK(int nError) override;
	virtual void OnKickOut(int reason, const char *pszRoomID, const char* pszCustomReason="") override;
	virtual void OnDisconnect(int errorCode, const char *pszRoomID) override;
	virtual void OnCustomCommand(int errorCode, int requestSeq, const char *pszRoomID) override;
	virtual void OnRecvCustomCommand(const char *pszUserId, const char *pszUserName, const char *pszContent, const char *pszRoomID) override;
	virtual void OnStreamUpdated(COMMON::ZegoStreamUpdateType type, COMMON::ZegoStreamInfo *pStreamInfo, unsigned int streamCount, const char *pszRoomID) override;
	virtual void OnStreamExtraInfoUpdated(COMMON::ZegoStreamInfo *pStreamInfo, unsigned int streamCount, const char *pszRoomID) override;
	virtual void OnSendRoomMessage(int errorCode, const char *pszRoomID, int sendSeq, unsigned long long messageId) override;
	virtual void OnRecvRoomMessage(ROOM::ZegoRoomMessage *pMessageInfo, unsigned int messageCount, const char *pszRoomID) override;
	virtual void OnUserUpdate(const COMMON::ZegoUserInfo *pUserInfo, unsigned int userCount, COMMON::ZegoUserUpdateType type) override;
	virtual void OnSendBigRoomMessage(int errorCode, const char *pszRoomID, int sendSeq, const char *pszmessageID) override;
	virtual void OnRecvBigRoomMessage(ROOM::ZegoBigRoomMessage *pMessageInfo, unsigned int messageCount, const char *pszRoomID) override;
private:
	HWND main_hwnd_ = nullptr;
	string room_id_ = "RoomMessage_Demo";

};

