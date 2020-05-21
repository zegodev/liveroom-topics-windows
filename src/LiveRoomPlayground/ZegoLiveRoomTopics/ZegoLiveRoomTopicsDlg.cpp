
// ZegoLiveRoomTopicsDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ZegoLiveRoomTopics.h"
#include "ZegoLiveRoomTopicsDlg.h"
#include "afxdialogex.h"

#include "ZegoLiveRoom/LiveRoom.h"

#include <windows.h>
#include <DbgHelp.h>
#pragma comment(lib,"DbgHelp.lib")


#define DUMP_FILE_NAME _T("ZegoLiveRoomTopicsDemo.dmp")

void CreateDumpFile(LPCTSTR lpstrDumpFilePathName, EXCEPTION_POINTERS *pException)
{
    HANDLE hDumpFile = CreateFile(lpstrDumpFilePathName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    MINIDUMP_EXCEPTION_INFORMATION dumpInfo;
    dumpInfo.ExceptionPointers = pException;
    dumpInfo.ThreadId = GetCurrentThreadId();
    dumpInfo.ClientPointers = TRUE;
    MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), hDumpFile, MiniDumpNormal, &dumpInfo, NULL, NULL);
    CloseHandle(hDumpFile);
}

LONG ApplicationCrashHandler(EXCEPTION_POINTERS *pException)
{
    CreateDumpFile(DUMP_FILE_NAME, pException);
    return EXCEPTION_EXECUTE_HANDLER;
}

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define TOPIC_PUBLISH                   _T("����")
#define TOPIC_PLAY                      _T("����")
#define TOPIC_VIDEO_COMMUNICATION       _T("��Ƶͨ��")
#define TOPIC_JOIN_LIVE                 _T("ֱ������")
#define TOPIC_MIXSTREAM                 _T("����")
#define TOPIC_EXTERNAL_VIDEO_FILTER     _T("�ⲿ��Ƶ�˾�")

#define TOPIC_MIDIA_PLAYER              _T("MediaPlayer")
#define TOPIC_MEDIA_SIDE_INFO           _T("Media Side Info")
#define TOPIC_MEDIA_RECORDER            _T("Media Recorder")
#define TOPIC_EXTERNAL_VIDEO_CAPTURE    _T("External Video Capture")
#define TOPIC_EXTERNAL_VIDEO_RENDER     _T("External Video Render")
#define TOPIC_SCREEN_RECORDER			_T("Screen Recorder")
#define TOPIC_ROOM_MESSAGE			_T("Room Message")






// TODO:

#ifndef BUILD_NUMBER

#define DEMO_VERSION "1.0.0"

#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnBnClickedOk();
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
    ON_BN_CLICKED(IDOK, &CAboutDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CZegoLiveRoomTopicsDlg �Ի���



CZegoLiveRoomTopicsDlg::CZegoLiveRoomTopicsDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_ZEGOLIVEROOMTOPICS_DIALOG, pParent),

#ifdef ZEGO_MEDIA_PLAYER
    media_play_dlg_ptr_(nullptr),
#endif
    media_recorder_dlg_ptr_(nullptr)
{
	//m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CZegoLiveRoomTopicsDlg::~CZegoLiveRoomTopicsDlg()
{
}

void CZegoLiveRoomTopicsDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_LIST_TOPIC_BASIC, basic_topic_list_contronl_);
    DDX_Control(pDX, IDC_LIST_TOPIC_ADVANCED, advanced_topic_list_contronl_);
}

