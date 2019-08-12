// JoinLiveRoomList.cpp: 实现文件
//

#include "stdafx.h"

#include "JoinLiveRoomList.h"
#include "afxdialogex.h"

#include "resource.h"


// JoinLiveRoomList 对话框

IMPLEMENT_DYNAMIC(JoinLiveRoomList, CDialogEx)

JoinLiveRoomList * JoinLiveRoomList::CreateDlgInstance(CWnd * pParent)
{
    JoinLiveRoomList * p = new JoinLiveRoomList(pParent);
    p->Create(IDD_DIALOG_JOIN_LIVE_ROOMLIST, pParent);
    return p;
}

JoinLiveRoomList::JoinLiveRoomList(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_JOIN_LIVE_ROOMLIST, pParent)
{

}

JoinLiveRoomList::~JoinLiveRoomList()
{
}

void JoinLiveRoomList::UpdateRoomList(std::vector<RoomInfo> room_list)
{
    room_list_.ResetContent();

    for (int i = 0; i < room_list.size(); ++i)
    {
        CStringA str;
        str.Format("%s", room_list[i].room_id.substr(kJoinLiveRoomIDPrefix.length(), room_list[i].room_id.length() - kJoinLiveRoomIDPrefix.length()).c_str());
        room_list_.AddString(CString(str));
    }
}

BOOL JoinLiveRoomList::OnInitDialog()
{
    
    CDialogEx::OnInitDialog();

    UpdateRoomList();
    SetTimer(kGetRoomListTimerID, 2000, nullptr);


    return TRUE;

}

void JoinLiveRoomList::OnRoomListUpdate(std::vector<RoomInfo> room_list)
{
    room_vec_info_.clear();

    for (int i = 0; i < room_list.size(); ++i)
    {
        if (room_list[i].room_id.length() > kJoinLiveRoomIDPrefix.length() &&
            room_list[i].room_id.substr(0, kJoinLiveRoomIDPrefix.length()) == kJoinLiveRoomIDPrefix)
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


void JoinLiveRoomList::UpdateRoomList()
{
    ZGManagerInstance()->GetRoomList(true, std::bind(&JoinLiveRoomList::OnRoomListUpdate, this, std::placeholders::_1));
}


void JoinLiveRoomList::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_JOINLIVE_ROOMID, room_id_);
    DDX_Control(pDX, IDC_JOINLIVE_ROOMLIST, room_list_);

}


BEGIN_MESSAGE_MAP(JoinLiveRoomList, CDialogEx)
    ON_BN_CLICKED(IDCANCEL, &JoinLiveRoomList::OnBnClickedCancel)
    ON_LBN_SELCHANGE(IDC_JOINLIVE_ROOMLIST, &JoinLiveRoomList::OnLbnSelchangeJoinliveRoomlist)
    ON_BN_CLICKED(IDOK, &JoinLiveRoomList::OnBnClickedOk)
    ON_WM_TIMER()
    ON_MESSAGE(UI_CALLBACK_MSG, &JoinLiveRoomList::OnUICallbackMsg)
END_MESSAGE_MAP()


// JoinLiveRoomList 消息处理程序


void JoinLiveRoomList::OnBnClickedCancel()
{

}


void JoinLiveRoomList::OnLbnSelchangeJoinliveRoomlist()
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
        if (on_join_room_cb_)
        {
            CStringA room_id = CStringA(cur_sel_str_room_id.GetBuffer());
            std::string real_room_id = kJoinLiveRoomIDPrefix + std::string(room_id.GetBuffer());
            on_join_room_cb_(real_room_id);
        }
    }

}

void JoinLiveRoomList::OnTimer(UINT_PTR nIDEvent)
{
    if (nIDEvent == kGetRoomListTimerID)
    {
        UpdateRoomList();
    }

    CDialogEx::OnTimer(nIDEvent);
}



void JoinLiveRoomList::OnBnClickedOk()
{
    UpdateData(TRUE);

    if (room_id_ == _T(""))
    {
        MessageBox(_T("请输入roomID"));
        return;
    }

    CStringA str(room_id_.GetBuffer());

    if (on_create_room_cb_)
    {
        std::string real_room_id = kJoinLiveRoomIDPrefix + std::string(str.GetBuffer());
        on_create_room_cb_(real_room_id);
    }
}

afx_msg LRESULT JoinLiveRoomList::OnUICallbackMsg(WPARAM wParam, LPARAM lParam)
{
    PostUIData * pdata = (PostUIData *)wParam;
    if (pdata != nullptr && pdata->cb_in_ui_fun)
    {
        pdata->cb_in_ui_fun();
    }
    return 0;
}
