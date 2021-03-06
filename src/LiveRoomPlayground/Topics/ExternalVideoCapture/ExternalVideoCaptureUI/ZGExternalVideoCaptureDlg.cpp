
// ZGExternalVideoCaptureDlgDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "ZGExternalVideoCaptureDlg.h"
#include "afxdialogex.h"

#include "Topics/ExternalVideoCapture/ExternalVideoCaptureUI/resource.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


CZGExternalVideoCaptureDlg * CZGExternalVideoCaptureDlg::CreateDlgInstance(CWnd * pParent)
{
    CZGExternalVideoCaptureDlg * p = new CZGExternalVideoCaptureDlg(pParent);
    p->Create(IDD_ZGEXTERNALVIDEOCAPTUREDLG_DIALOG, pParent);
    return p;
}

// CZGExternalVideoCaptureDlgDlg 对话框

CZGExternalVideoCaptureDlg::CZGExternalVideoCaptureDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_ZGEXTERNALVIDEOCAPTUREDLG_DIALOG, pParent)
{
}

void CZGExternalVideoCaptureDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CZGExternalVideoCaptureDlg, CDialogEx)
    ON_MESSAGE(UI_CALLBACK_MSG, &CZGExternalVideoCaptureDlg::OnUICallbackMsg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
    ON_BN_CLICKED(IDC_PUBLISHING, &CZGExternalVideoCaptureDlg::OnBnClickedPublishing)
    ON_BN_CLICKED(IDC_IMAGE, &CZGExternalVideoCaptureDlg::OnBnClickedImage)
    ON_BN_CLICKED(IDC_DESTOP, &CZGExternalVideoCaptureDlg::OnBnClickedDestop)
    ON_BN_CLICKED(IDC_CAMERA, &CZGExternalVideoCaptureDlg::OnBnClickedCamera)
    ON_WM_ERASEBKGND()
    ON_WM_NCDESTROY()
END_MESSAGE_MAP()


// CZGExternalVideoCaptureDlgDlg 消息处理程序

BOOL CZGExternalVideoCaptureDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO: 在此添加额外的初始化代码

    ((CButton*)GetDlgItem(IDC_CAMERA))->SetCheck(TRUE);
    camera_name_list_ = demo_helper_.GetCameraDeviceNameList();
	if (camera_id_list_.size() <= 0)
	{
		return FALSE;
	}

    cur_capture_source_ = _T("Capture Source:  ") + CString(CStringA(camera_name_list_[select_camera_index_].c_str()));
    if (select_camera_index_ < camera_name_list_.size())
    {
        GetDlgItem(IDC_SELECT_INFO)->SetWindowText(cur_capture_source_);
    }

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}


// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CZGExternalVideoCaptureDlg::OnPaint()
{
    if (IsIconic())
    {
        CPaintDC dc(this); // device context for painting
    }
    else
    {
        CDialogEx::OnPaint();
    }
}

BOOL CZGExternalVideoCaptureDlg::OnEraseBkgnd(CDC  *pDC)
{
    return TRUE;
}


