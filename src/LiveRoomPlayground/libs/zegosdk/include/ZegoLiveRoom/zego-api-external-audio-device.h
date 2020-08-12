//
//  zego-api-external-audio-device.h
//  zegoavkit
//
//  Copyright © 2018年 Zego. All rights reserved.
//

#ifndef zego_api_external_audio_device_h
#define zego_api_external_audio_device_h

#include <zego-api-defines.h>

#include "audio_in_output.h"
#include "zego-api-audio-frame.h"

#ifdef __cplusplus
extern "C" {
#endif
    
    /** 辅助推流通道音频采集源类别 */
    typedef enum ZegoAuxPublishChannelAudioSrcType
    {
        kZegoAuxPublishChannelAudioSrcTypeNone                      = -1,   /**< 无声 */
        kZegoAuxPublishChannelAudioSrcTypeSameAsMainPublishChannel  = 0,    /**< 和主推流通道一样 */
        kZegoAuxPublishChannelAudioSrcTypeExternalCapture           = 1,    /**< 使用外部采集 */
        kZegoAuxPublishChannelAudioSrcTypePlayer                    = 2,    /**< 使用媒体播放器的音源 */
    } ZegoAuxPublishChannelAudioSrcType;
    
    /**
     主推流通道外部音频采集开关

     @param enable 是否启用外部音频采集
     @return 详见 enum ZegoErrorCode
     @note 必须在开启预览或者启动推流/拉流 前调用才有效；
     @discussion 开发者采用外部采集和渲染后，SDK 内部不负责声音增强、噪音抑制、回音消除等功能，需要用户自己实现。
     */
    ZEGOAVKIT_API int zego_external_audio_device_enable(bool enable);
    
    /**
     选择辅助推流通道的音频采集源
     
     @param type 辅助推流通道音频采集源类别，参考 enum ZegoAuxPublishChannelAudioSrcType
     @return 详见 enum ZegoErrorCode
     @note 必须在开启预览或者启动推流/拉流 前调用才有效；
     */
    ZEGOAVKIT_API int zego_external_audio_device_set_audio_src_for_auxiliary_publish_channel(enum ZegoAuxPublishChannelAudioSrcType type);
    
    /**
     开始外部音频采集（通知 SDK）

     @param publish_channel_index 选定推流通道
     @return 详见 enum ZegoErrorCode
     */
    ZEGOAVKIT_API int zego_external_audio_device_start_capture(int publish_channel_index);
    
    /**
     结束外部音频采集（通知 SDK）

     @param publish_channel_index 选定推流通道
     @return 详见 enum ZegoErrorCode
     */
    ZEGOAVKIT_API int zego_external_audio_device_stop_capture(int publish_channel_index);
    
    /**
     把采集到的音频数据(AAC 或者 PCM） 塞给 SDK。

     @param publish_channel_index 选定推流通道
     @param audio_frame 采集到的音频帧
     @return 详见 enum ZegoErrorCode
     @note 必须在启动推流以后，且已经调用了 zego_external_audio_device_start_capture API，此接口调用才有效
     */
    ZEGOAVKIT_API int zego_external_audio_device_on_record_audio_frame(int publish_channel_index, struct ZegoAudioFrame* audio_frame);
    
    /**
     开始外部音频播放（通知 SDK）

     @return 详见 enum ZegoErrorCode
     */
    ZEGOAVKIT_API int zego_external_audio_device_start_render();
    
    /**
     结束外部音频播放（通知 SDK）

     @return 详见 enum ZegoErrorCode
     */
    ZEGOAVKIT_API int zego_external_audio_device_stop_render();
    
    /**
     从 SDK 取音频渲染数据(PCM)

     @param audio_frame 得到的音频数据
     @return 详见 enum ZegoErrorCode
     @note 必须在启动拉流以后，且已经调用了 zego_external_audio_device_start_render API，此接口调用才有效
     */
    ZEGOAVKIT_API int zego_external_audio_device_on_playback_audio_frame(struct ZegoAudioFrame* audio_frame);
    
#ifdef __cplusplus
} // __cplusplus defined.
#endif

#endif /* zego_api_external_audio_capture_h */
