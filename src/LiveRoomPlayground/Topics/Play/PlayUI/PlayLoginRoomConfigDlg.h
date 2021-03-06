#pragma once

#include "Play/PlayDemo.h"

// PlayLoginRoomConfigDlg 对话框

class PlayLoginRoomConfigDlgEvent
{
public:
    virtual void OnLoginRoomButtonEvent() = 0;

};

class PlayLoginRoomConfigDlg : public CDialogEx
{
	DECLARE_DYNAMIC(PlayLoginRoomConfigDlg)

public:
	PlayLoginRoomConfigDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~PlayLoginRoomConfigDlg();

    void SetEventCallBack(PlayLoginRoomConfigDlgEvent * cb);

    PlayLoginRomConfig GetConfig();

    // 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PLAY_LOGINROOM_CONFIG_DLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

    PlayLoginRoomConfigDlgEvent * event_cb_ = nullptr;

    CString room_id_;

    PlayLoginRomConfig login_room_config_;
public:
    afx_msg void OnBnClickedButtonPlayTopicLoginroom();
};
