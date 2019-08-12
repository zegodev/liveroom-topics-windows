#pragma once

#include <string>
#include <vector>
#include "AppSupport/ZGUIOperator.h"
#include "AppSupport/ZGManager.h"


class RoomListDlgEvent
{
public:
    virtual void OnCreateRoomClickedEvent() = 0;
    virtual void OnEnterRoomClickedEvent(std::string room_id) = 0;
};

// MixStreamRoomListDlg 对话框

class MixStreamRoomListDlg : public CDialogEx
{
	DECLARE_DYNAMIC(MixStreamRoomListDlg)

public:

    void SetDlgEventCallBack(RoomListDlgEvent * cb);

	MixStreamRoomListDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~MixStreamRoomListDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_MIXSTREAM_ROOMLIST_DLG };
#endif


    virtual BOOL OnInitDialog() override;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnBnClickedOk();
    void OnRoomListUpdate(std::vector<RoomInfo> room_list);
    void UpdateRoomList();

    void UpdateRoomList(std::vector<RoomInfo> room_list);

    afx_msg LRESULT OnUICallbackMsg(WPARAM wParam, LPARAM lParam);
    CListBox room_list_;
    std::vector<RoomInfo> room_vec_info_;
    const std::string kMixStreamRoomIDPrefix = "MixStreamRoom-";


    RoomListDlgEvent *dlg_cb_ = nullptr;
    afx_msg void OnLbnSelchangeMixstreamRoomlist();

    void OnTimer(UINT_PTR nIDEvent);
    const int kGetRoomListTimerID = 555;
    afx_msg void OnBnClickedCancel();
};
