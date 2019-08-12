
// JoinLiveDlg.cpp: 实现文件
//

#include "stdafx.h"

#include "JoinLiveDlg.h"
#include "afxdialogex.h"

#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CJoinLiveDlg 对话框

CJoinLiveDlg * CJoinLiveDlg::CreateDlgInstance(CWnd * pParent)
{
    CJoinLiveDlg * p = new CJoinLiveDlg(pParent);
    p->Create(IDD_JOINLIVE_DIALOG, pParent);
    return p;
}

CJoinLiveDlg::CJoinLiveDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_JOINLIVE_DIALOG, pParent)
{

}

void CJoinLiveDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
    
}



void CJoinLiveDlg::OnJoinLivingLogoutEvent()
{
    join_living_dlg_ptr_->ShowWindow(FALSE);
    join_room_list_dlg_ptr_->ShowWindow(TRUE);
}

void CJoinLiveDlg::OnCreateRoomEvent(std::string room_id)
{
    join_living_dlg_ptr_->ShowWindow(TRUE);
    join_room_list_dlg_ptr_->ShowWindow(FALSE);

    join_living_dlg_ptr_->CreateRoom(room_id);
}

void CJoinLiveDlg::OnJoinRoomEvent(std::string room_id)
{
    join_living_dlg_ptr_->ShowWindow(TRUE);
    join_room_list_dlg_ptr_->ShowWindow(FALSE);

    join_living_dlg_ptr_->JoinRoom(room_id);
}


void CJoinLiveDlg::InitDlg()
{
    if (join_room_list_dlg_ptr_ == nullptr)
    {
        join_room_list_dlg_ptr_ = JoinLiveRoomList::CreateDlgInstance(this);

        join_room_list_dlg_ptr_->on_create_room_cb_ = std::bind(&CJoinLiveDlg::OnCreateRoomEvent, this, std::placeholders::_1);
        join_room_list_dlg_ptr_->on_join_room_cb_ = std::bind(&CJoinLiveDlg::OnJoinRoomEvent, this, std::placeholders::_1);

        CRect rect;
        GetDlgItem(IDC_STATIC_JOINLIVE_PANE)->GetWindowRect(rect);
        ScreenToClient(&rect);
        join_room_list_dlg_ptr_->MoveWindow(rect.left, rect.top, rect.Width(), rect.Height());
    }
    if (join_living_dlg_ptr_ == nullptr)
    {
        join_living_dlg_ptr_ = JoinLiveLiving::CreateDlgInstance(this);
        join_living_dlg_ptr_->on_logout_cb_ = std::bind(&CJoinLiveDlg::OnJoinLivingLogoutEvent, this);

        CRect rect;
        GetDlgItem(IDC_STATIC_JOINLIVE_PANE)->GetWindowRect(rect);
        ScreenToClient(&rect);
        join_living_dlg_ptr_->MoveWindow(rect.left, rect.top, rect.Width(), rect.Height());
    }

    join_living_dlg_ptr_->ShowWindow(FALSE);
    join_room_list_dlg_ptr_->ShowWindow(TRUE);
}

BEGIN_MESSAGE_MAP(CJoinLiveDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
    ON_BN_CLICKED(IDOK, &CJoinLiveDlg::OnBnClickedOk)
    ON_BN_CLICKED(IDCANCEL, &CJoinLiveDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// CJoinLiveDlg 消息处理程序

BOOL CJoinLiveDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

    

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}


void CJoinLiveDlg::OnSysCommand(UINT nID, LPARAM lParam)
{

    CDialogEx::OnSysCommand(nID, lParam);
	
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CJoinLiveDlg::OnPaint()
{
    CDialogEx::OnPaint();
	
}


void CJoinLiveDlg::OnBnClickedOk()
{

}


void CJoinLiveDlg::OnBnClickedCancel()
{

}
