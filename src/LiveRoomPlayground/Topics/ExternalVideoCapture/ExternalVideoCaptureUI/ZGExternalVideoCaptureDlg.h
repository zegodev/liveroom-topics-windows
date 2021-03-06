
// ZGExternalVideoCaptureDlgDlg.h: 头文件
//

#pragma once

#include "AppSupport/ZGUIOperator.h"

#include "Topics/ExternalVideoCapture/ZGExternalVideoCaptureManager.h"
#include "Topics/ExternalVideoCapture/ZGExternalVideoCatpureDemoHelper.h"
#include "Topics/ExternalVideoCapture/ZGVideoRender.h"


// CZGExternalVideoCaptureDlgDlg 对话框
class CZGExternalVideoCaptureDlg : public CDialogEx
{
// 构造
public:
    static CZGExternalVideoCaptureDlg * CreateDlgInstance(CWnd * pParent);
    CZGExternalVideoCaptureDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ZGEXTERNALVIDEOCAPTUREDLG_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:

    void PostNcDestroy()
    {
        CDialogEx::PostNcDestroy();
        delete this;
    }

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()

    BOOL OnEraseBkgnd(CDC *pDC);
    afx_msg void OnBnClickedPublishing();
    afx_msg void OnBnClickedImage();
    afx_msg void OnBnClickedDestop();
    afx_msg void OnBnClickedCamera();

private:

    void OnCapturedVideoDataUpdate(std::shared_ptr<ZGExternalVideoData> video_data);

    void OnPublishStateUpdate(string state);
    afx_msg LRESULT OnUICallbackMsg(WPARAM wParam, LPARAM lParam);
    ZGExternalVideoCaptureManager external_capture_manager_;
    ZGExternalVideoCatpureDemoHelper demo_helper_;

    // 开启外部采集时，需要自己渲染
    // 渲染模块
    ZGVideoRender captured_video_render_;
    
    // 图像路径
    string image_file_path_ = "";
    // 摄像头id列表
    std::vector<string> camera_id_list_;
    // 摄像头名字列表
    std::vector<string> camera_name_list_;
    // 选择摄像头的索引
    int select_camera_index_ = 0;
    // 当前选择的外部采集源信息
    CString cur_capture_source_;
public:
    afx_msg void OnNcDestroy();
};