BEGIN_MESSAGE_MAP(CZegoLiveRoomTopicsDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
    ON_WM_LBUTTONDOWN()
    ON_LBN_SELCHANGE(IDC_LIST_TOPIC_BASIC, &CZegoLiveRoomTopicsDlg::OnLbnSelchangeListTopicBasic)
    ON_LBN_SELCHANGE(IDC_LIST_TOPIC_ADVANCED, &CZegoLiveRoomTopicsDlg::OnLbnSelchangeListTopicAdvanced)
    ON_BN_CLICKED(IDC_BUTTON_DOC, &CZegoLiveRoomTopicsDlg::OnBnClickedButtonDoc)
    ON_BN_CLICKED(IDC_BUTTON_CODE, &CZegoLiveRoomTopicsDlg::OnBnClickedButtonCode)
    ON_BN_CLICKED(IDC_BUTTON_FAQ, &CZegoLiveRoomTopicsDlg::OnBnClickedButtonFaq)
END_MESSAGE_MAP()


// CZegoLiveRoomTopicsDlg ��Ϣ�������

BOOL CZegoLiveRoomTopicsDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	//SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	//SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

    SetUnhandledExceptionFilter((LPTOP_LEVEL_EXCEPTION_FILTER)ApplicationCrashHandler);

    InitTopicList();


    CStringA stra_version;
    stra_version.Format("%s", ZEGO::LIVEROOM::GetSDKVersion());

    GetDlgItem(IDC_STATIC_SDK_VERSION)->SetWindowText(CString(stra_version.GetBuffer()));

    stra_version.Format("%s", ZEGO::LIVEROOM::GetSDKVersion2());
    GetDlgItem(IDC_STATIC_VE_VERSION)->SetWindowText(CString(stra_version.GetBuffer()));

    string build_number;
#ifdef BUILD_NUMBER

    build_number = string(BUILD_NUMBER);

    if (build_number != "")
    {
        int len = build_number.length();
        if (len < 5)
        {
            for (int i = 0; i < 5 - len; ++i)
            {
                build_number = "0" + build_number;
            }
        }

        build_number = build_number.insert(1, ".");
        build_number = build_number.insert(3, ".");
        build_number = build_number.insert(5, ".");

        GetDlgItem(IDC_STATIC_DEMO_VERSION)->SetWindowText(CString(CStringA(build_number.c_str()).GetBuffer()));

    }
    else {
        stra_version.Format("%s", DEMO_VERSION);
        GetDlgItem(IDC_STATIC_DEMO_VERSION)->SetWindowText(CString(stra_version.GetBuffer()));
    }

#else
    stra_version.Format("%s", DEMO_VERSION);
    GetDlgItem(IDC_STATIC_DEMO_VERSION)->SetWindowText(CString(stra_version.GetBuffer()));
#endif
    

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CZegoLiveRoomTopicsDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CZegoLiveRoomTopicsDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

// 		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);
// 
// 		// ʹͼ���ڹ����������о���
// 		int cxIcon = GetSystemMetrics(SM_CXICON);
// 		int cyIcon = GetSystemMetrics(SM_CYICON);
// 		CRect rect;
// 		GetClientRect(&rect);
// 		int x = (rect.Width() - cxIcon + 1) / 2;
// 		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		//dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
// HCURSOR CZegoLiveRoomTopicsDlg::OnQueryDragIcon()
// {
// 	return static_cast<HCURSOR>(m_hIcon);
// }

void CZegoLiveRoomTopicsDlg::InitTopicList()
{
    basic_topic_list_contronl_.AddString(TOPIC_PUBLISH);
    basic_topic_list_contronl_.AddString(TOPIC_PLAY);

    advanced_topic_list_contronl_.AddString(TOPIC_VIDEO_COMMUNICATION);
    advanced_topic_list_contronl_.AddString(TOPIC_JOIN_LIVE);
    advanced_topic_list_contronl_.AddString(TOPIC_MIXSTREAM);
    advanced_topic_list_contronl_.AddString(TOPIC_EXTERNAL_VIDEO_FILTER);

    advanced_topic_list_contronl_.AddString(TOPIC_MIDIA_PLAYER);
    advanced_topic_list_contronl_.AddString(TOPIC_MEDIA_SIDE_INFO);
    advanced_topic_list_contronl_.AddString(TOPIC_MEDIA_RECORDER);
    advanced_topic_list_contronl_.AddString(TOPIC_EXTERNAL_VIDEO_CAPTURE);
    advanced_topic_list_contronl_.AddString(TOPIC_EXTERNAL_VIDEO_RENDER);

	advanced_topic_list_contronl_.AddString(TOPIC_SCREEN_RECORDER);
	//advanced_topic_list_contronl_.AddString(TOPIC_ROOM_MESSAGE);
    
}

void CZegoLiveRoomTopicsDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
    CDialogEx::OnLButtonDown(nFlags, point);
}

