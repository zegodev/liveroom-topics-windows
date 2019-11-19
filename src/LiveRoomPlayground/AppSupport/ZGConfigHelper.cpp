#include "stdafx.h"
#include "ZGConfigHelper.h"

#include "ZGManager.h"

#include "ZGUtilTools.h"


void ZGConfigHelper::EnableCamera(bool enable)
{
    if (ZGManagerInstance()->SdkIsInited())
    {
        PrintLogToView("%s", enable ? "使用Camera" : "不使用Camera");
        LIVEROOM::EnableCamera(enable);
    }
}

void ZGConfigHelper::EnableMic(bool enable)
{
    if (ZGManagerInstance()->SdkIsInited())
    {
        PrintLogToView("%s", enable ? "使用Mic" : "不使用Mic");
        LIVEROOM::EnableMic(enable);
    }
}

void ZGConfigHelper::EnableSpeaker(bool enable)
{
    if (ZGManagerInstance()->SdkIsInited())
    {
        PrintLogToView("设置静音开关，设置为：%s" , enable ? "不静音":"静音");
        LIVEROOM::EnableSpeaker(enable);
    }
}

void ZGConfigHelper::SetPreviewViewMode(ZEGO::LIVEROOM::ZegoVideoViewMode view_mode)
{
    if (ZGManagerInstance()->SdkIsInited())
    {
        PrintLogToView("设置预览视图模式为 = %d", view_mode);
        LIVEROOM::SetPreviewViewMode(view_mode);
    }
}


void ZGConfigHelper::EnableCaptureMirror(bool enable)
{
    if (ZGManagerInstance()->SdkIsInited())
    {
        PrintLogToView("设置镜像模式为：%s", enable ? "使用镜像模式" : "不使用镜像模式");
        LIVEROOM::EnablePreviewMirror(enable);
    }
}

void ZGConfigHelper::ActivateVideoPublishStream(bool enable)
{
	if (ZGManagerInstance()->SdkIsInited())
	{
		int result = LIVEROOM::MuteVideoPublish(enable);
		PrintLogToView("ActivateVideoPublishStream:%d(bool), Result:%d", enable, result);
	}
}

void ZGConfigHelper::ActivateAudioPublishStream(bool enable)
{
	if (ZGManagerInstance()->SdkIsInited())
	{
        int result = LIVEROOM::MuteAudioPublish(enable);
        PrintLogToView("ActivateAudioPublishStream:%d(bool), Result:%d", enable, result);
	}
}

void ZGConfigHelper::ActivateVideoPlayStream(std::string streamID, bool enable)
{
	if (ZGManagerInstance()->SdkIsInited())
	{
		int result = LIVEROOM::ActivateVideoPlayStream(streamID.c_str(), enable);
		PrintLogToView("ActivateVideoPlayStream:%d(bool), Result:%d", enable, result);
	}
}

void ZGConfigHelper::ActivateAudioPlayStream(std::string streamID, bool enable)
{
	if (ZGManagerInstance()->SdkIsInited())
	{
		int result = LIVEROOM::ActivateAudioPlayStream(streamID.c_str(), enable);
		PrintLogToView("ActivateAudioPlayStream:%d(bool), Result:%d", enable, result);
	}
}

void ZGConfigHelper::SetPlayVolume(int play_volume)
{
    if (ZGManagerInstance()->SdkIsInited())
    {
        PrintLogToView("设置播放音量为:%d", play_volume);
        LIVEROOM::SetPlayVolume(play_volume);
    }
}

void ZGConfigHelper::SetPlayViewMode(ZEGO::LIVEROOM::ZegoVideoViewMode view_mode, std::string stream_id)
{
    if (ZGManagerInstance()->SdkIsInited())
    {
        PrintLogToView("设置拉流播放视图模式为 = %d", view_mode);
        LIVEROOM::SetViewMode(view_mode, stream_id.c_str());
    }
}

void ZGConfigHelper::SetPublishResolution(int w, int h)
{
    if (ZGManagerInstance()->SdkIsInited())
    {
        PrintLogToView("设置采集分辨率：宽 = %d， 高 = %d", w, h);        
        LIVEROOM::SetVideoCaptureResolution(w, h);
        PrintLogToView("设置编码分辨率：宽 = %d， 高 = %d", w, h);
        LIVEROOM::SetVideoEncodeResolution(w, h);
    }
}

void ZGConfigHelper::SetVideoBitrate(int video_bitrate)
{
    if (ZGManagerInstance()->SdkIsInited())
    {
        PrintLogToView("设置视频码率：bitrate = %d (bps/s)", video_bitrate);
        LIVEROOM::SetVideoBitrate(video_bitrate);
    }
}

void ZGConfigHelper::SetPublishVideoFps(int fps)
{
    if (ZGManagerInstance()->SdkIsInited())
    {
        PrintLogToView("设置视频帧率：fps = %d ", fps);
        LIVEROOM::SetVideoFPS(fps);
    }
}

void ZGConfigHelper::SetVideoDevice(std::string device_id)
{
    PrintLogToView("设置摄像头设备为：device id = %s ", device_id.c_str());
    LIVEROOM::SetVideoDevice(device_id.c_str());
}

