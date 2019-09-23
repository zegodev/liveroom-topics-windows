#include "stdafx.h"
#include "ZegoRoomMessageDemo.h"


ZegoRoomMessageDemo::ZegoRoomMessageDemo()
{
}


ZegoRoomMessageDemo::~ZegoRoomMessageDemo()
{
}

void ZegoRoomMessageDemo::LoginRoom(string user_id)
{
	bool is_test_env = true;
	ZGLog("使用%s环境", is_test_env ? "测试环境" : "正式环境");

	LIVEROOM::SetUseTestEnv(is_test_env);

	string user_name = user_id;

	ZGLog("设置userid = %s，username = %s", user_id.c_str(), user_name.c_str());
	LIVEROOM::SetUser(user_id.c_str(), user_name.c_str());

	// 配置观众可以创建房间，开启房间用户列表更新
	LIVEROOM::SetRoomConfig(true, true);

	LIVEROOM::SetRoomCallback(this);
	LIVEROOM::SetIMCallback(this);
	
	ZGLog("执行初始化");
	if (!ZGManagerInstance()->InitSdk())
	{
		ZGLog("初始化失败");
	}
}

void ZegoRoomMessageDemo::SendCustomMessage(string peer_id, string content)
{
	ROOM::ZegoUser *user = new ROOM::ZegoUser();
	strcpy(user->szUserId, peer_id.c_str());
	strcpy(user->szUserName, peer_id.c_str());	
	LIVEROOM::SendCustomCommand(user, 1, content.c_str());
}

void ZegoRoomMessageDemo::SendRoomMessage(string content)
{
	LIVEROOM::SendRoomMessageEx(ROOM::Text, ROOM::Chat, content.c_str());
}

void ZegoRoomMessageDemo::SendBigRoomMessage(string content)
{
	LIVEROOM::SendBigRoomMessage(ROOM::Text, ROOM::Chat, content.c_str());
}

void ZegoRoomMessageDemo::InitMainHwnd(HWND hwnd)
{
	main_hwnd_ = hwnd;
}

void ZegoRoomMessageDemo::OnLoginRoom(int errorCode, const char *pszRoomID, const COMMON::ZegoStreamInfo *pStreamInfo, unsigned int streamCount)
{
	if (errorCode == 0)
	{
		ZGLog("登录成功");
	}
	else
	{
		ZGLog("登录失败");
	}
}

void ZegoRoomMessageDemo::OnInitSDK(int nError)
{
	if (nError == 0)
	{
		ZGLog("初始化sdk成功");

		
		// 初始化成功，切线程然后在开始登录房间
		PostUIData * pdata = CreateUIData();
		pdata->cb_in_ui_fun = [pdata, this]()->void
		{
			ZGLog("开始登录，room id = %s", room_id_.c_str());

			if (!LIVEROOM::LoginRoom(room_id_.c_str(), COMMON::Anchor))
			{
				ZGLog("登录失败");
			}

			DestroyUIData(pdata);
		};

		GlobalPostMsgDataToUI(main_hwnd_, pdata);
	}
	else
	{
		ZGLog("初始化失败，错误码为：%d", nError);
	}
}

void ZegoRoomMessageDemo::OnKickOut(int reason, const char *pszRoomID)
{

}

void ZegoRoomMessageDemo::OnDisconnect(int errorCode, const char *pszRoomID)
{

}

void ZegoRoomMessageDemo::OnCustomCommand(int errorCode, int requestSeq, const char *pszRoomID)
{
	ZGLog("OnCustomCommand: errorCode=%d, requestSeq=%d, roomId=%s", errorCode, requestSeq, pszRoomID);
}

void ZegoRoomMessageDemo::OnRecvCustomCommand(const char *pszUserId, const char *pszUserName, const char *pszContent, const char *pszRoomID)
{
	ZGLog("OnRecvCustomCommand: pszUserId=%s, pszUserName=%s, pszContent=%s,pszRoomID=%s", pszUserId, pszUserName, pszContent, pszRoomID);
}

void ZegoRoomMessageDemo::OnStreamUpdated(COMMON::ZegoStreamUpdateType type, COMMON::ZegoStreamInfo *pStreamInfo, unsigned int streamCount, const char *pszRoomID)
{

}

void ZegoRoomMessageDemo::OnStreamExtraInfoUpdated(COMMON::ZegoStreamInfo *pStreamInfo, unsigned int streamCount, const char *pszRoomID)
{

}

void ZegoRoomMessageDemo::OnSendRoomMessage(int errorCode, const char *pszRoomID, int sendSeq, unsigned long long messageId)
{
	ZGLog("OnSendRoomMessage: errorCode=%d, pszRoomID=%s, sendSeq=%d, messageId=%ull", errorCode, pszRoomID, sendSeq, messageId);
}

void ZegoRoomMessageDemo::OnRecvRoomMessage(ROOM::ZegoRoomMessage *pMessageInfo, unsigned int messageCount, const char *pszRoomID)
{
	ZGLog("OnRecvRoomMessage: messageCount=%d, pszRoomID=%s", messageCount, pszRoomID);
}

void ZegoRoomMessageDemo::OnUserUpdate(const COMMON::ZegoUserInfo *pUserInfo, unsigned int userCount, COMMON::ZegoUserUpdateType type)
{

}

void ZegoRoomMessageDemo::OnSendBigRoomMessage(int errorCode, const char *pszRoomID, int sendSeq, const char *pszmessageID)
{
	ZGLog("OnSendBigRoomMessage: errorCode=%d, pszRoomID=%s, sendSeq=%d, messageId=%s", errorCode, pszRoomID, sendSeq, pszmessageID);
}

void ZegoRoomMessageDemo::OnRecvBigRoomMessage(ROOM::ZegoBigRoomMessage *pMessageInfo, unsigned int messageCount, const char *pszRoomID)
{
	ZGLog("OnRecvBigRoomMessage: messageCount=%d, pszRoomID=%s", messageCount, pszRoomID);
}
