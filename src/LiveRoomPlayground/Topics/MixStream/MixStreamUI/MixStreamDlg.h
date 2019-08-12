
// MixStreamDlg.h: 头文件
//

#pragma once

#include "MixStreamBasicConfigDlg.h"
#include "MixStreamAdvanceConfigDlg.h"
#include "MixStreamPublishDlg.h"
#include "MixStreamRoomListDlg.h"

#include "MixStream/MixStreamDemo.h"


// CMixStreamDlg 对话框
class CMixStreamDlg : public CDialogEx, public BasicMixStreamConfigDlgEvent, public RoomListDlgEvent
{
// 构造
public:
    static CMixStreamDlg * CreateDlgInstance(CWnd * pParent);
    CMixStreamDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MIXSTREAM_DIALOG };
#endif

    virtual void OnCreateRoomClickedEvent() override;

    virtual void OnEnterRoomClickedEvent(std::string room_id) override;

protected:

	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

    void OnLogout();

// 实现
protected:

    MixStreamRoomListDlg room_list_dlg_;

    MixStreamBasicConfigDlg basic_config_dlg_;
    MixStreamAdvanceConfigDlg advance_config_dlg_;
    
    MixStreamPublishDlg publish_dlg_;

    MixStreamDemo mix_stream_demo_;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	
	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnBnClickedOk();
    afx_msg void OnBnClickedCancel();
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    void InitDlg();

    // 通过 BasicMixStreamConfigDlgEvent 继承
    virtual void OnBasicConfigOkClickedEvent() override;
    virtual void OnAdvanceConfigClickedEvent() override;
};
