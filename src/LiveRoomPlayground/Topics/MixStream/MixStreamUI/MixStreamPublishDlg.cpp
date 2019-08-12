// MixStreamPublishDlg.cpp: 实现文件
//

#include "stdafx.h"

#include "MixStreamPublishDlg.h"
#include "afxdialogex.h"

#include <thread>

#include "resource.h"

// MixStreamPublishDlg 对话框

IMPLEMENT_DYNAMIC(MixStreamPublishDlg, CDialogEx)


MixStreamPublishDlg::MixStreamPublishDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MIXSTREAM_PUBLISH, pParent)
{
    user_list_info_.resize(kMaxUserCount);
}

MixStreamPublishDlg::~MixStreamPublishDlg()
{
}

void MixStreamPublishDlg::OnLoginEvent(ZGMixStreamLoginEvent event, ZGMixStreamJoinLiveStatus status)
{
    CStringA str;
    str.Format("OnLoginEvent, event = %d, status = %d", event, status);
    AddLogToList(str.GetBuffer());
}

void MixStreamPublishDlg::OnUserJoin(ZegoMixStreamUser user)
{
    if (is_anchor_ == true)
    {
        CStringA str;
        str.Format("OnUserJoin, user id = %s, user name = %s", user.user_id.c_str(), user.user_name.c_str());
        AddLogToList(str.GetBuffer());

        PostUIData * pdata = CreateUIData();
        pdata->cb_in_ui_fun = [pdata, user, this]()->void
        {
            ProcessUserJoinRoom(user);

            DestroyUIData(pdata);
        };

        PostMsgDataToUI(pdata);
    }
}

void MixStreamPublishDlg::OnUserLeave(ZegoMixStreamUser user)
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

void MixStreamPublishDlg::ProcessUserJoinRoom(ZegoMixStreamUser user)
{
    bool processed = false;
    for (int i = 0; i < kMaxUserCount; ++i)
    {
        if (!user_list_info_[i].is_user_in_room)
        {
            processed = true;
            user_list_info_[i].is_user_in_room = true;
            user_list_info_[i].user.user_id = user.user_id;
            user_list_info_[i].user.user_name = user.user_name;

            mix_stream_demo_.SetRemoteVideoView(user_list_info_[i].view_hwnd, user, ZEGO::LIVEROOM::ZegoVideoViewModeScaleAspectFit);
            break;
        }
    }

    if (!processed)
    {
        AddLogToList("已拉流的人数超过指定数，不在进行拉流");
    }
}

void MixStreamPublishDlg::PrintToView(const char * log_str)
{
    AddLogToList(log_str);
}

void MixStreamPublishDlg::OnMixStreamUpdate(ZegoMixStreamUser anchor, string mix_stream_id)
{
    room_anchor_ = anchor;
    mixstream_id_ = mix_stream_id;

    CStringA str;
    str.Format("获取到房间内的混流流id信息：mix_stream_id= %s", mix_stream_id.c_str());
    AddLogToList(str.GetBuffer());

    PostUIData * pdata = CreateUIData();
    pdata->cb_in_ui_fun = [mix_stream_id, pdata, this]()->void
    {
        AddLogToList("开始拉混流");
        // 通过混流id，拉流
        mix_stream_demo_.StartPlayingStream(mix_stream_id.c_str(), GetDlgItem(IDC_MIXSTREAM_VIDEO2)->GetSafeHwnd());

        CStringA mix_str;
        mix_str.Format("%s", mix_stream_id.c_str());

        GetDlgItem(IDC_MIXSTREAM_PUBLISH_MIXSTREAM_ID)->SetWindowText(CString(mix_str.GetBuffer()));

        DestroyUIData(pdata);
    };

    PostMsgDataToUI(pdata);
}

void MixStreamPublishDlg::OnSoundLevelUpdate(const std::vector<MixStreamSoundLevelInfo> & sound_levels)
{
    for (int i = 0;i < sound_levels.size(); ++i)
    {
        if (sound_levels[i].user_id == room_anchor_.user_id)
        {
            PostUIData * pdata = CreateUIData();
            int sound_level = sound_levels[i].sound_level;
            pdata->cb_in_ui_fun = [sound_level, pdata, this]()->void
            {
                CStringA sound_level_str;
                sound_level_str.Format("%d", sound_level);

                GetDlgItem(IDC_MIXSTREAM_PUBLISH_ANCHOR_SOUND_VALUE)->SetWindowText(CString(sound_level_str.GetBuffer()));

                DestroyUIData(pdata);
            };

            PostMsgDataToUI(pdata);
        }
        else {

            PostUIData * pdata = CreateUIData();
            int sound_level = sound_levels[i].sound_level;
            pdata->cb_in_ui_fun = [sound_level, pdata, this]()->void
            {
                CStringA sound_level_str;
                sound_level_str.Format("%d", sound_level);

                GetDlgItem(IDC_MIXSTREAM_JOINER_SOUND_VALUE)->SetWindowText(CString(sound_level_str.GetBuffer()));

                DestroyUIData(pdata);
            };

            PostMsgDataToUI(pdata);
        }
    }
}

void MixStreamPublishDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_MIXSTREAM_PUBLISH_LOG, log_list_);
}