void CZGExternalVideoCaptureDlg::OnBnClickedPublishing()
{
    // 停止上次的外部采集
    external_capture_manager_.StopExternalCaptureThread();
    // 退出上次的登录和推拉流
    demo_helper_.Stop();

    demo_helper_.InitMainHwnd(this->GetSafeHwnd());

    // 设置登录、推拉流状态回调
    demo_helper_.status_cb_ = std::bind(&CZGExternalVideoCaptureDlg::OnPublishStateUpdate, this, std::placeholders::_1);

    // 开启外部采集，开启外部采集后，SetPreviewView、StartPreview无效，需要自己渲染
    external_capture_manager_.InitExternalCapture();
    
    BOOL is_image_check = ((CButton*)GetDlgItem(IDC_IMAGE))->GetCheck();
    BOOL is_screen_check = ((CButton*)GetDlgItem(IDC_DESTOP))->GetCheck();
    BOOL is_camera_check = ((CButton*)GetDlgItem(IDC_CAMERA))->GetCheck();
    
    if (is_image_check)
    {
        if (image_file_path_ == "")
        {
            MessageBox(_T("Invalid Image Path"));
            return;
        }
        demo_helper_.SetInitSdkCallBack([&](int nError)->void
        {
            // SDK 初始化成功
            if (nError == 0)
            {
                ZGExternalVideoSourceParams params;
                params.image_path = image_file_path_;

                // 设置图像路径参数
                external_capture_manager_.SetExternalSourceParams(params);
                // 创建视频外部采集图像源，从bmp文件读取rgb数据作为外部采集源
                external_capture_manager_.CreateExternalSource(ZGExternalVideoSource_Image);
                // 设置采集帧率fps
                external_capture_manager_.SetCaptureFps(5);
                demo_helper_.SetBitrate(600000);
            }
            else {
                // SDK 初始化失败
                MessageBox(_T("Init Sdk Failed."));
                return;
            }
        });
    }else if (is_camera_check)
    {
        demo_helper_.SetInitSdkCallBack([&](int nError)->void
        {

            PostUIData * pdata = CreateUIData();
            pdata->cb_in_ui_fun = [nError, pdata, this]()->void
            {
                // SDK 初始化成功
                if (nError == 0)
                {
                    // 获取摄像头列表
                    camera_id_list_ = demo_helper_.GetCameraDeviceIDList();
                    if (camera_id_list_.size() == 0)
                    {
                        MessageBox(_T("camera not found"));
                        return;
                    }
                    ZGExternalVideoSourceParams source_param;
                    source_param.camera_id = camera_id_list_[select_camera_index_];
                    // 设置摄像头id
                    external_capture_manager_.SetExternalSourceParams(source_param);
                    // 创建外部采集摄像头源，从摄像头采集到的数据作为外部采集源
                    external_capture_manager_.CreateExternalSource(ZGExternalVideoSource_Camera);
                    // 设置采集帧率fps
                    external_capture_manager_.SetCaptureFps(25);
                    demo_helper_.SetBitrate(800000);
                }
                else {
                    MessageBox(_T("Init Sdk Failed."));
                    return;
                }

                DestroyUIData(pdata);
            };

            PostMsgDataToUI(pdata);


        });
    }else if (is_screen_check)
    {
        demo_helper_.SetInitSdkCallBack([&](int nError)->void
        {
            PostUIData * pdata = CreateUIData();
            pdata->cb_in_ui_fun = [nError, pdata, this]()->void
            {
                if (nError == 0)
                {
                    // 创建外部采集屏幕源，截取屏幕数据作为外部采集源
                    external_capture_manager_.CreateExternalSource(ZGExternalVideoSource_Screen);
                    // 设置采集帧率
                    external_capture_manager_.SetCaptureFps(10);
                    demo_helper_.SetBitrate(3000000);
                }
                else {
                    MessageBox(_T("Init Sdk Failed."));
                    return;
                }

                DestroyUIData(pdata);
            };

            PostMsgDataToUI(pdata);


        });
    }
    
    // 设置外部采集数据回调，在OnCapturedVideoDataUpdate中拿到数据并渲染。
    external_capture_manager_.SetVideoDataCallBack(std::bind(&CZGExternalVideoCaptureDlg::OnCapturedVideoDataUpdate, this, std::placeholders::_1));
    // 启动外部采集线程
    external_capture_manager_.StartExternalCaptureThread();

    // 设置远端的视频视图hwnd
    demo_helper_.InitRemoteVideoView(GetDlgItem(IDC_REMOTE_VIDEO)->GetSafeHwnd());
    // 登录并推拉流
    demo_helper_.LoginRoomAndPublishing();
    // 设置渲染的视图，用于渲染外部采集到的数据
    captured_video_render_.InitVideoHwnd(GetDlgItem(IDC_LOCAL_VIDEO)->GetSafeHwnd());
}


