
// MixStreamDlg.cpp: 实现文件
//

#include "stdafx.h"

#include "MixStreamDlg.h"
#include "afxdialogex.h"

#include "Resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMixStreamDlg 对话框

CMixStreamDlg * CMixStreamDlg::CreateDlgInstance(CWnd * pParent)
{
    CMixStreamDlg * p = new CMixStreamDlg(pParent);
    p->Create(IDD_MIXSTREAM_DIALOG, pParent);
    return p;
}

CMixStreamDlg::CMixStreamDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MIXSTREAM_DIALOG, pParent)
{
	
}

void CMixStreamDlg::OnCreateRoomClickedEvent()
{
    room_list_dlg_.ShowWindow(SW_HIDE);

    CRect rect;
    GetDlgItem(IDC_MIXSTREAM_PANNEL)->GetWindowRect(rect);
    ScreenToClient(&rect);

    basic_config_dlg_.MoveWindow(rect.left, rect.top, rect.Width(), rect.Height());
    basic_config_dlg_.ShowWindow(SW_SHOW);
}

void CMixStreamDlg::OnEnterRoomClickedEvent(std::string room_id)
{
    room_list_dlg_.ShowWindow(SW_HIDE);

    CRect rect;
    GetDlgItem(IDC_MIXSTREAM_PANNEL)->GetWindowRect(rect);
    ScreenToClient(&rect);
    publish_dlg_.MoveWindow(rect.left, rect.top, rect.Width(), rect.Height());
    publish_dlg_.ShowWindow(SW_SHOW);

    bool is_anchor = false;

    publish_dlg_.StartPublish(room_id, is_anchor);
}

void CMixStreamDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

void CMixStreamDlg::OnLogout()
{
    InitDlg();
}

BEGIN_MESSAGE_MAP(CMixStreamDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
    ON_BN_CLICKED(IDOK, &CMixStreamDlg::OnBnClickedOk)
    ON_BN_CLICKED(IDCANCEL, &CMixStreamDlg::OnBnClickedCancel)
    ON_WM_CREATE()
END_MESSAGE_MAP()


// CMixStreamDlg 消息处理程序

BOOL CMixStreamDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();


	// TODO: 在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CMixStreamDlg::OnPaint()
{
    CDialogEx::OnPaint();
}


void CMixStreamDlg::OnBnClickedOk()
{

}


void CMixStreamDlg::OnBnClickedCancel()
{

}

int CMixStreamDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (CDialogEx::OnCreate(lpCreateStruct) == -1)
        return -1;

    basic_config_dlg_.Create(IDD_MIXSTREAM_BASIC_CONFIG, this);
    basic_config_dlg_.event_cb_ = this;
    
    advance_config_dlg_.Create(IDD_DIALOG_MIXSTREAM_ADVANCE_CONFIG, this);

    publish_dlg_.Create(IDD_MIXSTREAM_PUBLISH, this);
    publish_dlg_.on_logout_cb_ = std::bind(&CMixStreamDlg::OnLogout, this);

    room_list_dlg_.Create(IDD_DIALOG_MIXSTREAM_ROOMLIST_DLG, this);
    room_list_dlg_.SetDlgEventCallBack(this);

    return 0;
}


void CMixStreamDlg::InitDlg()
{
    basic_config_dlg_.ShowWindow(SW_HIDE);
    publish_dlg_.ShowWindow(SW_HIDE);


    CRect rect;
    GetDlgItem(IDC_MIXSTREAM_PANNEL)->GetWindowRect(rect);
    ScreenToClient(&rect);

    room_list_dlg_.MoveWindow(rect.left, rect.top, rect.Width(), rect.Height());
    room_list_dlg_.ShowWindow(SW_SHOW);
}

void CMixStreamDlg::OnBasicConfigOkClickedEvent()
{
    basic_config_dlg_.ShowWindow(SW_HIDE);

    CRect rect;
    GetDlgItem(IDC_MIXSTREAM_PANNEL)->GetWindowRect(rect);
    ScreenToClient(&rect);
    publish_dlg_.MoveWindow(rect.left, rect.top, rect.Width(), rect.Height());
    publish_dlg_.ShowWindow(SW_SHOW);

    string room_id = CStringA(basic_config_dlg_.room_id_.GetBuffer()).GetBuffer();
    
    bool is_anchor = true;

    const std::string kMixStreamRoomIDPrefix = "MixStreamRoom-";
    string real_room_id = kMixStreamRoomIDPrefix + room_id;

    publish_dlg_.publish_stream_id_ = CStringA(basic_config_dlg_.publish_stream_id_.GetBuffer()).GetBuffer();
    publish_dlg_.mixstream_id_ = CStringA(basic_config_dlg_.output_mixstream_id_.GetBuffer()).GetBuffer();

    publish_dlg_.StartPublish(real_room_id, is_anchor);
}

void CMixStreamDlg::OnAdvanceConfigClickedEvent()
{
    MixStreamAdvanceConfigDlg mix_stream_config_dlg;
    mix_stream_config_dlg.DoModal();
}