void ZGConfigHelper::SetMicDevice(std::string device_id)
{
    PrintLogToView("设置麦克风设备为：device id = %s ", device_id.c_str());
    LIVEROOM::SetAudioDevice(AV::AudioDevice_Input,device_id.c_str());
}

void ZGConfigHelper::SetSpeakerDevice(std::string device_id)
{
    PrintLogToView("设置扬声器设备为：device id = %s ", device_id.c_str());
    LIVEROOM::SetAudioDevice(AV::AudioDevice_Output, device_id.c_str());
}

void ZGConfigHelper::SetViewLogCallBack(ILogToView * log_cb)
{
    view_log_ = log_cb;
}

std::vector<VideoDeviceInfo> ZGConfigHelper::GetVideoDevicesList()
{
    int video_device_count = 0;
   ZEGO::AV::DeviceInfo * device_info = LIVEROOM::GetVideoDeviceList(video_device_count);
    std::vector<VideoDeviceInfo> video_device_list;
    for (int i = 0; i < video_device_count; ++i)
    {
        VideoDeviceInfo item;
        item.device_id = device_info[i].szDeviceId;
        item.device_name = UTF8ToGBK(device_info[i].szDeviceName);
        item.index = i;
        video_device_list.push_back(item);
     }
    return video_device_list;
}


bool ZGConfigHelper::GetDefaultVideoDevice(VideoDeviceInfo & info)
{
    char device_id[512] = {0};
    unsigned int len = 512;
    LIVEROOM::GetDefaultVideoDeviceId(device_id, &len);
    std::string default_id = device_id;
    std::vector<VideoDeviceInfo> vec = GetVideoDevicesList();
    for (int i = 0;i < vec.size(); ++i)
    {
        if (vec[i].device_id == default_id)
        {
            info = vec[i];
            PrintLogToView("读取默认的摄像头设备为：");
            PrintLogToView("device id = %s", info.device_id.c_str());
            PrintLogToView("device name = %s", info.device_name.c_str());
            return true;
        }
    }

    return false;
}

std::vector<AudioDeviceInfo> ZGConfigHelper::GetMicDevicesList()
{
    int audio_device_count = 0;

    AV::DeviceInfo * device_info  = LIVEROOM::GetAudioDeviceList(AV::AudioDevice_Input, audio_device_count);

    std::vector<AudioDeviceInfo> audio_device_list;
    for (int i = 0; i < audio_device_count; ++i)
    {
        AudioDeviceInfo item;
        item.device_id = device_info[i].szDeviceId;
        item.device_name = UTF8ToGBK(device_info[i].szDeviceName);
        item.index = i;

        audio_device_list.push_back(item);
    }
    return audio_device_list;
}

bool ZGConfigHelper::GetDefaultMicDevice(AudioDeviceInfo & info)
{
    char device_id[512] = { 0 };
    unsigned int len = 512;
    LIVEROOM::GetDefaultAudioDeviceId(AV::AudioDevice_Input, device_id, &len);
    std::string default_id = device_id;

    std::vector<AudioDeviceInfo> vec = GetMicDevicesList();
    for (int i = 0; i < vec.size(); ++i)
    {
        if (vec[i].device_id == default_id)
        {
            info = vec[i];
            PrintLogToView("读取默认的麦克风设备为：");
            PrintLogToView("device id = %s", info.device_id.c_str());
            PrintLogToView("device name = %s", info.device_name.c_str());
            return true;
        }
    }
    return false;
}

std::vector< AudioDeviceInfo> ZGConfigHelper::GetSpeakerDevicesList()
{
    int audio_device_count = 0;

    AV::DeviceInfo * device_info = LIVEROOM::GetAudioDeviceList(AV::AudioDevice_Output, audio_device_count);

    std::vector<AudioDeviceInfo> audio_device_list;
    for (int i = 0; i < audio_device_count; ++i)
    {
        AudioDeviceInfo item;
        item.device_id = device_info[i].szDeviceId;
        item.device_name = UTF8ToGBK(device_info[i].szDeviceName);
        item.index = i;

        audio_device_list.push_back(item);
    }
    return audio_device_list;
}

bool ZGConfigHelper::GetDefaultSpeakerDevice(AudioDeviceInfo & info)
{
    char device_id[512] = { 0 };
    unsigned int len = 512;
    LIVEROOM::GetDefaultAudioDeviceId(AV::AudioDevice_Output, device_id, &len);
    std::string default_id = device_id;

    std::vector<AudioDeviceInfo> vec = GetSpeakerDevicesList();
    for (int i = 0; i < vec.size(); ++i)
    {
        if (vec[i].device_id == default_id)
        {
            info = vec[i];
            PrintLogToView("读取默认的扬声器设备为：");
            PrintLogToView("device id = %s", info.device_id.c_str());
            PrintLogToView("device name = %s", info.device_name.c_str());
            return true;
        }
    }
    return false;
}

void ZGConfigHelper::PrintLogToView(const char * format, ...)
{
    char log_buf[1024] = { 0 };
    va_list la;
    va_start(la, format);
    vsprintf_s(log_buf, format, la);
    va_end(la);

    if (view_log_ != nullptr)
    {
        view_log_->PrintToView(log_buf);
    }
}

ZGConfigHelper::ZGConfigHelper()
{
}


ZGConfigHelper::~ZGConfigHelper()
{
}
