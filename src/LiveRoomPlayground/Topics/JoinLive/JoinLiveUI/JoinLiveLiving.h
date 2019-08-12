#pragma once

#include <string>
#include <functional>

#include "JoinLive/JoinLiveDemo.h"

#include "AppSupport/ZGLog.h"

// JoinLiveLiving 对话框

class JoinLiveLiving : 
    public CDialogEx, 
    public ILogToView,
    public JoinLiveCallBack
{
	DECLARE_DYNAMIC(JoinLiveLiving)

public:
    static JoinLiveLiving * CreateDlgInstance(CWnd * pParent);
    JoinLiveLiving(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~JoinLiveLiving();

    std::function<void ()> on_logout_cb_ = nullptr;

    virtual void OnLoginEvent(ZGJoinLiveLoginEvent event, ZGJoinLiveStatus status) override;
    virtual void OnUserJoin(ZegoJoinLiveUser user)override;
    virtual void OnUserLeave(ZegoJoinLiveUser user)override;

    void ProcessUserJoinRoom(ZegoJoinLiveUser user);
    void ProcessUserLeaveRoom(ZegoJoinLiveUser user);
    void ResetControlBackgroundColorIfUserLeave();
    void ClearControlColor(HWND hwnd);
    void PrintToView(const char * log_str) override;

    void AddLogToList(std::string str_log);

    // 主播创建房间
    void CreateRoom(std::string room_id);

    // 观众加入房间
    void JoinRoom(std::string room_id);

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_JOINLIVE_LIVING };
#endif


    virtual BOOL OnInitDialog() override;

protected:

    JoinLiveDemo join_live_;

    CListBox log_list_;

    typedef struct ZegoJoinLivingRoomInfo {
        ZegoJoinLiveUser user;
        HWND view_hwnd = nullptr;
        bool is_user_in_room = false;
    }ZegoJoinLivingRoomInfo;

    const int kMaxOtherUserCount = 3;
    std::vector<ZegoJoinLivingRoomInfo> user_list_info_;

    void Init();

    void ResetContronlStatus();

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnBnClickedButtonJoin();
    afx_msg void OnBnClickedOk();
    void SetHScroll();
    afx_msg LRESULT OnUICallbackMsg(WPARAM wParam, LPARAM lParam);
};
