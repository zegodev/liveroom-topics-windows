
// VideoCommunicationDlg.h: 头文件
//

#pragma once

#include <vector>

#include "VideoCommunication/VideoCommunicationDemo.h"

#include "AppSupport/ZGLog.h"

// CVideoCommunicatonDlg 对话框
class CVideoCommunicationDlg : public CDialogEx, public VideoCommunicationCallBack, public ILogToView
{
// 构造
public:
    static CVideoCommunicationDlg * CreateDlgInstance(CWnd * pParent);
    CVideoCommunicationDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_VIDEOCOMMUNICATON_DIALOG };
#endif


    virtual void OnLoginEvent(ZGVideoCommunicationLoginEvent event, ZGVideoCommunicationStatus status) override;

    virtual void OnUserJoin(ZegoUser user) override;

    virtual void OnUserLeave(ZegoUser user) override;


    void ProcessUserLeaveRoom(ZegoUser user);
    void ResetControlBackgroundColorIfUserLeave();
    void ClearControlColor(HWND hwnd);
    virtual void PrintToView(const char * log_str) override;

    void AddLogToList(std::string str_log);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

// 实现
protected:


    typedef struct ZegoUserInfo {
        ZegoUser user;
        HWND view_hwnd;
        bool is_user_in_room = false;
    }ZegoUserInfo;
	
    const int kMaxOtherUserCount = 11;

    std::vector<ZegoUserInfo> user_list_info_;

    VideoCommunicationDemo video_communication_;

    CListBox log_list_;

    void SetHScroll();

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnBnClickedCancel();
    afx_msg void OnBnClickedOk();
    CString room_id_;
    afx_msg void OnBnClickedButtonVideocommunicationLogout();
    void ProcessUserJoinRoom(ZegoUser user);
    afx_msg LRESULT OnUICallbackMsg(WPARAM wParam, LPARAM lParam);
    afx_msg void OnLbnSelchangeListVideocommunicationLogList();
};
