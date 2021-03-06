
// ExternalVideoRenderDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "ExternalVideoRenderDlg.h"
#include "afxdialogex.h"

#include <stdint.h>

#include "ExternalVideoRender/ExternalVideoRenderUI/resource.h"
#include "AppSupport/ZGLog.h"
#include "AppSupport/ZGHelper.h"

using namespace std::placeholders;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CExternalVideoRenderDlg 对话框

CExternalVideoRenderDlg * CExternalVideoRenderDlg::CreateDlgInstance(CWnd * pParent)
{
    CExternalVideoRenderDlg * p = new CExternalVideoRenderDlg(pParent);
    p->Create(IDD_EXTERNALVIDEORENDER_DIALOG, pParent);
    return p;
}

CExternalVideoRenderDlg::CExternalVideoRenderDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_EXTERNALVIDEORENDER_DIALOG, pParent)
{
}

void CExternalVideoRenderDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CExternalVideoRenderDlg, CDialogEx)
    ON_MESSAGE(UI_CALLBACK_MSG, &CExternalVideoRenderDlg::OnUICallbackMsg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
    ON_BN_CLICKED(IDC_BUTTON_PUBLISH, &CExternalVideoRenderDlg::OnBnClickedButtonPublish)
    ON_BN_CLICKED(IDC_BUTTON_PLAY, &CExternalVideoRenderDlg::OnBnClickedButtonPlay)
	ON_BN_CLICKED(IDC_VIDEO_RENDER_TYPE_NONE, &CExternalVideoRenderDlg::OnBnClickedVideoRenderTypeNone)
	ON_BN_CLICKED(IDC_VIDEO_RENDER_TYPE_RGB, &CExternalVideoRenderDlg::OnBnClickedVideoRenderTypeRgb)
	ON_BN_CLICKED(IDC_VIDEO_RENDER_TYPE_YUV, &CExternalVideoRenderDlg::OnBnClickedExternalVideoRenderTypeYuv)
	ON_BN_CLICKED(IDC_VIDEO_RENDER_TYPE_EXTERNAL_INTERNAL_YUV, &CExternalVideoRenderDlg::OnBnClickedVideoRenderTypeExternalInternalYuv)
	ON_BN_CLICKED(IDC_VIDEO_RENDER_TYPE_EXTERNAL_INTERNAL_RGB, &CExternalVideoRenderDlg::OnBnClickedVideoRenderTypeExternalInternalRgb)
END_MESSAGE_MAP()


// CExternalVideoRenderDlg 消息处理程序

BOOL CExternalVideoRenderDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

    ((CButton*)GetDlgItem(IDC_VIDEO_RENDER_TYPE_RGB))->SetCheck(true);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CExternalVideoRenderDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	CDialogEx::OnSysCommand(nID, lParam);
	
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CExternalVideoRenderDlg::OnPaint()
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
	}
	else
	{
		CDialogEx::OnPaint();
	}
}


void CExternalVideoRenderDlg::OnLocalVideoUpdate(string stream_id, const unsigned char *pData, int dataLen, int width, int height)
{
    ZGENTER_FUN_LOG;
    if (cur_use_external_render_type_ == ZEGO::EXTERNAL_RENDER::VIDEO_RENDER_TYPE_EXTERNAL_INTERNAL_RGB)
    {
        // 该模式下，使用sdk内部渲染，并回调视频数据，业务上可以利用该数据按需处理
    }
    else {
        // 使用回调的数据进行渲染
        local_video_render_.UpdateVideoData((uint8_t *)pData, dataLen, width, height);
        local_video_render_.OnPaintVideo();
    }

}

void CExternalVideoRenderDlg::OnRemoteVideoUpdate(string stream_id, const unsigned char *pData, int dataLen, int width, int height)
{
    ZGENTER_FUN_LOG;
    if (cur_use_external_render_type_ >= ZEGO::EXTERNAL_RENDER::VIDEO_RENDER_TYPE_EXTERNAL_INTERNAL_RGB)
    {
        // 该模式下，使用sdk内部渲染，并回调视频数据，业务上可以利用该数据按需处理
    }
    else {
        // 使用回调的数据渲染
        remote_video_render_.UpdateVideoData((uint8_t *)pData, dataLen, width, height);
        remote_video_render_.OnPaintVideo();
    }

}

