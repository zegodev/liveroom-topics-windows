
// VideoCommunicatonDlg.cpp: 实现文件
//

#include "stdafx.h"

#include "VideoCommunicationDlg.h"
#include "afxdialogex.h"

#include "resource.h"

#include <thread>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CVideoCommunicatonDlg 对话框

CVideoCommunicationDlg * CVideoCommunicationDlg::CreateDlgInstance(CWnd * pParent)
{
    CVideoCommunicationDlg * p = new CVideoCommunicationDlg(pParent);
    p->Create(IDD_VIDEOCOMMUNICATON_DIALOG, pParent);
    return p;
}


CVideoCommunicationDlg::CVideoCommunicationDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_VIDEOCOMMUNICATON_DIALOG, pParent)
    , room_id_(_T(""))
{
    user_list_info_.resize(kMaxOtherUserCount);
}

void CVideoCommunicationDlg::OnLoginEvent(ZGVideoCommunicationLoginEvent event, ZGVideoCommunicationStatus status)
{
    CStringA str;
    str.Format("OnLoginEvent, event = %d, status = %d", event, status);
    AddLogToList(str.GetBuffer());

    if (status == kZGVideoCommunicationLoginStatusLogin)
    {
        GetDlgItem(IDC_BUTTON_VIDEOCOMMUNICATION_LOGOUT)->EnableWindow(TRUE);
    }

    else if (status == kZGVideoCommunicationLoginStatusLogout)
    {
        GetDlgItem(IDOK)->EnableWindow(TRUE);
        GetDlgItem(IDC_EDIT_VIDEOCOMMUNICATION_ROOMID)->EnableWindow(TRUE);
        GetDlgItem(IDC_BUTTON_VIDEOCOMMUNICATION_LOGOUT)->EnableWindow(FALSE);
    }
}

void CVideoCommunicationDlg::OnUserJoin(ZegoUser user)
{
    CStringA str;
    str.Format("OnUserJoin, user id = %s, user name = %s", user.user_id.c_str(), user.user_name.c_str());
    AddLogToList(str.GetBuffer());
    ProcessUserJoinRoom(user);
}

void CVideoCommunicationDlg::OnUserLeave(ZegoUser user)
{
    CStringA str;
    str.Format("OnUserLeave, user id = %s, user name = %s", user.user_id.c_str(), user.user_name.c_str());
    AddLogToList(str.GetBuffer());
    ProcessUserLeaveRoom(user);
}

void CVideoCommunicationDlg::ProcessUserLeaveRoom(ZegoUser user)
{
    for (int i = 0; i < kMaxOtherUserCount; ++i)
    {
        if (user_list_info_[i].is_user_in_room)
        {
            if (user_list_info_[i].user.user_id == user.user_id
                && user_list_info_[i].user.user_name == user.user_name)
            {
                user_list_info_[i].is_user_in_room = false;

                break;
            }
        }
    }

    std::thread([=]() {

        std::this_thread::sleep_for(std::chrono::milliseconds(200));

        ResetControlBackgroundColorIfUserLeave();

    }).detach();

}


void CVideoCommunicationDlg::ResetControlBackgroundColorIfUserLeave()
{
    for (int i = 0; i < kMaxOtherUserCount; ++i)
    {
        if (!user_list_info_[i].is_user_in_room)
        {
            // 没有视频了则清空控件颜色为原来的背景颜色
            PostUIData * pdata = CreateUIData();
            HWND need_reset_hwnd = user_list_info_[i].view_hwnd;
            pdata->cb_in_ui_fun = [need_reset_hwnd, pdata, this]()->void
            {
                ClearControlColor(need_reset_hwnd);

                DestroyUIData(pdata);
            };

            PostMsgDataToUI(pdata);
        }

    }
}

void CVideoCommunicationDlg::ClearControlColor(HWND hwnd)
{
    COLORREF color = RGB(105, 105, 105);
    CRect control_rect;
    CBrush new_brush;
    CBrush *old_brush_ptr;
    CClientDC clientDC(this);

    new_brush.CreateSolidBrush(color);

    FromHandle(hwnd)->GetWindowRect(&control_rect);
    ScreenToClient(&control_rect);

    old_brush_ptr = clientDC.SelectObject(&new_brush);
    clientDC.Rectangle(control_rect);

    clientDC.SelectObject(old_brush_ptr);
    new_brush.DeleteObject();
}

void CVideoCommunicationDlg::PrintToView(const char * log_str)
{
    AddLogToList(log_str);
}

void CVideoCommunicationDlg::AddLogToList(std::string str_log)
{
    PostUIData * pdata = CreateUIData();
    pdata->cb_in_ui_fun = [str_log, pdata, this]()->void
    {
        CStringA str;
        str.Format("%s", str_log.c_str());
        log_list_.AddString(CString(str));
        SetHScroll();

        DestroyUIData(pdata);
    };

    PostMsgDataToUI(pdata);

}

void CVideoCommunicationDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_EDIT_VIDEOCOMMUNICATION_ROOMID, room_id_);
    DDX_Control(pDX, IDC_LIST_VIDEOCOMMUNICATION_LOG_LIST, log_list_);
}

