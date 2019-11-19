//
// ZGConfigHelper.h
//
// Copyright 2019Äê Zego. All rights reserved.
// 

#ifndef ZGConfigHelper_h__
#define ZGConfigHelper_h__

#include <string>
#include <vector>
#include "ZegoLiveRoom/zego-api-defines.h"
#include "ZegoLiveRoom/LiveRoomDefines.h"

#include "ZGLog.h"

typedef struct VideoDeviceInfo
{
    std::string device_id;
    std::string device_name;
    int index;

}VideoDeviceInfo;

typedef struct AudioDeviceInfo
{
    std::string device_id;
    std::string device_name;
    int index;

}AudioDeviceInfo;


class ZGConfigHelper
{
public:

    static ZGConfigHelper* Instance()
    {
        static ZGConfigHelper z;
        return &z;
    }

    std::vector<VideoDeviceInfo> GetVideoDevicesList();
    bool GetDefaultVideoDevice(VideoDeviceInfo & info);

    std::vector< AudioDeviceInfo> GetMicDevicesList();
    bool GetDefaultMicDevice(AudioDeviceInfo & info);

    std::vector< AudioDeviceInfo> GetSpeakerDevicesList();
    bool GetDefaultSpeakerDevice(AudioDeviceInfo & info);

    void EnableCamera(bool enable);
    void EnableMic(bool enable);
    void EnableSpeaker(bool enable);
    void SetPreviewViewMode(ZEGO::LIVEROOM::ZegoVideoViewMode view_mode);
    void SetPlayViewMode(ZEGO::LIVEROOM::ZegoVideoViewMode view_mode, std::string stream_id);
    void EnableCaptureMirror(bool enable);
	void ActivateVideoPublishStream(bool enable);
	void ActivateAudioPublishStream(bool enable);
	void ActivateVideoPlayStream(std::string streamID, bool enable);
	void ActivateAudioPlayStream(std::string streamID, bool enable);
    void SetPlayVolume(int play_volume);
    void SetPublishResolution(int w, int h);
    void SetVideoBitrate(int video_bitrate);
    void SetPublishVideoFps(int fps);
    void SetVideoDevice(std::string device_id);
    void SetMicDevice(std::string device_id);
    void SetSpeakerDevice(std::string device_id);

    void SetViewLogCallBack(ILogToView * log_cb);

private:

    void PrintLogToView(const char * format, ...);

    ILogToView * view_log_ = nullptr;


    ZGConfigHelper();
    ~ZGConfigHelper();
};


#define ZGConfigHelperInstance ZGConfigHelper::Instance

#endif // ZGConfigHelper_h__



