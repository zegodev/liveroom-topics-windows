
// ExternalVideoFilterDlg.h: 头文件
//

#pragma once

#include "Topics/ExternalVideoFilter/ZGExternalVideoFilterDemo.h"
#include "Topics/ExternalVideoFilter/ZGExternalVideoFilterDemoHelper.h"

#include "AppSupport/ZGUIOperator.h"

// CExternalVideoFilterDlg 对话框
class CExternalVideoFilterDlg : public CDialogEx
{
// 构造
public:

    static CExternalVideoFilterDlg * CreateDlgInstance(CWnd * pParent);
	

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_EXTERNALVIDEOFILTER_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

    CExternalVideoFilterDlg(CWnd* pParent = nullptr);	// 标准构造函数
// 实现
protected:

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()

    void OnStatusUpdate(std::string status);

    ZGExternalVideoFilterDemo demo_;
    ZGExternalVideoFilterDemoHelper helper_;

public:
    afx_msg void OnBnClickedButtonExternalFilterPublish();
    afx_msg LRESULT OnUICallbackMsg(WPARAM wParam, LPARAM lParam);
    afx_msg void OnBnClickedCheckVideoExternalFilterEnableBeauty();
    void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
};
