#pragma once


#include <string>
#include <functional>

#include "MixStream/MixStreamDemo.h"
#include "AppSupport/ZGUIOperator.h"
#include "AppSupport/ZGLog.h"

#include "MixStream/MixStreamUI/MixStreamAdvanceConfigDlg.h"


// MixStreamPublishDlg 对话框

class MixStreamPublishDlg : public CDialogEx, public MixStreamDemoCallBack, public ILogToView
{
	DECLARE_DYNAMIC(MixStreamPublishDlg)

public:

    std::function<void()> on_logout_cb_ = nullptr;
    
    MixStreamPublishDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~MixStreamPublishDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MIXSTREAM_PUBLISH };
#endif

    virtual void OnLoginEvent(ZGMixStreamLoginEvent event, ZGMixStreamJoinLiveStatus status) override;

    virtual void OnUserJoin(ZegoMixStreamUser user) override;

    virtual void OnUserLeave(ZegoMixStreamUser user) override;

    typedef struct ZegoJoinLivingRoomInfo {
        ZegoMixStreamUser user;
        HWND view_hwnd = nullptr;
        bool is_user_in_room = false;
    }ZegoJoinLivingRoomInfo;

    const int kMaxUserCount = 1;
    std::vector<ZegoJoinLivingRoomInfo> user_list_info_;

    void ProcessUserJoinRoom(ZegoMixStreamUser user);
    virtual void PrintToView(const char * log_str) override;

    virtual void OnSoundLevelUpdate(const std::vector<MixStreamSoundLevelInfo> & sound_levels) override;


    virtual void OnMixStreamUpdate(ZegoMixStreamUser anchor, string mix_stream_id) override;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:

    void StopPlayingMixStream();

    bool is_anchor_ = false;

    ZegoMixStreamUser room_anchor_;// 主播

    MixStreamDemo mix_stream_demo_;

    CListBox log_list_;

    string publish_stream_id_; // 推流流id
    string mixstream_id_;// 混流流id

    MixStreamAdvanceConfigDlg mix_stream_config_dlg_;

    void StartPublish(std::string room_id, bool is_anchor = true);
    
    afx_msg void OnBnClickedMixPublishLogout();
    afx_msg void OnBnClickedMixPublishJoinlive();
    afx_msg LRESULT OnUICallbackMsg(WPARAM wParam, LPARAM lParam);
    void SetHScroll();
    void AddLogToList(std::string str_log);
    afx_msg void OnBnClickedButtonMixStreamAdvanceConfig();

    void ProcessUserLeaveRoom(ZegoMixStreamUser user);
    void ResetControlBackgroundColorIfUserLeave();
    void ClearControlColor(HWND hwnd);
};
