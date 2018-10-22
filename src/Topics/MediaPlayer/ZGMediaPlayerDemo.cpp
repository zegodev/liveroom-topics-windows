#include "stdafx.h"
#include "ZGMediaPlayerDemo.h"

#include <thread>
#include <chrono>
#include <vector>
#include <string>

#include "AppSupport/ZGManager.h"
#include "AppSupport/ZGUtilTools.h"
#include "AppSupport/ZGLog.h"
#include "AppSupport/ZGHelper.h"
#include "AppSupport/AppIDHelper.h"

using std::vector;
using std::string;

ZGMediaPlayerDemo::ZGMediaPlayerDemo()
    :player_state_(ZGPlayerState_Stopped),
     player_state_cb_(nullptr),
     current_duration_(0),
     total_duration_(0),
     //last_update_duration_(0),
     play_error_code_(0),
     update_progress_thread_started_(false)
{

}

ZGMediaPlayerDemo::~ZGMediaPlayerDemo()
{
}

bool ZGMediaPlayerDemo::InitMediaPlayer()
{
    //LIVEROOM::SetVerbose(true);

    ZGManagerInstance()->EnableExternalVideoCapture(&video_capture_, nullptr);

    unsigned int appid = GetAppId();
    int sign_len = 0;
    unsigned char * sign_data = GetAppSign(sign_len);

    bool ret = ZGManagerInstance()->InitSdk(appid, sign_data, sign_len);
    if (!ret)
    {
        return false;
    }

    MEDIAPLAYER::InitWithType(MEDIAPLAYER::ZegoMediaPlayerType::ZegoMediaPlayerTypeAux);
    MEDIAPLAYER::SetEventCallback(this);
    MEDIAPLAYER::SetVideoDataCallback(&video_capture_, MEDIAPLAYER::ZegoMediaPlayerVideoPixelFormatRGBA32);
    ret = LIVEROOM::StartPreview();
    if (!ret)
    {
        ZGLog("preview failed");
        return false;
    }

    publish_helper_.StartPublishing();
    publish_helper_.SetPublishStateObserver(std::bind(&ZGMediaPlayerDemo::OnPublishStateUpdate, this, std::placeholders::_1));

    return true;
}

void ZGMediaPlayerDemo::InitPlayerStateCallBack(ZGMediaPlayerDemoCallBack * cb)
{
    player_state_cb_ = cb;
}

void ZGMediaPlayerDemo::SetVideoView(void * view)
{
    MEDIAPLAYER::SetView(view);
}

void ZGMediaPlayerDemo::SetVolume(int volume)
{
    MEDIAPLAYER::SetVolume(volume);
}

void ZGMediaPlayerDemo::StartPlaying(const char * url, bool repeat)
{

    if (player_state_ == ZGPlayerState_Playing)
    {
        MEDIAPLAYER::Stop();
    }

    MEDIAPLAYER::SetPlayerType(MEDIAPLAYER::ZegoMediaPlayerTypeAux);
    MEDIAPLAYER::Start(url, repeat);

    SetCurrentPlayState(ZGPlayerState_Playing);
    SetCurrentPlaySubState(ZGPlayingSubState_Requesting);

}

void ZGMediaPlayerDemo::Stop()
{
    SetCurrentPlayState(ZGPlayerState_Stopping);
    MEDIAPLAYER::Stop();

    if (player_state_cb_)
    {
        player_state_cb_->OnPlayerStop();
    }
}

void ZGMediaPlayerDemo::Pause()
{
    if (player_state_ == ZGPlayerState_Playing && player_sub_state_ != ZGPlayingSubState_Paused)
    {
        MEDIAPLAYER::Pause();
    }
}

void ZGMediaPlayerDemo::Resume()
{
    if (player_state_ == ZGPlayerState_Playing && player_sub_state_ == ZGPlayingSubState_Paused)
    {
        MEDIAPLAYER::Resume();
    }
}

void ZGMediaPlayerDemo::SeekTo(long millisec)
{

    MEDIAPLAYER::SeekTo(millisec);

    //last_update_duration_ = millisec;

    if (player_state_cb_)
    {
        string str = StringFormat("%d/%d", /*last_update_duration_*/millisec / 1000, total_duration_ / 1000);
        player_state_cb_->OnPlayerProgress(/*last_update_duration_*/millisec, total_duration_, str);
    }
}

int ZGMediaPlayerDemo::GetAudioStreamCount()
{
    return MEDIAPLAYER::GetAudioStreamCount();
}

void ZGMediaPlayerDemo::OnPlayPause()
{
    SetCurrentPlaySubState(ZGPlayingSubState_Paused);
}

void ZGMediaPlayerDemo::OnPlayResume()
{
    SetCurrentPlaySubState(ZGPlayingSubState_PlayBegin);
}

void ZGMediaPlayerDemo::OnPlayStop()
{
    SetCurrentPlayState(ZGPlayerState_Stopped);
}

void ZGMediaPlayerDemo::OnBufferBegin()
{
}

void ZGMediaPlayerDemo::OnBufferEnd()
{
}

