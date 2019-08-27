
// ScreenRecorderDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "ScreenRecorderDlg.h"
#include "afxdialogex.h"
#include "Topics/ScreenRecorder/ScreenRecorderUI/resource.h"
#include "AppSupport/ZGUtilTools.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CScreenRecorderDlg 对话框

CScreenRecorderDlg * CScreenRecorderDlg::CreateDlgInstance(CWnd * pParent)
{
	CScreenRecorderDlg * p = new CScreenRecorderDlg(pParent);
	p->Create(IDD_SCREENRECORDER_DIALOG, pParent);
	return p;
}

CScreenRecorderDlg::CScreenRecorderDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SCREENRECORDER_DIALOG, pParent)
{
	
}

void CScreenRecorderDlg::OnScreenRecorderFileStatus(const unsigned int duration, const unsigned int file_size)
{
	typedef struct UIDataStruct
	{
		string desc_record_file;
	} UIDataStruct;
	UIDataStruct ui_data;
	ui_data.desc_record_file = StringFormat("file duration:%d , file size:%d", duration, file_size);

	PostUIData * pdata = CreateUIData();
	pdata->cb_in_ui_fun = [ui_data, pdata, this]()->void
	{
		auto file_info = CString(CStringA(ui_data.desc_record_file.c_str()).GetBuffer());
		GetDlgItem(IDC_STATIC_RECORD_FILE_INFO)->SetWindowTextW(file_info);
		DestroyUIData(pdata);
	};

	PostMsgDataToUI(pdata);
}

void CScreenRecorderDlg::OnScreenRecordStateUpdate(string record_state)
{
	typedef struct UIDataStruct
	{
		string desc_record_state;
	} UIDataStruct;
	UIDataStruct ui_data;
	ui_data.desc_record_state = record_state;

	PostUIData * pdata = CreateUIData();
	pdata->cb_in_ui_fun = [ui_data, pdata, this]()->void
	{
		auto file_info = CString(CStringA(ui_data.desc_record_state.c_str()).GetBuffer());
		GetDlgItem(IDC_STATIC_RECORD_FILE_INFO)->SetWindowTextW(file_info);
		DestroyUIData(pdata);
	};

	PostMsgDataToUI(pdata);
}

void CScreenRecorderDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CScreenRecorderDlg, CDialogEx)
	ON_MESSAGE(UI_CALLBACK_MSG, &CScreenRecorderDlg::OnUICallbackMsg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_CHECK_PUBLISH, &CScreenRecorderDlg::OnBnClickedCheckPublish)
	ON_BN_CLICKED(IDC_BUTTON_START_RECORD, &CScreenRecorderDlg::OnBnClickedButtonStartRecord)
	ON_BN_CLICKED(IDC_BUTTON_STOP_RECORD, &CScreenRecorderDlg::OnBnClickedButtonStopRecord)
	ON_BN_CLICKED(IDC_BUTTON_PLAY, &CScreenRecorderDlg::OnBnClickedButtonPlay)
	ON_BN_CLICKED(IDC_BUTTON_RECORD_FILE_PATH, &CScreenRecorderDlg::OnBnClickedButtonRecordFilePath)
END_MESSAGE_MAP()


// CScreenRecorderDlg 消息处理程序

BOOL CScreenRecorderDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO: 在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CScreenRecorderDlg::OnPaint()
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

LRESULT CScreenRecorderDlg::OnUICallbackMsg(WPARAM wParam, LPARAM lParam)
{
	PostUIData * pdata = (PostUIData *)wParam;
	if (pdata != nullptr && pdata->cb_in_ui_fun)
	{
		pdata->cb_in_ui_fun();
	}
	return 0;
}

void CScreenRecorderDlg::InitDlg()
{

}

void CScreenRecorderDlg::LoginAndPreview()
{
	demo_.SetDemoCallBack(this);
	demo_helper_.LoginAndPreview(GetDlgItem(IDC_STATIC_LOCAL_VIDEO)->GetSafeHwnd());
}


void CScreenRecorderDlg::OnBnClickedCheckPublish()
{
	CButton * pub_check = (CButton *)GetDlgItem(IDC_CHECK_PUBLISH);

	if (pub_check->GetCheck())
	{
		demo_helper_.StartPublish();
	}
	else
	{
		demo_helper_.StopPublish();
	}
}


void CScreenRecorderDlg::OnBnClickedButtonStartRecord()
{
	config_.record_type = MEDIA_RECORDER::ZEGO_MEDIA_RECORD_BOTH;
	config_.record_format = MEDIA_RECORDER::ZEGO_MEDIA_RECORD_MP4;
	demo_.StartRecord(config_);
}


void CScreenRecorderDlg::OnBnClickedButtonStopRecord()
{
	demo_.StopRecord();
}


void CScreenRecorderDlg::OnBnClickedButtonPlay()
{
	ShellExecuteA(this->GetSafeHwnd(), "open", config_.record_file_path.c_str(), NULL, NULL, SW_SHOW);
}


void CScreenRecorderDlg::OnBnClickedButtonRecordFilePath()
{
	CString filter =  _T("文件 (*.mp4;)|*.mp4;|");
	CString defaultDir = _T("D:\\");
	CString default_file_name = _T("record.mp4");
	
	BOOL isOpen = FALSE;

	CString filePath(CStringA(config_.record_file_path.c_str()).GetBuffer());

	CFileDialog openFileDlg(isOpen, defaultDir, default_file_name, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, filter, NULL);
	openFileDlg.GetOFN().lpstrInitialDir = filePath;
	INT_PTR result = openFileDlg.DoModal();

	if (result == IDOK) {
		filePath = openFileDlg.GetPathName();

		save_file_path_ = filePath;
		config_.record_file_path = CStringA(save_file_path_.GetBuffer());

		GetDlgItem(IDC_EDIT_RECORD_FILE_PATH)->SetWindowTextW(save_file_path_);
	}
}
