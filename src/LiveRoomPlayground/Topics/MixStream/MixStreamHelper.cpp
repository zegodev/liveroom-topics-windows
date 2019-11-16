#include "stdafx.h"
#include "MixStreamHelper.h"

#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>


using namespace ZEGO;

static MixStreamConfig kMixStreamConfig;

MixStreamHelper::MixStreamHelper()
{
}


MixStreamHelper::~MixStreamHelper()
{
}

MixStreamConfig& MixStreamHelper::GetMixStreamConfig()
{
    return kMixStreamConfig;
}

void MixStreamHelper::ClearInputStream()
{
    input_list.clear();
}

void MixStreamHelper::AddInputStream(string stream_id, int32_t top, int32_t left, int32_t bottom, int32_t right, int32_t sound_level_id, int32_t content_control)
{
    MIXSTREAM::ZegoMixStreamInput i;
    memcpy(i.szStreamID, stream_id.data(), stream_id.size());
    i.szStreamID[stream_id.size()] = '\0';

    i.layout.top = top;
    i.layout.left = left;
    i.layout.bottom = bottom;
    i.layout.right = right;

    i.uSoundLevelID = sound_level_id;
    i.nContentControl = content_control;

    input_list.push_back(i);
}

void MixStreamHelper::ClearOutputAddr()
{
    output_list.clear();
}

void MixStreamHelper::AddOutputAddr(string stream_name_or_url, bool is_url)
{
    MIXSTREAM::ZegoMixStreamOutput o;
    o.isUrl = is_url;
    memcpy(o.target, stream_name_or_url.c_str(), stream_name_or_url.size());
    o.target[stream_name_or_url.size()] = '\0';

    output_list.push_back(o);
}

void MixStreamHelper::SetOutputVideoConfig(int fps, int w, int h, int bitrate)
{
    output_video_fps_ = fps;
    output_video_width_ = w;
    output_video_height_ = h;
    output_video_bitrate_ = bitrate;

}

void MixStreamHelper::SetOutputAudioConfig(int bitrate, int channel_count, bool with_sound)
{
    output_audio_bitrate_ = bitrate;

    output_audio_channel_count_ = channel_count;

    output_audio_with_sound_ = with_sound;
}

void MixStreamHelper::GetMixStreamConfig(MIXSTREAM::ZegoMixStreamConfig & config)
{

    // * user data
    if (media_side_info_user_data_ != "")
    {
        config.pUserData = (const unsigned char *)media_side_info_user_data_.data();
    }
    

    config.pInputStreamList = input_list.data();
    config.nInputStreamCount = input_list.size();

    config.pOutputList = output_list.data();
    config.nOutputStreamCount = output_list.size();

    // Êä³öÊÓÆµÅäÖÃ
    config.nOutputFps = output_video_fps_;
    config.nOutputBitrate = output_video_bitrate_;
    config.nOutputRateControlMode = output_video_rate_control_mode_;
    config.nOutputQuality = output_video_quality_;
    config.nOutputWidth = output_video_width_;
    config.nOutputHeight = output_video_height_;
    config.nOutputBackgroundColor = output_video_bg_color_;

    config.pOutputBackgroundImage = output_video_bg_image_.data();

    if (output_video_watermark_.length() > 0)
    {
        // Ë®Ó¡
        memcpy(config.pOutputWatermark->image, output_video_watermark_.data(), output_video_watermark_.size());
        config.pOutputWatermark->image[output_video_watermark_.size()] = '\0';
    }

    // 
    config.nOutputAudioBitrate = output_audio_bitrate_;
    config.nOutputAudioConfig = output_audio_config_;
    config.nChannels = output_audio_channel_count_;
    config.bWithSoundLevel = output_audio_with_sound_;

}

bool MixStreamHelper::ParseExtraInfoForMixStreamId(string extra_info, string & mix_stream_id)
{
    // {"first":true,"rtmp":"%s","Hls":"%s","mixStreamID":""}
    rapidjson::Document d;
    if (d.Parse(extra_info.c_str()).HasParseError() || !d.IsObject())
    {
        return false;
    }
    if (d.HasMember("mixStreamID"))
    {
        string mix_id = d["mixStreamID"].GetString();
        if (mix_id != "")
        {
            mix_stream_id = mix_id;
            return true;
        }
    }
    return false;
}

