#pragma once

#include <string>
#include <vector>
#include <stdint.h>

#include "ZegoLiveRoom/zego-api-mix-stream.h"

using std::string;


typedef struct MixStreamConfig
{
    int output_w = 640;
    int output_h = 360;
    int fps = 15;
    int bitrate = 600000;
    bool with_sound_level = true;
    int audio_channel_count = 1;

}MixStreamConfig;

class MixStreamHelper
{
public:
    MixStreamHelper();
    ~MixStreamHelper();

    static MixStreamConfig& GetMixStreamConfig();

    void ClearInputStream();

    void AddInputStream(string stream_id, 
        int32_t top,
        int32_t left,
        int32_t bottom,
        int32_t right,
        int32_t sound_level_id,
        int32_t content_control = 0/* 默认音视频都带*/);

    void ClearOutputAddr();

    void AddOutputAddr(string stream_name_or_url, bool is_url);

    void SetOutputVideoConfig(int fps, int w, int h, int bitrate);

    void SetOutputAudioConfig(int bitrate, int channel_count, bool with_sound);

    void GetMixStreamConfig(ZEGO::MIXSTREAM::ZegoMixStreamConfig & config);

    bool ParseExtraInfoForMixStreamId(string extra_info, string & mix_stream_id);

protected:

    std::vector<ZEGO::MIXSTREAM::ZegoMixStreamInput> input_list;

    // 输出流配置
    std::vector<ZEGO::MIXSTREAM::ZegoMixStreamOutput> output_list;

    int32_t output_video_fps_;
    int32_t output_video_bitrate_;
    int32_t output_video_width_;
    int32_t output_video_height_;
    /** 0xRRGGBBxx */
    int64_t output_video_bg_color_ = 0;
    /** 混流背景图，支持预设图片，如 (preset-id://xxx) */
    std::string output_video_bg_image_;
    /** 0 表示 CBR 恒定码率，1 表示 CRF 恒定质量 */
    int32_t output_video_rate_control_mode_ = 0;
    /** 输出码率控制模式设置为 CRF恒定质量时有效，有效值范围 0-51，默认值是 23 */
    int32_t output_video_quality_ = 23;

    int output_audio_bitrate_;
    int output_audio_config_ = 0;
    int output_audio_channel_count_;
    bool output_audio_with_sound_;

    string output_video_watermark_;

    string mixstream_side_info_data_;

    string media_side_info_user_data_;
    

};