void CZegoLiveRoomTopicsDlg::ShowDlg(void * dlg)
{
    for (auto p : dlg_list_)
    {
        if (p == dlg)
        {
            ((CDialogEx*)p)->ShowWindow(SW_SHOW);
        }
        else 
        {
            ((CDialogEx*)p)->ShowWindow(SW_HIDE);
        }
    }
}

void CZegoLiveRoomTopicsDlg::OnLbnSelchangeListTopic()
{

}



void CZegoLiveRoomTopicsDlg::OnLbnSelchangeListTopicBasic()
{
    int cur_sel = basic_topic_list_contronl_.GetCurSel();
    bool have_processed = false;
    if (cur_sel >= 0)
    {
        CString str;
        basic_topic_list_contronl_.GetText(cur_sel, str);
        if (str == TOPIC_PUBLISH)
        {
            if (publish_dlg_ptr_ == nullptr)
            {
                publish_dlg_ptr_ = CPublishDlg::CreateDlgInstance(this);
                dlg_list_.push_back(publish_dlg_ptr_);
            }
            CRect rect;
            GetDlgItem(IDC_PANEL)->GetWindowRect(rect);
            ScreenToClient(&rect);
            publish_dlg_ptr_->MoveWindow(rect.left, rect.top, rect.Width(), rect.Height());
            ShowDlg(publish_dlg_ptr_);
            publish_dlg_ptr_->InitDlg();
            have_processed = true;
        }else if (str == TOPIC_PLAY)
        {
            if (play_dlg_ptr_ == nullptr)
            {
                play_dlg_ptr_ = CPlayDlg::CreateDlgInstance(this);
                dlg_list_.push_back(play_dlg_ptr_);
            }
            CRect rect;
            GetDlgItem(IDC_PANEL)->GetWindowRect(rect);
            ScreenToClient(&rect);
            play_dlg_ptr_->MoveWindow(rect.left, rect.top, rect.Width(), rect.Height());
            ShowDlg(play_dlg_ptr_);
            
            play_dlg_ptr_->InitDlg();
            
            have_processed = true;
        }
    }

    if (have_processed)
    {
        advanced_topic_list_contronl_.SetCurSel(-1);

        GetDlgItem(IDC_STATIC_MAIN_INFO0)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_STATIC_MAIN_INFO1)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_STATIC_MAIN_INFO2)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_STATIC_MAIN_INFO3)->ShowWindow(SW_HIDE);

        GetDlgItem(IDC_STATIC_SDK_VERSION)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_STATIC_VE_VERSION)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_STATIC_DEMO_VERSION)->ShowWindow(SW_HIDE);
    }
}