void CExternalVideoRenderDlg::OnStatusUpdate(std::string status)
{
    PostUIData * pdata = CreateUIData();
    pdata->cb_in_ui_fun = [status, pdata, this]()->void
    {
        GetDlgItem(IDC_EXTERNAL_RENDER_STATUS)->SetWindowText(CString(CStringA(status.c_str())));

        if (status == "LoginOk")
        {
            // 登录成功后开始预览
            helper_.StartPreview(GetDlgItem(IDC_EXTERNAL_VIDEO_LOCAL)->GetSafeHwnd());
            // 开始推流
            helper_.StartPublishing();
        }

        DestroyUIData(pdata);
    };

    PostMsgDataToUI(pdata);
}

void CExternalVideoRenderDlg::OnBnClickedButtonPublish()
{
    local_video_render_.InitVideoHwnd(GetDlgItem(IDC_EXTERNAL_VIDEO_LOCAL)->GetSafeHwnd());
    remote_video_render_.InitVideoHwnd(GetDlgItem(IDC_EXTERNAL_VIDEO_REMOTE)->GetSafeHwnd());

    cur_use_external_render_type_ = cur_set_external_render_type_;

    // 开启外部渲染
    demo_.EnableExternalRender(cur_use_external_render_type_);

    ZGLog("cur_external_render_type_ = %d", cur_use_external_render_type_);

    // 设置获取视频数据的回调函数
    demo_.SetVideoDataCallBack(std::bind(&CExternalVideoRenderDlg::OnLocalVideoUpdate, this, _1, _2, _3, _4, _5), std::bind(&CExternalVideoRenderDlg::OnRemoteVideoUpdate, this, _1, _2, _3, _4, _5));
    // 设置状态函数回调
    helper_.SetPublishStateObserver(std::bind(&CExternalVideoRenderDlg::OnStatusUpdate, this, _1));
    // 登录房间
    helper_.LoginRoom();
    
}

void CExternalVideoRenderDlg::OnBnClickedButtonPlay()
{
    helper_.StartPlaying(GetDlgItem(IDC_EXTERNAL_VIDEO_REMOTE)->GetSafeHwnd());
    // 设置内部渲染的同时，回调拉流的视频数据
    // 在拉流之后调用
    ZEGO::EXTERNAL_RENDER::EnableVideoRender(true, ZGHelperInstance()->GetDeviceUUID().c_str());
}


afx_msg LRESULT CExternalVideoRenderDlg::OnUICallbackMsg(WPARAM wParam, LPARAM lParam)
{
    PostUIData * pdata = (PostUIData *)wParam;
    if (pdata != nullptr && pdata->cb_in_ui_fun)
    {
        pdata->cb_in_ui_fun();
    }
    return 0;
}




void CExternalVideoRenderDlg::OnBnClickedVideoRenderTypeNone()
{
	// TODO: 在此添加控件通知处理程序代码
	cur_set_external_render_type_ = EXTERNAL_RENDER::VIDEO_RENDER_TYPE_NONE;
}


void CExternalVideoRenderDlg::OnBnClickedVideoRenderTypeRgb()
{
	// TODO: 在此添加控件通知处理程序代码
	cur_set_external_render_type_ = EXTERNAL_RENDER::VIDEO_RENDER_TYPE_RGB;

}


void CExternalVideoRenderDlg::OnBnClickedExternalVideoRenderTypeYuv()
{
	// TODO: 在此添加控件通知处理程序代码
	cur_set_external_render_type_ = EXTERNAL_RENDER::VIDEO_RENDER_TYPE_YUV;
}



void CExternalVideoRenderDlg::OnBnClickedVideoRenderTypeExternalInternalYuv()
{
	// TODO: 在此添加控件通知处理程序代码
	cur_set_external_render_type_ = EXTERNAL_RENDER::VIDEO_RENDER_TYPE_EXTERNAL_INTERNAL_YUV;
}


void CExternalVideoRenderDlg::OnBnClickedVideoRenderTypeExternalInternalRgb()
{
	// TODO: 在此添加控件通知处理程序代码
	cur_set_external_render_type_ = EXTERNAL_RENDER::VIDEO_RENDER_TYPE_EXTERNAL_INTERNAL_RGB;

}