void CZGExternalVideoCaptureDlg::OnBnClickedImage()
{
    CString filter = _T("文件 (*.bmp;)|*.bmp;|");
    CString defaultDir = _T("");
    CString default_file_name = _T("");

    BOOL isOpen = TRUE;
    CFileDialog openFileDlg(isOpen, defaultDir, default_file_name, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, filter, NULL);
    openFileDlg.GetOFN().lpstrInitialDir = _T("");
    INT_PTR result = openFileDlg.DoModal();

    if (result == IDOK) {
        CString str = openFileDlg.GetPathName();

        cur_capture_source_ = _T("Capture Source:  ") + str;
        GetDlgItem(IDC_SELECT_INFO)->SetWindowText(cur_capture_source_);
        //MessageBox(str);
        image_file_path_ = CStringA(str.GetBuffer()).GetBuffer();


        ZGExternalVideoSourceParams params;
        params.image_path = image_file_path_;

        // 设置图像路径参数
        external_capture_manager_.SetExternalSourceParams(params);
        // 创建视频外部采集图像源，从bmp文件读取rgb数据作为外部采集源
        external_capture_manager_.CreateExternalSource(ZGExternalVideoSource_Image);

    }
}

void CZGExternalVideoCaptureDlg::OnBnClickedDestop()
{
    cur_capture_source_ = _T("Capture Source:  Screen");
    GetDlgItem(IDC_SELECT_INFO)->SetWindowText(cur_capture_source_);

    // 创建外部采集屏幕源，截取屏幕数据作为外部采集源
    external_capture_manager_.CreateExternalSource(ZGExternalVideoSource_Screen);


}

void CZGExternalVideoCaptureDlg::OnBnClickedCamera()
{
    camera_name_list_ = demo_helper_.GetCameraDeviceNameList();

    cur_capture_source_ = _T("Capture Source:  ") + CString(CStringA(camera_name_list_[select_camera_index_].c_str()));
    if (select_camera_index_ < camera_name_list_.size())
    {
        GetDlgItem(IDC_SELECT_INFO)->SetWindowText(cur_capture_source_);
    }

    // 获取摄像头列表
    camera_id_list_ = demo_helper_.GetCameraDeviceIDList();
    if (camera_id_list_.size() == 0)
    {
        MessageBox(_T("camera not found"));
        return;
    }
    ZGExternalVideoSourceParams source_param;
    source_param.camera_id = camera_id_list_[select_camera_index_];
    // 设置摄像头id
    external_capture_manager_.SetExternalSourceParams(source_param);
    // 创建外部采集摄像头源，从摄像头采集到的数据作为外部采集源
    external_capture_manager_.CreateExternalSource(ZGExternalVideoSource_Camera);
}

void CZGExternalVideoCaptureDlg::OnCapturedVideoDataUpdate(std::shared_ptr<ZGExternalVideoData> video_data)
{
    if (video_data->fomat.pixel_format == PIXEL_FORMAT_BGRA32)
    {
        // 更新要绘制的BRGA数据
        captured_video_render_.UpdateVideoData(video_data->data.get(), video_data->len, video_data->fomat.width, video_data->fomat.height);

        PostUIData * pdata = CreateUIData();
        pdata->cb_in_ui_fun = [pdata, this]()->void
        {
            // 外部采集的数据需要自己处理绘制，在这里绘制
            captured_video_render_.OnPaintVideo();

            DestroyUIData(pdata);
        };
        PostMsgDataToUI(pdata);
    }
}


void CZGExternalVideoCaptureDlg::OnPublishStateUpdate(string state)
{
    typedef struct UIDataStruct
    {
        CString str;
    } UIDataStruct;

    UIDataStruct ui_data;

    CString str(CStringA(state.c_str()));
    ui_data.str = str;

    PostUIData * pdata = CreateUIData();
    pdata->cb_in_ui_fun = [ui_data, pdata, this]()->void
    {
        CString show_status_str = ui_data.str + " , " + cur_capture_source_;
        GetDlgItem(IDC_PUBLISH_STATUS)->SetWindowText(show_status_str);

        DestroyUIData(pdata);
    };

    PostMsgDataToUI(pdata);

}

void CZGExternalVideoCaptureDlg::OnNcDestroy()
{

    demo_helper_.Stop();
    external_capture_manager_.StopExternalCaptureThread();


    CDialogEx::OnNcDestroy();

}


afx_msg LRESULT CZGExternalVideoCaptureDlg::OnUICallbackMsg(WPARAM wParam, LPARAM lParam)
{
    PostUIData * pdata = (PostUIData *)wParam;
    if (pdata != nullptr && pdata->cb_in_ui_fun)
    {
        pdata->cb_in_ui_fun();
    }
    return 0;
}


