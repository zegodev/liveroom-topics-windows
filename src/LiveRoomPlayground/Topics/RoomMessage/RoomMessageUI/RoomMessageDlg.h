
// RoomMessageDlg.h: 头文件
//

#pragma once
#include "RoomMessage/ZegoRoomMessageDemo.h"
#include <string>

using std::string;

// CRoomMessageDlg 对话框
class CRoomMessageDlg : public CDialogEx
{
// 构造
public:
	static CRoomMessageDlg * CreateDlgInstance(CWnd * pParent);
	CRoomMessageDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ROOMMESSAGE_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()

public:
	afx_msg LRESULT OnUICallbackMsg(WPARAM wParam, LPARAM lParam);

	void InitDlg();
	afx_msg void OnBnClickedButtonLogin();
	afx_msg void OnBnClickedButtonSendCustomCommand();
	afx_msg void OnBnClickedButtonSendRoomMessage();

	ZegoRoomMessageDemo demo_;
	afx_msg void OnBnClickedButtonSendBigRoomMessage();
};
