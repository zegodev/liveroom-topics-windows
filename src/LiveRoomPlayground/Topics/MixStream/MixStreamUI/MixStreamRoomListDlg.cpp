// MixStreamRoomListDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "ZegoLiveRoomTopics.h"
#include "MixStreamRoomListDlg.h"
#include "afxdialogex.h"

#include "resource.h"

void MixStreamRoomListDlg::SetDlgEventCallBack(RoomListDlgEvent * cb)
{
    dlg_cb_ = cb;
}

// MixStreamRoomListDlg 对话框

IMPLEMENT_DYNAMIC(MixStreamRoomListDlg, CDialogEx)

MixStreamRoomListDlg::MixStreamRoomListDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_MIXSTREAM_ROOMLIST_DLG, pParent)
{

}

MixStreamRoomListDlg::~MixStreamRoomListDlg()
{
}

BOOL MixStreamRoomListDlg::OnInitDialog()
{
    
    CDialogEx::OnInitDialog();

    UpdateRoomList();
    SetTimer(kGetRoomListTimerID, 2000, nullptr);

    return TRUE;
}

void MixStreamRoomListDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_MIXSTREAM_ROOMLIST, room_list_);
}


BEGIN_MESSAGE_MAP(MixStreamRoomListDlg, CDialogEx)
    ON_BN_CLICKED(IDOK, &MixStreamRoomListDlg::OnBnClickedOk)
    ON_MESSAGE(UI_CALLBACK_MSG, &MixStreamRoomListDlg::OnUICallbackMsg)
    ON_LBN_SELCHANGE(IDC_MIXSTREAM_ROOMLIST, &MixStreamRoomListDlg::OnLbnSelchangeMixstreamRoomlist)
    ON_WM_TIMER()
    ON_BN_CLICKED(IDCANCEL, &MixStreamRoomListDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// MixStreamRoomListDlg 消息处理程序


void MixStreamRoomListDlg::OnBnClickedOk()
{
    if (dlg_cb_ != nullptr)
    {
        dlg_cb_->OnCreateRoomClickedEvent();
    }
}

void MixStreamRoomListDlg::UpdateRoomList(std::vector<RoomInfo> room_list)
{
    room_list_.ResetContent();

    for (int i = 0; i < room_list.size(); ++i)
    {
        CStringA str;
        str.Format("%s", room_list[i].room_id.substr(kMixStreamRoomIDPrefix.length(), room_list[i].room_id.length() - kMixStreamRoomIDPrefix.length()).c_str());
        room_list_.AddString(CString(str));
    }
}


void MixStreamRoomListDlg::OnRoomListUpdate(std::vector<RoomInfo> room_list)
{
    room_vec_info_.clear();

    for (int i = 0; i < room_list.size(); ++i)
    {
        if (room_list[i].room_id.length() > kMixStreamRoomIDPrefix.length() &&
            room_list[i].room_id.substr(0, kMixStreamRoomIDPrefix.length()) == kMixStreamRoomIDPrefix)
        {
            room_vec_info_.push_back(room_list[i]);
        }
    }

    PostUIData * pdata = CreateUIData();
    pdata->cb_in_ui_fun = [room_list, pdata, this]()->void
    {

        UpdateRoomList(room_vec_info_);

        DestroyUIData(pdata);
    };

    PostMsgDataToUI(pdata);
}


void MixStreamRoomListDlg::UpdateRoomList()
{
    ZGManagerInstance()->GetRoomList(true, std::bind(&MixStreamRoomListDlg::OnRoomListUpdate, this, std::placeholders::_1));
}


afx_msg LRESULT MixStreamRoomListDlg::OnUICallbackMsg(WPARAM wParam, LPARAM lParam)
{
    PostUIData * pdata = (PostUIData *)wParam;
    if (pdata != nullptr && pdata->cb_in_ui_fun)
    {
        pdata->cb_in_ui_fun();
    }
    return 0;
}


void MixStreamRoomListDlg::OnLbnSelchangeMixstreamRoomlist()
{
    int cur_sel_index = room_list_.GetCurSel();
    if (cur_sel_index < 0)
    {
        return;
    }

    CString cur_sel_str_room_id;
    room_list_.GetText(cur_sel_index, cur_sel_str_room_id);

    CString tip_caption;
    tip_caption.Format(_T("是否加入房间%s?"), cur_sel_str_room_id);
    int ret = MessageBox(tip_caption, _T("加入房间提醒"), MB_OKCANCEL);
    if (ret == IDOK)
    {
        if (dlg_cb_ != nullptr)
        {
            CStringA room_id = CStringA(cur_sel_str_room_id.GetBuffer());
            std::string real_room_id = kMixStreamRoomIDPrefix + std::string(room_id.GetBuffer());
            dlg_cb_->OnEnterRoomClickedEvent(real_room_id);
        }
    }
}

void MixStreamRoomListDlg::OnTimer(UINT_PTR nIDEvent)
{
    if (nIDEvent == kGetRoomListTimerID)
    {
        UpdateRoomList();
    }

    CDialogEx::OnTimer(nIDEvent);
}



void MixStreamRoomListDlg::OnBnClickedCancel()
{

}
