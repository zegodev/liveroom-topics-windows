#pragma once

#include "Play/PlayDemo.h"


class PlayInitSDKConfigDlgEvent
{
public:
    virtual void OnInitSDKButtonEvent() = 0;

};

class PlayInitSDKConfigDlg : public CDialogEx
{
	DECLARE_DYNAMIC(PlayInitSDKConfigDlg)

public:
	PlayInitSDKConfigDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~PlayInitSDKConfigDlg();

    void SetEventCallBack(PlayInitSDKConfigDlgEvent * cb);

    PlayInitSDKConfig GetConfig();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PLAY_INITSDK_CONFIG_DLG };
#endif


    virtual BOOL OnInitDialog() override;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

    BOOL SetTipText(UINT id, NMHDR *pTTTStruct, LRESULT *pResult);
    
    bool CheckParams();

    PlayInitSDKConfigDlgEvent * dlg_event_cb_ = nullptr;

    unsigned int appid_ = 0;
    CString app_sign_;
    CString user_id_;
    CString user_name_;

    PlayInitSDKConfig init_sdk_config_;


    virtual void OnOK() override;


    virtual void OnCancel() override;


public:
    afx_msg void OnBnClickedButtonPlayTopicInitsdk();
};
