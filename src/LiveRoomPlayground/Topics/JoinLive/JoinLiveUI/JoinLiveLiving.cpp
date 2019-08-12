// JoinLiveLiving.cpp: 实现文件
//

#include "stdafx.h"

#include "JoinLiveLiving.h"
#include "afxdialogex.h"

#include "resource.h"

#include <thread>

// JoinLiveLiving 对话框

IMPLEMENT_DYNAMIC(JoinLiveLiving, CDialogEx)

JoinLiveLiving * JoinLiveLiving::CreateDlgInstance(CWnd * pParent)
{
    JoinLiveLiving * p = new JoinLiveLiving(pParent);
    p->Create(IDD_DIALOG_JOINLIVE_LIVING, pParent);
    return p;
}

JoinLiveLiving::JoinLiveLiving(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_JOINLIVE_LIVING, pParent)
{
    user_list_info_.resize(kMaxOtherUserCount);
}

JoinLiveLiving::~JoinLiveLiving()
{
}

void JoinLiveLiving::OnLoginEvent(ZGJoinLiveLoginEvent event, ZGJoinLiveStatus status)
{
    CStringA str;
    str.Format("OnLoginEvent, event = %d, status = %d", event, status);
    AddLogToList(str.GetBuffer());
}

void JoinLiveLiving::OnUserJoin(ZegoJoinLiveUser user)
{
    CStringA str;
    str.Format("OnUserJoin, user id = %s, user name = %s", user.user_id.c_str(), user.user_name.c_str());
    AddLogToList(str.GetBuffer());


    PostUIData * pdata = CreateUIData();
    pdata->cb_in_ui_fun = [pdata,user, this]()->void
    {
        ProcessUserJoinRoom(user);

        DestroyUIData(pdata);
    };

    PostMsgDataToUI(pdata);
   

}

void JoinLiveLiving::OnUserLeave(ZegoJoinLiveUser user)
{
    CStringA str;
    str.Format("OnUserLeave, user id = %s, user name = %s", user.user_id.c_str(), user.user_name.c_str());
    AddLogToList(str.GetBuffer());

    PostUIData * pdata = CreateUIData();
    pdata->cb_in_ui_fun = [pdata, user, this]()->void
    {
        ProcessUserLeaveRoom(user);

        DestroyUIData(pdata);
    };

    PostMsgDataToUI(pdata);

    
}

void JoinLiveLiving::ProcessUserJoinRoom(ZegoJoinLiveUser user)
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
            
            join_live_.SetRemoteVideoView(user_list_info_[i].view_hwnd, user, ZEGO::LIVEROOM::ZegoVideoViewModeScaleAspectFill);
            break;
        }
    }

    if (!processed)
    {
        AddLogToList("已拉流的人数超过指定数，不在进行拉流");
    }
}

void JoinLiveLiving::ProcessUserLeaveRoom(ZegoJoinLiveUser user)
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

void JoinLiveLiving::ResetControlBackgroundColorIfUserLeave()
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

void JoinLiveLiving::ClearControlColor(HWND hwnd)
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

void JoinLiveLiving::CreateRoom(std::string room_id)
{
    Init();

    GetDlgItem(IDC_BUTTON_JOIN)->ShowWindow(SW_HIDE);
    join_live_.SetLocalVideoView(GetDlgItem(IDC_JOIN_LIVE_VIDEO_1)->GetSafeHwnd());
    join_live_.LoginRoom(room_id, true);

}

void JoinLiveLiving::JoinRoom(std::string room_id)
{
    Init();
    GetDlgItem(IDC_BUTTON_JOIN)->ShowWindow(SW_SHOW);
    join_live_.LoginRoom(room_id, false);
}

BOOL JoinLiveLiving::OnInitDialog()
{
    CDialogEx::OnInitDialog();



    return TRUE;
}

void JoinLiveLiving::Init()
{

    log_list_.ResetContent();

    user_list_info_[0].view_hwnd = GetDlgItem(IDC_JOIN_LIVE_VIDEO_2)->GetSafeHwnd();
    user_list_info_[1].view_hwnd = GetDlgItem(IDC_JOIN_LIVE_VIDEO_3)->GetSafeHwnd();
    user_list_info_[2].view_hwnd = GetDlgItem(IDC_JOIN_LIVE_VIDEO_4)->GetSafeHwnd();

    join_live_.SetDemoCallBack(this);
    join_live_.SetLogCallBack(this);
    join_live_.InitMainHwnd(GetSafeHwnd());
}

void JoinLiveLiving::ResetContronlStatus()
{
    std::thread([=]() {

        std::this_thread::sleep_for(std::chrono::milliseconds(200));

        // 没有视频了则清空控件颜色为原来的背景颜色
        PostUIData * pdata = CreateUIData();
        HWND need_reset_hwnd = GetDlgItem(IDC_JOIN_LIVE_VIDEO_1)->GetSafeHwnd();;
        pdata->cb_in_ui_fun = [need_reset_hwnd, pdata, this]()->void
        {
            ClearControlColor(need_reset_hwnd);

            DestroyUIData(pdata);
        };

        PostMsgDataToUI(pdata);

    }).detach();

    for (int i = 0; i < kMaxOtherUserCount; ++i)
    {
        user_list_info_[i].is_user_in_room = false;
    }

    ResetControlBackgroundColorIfUserLeave();
}

void JoinLiveLiving::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_JOINLIVE_LOG_LIST, log_list_);
}


BEGIN_MESSAGE_MAP(JoinLiveLiving, CDialogEx)
    ON_BN_CLICKED(IDC_BUTTON_JOIN, &JoinLiveLiving::OnBnClickedButtonJoin)
    ON_BN_CLICKED(IDOK, &JoinLiveLiving::OnBnClickedOk)
    ON_MESSAGE(UI_CALLBACK_MSG, &JoinLiveLiving::OnUICallbackMsg)
END_MESSAGE_MAP()


// JoinLiveLiving 消息处理程序


void JoinLiveLiving::OnBnClickedButtonJoin()
{
    join_live_.SetLocalVideoView(GetDlgItem(IDC_JOIN_LIVE_VIDEO_1)->GetSafeHwnd());
    join_live_.StartPublish();

    
}
void JoinLiveLiving::PrintToView(const char * log_str)
{
    AddLogToList(log_str);
}

void JoinLiveLiving::AddLogToList(std::string str_log)
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

// 退出登录
void JoinLiveLiving::OnBnClickedOk()
{
    ResetContronlStatus();

    join_live_.LogoutRoom();

    if (on_logout_cb_)
    {
        on_logout_cb_();
    }
}

void JoinLiveLiving::SetHScroll()
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
        temp = (long)SendDlgItemMessage(IDC_JOINLIVE_LOG_LIST, LB_GETHORIZONTALEXTENT, 0, 0);
        if (s.cx > temp)
        {
            SendDlgItemMessage(IDC_JOINLIVE_LOG_LIST, LB_SETHORIZONTALEXTENT, (WPARAM)s.cx + 1000, 0);
        }
    }
    ReleaseDC(dc);
}


afx_msg LRESULT JoinLiveLiving::OnUICallbackMsg(WPARAM wParam, LPARAM lParam)
{
    PostUIData * pdata = (PostUIData *)wParam;
    if (pdata != nullptr && pdata->cb_in_ui_fun)
    {
        pdata->cb_in_ui_fun();
    }
    return 0;
}