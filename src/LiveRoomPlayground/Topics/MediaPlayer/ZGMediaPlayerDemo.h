//
// ZGMediaPlayerDemo.h
//
// Copyright 2018Äê Zego. All rights reserved.
//

#ifndef ZGMediaPlayerDemo_h__
#define ZGMediaPlayerDemo_h__

#include <functional>
#include <string>

#include "ZegoLiveRoom/zego-api-mediaplayer.h"
#include "ZegoLiveRoom/LiveRoomCallback.h"
#include "ZegoLiveRoom/LiveRoom.h"

#include "ZGVideoCaptureForMediaPlayer.h"
#include "ZGMediaPlayerPublishHelper.h"

using std::string;

using namespace ZEGO;

using AVE::IMediaPlayer;

using COMMON::ZegoStreamInfo;
using COMMON::ZegoStreamUpdateType;
using COMMON::ZegoPublishingStreamInfo;
using COMMON::ZegoUserInfo;
using COMMON::ZegoUserUpdateType;
using MEDIAPLAYER::ZegoMediaPlayerVideoDataFormat;
using MEDIAPLAYER::ZegoMediaPlayerIndex;

typedef enum
{
    ZGPlayerState_Stopped,
    ZGPlayerState_Stopping,
    ZGPlayerState_Playing
} ZGPlayerState;

typedef enum
{
    ZGPlayingSubState_Requesting,
    ZGPlayingSubState_PlayBegin,
    ZGPlayingSubState_Paused,
    ZGPlayingSubState_Buffering,
    ZGPlayingSubState_Error
} ZGPlayingSubState;

class ZGMediaPlayerDemoCallBack
{
public:
    virtual void OnPlayerState(string play_state) = 0;
    virtual void OnPlayerProgress(long current, long max, string desc) = 0;
    virtual void OnPlayerStop() = 0;
    virtual void OnPublishState(string pub_state) = 0;
    virtual void OnPlayerError(int ec) = 0;
};

class ZGMediaPlayerDemo :
    public MEDIAPLAYER::IZegoMediaPlayerEventWithIndexCallback
{
public:
    ZGMediaPlayerDemo();
    ~ZGMediaPlayerDemo();

    bool InitMediaPlayer();
    void InitPlayerStateCallBack(ZGMediaPlayerDemoCallBack * cb);
    void SetVideoView(void * view);
    void StartPlaying(const char * url, bool repeat);
    void Stop();
    void Pause();
    void Resume();
    void SetVolume(int volume);
    void SeekTo(long millisec);
    int GetAudioStreamCount();
    void SetAudioStream(long stream_index);

    void InitMainHwnd(HWND hwnd);

protected:

    virtual void OnPlayPause(ZegoMediaPlayerIndex index) override;
    virtual void OnPlayResume(ZegoMediaPlayerIndex index) override;
    virtual void OnPlayStop(ZegoMediaPlayerIndex index) override;
    virtual void OnBufferBegin(ZegoMediaPlayerIndex index) override;
    virtual void OnBufferEnd(ZegoMediaPlayerIndex index) override;

    void UpdatePlayProgressThread();
    void UpdateCurrentPlayState();
    void UpdateProgressDesc();
    void SetCurrentPlayState(ZGPlayerState s);
    void SetCurrentPlaySubState(ZGPlayingSubState s);

    virtual void OnPlayStart(ZegoMediaPlayerIndex index) override;
    virtual void OnPlayError(const int code, ZegoMediaPlayerIndex index) override;
    virtual void OnVideoBegin(ZegoMediaPlayerIndex index) override;
    virtual void OnAudioBegin(ZegoMediaPlayerIndex index) override;
    virtual void OnPlayEnd(ZegoMediaPlayerIndex index) override;
    virtual void OnSeekComplete(const int code, const long timestamp_ms, ZegoMediaPlayerIndex index) override;

    void OnPublishStateUpdate(string str_state);
private:

    ZGMediaPlayerPublishHelper   publish_helper_;
    ZGVideoCaptureForMediaPlayer video_capture_;
    ZGPlayerState                player_state_;
    ZGPlayingSubState            player_sub_state_;
    ZGMediaPlayerDemoCallBack*   player_state_cb_;
    long                         total_duration_;
    long                         current_duration_;
    //long                         last_update_duration_;
    int                          play_error_code_;
    bool                         update_progress_thread_started_;

    MEDIAPLAYER::ZegoMediaPlayerIndex  player_index_ = MEDIAPLAYER::ZegoMediaPlayerIndexFourth;
};



#endif // ZGMediaPlayerDemo_h__