BEGIN_MESSAGE_MAP(CVideoCommunicationDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
    ON_BN_CLICKED(IDCANCEL, &CVideoCommunicationDlg::OnBnClickedCancel)
    ON_BN_CLICKED(IDOK, &CVideoCommunicationDlg::OnBnClickedOk)
    ON_BN_CLICKED(IDC_BUTTON_VIDEOCOMMUNICATION_LOGOUT, &CVideoCommunicationDlg::OnBnClickedButtonVideocommunicationLogout)
    ON_MESSAGE(UI_CALLBACK_MSG, &CVideoCommunicationDlg::OnUICallbackMsg)
    ON_LBN_SELCHANGE(IDC_LIST_VIDEOCOMMUNICATION_LOG_LIST, &CVideoCommunicationDlg::OnLbnSelchangeListVideocommunicationLogList)
END_MESSAGE_MAP()


// CVideoCommunicatonDlg 消息处理程序

BOOL CVideoCommunicationDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

    for (int i = 0;i < kMaxOtherUserCount; ++i)
    {
        user_list_info_[i].view_hwnd = GetDlgItem(IDC_STATIC_VIDEO_COMMUNICATION_2 + i)->GetSafeHwnd();
    }

    video_communication_.InitMainHwnd(GetSafeHwnd());
    video_communication_.SetDemoCallBack(this);
    video_communication_.SetLogCallBack(this);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CVideoCommunicationDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	
	CDialogEx::OnSysCommand(nID, lParam);
	
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CVideoCommunicationDlg::OnPaint()
{
	if (IsIconic())
	{
	}
	else
	{
		CDialogEx::OnPaint();
	}
}


void CVideoCommunicationDlg::OnBnClickedCancel()
{

}


void CVideoCommunicationDlg::OnBnClickedOk()
{
    if (!UpdateData(TRUE))
    {
        return;
    }

    CStringA str(room_id_.GetBuffer());
    if (room_id_ == _T(""))
    {
        MessageBox(_T("请输入房间id"));
        return;
    }

    GetDlgItem(IDOK)->EnableWindow(FALSE);
    GetDlgItem(IDC_EDIT_VIDEOCOMMUNICATION_ROOMID)->EnableWindow(FALSE);
    video_communication_.SetLocalVideoView(GetDlgItem(IDC_STATIC_VIDEO_COMMUNICATION_1)->GetSafeHwnd());
    video_communication_.LoginRoom(str.GetBuffer());

}

void CVideoCommunicationDlg::SetHScroll()
{
    CDC* dc = GetDC();
    SIZE s;
    int index;
    CString str;
    long temp;
    for (index = 0; index < log_list_.GetCount(); index++)
    {
        log_list_.GetText(index, str);
        s = dc->GetTextExtent(str, str.GetLength() + 1);
        // 获取字符串的像素大小
        // 如果新的字符串宽度大于先前的水平滚动条宽度，则重新设置滚动条宽度
        // 得到滚动条的宽度
        temp = (long)SendDlgItemMessage(IDC_LIST_VIDEOCOMMUNICATION_LOG_LIST, LB_GETHORIZONTALEXTENT, 0, 0);
        if (s.cx > temp)
        {
            SendDlgItemMessage(IDC_LIST_VIDEOCOMMUNICATION_LOG_LIST, LB_SETHORIZONTALEXTENT, (WPARAM)s.cx + 1000, 0);
        }
    }
    ReleaseDC(dc);
}


void CVideoCommunicationDlg::OnBnClickedButtonVideocommunicationLogout()
{
    video_communication_.LogoutRoom();

    std::thread([=]() {

        std::this_thread::sleep_for(std::chrono::milliseconds(200));

        // 没有视频了则清空控件颜色为原来的背景颜色
        PostUIData * pdata = CreateUIData();
        HWND need_reset_hwnd = GetDlgItem(IDC_STATIC_VIDEO_COMMUNICATION_1)->GetSafeHwnd();;
        pdata->cb_in_ui_fun = [need_reset_hwnd, pdata, this]()->void
        {
            ClearControlColor(need_reset_hwnd);

            DestroyUIData(pdata);
        };

        PostMsgDataToUI(pdata);

    }).detach();

    for (int i = 0; i < kMaxOtherUserCount; ++i)
    {
        user_list_info_[i].is_user_in_room =false;
    }

    ResetControlBackgroundColorIfUserLeave();

}

void CVideoCommunicationDlg::ProcessUserJoinRoom(ZegoUser user)
{
    bool processed = false;
    for (int i = 0; i < kMaxOtherUserCount; ++i)
    {
        if (!user_list_info_[i].is_user_in_room)
        {
            processed = true;
            user_list_info_[i].is_user_in_room = true;
            user_list_info_[i].user.user_id = user.user_id;
            user_list_info_[i].user.user_name = user.user_name;
            // 配置该用户的视频显示视图
            video_communication_.SetRemoteVideoView(user_list_info_[i].view_hwnd, user, ZEGO::LIVEROOM::ZegoVideoViewModeScaleAspectFill);
            break;
        }
    }

    if (!processed)
    {
        AddLogToList("已拉流的用户人数超过指定数，不在进行拉流");
    }
}

afx_msg LRESULT CVideoCommunicationDlg::OnUICallbackMsg(WPARAM wParam, LPARAM lParam)
{
    PostUIData * pdata = (PostUIData *)wParam;
    if (pdata != nullptr && pdata->cb_in_ui_fun)
    {
        pdata->cb_in_ui_fun();
    }
    return 0;
}

void CVideoCommunicationDlg::OnLbnSelchangeListVideocommunicationLogList()
{
    // 复制选择行的日志
    int cur_sel = log_list_.GetCurSel();
    if (cur_sel > 0)
    {
        CString source;
        log_list_.GetText(cur_sel, source);
        if (OpenClipboard())
        {
            HGLOBAL clipbuffer;
            CStringA str(source.GetBuffer());

            char * buffer;

            EmptyClipboard();
            clipbuffer = GlobalAlloc(GMEM_DDESHARE, str.GetLength() + 1);

            buffer = (char*)GlobalLock(clipbuffer);
            memcpy(buffer, str.GetBuffer(), str.GetLength());

            GlobalUnlock(clipbuffer);
            SetClipboardData(CF_TEXT, clipbuffer);
            CloseClipboard();
        }
    }
}
