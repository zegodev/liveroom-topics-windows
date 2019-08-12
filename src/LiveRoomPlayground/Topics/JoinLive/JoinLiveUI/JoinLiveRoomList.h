#pragma once

#include <functional>
#include <string>
#include <vector>

#include "AppSupport/ZGManager.h"
#include "AppSupport/ZGUIOperator.h"


// JoinLiveRoomList 对话框

class JoinLiveRoomList : public CDialogEx
{
	DECLARE_DYNAMIC(JoinLiveRoomList)

public:
    static JoinLiveRoomList * CreateDlgInstance(CWnd * pParent);
    JoinLiveRoomList(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~JoinLiveRoomList();

    std::function<void(std::string room_id)> on_create_room_cb_ = nullptr;
    std::function<void(std::string room_id)> on_join_room_cb_ = nullptr;

    void UpdateRoomList(std::vector<RoomInfo> room_list);

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_JOIN_LIVE_ROOMLIST };
#endif


    virtual BOOL OnInitDialog() override;

protected:

    std::vector<RoomInfo> room_vec_info_;
    const int kGetRoomListTimerID = 555;
    void OnRoomListUpdate(std::vector<RoomInfo> room_list);

    CListBox room_list_;

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

    CString room_id_ = _T("");

    const std::string kJoinLiveRoomIDPrefix = "JoinLiveRoom-";

    void OnTimer(UINT_PTR nIDEvent);
    void UpdateRoomList();

	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnBnClickedCancel();
    afx_msg void OnLbnSelchangeJoinliveRoomlist();
    afx_msg void OnBnClickedOk();

    afx_msg LRESULT OnUICallbackMsg(WPARAM wParam, LPARAM lParam);
};
