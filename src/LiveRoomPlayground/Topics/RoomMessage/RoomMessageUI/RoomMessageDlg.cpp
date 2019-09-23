
// RoomMessageDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "RoomMessageDlg.h"
#include "afxdialogex.h"
#include "Topics/RoomMessage/RoomMessageUI/resource.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


CRoomMessageDlg * CRoomMessageDlg::CreateDlgInstance(CWnd * pParent)
{
	CRoomMessageDlg * p = new CRoomMessageDlg(pParent);
	p->Create(IDD_ROOMMESSAGE_DIALOG, pParent);
	return p;
}

// CRoomMessageDlg 对话框



CRoomMessageDlg::CRoomMessageDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_ROOMMESSAGE_DIALOG, pParent)
{
	
}

void CRoomMessageDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CRoomMessageDlg, CDialogEx)
	ON_MESSAGE(UI_CALLBACK_MSG, &CRoomMessageDlg::OnUICallbackMsg)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BUTTON_LOGIN, &CRoomMessageDlg::OnBnClickedButtonLogin)
	ON_BN_CLICKED(IDC_BUTTON_SEND_CUSTOM_COMMAND, &CRoomMessageDlg::OnBnClickedButtonSendCustomCommand)
	ON_BN_CLICKED(IDC_BUTTON_SEND_ROOM_MESSAGE, &CRoomMessageDlg::OnBnClickedButtonSendRoomMessage)
	ON_BN_CLICKED(IDC_BUTTON_SEND_BIG_ROOM_MESSAGE, &CRoomMessageDlg::OnBnClickedButtonSendBigRoomMessage)
END_MESSAGE_MAP()


// CRoomMessageDlg 消息处理程序

BOOL CRoomMessageDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();


	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CRoomMessageDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

	}
	else
	{
		CDialogEx::OnPaint();
	}
}

LRESULT CRoomMessageDlg::OnUICallbackMsg(WPARAM wParam, LPARAM lParam)
{
	PostUIData * pdata = (PostUIData *)wParam;
	if (pdata != nullptr && pdata->cb_in_ui_fun)
	{
		pdata->cb_in_ui_fun();
	}
	return 0;
}

void CRoomMessageDlg::InitDlg()
{
	demo_.InitMainHwnd(GetSafeHwnd());
}



void CRoomMessageDlg::OnBnClickedButtonLogin()
{
	// TODO: 在此添加控件通知处理程序代码
	CString user_id;
	GetDlgItem(IDC_EDIT_USER_ID)->GetWindowTextW(user_id);
	string userId = CStringA(user_id.GetBuffer());
	demo_.LoginRoom(userId);
}


void CRoomMessageDlg::OnBnClickedButtonSendCustomCommand()
{
	// TODO: 在此添加控件通知处理程序代码
	CString peer_id;
	GetDlgItem(IDC_EDIT_PEER_ID)->GetWindowTextW(peer_id);
	string peerId = CStringA(peer_id.GetBuffer());
	demo_.SendCustomMessage(peerId, "this is a custom command");
}


void CRoomMessageDlg::OnBnClickedButtonSendRoomMessage()
{
	// TODO: 在此添加控件通知处理程序代码
	demo_.SendRoomMessage("this is a room message");
}


void CRoomMessageDlg::OnBnClickedButtonSendBigRoomMessage()
{
	// TODO: 在此添加控件通知处理程序代码
	demo_.SendBigRoomMessage("this is a big room message");
}
