
// ZegoLiveRoomTopicsDlg.h : 头文件
//

#pragma once


#include <vector>

#include "MediaPlayer/MediaPlayerUI/MediaPlayerDialog.h"
#include "MediaSideInfo/MediaSideInfoUI/MediaSideInfoDlg.h"
#include "MediaRecorder/MediaRecorderUI/MediaRecorderDlg.h"
#include "MediaSideInfo/MediaSideInfoUI/MediaSideInfoPannelDlg.h"
#include "ExternalVideoCapture/ExternalVideoCaptureUI/ZGExternalVideoCaptureDlg.h"

using std::vector;

// CZegoLiveRoomTopicsDlg 对话框
class CZegoLiveRoomTopicsDlg : public CDialogEx
{
// 构造
public:
	CZegoLiveRoomTopicsDlg(CWnd* pParent = NULL);	// 标准构造函数
    ~CZegoLiveRoomTopicsDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ZEGOLIVEROOMTOPICS_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

    void ShowDlg(void * dlg);

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

    MediaPlayerDialog * media_play_dlg_ptr_ = nullptr;    
    CMediaRecorderDlg * media_recorder_dlg_ptr_ = nullptr;
    MediaSideInfoPannelDlg * media_side_info_dlg_ptr_ = nullptr;
    CZGExternalVideoCaptureDlg * external_video_capture_dlg_ptr_ = nullptr;

    std::vector<void*> dlg_list_;

private:
    void InitTopicList();
    CListBox topic_list_contronl_;
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnLbnSelchangeListTopic();
};
