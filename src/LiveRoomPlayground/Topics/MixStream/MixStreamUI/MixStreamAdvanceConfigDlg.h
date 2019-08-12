#pragma once





// MixStreamAdvanceConfigDlg 对话框

class MixStreamAdvanceConfigDlg : public CDialogEx
{
	DECLARE_DYNAMIC(MixStreamAdvanceConfigDlg)

public:
    MixStreamAdvanceConfigDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~MixStreamAdvanceConfigDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_MIXSTREAM_ADVANCE_CONFIG };
#endif


    virtual BOOL OnInitDialog() override;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnBnClickedOk();
    afx_msg void OnBnClickedCancel();
    afx_msg void OnBnClickedCheckChannelType();
    afx_msg void OnBnClickedCheckSoundFlag();
    CButton channel_type_;
    CComboBox output_fps_;
    CComboBox output_video_bitrate_;
    CComboBox output_resolution_;
    CButton sound_flag_;
    afx_msg void OnCbnSelchangeComboMixstreamOutputResolution();
    afx_msg void OnCbnSelchangeComboMixstreamOutputFramerate();
    afx_msg void OnCbnSelchangeComboMixstreamOutputFps();
};