void ZGMediaPlayerDemo::UpdatePlayProgressThread()
{
    update_progress_thread_started_ = true;
    while (true)
    {
        if (player_state_ == ZGPlayerState_Stopped)
        {
            break;
        }
        UpdateProgressDesc();
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }

    update_progress_thread_started_ = false;
}

void ZGMediaPlayerDemo::UpdateCurrentPlayState()
{
    string str_state;
    string str_sub_state;
    switch (player_state_)
    {
    case ZGPlayerState_Stopped:
        str_state = "Stopped";
        break;
    case ZGPlayerState_Stopping:
        str_state = "Stopping";
        break;
    case ZGPlayerState_Playing:
    {
        switch (player_sub_state_)
        {
        case ZGPlayingSubState_Requesting:
            str_sub_state = "Requesting";
            break;
        case ZGPlayingSubState_PlayBegin:
            str_sub_state = "Begin";
            break;
        case ZGPlayingSubState_Paused:
            str_sub_state = "Paused";
            break;
        case ZGPlayingSubState_Buffering:
            str_sub_state = "Buffering";
            break;
        case ZGPlayingSubState_Error:
            str_sub_state = "Error, code = " + StringFormat("%d", play_error_code_);

            break;
        default:
            break;
        }
        str_state = "Playing: " + str_sub_state;
        break;
    }
    default:
        break;
    }
    if (player_state_cb_)
    {
        player_state_cb_->OnPlayerState(str_state.c_str());
    }
}

void ZGMediaPlayerDemo::UpdateProgressDesc()
{
    long cur_duration = MEDIAPLAYER::GetCurrentDuration();
    //if ((cur_duration - last_update_duration_)*1.0 / total_duration_ >= 0.001 || cur_duration == total_duration_)
    {
        if (player_state_cb_)
        {
            string str = StringFormat("%d/%d", cur_duration / 1000, total_duration_ / 1000);
            player_state_cb_->OnPlayerProgress(cur_duration, total_duration_, str);
            //last_update_duration_ = cur_duration;
            //ZGLog("set last duration %ld", last_update_duration_);
            //if (cur_duration == total_duration_)
            //{
            //    last_update_duration_ = 0;
            //}
        }
    }
}

void ZGMediaPlayerDemo::SetCurrentPlayState(ZGPlayerState s)
{
    player_state_ = s;
    UpdateCurrentPlayState();
}

void ZGMediaPlayerDemo::SetCurrentPlaySubState(ZGPlayingSubState s)
{
    player_sub_state_ = s;
    UpdateCurrentPlayState();
}

void ZGMediaPlayerDemo::OnPlayStart()
{
    ZGENTER_FUN_LOG;

    SetCurrentPlaySubState(ZGPlayingSubState_PlayBegin);

    // get duration and update
    total_duration_ = MEDIAPLAYER::GetDuration();
    current_duration_ = 0;
    //last_update_duration_ = 0;
    if (player_state_cb_)
    {
        string str = StringFormat("%d/%d", 0, total_duration_ / 1000);
        player_state_cb_->OnPlayerProgress(0, total_duration_, str);
    }

    if (!update_progress_thread_started_)
    {
        std::thread t(&ZGMediaPlayerDemo::UpdatePlayProgressThread, this);
        t.detach();
    }
}

void ZGMediaPlayerDemo::OnPlayError(int code)
{
    if (player_state_cb_)
    {
        player_state_cb_->OnPlayerError(code);
    }
    play_error_code_ = code;
    SetCurrentPlaySubState(ZGPlayingSubState_Error);
}

void ZGMediaPlayerDemo::OnVideoBegin()
{
    ZGENTER_FUN_LOG;
}

void ZGMediaPlayerDemo::OnAudioBegin()
{
    ZGENTER_FUN_LOG;
}

void ZGMediaPlayerDemo::OnPlayEnd()
{
    ZGENTER_FUN_LOG;
    SetCurrentPlayState(ZGPlayerState_Stopped);
    if (player_state_cb_)
    {
        string str = StringFormat("%d/%d", total_duration_ / 1000, total_duration_ / 1000);
        player_state_cb_->OnPlayerProgress(total_duration_, total_duration_, str);
    }
    //last_update_duration_ = 0;
}

void ZGMediaPlayerDemo::OnSeekComplete(int code, long timestamp_ms)
{
    ZGENTER_FUN_LOG;
    ZGLog("code = %d , timestamp_ms = %d, curduration : %ld , total duration : %ld", code, timestamp_ms, MEDIAPLAYER::GetCurrentDuration(), MEDIAPLAYER::GetDuration());
    //if (timestamp_ms == 0)
    //{
    //    last_update_duration_ = 0;
    //}
}


void ZGMediaPlayerDemo::OnPublishStateUpdate(string str_state)
{
    if (player_state_cb_)
    {
        player_state_cb_->OnPublishState(str_state);
    }
}

void ZGMediaPlayerDemo::SetAudioStream(long stream_index)
{
    MEDIAPLAYER::SetAudioStream(stream_index);
}