BEGIN_MESSAGE_MAP(MixStreamPublishDlg, CDialogEx)
    ON_BN_CLICKED(IDC_MIX_PUBLISH_LOGOUT, &MixStreamPublishDlg::OnBnClickedMixPublishLogout)
    ON_BN_CLICKED(IDC_MIX_PUBLISH_JOINLIVE, &MixStreamPublishDlg::OnBnClickedMixPublishJoinlive)
    ON_MESSAGE(UI_CALLBACK_MSG, &MixStreamPublishDlg::OnUICallbackMsg)
    ON_BN_CLICKED(IDC_BUTTON_MIX_STREAM_ADVANCE_CONFIG, &MixStreamPublishDlg::OnBnClickedButtonMixStreamAdvanceConfig)
END_MESSAGE_MAP()


// MixStreamPublishDlg 消息处理程序

void MixStreamPublishDlg::StopPlayingMixStream()
{
    mix_stream_demo_.StopPlayingStream(mixstream_id_.c_str());
}

void MixStreamPublishDlg::StartPublish(std::string room_id, bool is_anchor/* = true*/)
{
    is_anchor_ = is_anchor;

    log_list_.ResetContent();

    user_list_info_[0].view_hwnd = GetDlgItem(IDC_MIXSTREAM_VIDEO2)->GetSafeHwnd();

    if (is_anchor)
    {
        GetDlgItem(IDC_MIX_PUBLISH_JOINLIVE)->ShowWindow(SW_HIDE);
    }
    else {
        GetDlgItem(IDC_BUTTON_MIX_STREAM_ADVANCE_CONFIG)->ShowWindow(SW_HIDE);
    }

    mix_stream_demo_.SetLogCallBack(this);
    mix_stream_demo_.SetDemoCallBack(this);
    mix_stream_demo_.InitMainHwnd(this->GetSafeHwnd());

    mix_stream_demo_.SetLocalVideoView(GetDlgItem(IDC_MIXSTREAM_VIDEO1)->GetSafeHwnd());

    bool need_publish = is_anchor ? true : false;

    mix_stream_demo_.StopMixStream();

    mix_stream_demo_.LogoutRoom();
    
    mix_stream_demo_.InitStreamId(publish_stream_id_, mixstream_id_);

    mix_stream_demo_.LoginRoom(room_id.c_str(), need_publish);

}

void MixStreamPublishDlg::OnBnClickedMixPublishLogout()
{
    mix_stream_demo_.LogoutRoom();

    mix_stream_demo_.SetLocalVideoView(nullptr);

    std::thread([=]() {

        std::this_thread::sleep_for(std::chrono::milliseconds(200));

        // 没有视频了则清空控件颜色为原来的背景颜色
        PostUIData * pdata = CreateUIData();
        HWND need_reset_hwnd = GetDlgItem(IDC_MIXSTREAM_VIDEO1)->GetSafeHwnd();;
        pdata->cb_in_ui_fun = [need_reset_hwnd, pdata, this]()->void
        {
            ClearControlColor(need_reset_hwnd);

            DestroyUIData(pdata);
        };

        PostMsgDataToUI(pdata);

    }).detach();

    for (int i = 0; i < kMaxUserCount; ++i)
    {
        user_list_info_[i].is_user_in_room = false;
    }

    ResetControlBackgroundColorIfUserLeave();

    if (on_logout_cb_)
    {
        on_logout_cb_();
    }

}

void MixStreamPublishDlg::OnBnClickedMixPublishJoinlive()
{
    mix_stream_demo_.SetLocalVideoView(GetDlgItem(IDC_MIXSTREAM_VIDEO1)->GetSafeHwnd());
    mix_stream_demo_.StartPublish();

    StopPlayingMixStream();

    ProcessUserJoinRoom(room_anchor_);
}

afx_msg LRESULT MixStreamPublishDlg::OnUICallbackMsg(WPARAM wParam, LPARAM lParam)
{
    PostUIData * pdata = (PostUIData *)wParam;
    if (pdata != nullptr && pdata->cb_in_ui_fun)
    {
        pdata->cb_in_ui_fun();
    }
    return 0;
}


void MixStreamPublishDlg::SetHScroll()
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
        temp = (long)SendDlgItemMessage(IDC_MIXSTREAM_PUBLISH_LOG, LB_GETHORIZONTALEXTENT, 0, 0);
        if (s.cx > temp)
        {
            SendDlgItemMessage(IDC_MIXSTREAM_PUBLISH_LOG, LB_SETHORIZONTALEXTENT, (WPARAM)s.cx, 0);
        }
    }
    ReleaseDC(dc);
}

void MixStreamPublishDlg::AddLogToList(std::string str_log)
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

void MixStreamPublishDlg::OnBnClickedButtonMixStreamAdvanceConfig()
{
    if (mix_stream_config_dlg_.DoModal() == IDOK)
    {
        mix_stream_demo_.UpdateMixStream();
    }
}


void MixStreamPublishDlg::ProcessUserLeaveRoom(ZegoMixStreamUser user)
{
    for (int i = 0; i < kMaxUserCount; ++i)
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

void MixStreamPublishDlg::ResetControlBackgroundColorIfUserLeave()
{
    for (int i = 0; i < kMaxUserCount; ++i)
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

void MixStreamPublishDlg::ClearControlColor(HWND hwnd)
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
