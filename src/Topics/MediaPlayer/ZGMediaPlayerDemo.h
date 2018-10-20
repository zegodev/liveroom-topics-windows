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
    public MEDIAPLAYER::IZegoMediaPlayerEventCallback
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

protected:

    virtual void OnPlayPause() override;
    virtual void OnPlayResume() override;
    virtual void OnPlayStop() override;
    virtual void OnBufferBegin() override;
    virtual void OnBufferEnd() override;

    void UpdatePlayProgressThread();
    void UpdateCurrentPlayState();
    void UpdateProgressDesc();
    void SetCurrentPlayState(ZGPlayerState s);
    void SetCurrentPlaySubState(ZGPlayingSubState s);

    virtual void OnPlayStart() override;
    virtual void OnPlayError(int code) override;
    virtual void OnVideoBegin() override;
    virtual void OnAudioBegin() override;
    virtual void OnPlayEnd() override;
    virtual void OnSeekComplete(int code, long timestamp_ms) override;

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
};



#endif // ZGMediaPlayerDemo_h__

