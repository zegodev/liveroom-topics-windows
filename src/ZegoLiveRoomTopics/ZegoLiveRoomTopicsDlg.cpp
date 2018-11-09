
// ZegoLiveRoomTopicsDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ZegoLiveRoomTopics.h"
#include "ZegoLiveRoomTopicsDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


#define TOPIC_MIDIA_PLAYER      _T("MediaPlayer")
#define TOPIC_MEDIA_SIDE_INFO   _T("Media Side Info")


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CZegoLiveRoomTopicsDlg 对话框



CZegoLiveRoomTopicsDlg::CZegoLiveRoomTopicsDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_ZEGOLIVEROOMTOPICS_DIALOG, pParent),
    media_play_dlg_ptr_(nullptr),
    media_side_info_dlg_ptr_(nullptr)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CZegoLiveRoomTopicsDlg::~CZegoLiveRoomTopicsDlg()
{
}

void CZegoLiveRoomTopicsDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_LIST_TOPIC, topic_list_contronl_);
}

BEGIN_MESSAGE_MAP(CZegoLiveRoomTopicsDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
    ON_WM_LBUTTONDOWN()
    ON_LBN_SELCHANGE(IDC_LIST_TOPIC, &CZegoLiveRoomTopicsDlg::OnLbnSelchangeListTopic)
END_MESSAGE_MAP()


// CZegoLiveRoomTopicsDlg 消息处理程序

BOOL CZegoLiveRoomTopicsDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
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

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

    InitTopicList();

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CZegoLiveRoomTopicsDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CZegoLiveRoomTopicsDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CZegoLiveRoomTopicsDlg::InitTopicList()
{
    topic_list_contronl_.AddString(TOPIC_MIDIA_PLAYER);
    topic_list_contronl_.AddString(TOPIC_MEDIA_SIDE_INFO);
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
    int cur_sel = topic_list_contronl_.GetCurSel();
    if (cur_sel >= 0)
    {
        CString str;
        topic_list_contronl_.GetText(cur_sel, str);
        //MessageBox(str);
        if (str == TOPIC_MIDIA_PLAYER)
        {
            if (media_play_dlg_ptr_ == nullptr)
            {
                media_play_dlg_ptr_ = new MediaPlayerDialog();
                media_play_dlg_ptr_->Create(IDD_MEDIAPLAYER_DIALOG, this);
                dlg_list_.push_back(media_play_dlg_ptr_);
            }

            CRect rect;
            GetDlgItem(IDC_PANEL)->GetWindowRect(rect);
            ScreenToClient(&rect);
            media_play_dlg_ptr_->MoveWindow(rect.left, rect.top, rect.Width(), rect.Height());
            ShowDlg(media_play_dlg_ptr_);
            media_play_dlg_ptr_->StartMediaPlayer();
        }
        else if (str == TOPIC_MEDIA_SIDE_INFO)
        {
            if (media_side_info_dlg_ptr_ == nullptr)
            {
                media_side_info_dlg_ptr_ = new CMediaSideInfoDlg();
                media_side_info_dlg_ptr_->Create(IDD_MEDIASIDEINFO_DIALOG, this);
                dlg_list_.push_back(media_side_info_dlg_ptr_);
            }

            CRect rect;
            GetDlgItem(IDC_PANEL)->GetWindowRect(rect);
            ScreenToClient(&rect);
            media_side_info_dlg_ptr_->MoveWindow(rect.left, rect.top, rect.Width(), rect.Height());
            ShowDlg(media_side_info_dlg_ptr_);
            media_side_info_dlg_ptr_->LoginRoom();
        }
    }
}

