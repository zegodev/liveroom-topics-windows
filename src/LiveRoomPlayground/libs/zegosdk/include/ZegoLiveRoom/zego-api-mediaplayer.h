//
//  zego-api-mediaplayer.h
//  ZegoLiveRoom
//
//  Copyright © 2018年 Zego. All rights reserved.
//

#ifndef zego_api_mediaplayer_h
#define zego_api_mediaplayer_h

#include <memory>

#include "zego-api-defines.h"
#include "media_player.h"
#include "zego-api-mediaplayer-defines.h"


namespace ZEGO
{
namespace MEDIAPLAYER
{
    
    /**
     获取播放器 

     @param type 播放器类型（AVE::IMediaPlayer::PlayerType）
     @return 播放器指针（AVE::IMediaPlayer*）
     @note 不要删除返回的 AVE::IMediaPlayer 指针，由引擎控制其生命周期
     @note 播放器只有在引擎启动的情况下才有效，需要在 InitSDK 后再调用，且不能和 InitSDK 在同一个函数里调用。
     @warning Deprecated，请使用 ZEGO::MEDIAPLAYER::InitWithType 初始化 ZegoMediaPlayer
     */
    ZEGOAVKIT_API AVE::IMediaPlayer* GetMediaPlayer(AVE::IMediaPlayer::PlayerType type);
    
    /**
     初始化播放器
     
     @param type @see ZegoMediaPlayerType
     @param index 播放器序号, 默认为 ZegoMediaPlayerIndexFirst
     */
    ZEGOAVKIT_API void InitWithType(const ZegoMediaPlayerType type, ZegoMediaPlayerIndex index = ZegoMediaPlayerIndexFirst);
    
    /**
     设置音量
     
     @param volume 音量，范围在0到100，默认值是50
     @param index 播放器序号, 默认为 ZegoMediaPlayerIndexFirst
     */
    ZEGOAVKIT_API void SetVolume(const int volume, ZegoMediaPlayerIndex index = ZegoMediaPlayerIndexFirst);
    
    /**
     开始播放
     
     @param path 媒体文件的路径
     @param repeat 是否重复播放
     @param index 播放器序号, 默认为 ZegoMediaPlayerIndexFirst
     */
    ZEGOAVKIT_API void Start(const char *path, const bool repeat, ZegoMediaPlayerIndex index = ZegoMediaPlayerIndexFirst);
    
    /**
     停止播放
     
     @param index 播放器序号, 默认为 ZegoMediaPlayerIndexFirst
     */
    ZEGOAVKIT_API void Stop(ZegoMediaPlayerIndex index = ZegoMediaPlayerIndexFirst);
    
    /**
     暂停播放
     
     @param index 播放器序号, 默认为 ZegoMediaPlayerIndexFirst
     */
    ZEGOAVKIT_API void Pause(ZegoMediaPlayerIndex index = ZegoMediaPlayerIndexFirst);
    
    /**
     恢复播放
     
     @param index 播放器序号, 默认为 ZegoMediaPlayerIndexFirst
     */
    ZEGOAVKIT_API void Resume(ZegoMediaPlayerIndex index = ZegoMediaPlayerIndexFirst);
    
    /**
     设置指定的进度进行播放
     
     @param duration 指定的进度，单位毫秒
     @param index 播放器序号, 默认为 ZegoMediaPlayerIndexFirst
     */
    ZEGOAVKIT_API void SeekTo(const long duration, ZegoMediaPlayerIndex index = ZegoMediaPlayerIndexFirst);
    
    /**
     获取整个文件的播放时长
     
     @param index 播放器序号, 默认为 ZegoMediaPlayerIndexFirst
     @return 文件的播放时长，单位毫秒
     
     */
    ZEGOAVKIT_API long GetDuration(ZegoMediaPlayerIndex index = ZegoMediaPlayerIndexFirst);
    
    /**
     获取当前播放的进度
     
     @param index 播放器序号, 默认为 ZegoMediaPlayerIndexFirst
     @return 当前播放进度，单位毫秒
     */
    ZEGOAVKIT_API long GetCurrentDuration(ZegoMediaPlayerIndex index = ZegoMediaPlayerIndexFirst);
    
    /**
     设置本地静默播放
     
     @param mute 是否静默播放
     @param index 播放器序号, 默认为 ZegoMediaPlayerIndexFirst
     @note 如果设置了 ZegoMediaPlayerTypeAux 模式, 推出的流是有声音的
     */
    ZEGOAVKIT_API void MuteLocal(bool mute, ZegoMediaPlayerIndex index = ZegoMediaPlayerIndexFirst);
    
    /**
     预加载资源
     
     @param path 媒体文件的路径
     @param index 播放器序号, 默认为 ZegoMediaPlayerIndexFirst
     @note 如果是视频, 会将首帧画面显示在显示控件上(通过 SetView 设置的). 之后需要播放资源时请调用 Resume 接口
     */
    ZEGOAVKIT_API void Load(const char* path, ZegoMediaPlayerIndex index = ZegoMediaPlayerIndexFirst);
    