void CZegoLiveRoomTopicsDlg::OnLbnSelchangeListTopicAdvanced()
{
    int cur_sel = advanced_topic_list_contronl_.GetCurSel();
    bool have_processed = false;
    if (cur_sel >= 0)
    {
        CString str;
        advanced_topic_list_contronl_.GetText(cur_sel, str);
        //MessageBox(str);
        if (str == TOPIC_MIDIA_PLAYER)
        {

#ifdef ZEGO_MEDIA_PLAYER
            if (media_play_dlg_ptr_ == nullptr)
            {
                media_play_dlg_ptr_ = MediaPlayerDialog::CreateDlgInstance(this);
                dlg_list_.push_back(media_play_dlg_ptr_);
            }

            CRect rect;
            GetDlgItem(IDC_PANEL)->GetWindowRect(rect);
            ScreenToClient(&rect);
            media_play_dlg_ptr_->MoveWindow(rect.left, rect.top, rect.Width(), rect.Height());
            ShowDlg(media_play_dlg_ptr_);
            media_play_dlg_ptr_->StartMediaPlayer();
#endif


            have_processed = true;
        }
        else if (str == TOPIC_MEDIA_SIDE_INFO)
        {
            if (media_side_info_dlg_ptr_ == nullptr)
            {
                media_side_info_dlg_ptr_ = MediaSideInfoPannelDlg::CreateDlgInstance(this);
                dlg_list_.push_back(media_side_info_dlg_ptr_);
            }
            CRect rect;
            GetDlgItem(IDC_PANEL)->GetWindowRect(rect);
            ScreenToClient(&rect);
            media_side_info_dlg_ptr_->MoveWindow(rect.left, rect.top, rect.Width(), rect.Height());
            ShowDlg(media_side_info_dlg_ptr_);
            have_processed = true;
        }
        else if (str == TOPIC_MEDIA_RECORDER)
        {
            if (media_recorder_dlg_ptr_ == nullptr)
            {
                media_recorder_dlg_ptr_ = CMediaRecorderDlg::CreateDlgInstance(this);
                dlg_list_.push_back(media_recorder_dlg_ptr_);
            }
            CRect rect;
            GetDlgItem(IDC_PANEL)->GetWindowRect(rect);
            ScreenToClient(&rect);
            media_recorder_dlg_ptr_->MoveWindow(rect.left, rect.top, rect.Width(), rect.Height());
            ShowDlg(media_recorder_dlg_ptr_);
            media_recorder_dlg_ptr_->LoginAndPreview();
            have_processed = true;

		}
		else if (str == TOPIC_EXTERNAL_VIDEO_CAPTURE)
		{
			if (external_video_capture_dlg_ptr_ == nullptr)
			{
				external_video_capture_dlg_ptr_ = CZGExternalVideoCaptureDlg::CreateDlgInstance(this);
				dlg_list_.push_back(external_video_capture_dlg_ptr_);
			}
			CRect rect;
			GetDlgItem(IDC_PANEL)->GetWindowRect(rect);
			ScreenToClient(&rect);
			external_video_capture_dlg_ptr_->MoveWindow(rect.left, rect.top, rect.Width(), rect.Height());
			ShowDlg(external_video_capture_dlg_ptr_);
			have_processed = true;
		}
		else if (str == TOPIC_EXTERNAL_VIDEO_RENDER)
		{
			if (external_video_render_dlg_ptr_ == nullptr)
			{
				external_video_render_dlg_ptr_ = CExternalVideoRenderDlg::CreateDlgInstance(this);
				dlg_list_.push_back(external_video_render_dlg_ptr_);
			}
			CRect rect;
			GetDlgItem(IDC_PANEL)->GetWindowRect(rect);
			ScreenToClient(&rect);
			external_video_render_dlg_ptr_->MoveWindow(rect.left, rect.top, rect.Width(), rect.Height());
			ShowDlg(external_video_render_dlg_ptr_);
			have_processed = true;
		}
		else if (str == TOPIC_EXTERNAL_VIDEO_FILTER)
		{
			if (external_video_filter_ == nullptr)
			{
				external_video_filter_ = CExternalVideoFilterDlg::CreateDlgInstance(this);
				dlg_list_.push_back(external_video_filter_);
			}
			CRect rect;
			GetDlgItem(IDC_PANEL)->GetWindowRect(rect);
			ScreenToClient(&rect);
			external_video_filter_->MoveWindow(rect.left, rect.top, rect.Width(), rect.Height());
			ShowDlg(external_video_filter_);
			have_processed = true;
		}
		else if (str == TOPIC_VIDEO_COMMUNICATION)
		{
			if (video_communication_ == nullptr)
			{
				video_communication_ = CVideoCommunicationDlg::CreateDlgInstance(this);
				dlg_list_.push_back(video_communication_);
			}
			CRect rect;
			GetDlgItem(IDC_PANEL)->GetWindowRect(rect);
			ScreenToClient(&rect);
			video_communication_->MoveWindow(rect.left, rect.top, rect.Width(), rect.Height());
			ShowDlg(video_communication_);
			have_processed = true;
		}
		else if (str == TOPIC_JOIN_LIVE)
		{
			if (join_live_dlg_ == nullptr)
			{
				join_live_dlg_ = CJoinLiveDlg::CreateDlgInstance(this);
				dlg_list_.push_back(join_live_dlg_);
			}
			CRect rect;
			GetDlgItem(IDC_PANEL)->GetWindowRect(rect);
			ScreenToClient(&rect);
			join_live_dlg_->MoveWindow(rect.left, rect.top, rect.Width(), rect.Height());
			ShowDlg(join_live_dlg_);

			join_live_dlg_->InitDlg();

			have_processed = true;
		}
		else if (str == TOPIC_MIXSTREAM)
		{
			if (mixstream_dlg_ == nullptr)
			{
				mixstream_dlg_ = CMixStreamDlg::CreateDlgInstance(this);
				dlg_list_.push_back(mixstream_dlg_);
			}
			CRect rect;
			GetDlgItem(IDC_PANEL)->GetWindowRect(rect);
			ScreenToClient(&rect);
			mixstream_dlg_->MoveWindow(rect.left, rect.top, rect.Width(), rect.Height());
			ShowDlg(mixstream_dlg_);

			mixstream_dlg_->InitDlg();

			have_processed = true;
		}
		else if (str == TOPIC_SCREEN_RECORDER)
		{
			if (screen_recorder_dlg_ == nullptr)
			{
				screen_recorder_dlg_ = CScreenRecorderDlg::CreateDlgInstance(this);
				dlg_list_.push_back(screen_recorder_dlg_);
			}
			CRect rect;
			GetDlgItem(IDC_PANEL)->GetWindowRect(rect);
			ScreenToClient(&rect);
			screen_recorder_dlg_->MoveWindow(rect.left, rect.top, rect.Width(), rect.Height());

			ShowDlg(screen_recorder_dlg_);

			screen_recorder_dlg_->InitDlg();
			screen_recorder_dlg_->LoginAndPreview();

			have_processed = true;
		}
		else if (str == TOPIC_ROOM_MESSAGE)
		{
			if (room_message_dlg == nullptr)
			{
				room_message_dlg = CRoomMessageDlg::CreateDlgInstance(this);
				dlg_list_.push_back(room_message_dlg);
			}
			CRect rect;
			GetDlgItem(IDC_PANEL)->GetWindowRect(rect);
			ScreenToClient(&rect);
			room_message_dlg->MoveWindow(rect.left, rect.top, rect.Width(), rect.Height());

			ShowDlg(room_message_dlg);

			room_message_dlg->InitDlg();

			have_processed = true;
		}
    }


    if (have_processed)
    {
        basic_topic_list_contronl_.SetCurSel(-1);

        GetDlgItem(IDC_STATIC_MAIN_INFO0)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_STATIC_MAIN_INFO1)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_STATIC_MAIN_INFO2)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_STATIC_MAIN_INFO3)->ShowWindow(SW_HIDE);

        GetDlgItem(IDC_STATIC_SDK_VERSION)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_STATIC_VE_VERSION)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_STATIC_DEMO_VERSION)->ShowWindow(SW_HIDE);
    }
}


void CZegoLiveRoomTopicsDlg::OnBnClickedButtonDoc()
{
    ShellExecute(NULL, L"open" , L"https://doc.zego.im/CN/197.html",  NULL, NULL, SW_SHOWNORMAL); 
}


void CZegoLiveRoomTopicsDlg::OnBnClickedButtonCode()
{
    ShellExecute(NULL, L"open", L"https://github.com/zegodev/liveroom-topics-windows", NULL, NULL, SW_SHOWNORMAL);
}


void CZegoLiveRoomTopicsDlg::OnBnClickedButtonFaq()
{
    ShellExecute(NULL, L"open", L"https://doc.zego.im/CN/496.html", NULL, NULL, SW_SHOWNORMAL);
}


void CAboutDlg::OnBnClickedOk()
{
    ShellExecute(NULL, L"open", L"https://console.zego.im/acount", NULL, NULL, SW_SHOWNORMAL);

}
