// MixStreamAdvanceConfigDlg.cpp: 实现文件
//

#include "stdafx.h"

#include "MixStreamAdvanceConfigDlg.h"
#include "afxdialogex.h"

#include "resource.h"

#include "MixStream/MixStreamHelper.h"

// MixStreamAdvanceConfigDlg 对话框

IMPLEMENT_DYNAMIC(MixStreamAdvanceConfigDlg, CDialogEx)

MixStreamAdvanceConfigDlg::MixStreamAdvanceConfigDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_MIXSTREAM_ADVANCE_CONFIG, pParent)
{

}

MixStreamAdvanceConfigDlg::~MixStreamAdvanceConfigDlg()
{

}

BOOL MixStreamAdvanceConfigDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    output_resolution_.ResetContent();
    output_resolution_.AddString(_T("(极清)1920x1080"));
    output_resolution_.AddString(_T("(超清)1280x720"));
    output_resolution_.AddString(_T("(高清)960x540"));
    output_resolution_.AddString(_T("(标准)640x360"));
    output_resolution_.AddString(_T("(流畅)480x270"));
    output_resolution_.AddString(_T("低清)320x180"));

    switch (MixStreamHelper::GetMixStreamConfig().output_w)
    {
    case 1920:
        output_resolution_.SetCurSel(0);
        break;
    case 1280:
        output_resolution_.SetCurSel(1);
        break;
    case 960:
        output_resolution_.SetCurSel(2);
        break;
    case 640:
        output_resolution_.SetCurSel(3);
        break;
    case 480:
        output_resolution_.SetCurSel(4);
        break;
    case 320:
        output_resolution_.SetCurSel(5);
        break;

    default:
        output_resolution_.SetCurSel(3);
        break;
    }
    

    output_video_bitrate_.ResetContent();
    output_video_bitrate_.AddString(_T("(3M)3000000"));
    output_video_bitrate_.AddString(_T("(1.5M)1500000"));
    output_video_bitrate_.AddString(_T("(1.2M)1200000"));
    output_video_bitrate_.AddString(_T("(600KB)600000"));
    output_video_bitrate_.AddString(_T("(400KB)400000"));
    output_video_bitrate_.AddString(_T("(300KB)300000"));

    switch (MixStreamHelper::GetMixStreamConfig().bitrate)
    {
    case 3000000:
        output_video_bitrate_.SetCurSel(0);
        break;
    case 1500000:
        output_video_bitrate_.SetCurSel(1);
        break;
    case 1200000:
        output_video_bitrate_.SetCurSel(2);
        break;
    case 600000:
        output_video_bitrate_.SetCurSel(3);
        break;
    case 400000:
        output_video_bitrate_.SetCurSel(4);
        break;
    case 300000:
        output_video_bitrate_.SetCurSel(5);
        break;
        output_video_bitrate_.SetCurSel(3);
    default:
        break;
    }

    output_fps_.ResetContent();
    output_fps_.AddString(_T("10"));
    output_fps_.AddString(_T("11"));
    output_fps_.AddString(_T("12"));
    output_fps_.AddString(_T("13"));
    output_fps_.AddString(_T("14"));
    output_fps_.AddString(_T("15"));
    output_fps_.AddString(_T("16"));
    output_fps_.AddString(_T("17"));
    output_fps_.AddString(_T("18"));
    output_fps_.AddString(_T("19"));
    output_fps_.AddString(_T("20"));
    output_fps_.AddString(_T("21"));
    output_fps_.AddString(_T("22"));
    output_fps_.AddString(_T("23"));
    output_fps_.AddString(_T("24"));
    output_fps_.AddString(_T("25"));
    output_fps_.AddString(_T("26"));
    output_fps_.AddString(_T("27"));
    output_fps_.AddString(_T("28"));
    output_fps_.AddString(_T("29"));
    output_fps_.AddString(_T("30"));


    if (MixStreamHelper::GetMixStreamConfig().fps > 10 && MixStreamHelper::GetMixStreamConfig().fps <= 30)
    {
        output_fps_.SetCurSel(MixStreamHelper::GetMixStreamConfig().fps - 10);
    }
    else {
        output_fps_.SetCurSel(5);
    }

    if (MixStreamHelper::GetMixStreamConfig().audio_channel_count == 2)
    {
        channel_type_.SetCheck(TRUE);
    }
    else {
        channel_type_.SetCheck(FALSE);
    }

    if (MixStreamHelper::GetMixStreamConfig().with_sound_level)
    {
        sound_flag_.SetCheck(TRUE);
    }
    else {
        sound_flag_.SetCheck(FALSE);
    }


    return TRUE;
}

void MixStreamAdvanceConfigDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_CHECK_CHANNEL_TYPE, channel_type_);
    DDX_Control(pDX, IDC_COMBO_MIXSTREAM_OUTPUT_FPS, output_fps_);
    DDX_Control(pDX, IDC_COMBO_MIXSTREAM_OUTPUT_FRAMERATE, output_video_bitrate_);
    DDX_Control(pDX, IDC_COMBO_MIXSTREAM_OUTPUT_RESOLUTION, output_resolution_);
    DDX_Control(pDX, IDC_CHECK_SOUND_FLAG, sound_flag_);
}


BEGIN_MESSAGE_MAP(MixStreamAdvanceConfigDlg, CDialogEx)
    ON_BN_CLICKED(IDOK, &MixStreamAdvanceConfigDlg::OnBnClickedOk)
    ON_BN_CLICKED(IDCANCEL, &MixStreamAdvanceConfigDlg::OnBnClickedCancel)
    ON_BN_CLICKED(IDC_CHECK_CHANNEL_TYPE, &MixStreamAdvanceConfigDlg::OnBnClickedCheckChannelType)
    ON_BN_CLICKED(IDC_CHECK_SOUND_FLAG, &MixStreamAdvanceConfigDlg::OnBnClickedCheckSoundFlag)
    ON_CBN_SELCHANGE(IDC_COMBO_MIXSTREAM_OUTPUT_RESOLUTION, &MixStreamAdvanceConfigDlg::OnCbnSelchangeComboMixstreamOutputResolution)
    ON_CBN_SELCHANGE(IDC_COMBO_MIXSTREAM_OUTPUT_FRAMERATE, &MixStreamAdvanceConfigDlg::OnCbnSelchangeComboMixstreamOutputFramerate)
    ON_CBN_SELCHANGE(IDC_COMBO_MIXSTREAM_OUTPUT_FPS, &MixStreamAdvanceConfigDlg::OnCbnSelchangeComboMixstreamOutputFps)
END_MESSAGE_MAP()


// MixStreamAdvanceConfigDlg 消息处理程序


void MixStreamAdvanceConfigDlg::OnBnClickedOk()
{
    CDialogEx::OnOK();
}


void MixStreamAdvanceConfigDlg::OnBnClickedCancel()
{
    CDialogEx::OnCancel();
}


void MixStreamAdvanceConfigDlg::OnBnClickedCheckChannelType()
{
    if ((CButton*)channel_type_.GetCheck())
    {
        MixStreamHelper::GetMixStreamConfig().audio_channel_count = 2;
    }
    else {
        MixStreamHelper::GetMixStreamConfig().audio_channel_count = 1;
    }
}


void MixStreamAdvanceConfigDlg::OnBnClickedCheckSoundFlag()
{
    if ((CButton*)sound_flag_.GetCheck())
    {
        MixStreamHelper::GetMixStreamConfig().with_sound_level = true;
    }
    else {
        MixStreamHelper::GetMixStreamConfig().with_sound_level = false;
    }
}

void MixStreamAdvanceConfigDlg::OnCbnSelchangeComboMixstreamOutputResolution()
{
    int sel_index = output_resolution_.GetCurSel();
    int w = 0, h = 0;
    switch (sel_index)
    {
    case 0:
        w = 1920;
        h = 1080;
        break;
    case 1:
        w = 1280;
        h = 720;
        break;
    case 2:
        w = 960;
        h = 540;
        break;
    case 3:
        w = 640;
        h = 360;
        break;
    case 4:
        w = 480;
        h = 270;
        break;
    case 5:
        w = 320;
        h = 180;
        break;
    default:
        break;
    }

    MixStreamHelper::GetMixStreamConfig().output_w = w;
    MixStreamHelper::GetMixStreamConfig().output_h = h;

}


void MixStreamAdvanceConfigDlg::OnCbnSelchangeComboMixstreamOutputFramerate()
{
    int sel_index = output_video_bitrate_.GetCurSel();
    int bitrates[] = { 3000000 , 1500000 , 1200000 , 600000 ,400000, 300000,12000000 };
    if (sel_index < sizeof(bitrates) / sizeof(int))
    {
        MixStreamHelper::GetMixStreamConfig().bitrate = bitrates[sel_index];
    }
}


void MixStreamAdvanceConfigDlg::OnCbnSelchangeComboMixstreamOutputFps()
{
    int sel_index = output_fps_.GetCurSel();
    int frame_rates[] = { 10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30 };
    if (sel_index < sizeof(frame_rates) / sizeof(int))
    {
        MixStreamHelper::GetMixStreamConfig().fps = frame_rates[sel_index];
    }
}