    /**
     设置显示视频的view
     
     @param view 播放的控件
     @param index 播放器序号, 默认为 ZegoMediaPlayerIndexFirst
     */
#if TARGET_OS_IPHONE || TARGET_OS_OSX || defined(ANDROID)
    ZEGOAVKIT_API void SetView(std::shared_ptr<void> view, ZegoMediaPlayerIndex index = ZegoMediaPlayerIndexFirst);
#else
    ZEGOAVKIT_API void SetView(void *view, ZegoMediaPlayerIndex index = ZegoMediaPlayerIndexFirst);
#endif
    
    
    /**
     设置播放文件的音轨
     
     @param streamIndex 音轨序号，可以通过 getAudioStreamCount 接口获取音轨个数
     @param index 播放器序号, 默认为 ZegoMediaPlayerIndexFirst
     */
    ZEGOAVKIT_API long SetAudioStream(const long streamIndex, ZegoMediaPlayerIndex index = ZegoMediaPlayerIndexFirst);
    
    /**
     设置播放器类型
     
     @param type @see ZegoMediaPlayerType
     @param index 播放器序号, 默认为 ZegoMediaPlayerIndexFirst
     */
    ZEGOAVKIT_API void SetPlayerType(const ZegoMediaPlayerType type, ZegoMediaPlayerIndex index = ZegoMediaPlayerIndexFirst);
    
    /**
     获取音轨个数
     
     @return 音轨个数
     @param index 播放器序号, 默认为 ZegoMediaPlayerIndexFirst
     */
    ZEGOAVKIT_API long GetAudioStreamCount(ZegoMediaPlayerIndex index = ZegoMediaPlayerIndexFirst);
    
    /**
     设置是否循环播放
     
     @param enable true: 循环播放，false: 不循环播放
     @param index 播放器序号, 默认为 ZegoMediaPlayerIndexFirst
     */
    ZEGOAVKIT_API void EnableRepeatMode(const bool enable, ZegoMediaPlayerIndex index = ZegoMediaPlayerIndexFirst);
    
    /**
     获取当前播放视频的截图

     @param index 播放器序号, 默认为 ZegoMediaPlayerIndexFirst
     @note 只有在调用 setView 设置了显示控件，以及播放状态的情况下，才能正常截图。
     */
    ZEGOAVKIT_API void TakeSnapshot(ZegoMediaPlayerIndex index = ZegoMediaPlayerIndexFirst);
    
    /**
     设置播放器事件回调
     
     @param callback 回调 IZegoMediaPlayerEventCallback
     @param index 播放器序号, 默认为 ZegoMediaPlayerIndexFirst
     */
    ZEGOAVKIT_API void SetEventCallback(IZegoMediaPlayerEventCallback *callback, ZegoMediaPlayerIndex index = ZegoMediaPlayerIndexFirst);
    
    /**
     设置播放器事件回调
     
     @param callback 回调 IZegoMediaPlayerEventWithIndexCallback
     @param index 播放器序号, 默认为 ZegoMediaPlayerIndexFirst
     @note 使用多实例媒体播放器时，应该使用这个接口设置回调，便于在回调中区分是哪个播放器实例的回调
     */
    ZEGOAVKIT_API void SetEventWithIndexCallback(IZegoMediaPlayerEventWithIndexCallback *callback, ZegoMediaPlayerIndex index = ZegoMediaPlayerIndexFirst);
    
    /**
     设置视频帧数据回调
     
     @param callback 回调 IZegoMediaPlayerVideoDataCallback
     @param index 播放器序号, 默认为 ZegoMediaPlayerIndexFirst
     @param format 需要返回的视频帧数据格式，@see ZegoMediaPlayerVideoPixelFormat
     */
    ZEGOAVKIT_API void SetVideoDataCallback(IZegoMediaPlayerVideoDataCallback *callback, ZegoMediaPlayerVideoPixelFormat format, ZegoMediaPlayerIndex index = ZegoMediaPlayerIndexFirst);
    
    /**
     设置视频帧数据回调
     
     @param callback 回调
     @param format 需要返回的视频帧数据格式，@see ZegoMediaPlayerVideoPixelFormat
     @param index 播放器序号, 默认为 ZegoMediaPlayerIndexFirst
     @note 使用多实例媒体播放器时，应该使用这个接口设置回调，便于在回调中区分是哪个播放器实例的回调
     */
    ZEGOAVKIT_API void SetVideoDataWithIndexCallback(IZegoMediaPlayerVideoDataWithIndexCallback *callback, ZegoMediaPlayerVideoPixelFormat format, ZegoMediaPlayerIndex index = ZegoMediaPlayerIndexFirst);
    
}
}



#endif /* zego_api_mediaplayer_h */
