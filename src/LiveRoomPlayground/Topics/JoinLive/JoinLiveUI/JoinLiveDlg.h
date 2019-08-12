
// JoinLiveDlg.h: 头文件
//

#pragma once

#include "JoinLiveRoomList.h"
#include "JoinLiveLiving.h"

#include <string>

#include "AppSupport/ZGManager.h"

using namespace APPSUPPORT;

// CJoinLiveDlg 对话框
class CJoinLiveDlg : public CDialogEx
{
// 构造
public:
    static CJoinLiveDlg * CreateDlgInstance(CWnd * pParent);
    CJoinLiveDlg(CWnd* pParent = nullptr);	// 标准构造函数

    void InitDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_JOINLIVE_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:


    void OnJoinLivingLogoutEvent();

    void OnCreateRoomEvent(std::string room_id);

    void OnJoinRoomEvent(std::string room_id);
    JoinLiveRoomList * join_room_list_dlg_ptr_ = nullptr;
    JoinLiveLiving* join_living_dlg_ptr_ = nullptr;

    

    const string kJoinLiveRoomIDPrefix = "JoinLiveRoom-";



	// 生成的消息映射函数
	virtual BOOL OnInitDialog();

    afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();

	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnBnClickedOk();
    afx_msg void OnBnClickedCancel();
};
