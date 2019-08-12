#pragma once


class BasicMixStreamConfigDlgEvent
{
public:
    virtual void OnBasicConfigOkClickedEvent() = 0;
    virtual void OnAdvanceConfigClickedEvent() = 0;
};

// MixStreamBasicConfigDlg 对话框

class MixStreamBasicConfigDlg : public CDialogEx
{
	DECLARE_DYNAMIC(MixStreamBasicConfigDlg)

public:
    MixStreamBasicConfigDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~MixStreamBasicConfigDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MIXSTREAM_BASIC_CONFIG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnBnClickedCancel();
    afx_msg void OnBnClickedOk();
    afx_msg void OnBnClickedMixAdvanceConfig();
    CString publish_stream_id_;
    CString output_mixstream_id_;
    CString room_id_;
    
    BasicMixStreamConfigDlgEvent * event_cb_ = nullptr;
};
