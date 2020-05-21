#ifndef ZEGOMediaPlayer_h
#define ZEGOMediaPlayer_h
#include "video_format.h"

namespace AVE
{
    class IMediaPlayer
    {
    public:
        enum PlayerType
        {
            TYPE_PLAYER = 0,    //only play
            TYPE_AUX            //play and mix into publish stream.
        };
        
        enum AudioChannel
        {
            LEFT_CHANNEL = 0x01,
            RIGHT_CHANNEL = 0x02,
            ALL_CHANNEL = LEFT_CHANNEL | RIGHT_CHANNEL
        };
        
        class EventCallback
        {
        public:
            virtual void OnPlayStart() = 0;
            virtual void OnPlayError(int code) = 0;
            virtual void OnVideoBegin() = 0;
            virtual void OnAudioBegin() = 0;
            virtual void OnPlayEnd() = 0;
            virtual void OnLoadComplete() = 0;
            virtual void OnSeekComplete(int code, long timestamp_ms) = 0;    //Units in millisecond
            virtual void OnPlayPause() = 0;
            virtual void OnPlayResume() = 0;
            virtual void OnPlayStop() = 0;
			virtual void OnBufferBegin() = 0;
			virtual void OnBufferEnd() = 0;
            virtual void OnSnapshot(void *image) = 0;
            virtual void OnProcessInterval(long timestamp_ms) = 0;
        };
        
        class VideoPlayCallback
        {
        public:
            virtual void OnPlayVideoData(const char* data, int len, const VideoDataFormat& format) = 0;
            virtual void OnPlayVideoData2(const char **pData, int* len, const VideoDataFormat& format) = 0;
        };
        
        class AudioPlayCallback
        {
        public:    //only pcm data  
            virtual void OnPlayAudioData(unsigned char *const data, int len, int sample_rate, int num_channels, int bit_depth) = 0;
        };
        
        virtual void SetEventCallback(EventCallback* callback) = 0;
		virtual void SetPlayerType(PlayerType type) = 0;
		virtual void SetVolume(int volume) = 0;  // local volume: 0 ~ 100, default volume is 60
		virtual void SetPublishVolume(int volume) = 0; // publish volume: 0 ~ 100, default volume is 60
        virtual int  GetVolume() = 0;        // local volume
        virtual int  GetPublishVolume() = 0; // publish volume
		virtual void MuteLocal(bool bMute) = 0;
        virtual void Load(const char* path) = 0;
        // repeat_play: play repeat or not;
        virtual void Start(const char* path, bool repeat_play = false) = 0;
        virtual void Stop() = 0;
        virtual void Pause() = 0;
        virtual void Resume() = 0;
        virtual void SeekTo(long timestamp_ms) = 0;                          //Units in millisecond
        virtual long GetDuration() = 0;
        virtual long GetCurrentDuration() = 0;
        virtual long SetProcessInterval(long interval_ms) = 0;
        
        virtual void SetView(void *view) = 0;
		virtual void SetViewMode(int nMode) = 0;
        virtual void ClearView() = 0;

        //format support:ARGB32/ABGR32/RGBA32/BGRA32/I420/NV12/NV21
        virtual void SetVideoPlayCallback(VideoPlayCallback *callback, VideoPixelFormat format) = 0;
        virtual void SetAudioPlayCallback(AudioPlayCallback *callback) = 0;
        virtual long SetAudioStream(long streamidx) = 0;
		virtual long GetAudioStreamCount() = 0;
        virtual void SetRepeatMode(bool repeat_play) = 0;
        virtual long TakeSnapshot() = 0;
        virtual long RequireHWDecoder() = 0;
        virtual void SetBackgroundColor(int color) = 0;   //0x00RRGGBB
        virtual void EnableAccurateSeek(bool bAccurate = false) = 0;
        virtual void SetAccurateSeekTimeout(long timeout_ms = 5000) = 0;
        virtual void SetActiveAudioChannel(AudioChannel channel) = 0;
        // key_shift_value can set any value between -8.0 ~ 8.0
        virtual int  SetAudioChannelKeyShift(AudioChannel channel, float key_shift_value) = 0;
        virtual void SetNetSourceCache(int max_cache_ms, int max_cache_size) = 0;
        virtual int  GetNetSourceCacheStat(int *cache_ms, int *cache_size) = 0;
        virtual void SetBufferThreshold(int buffer_end_threshold_ms) = 0;
        virtual void SetLoadResourceTimeout(int timeout_ms) = 0;
    };
    
    class IAudioEffectPlayer {
    public:
        class EffectEventCallback
        {
        public:
            virtual void OnPreloadComplete(unsigned int sound_id) = 0;
            virtual void OnPlayEnd(unsigned int sound_id) = 0;
        };
        
        virtual void SetEventCallback(EffectEventCallback* callback) = 0;
        virtual int PlayEffect(const char *path, unsigned int sound_id, int loop_count = 0, bool publish_out = false) = 0;
        virtual int StopEffect(unsigned int sound_id) = 0;
        virtual void PauseEffect(unsigned int sound_id) = 0;
        virtual void ResumeEffect(unsigned int sound_id) = 0;
        virtual void SetVolume(unsigned int sound_id, int volume) = 0;
        virtual int  SeekTo(unsigned int sound_id, long timestamp_ms) = 0;                          //Units in millisecond
        virtual long GetDuration(unsigned int sound_id) = 0;
        virtual long GetCurrentDuration(unsigned int sound_id) = 0;
        virtual void SetVolumeAll(int volume) = 0;
        virtual void PauseAll() = 0;
        virtual void ResumeAll() = 0;
        virtual void StopAll() = 0;
        virtual int  PreloadEffect(const char *path, unsigned int sound_id) = 0;
        virtual void UnloadEffect(unsigned int sound_id) = 0; 
    };
}

#endif /* ZEGOMediaPlayer_h */
