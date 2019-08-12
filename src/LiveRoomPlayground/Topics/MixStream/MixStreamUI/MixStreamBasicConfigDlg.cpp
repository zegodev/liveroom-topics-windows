// MixStreamBasicConfigDlg.cpp: 实现文件
//

#include "stdafx.h"

#include "MixStreamBasicConfigDlg.h"
#include "afxdialogex.h"

#include "Resource.h"

// MixStreamBasicConfigDlg 对话框

IMPLEMENT_DYNAMIC(MixStreamBasicConfigDlg, CDialogEx)


MixStreamBasicConfigDlg::MixStreamBasicConfigDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MIXSTREAM_BASIC_CONFIG, pParent)
    , publish_stream_id_(_T(""))
    , output_mixstream_id_(_T(""))
    , room_id_(_T(""))
{

}

MixStreamBasicConfigDlg::~MixStreamBasicConfigDlg()
{
}

void MixStreamBasicConfigDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_STREAM_ID, publish_stream_id_);
    DDX_Text(pDX, IDC_MIXSTREAM_STREAMID, output_mixstream_id_);
    DDX_Text(pDX, IDC_MIX_STREAM_ROOM_ID, room_id_);
}


BEGIN_MESSAGE_MAP(MixStreamBasicConfigDlg, CDialogEx)
    ON_BN_CLICKED(IDCANCEL, &MixStreamBasicConfigDlg::OnBnClickedCancel)
    ON_BN_CLICKED(IDOK, &MixStreamBasicConfigDlg::OnBnClickedOk)
    ON_BN_CLICKED(IDC_MIX_ADVANCE_CONFIG, &MixStreamBasicConfigDlg::OnBnClickedMixAdvanceConfig)
END_MESSAGE_MAP()


// MixStreamBasicConfigDlg 消息处理程序


void MixStreamBasicConfigDlg::OnBnClickedCancel()
{

}


void MixStreamBasicConfigDlg::OnBnClickedOk()
{
    UpdateData(TRUE);

    if (room_id_ == _T(""))
    {
        MessageBox(_T("请输入房间id"));
        return;
    }

    if (publish_stream_id_ == _T(""))
    {
        MessageBox(_T("请输入流id"));
        return;
    }

    if (output_mixstream_id_ == _T(""))
    {
        MessageBox(_T("请输入混流id"));
        return;
    }

    if (event_cb_ != nullptr)
    {
        event_cb_->OnBasicConfigOkClickedEvent();
    }
}


void MixStreamBasicConfigDlg::OnBnClickedMixAdvanceConfig()
{
    if (event_cb_ != nullptr)
    {
        event_cb_->OnAdvanceConfigClickedEvent();
    }
}
