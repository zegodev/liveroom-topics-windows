
// ScreenRecorderDlg.h: 头文件
//

#pragma once

#include "Topics/ScreenRecorder/ZGScreenRecorderDemo.h"
#include "Topics/ScreenRecorder/ZGScreenRecorderDemoHelper.h"
#include "AppSupport/ZGUIOperator.h"

// CScreenRecorderDlg 对话框
class CScreenRecorderDlg : public CDialogEx, public ZGScreenRecorderDemoCallBack
{
// 构造
public:
	static CScreenRecorderDlg * CreateDlgInstance(CWnd * pParent);
	CScreenRecorderDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SCREENRECORDER_DIALOG };
#endif

protected:
	virtual void OnScreenRecorderFileStatus(const unsigned int duration, const unsigned int file_size) override;
	virtual void OnScreenRecordStateUpdate(string record_state) override;
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();

	DECLARE_MESSAGE_MAP()


private:
	ZGScreenRecorderDemoHelper demo_helper_;
	ZGScreenRecorderDemo demo_;
	ZGScreenRecorderDemoConfig config_;


	CString save_file_path_;
	afx_msg LRESULT OnUICallbackMsg(WPARAM wParam, LPARAM lParam);

public:
	void InitDlg();
	void LoginAndPreview();

	afx_msg void OnBnClickedCheckPublish();
	afx_msg void OnBnClickedButtonStartRecord();
	afx_msg void OnBnClickedButtonStopRecord();
	afx_msg void OnBnClickedButtonPlay();
	afx_msg void OnBnClickedButtonRecordFilePath();
};
