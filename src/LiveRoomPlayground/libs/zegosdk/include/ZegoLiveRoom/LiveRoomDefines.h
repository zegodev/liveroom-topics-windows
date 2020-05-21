//
//  ZegoLiveRoomDefines.h
//  zegoliveroom
//
//  Copyright © 2017年 ZEGO. All rights reserved.
//

#ifndef ZegoLiveRoomDefines_h
#define ZegoLiveRoomDefines_h

#include "./AVDefines.h"
#include "./RoomDefines.h"


namespace ZEGO
{
    namespace LIVEROOM
    {
        using namespace COMMON;
        using COMMON::ZEGONetType;
        
        using COMMON::ZegoRoomRole;
        
        using COMMON::ZegoStreamInfo;
        
        using COMMON::ZegoStreamUpdateType;
        
        const int kInvalidSeq = -1;
        
        /** 本地预览视频视图的模式 */
        enum ZegoVideoViewMode
        {
            ZegoVideoViewModeScaleAspectFit = 0,    /**< 等比缩放，可能有黑边 */
            ZegoVideoViewModeScaleAspectFill = 1,   /**< 等比缩放填充整View，可能有部分被裁减 */
            ZegoVideoViewModeScaleToFill = 2,       /**< 填充整个View */
        };
        
        struct ZegoPublishQuality
        {
            double cfps;            ///< 视频帧率(采集)
            double vencFps;         ///< 视频帧率(编码)
            double fps;             ///< 视频帧率(网络发送)
            double kbps;            ///< 视频码率(kb/s)
            
            double acapFps;         ///< 音频帧率（采集）
            double afps;            ///< 音频帧率（网络发送）
            double akbps;           ///< 音频码率(kb/s)
        
            int rtt;                ///< 延时(ms)
            int pktLostRate;        ///< 丢包率(0~255)
            int quality;            ///< 质量(0~3)
            
            bool isHardwareVenc;    ///< 是否硬编
            int width;              ///< 视频宽度
            int height;             ///< 视频高度

            double totalBytes = 0.; ///< 已发送的总字节数，包括音频、视频及媒体次要信息等
            double audioBytes = 0.; ///< 已发送的音频字节数
            double videoBytes = 0.; ///< 已发送的视频字节数
            
            double cpuAppUsage = 0.;    ///< 当前 APP 的 CPU 使用率
            double cpuTotalUsage = 0.;  ///< 当前系统的 CPU 使用率
            
            double memoryAppUsage = 0.;         ///< 当前 APP 的内存使用率
            double memoryTotalUsage = 0.;       ///< 当前系统的内存使用率
            double memoryAppUsed = 0.;          ///< 当前 APP 的内存使用量, 单位 MB(win返回的是app实际占用内存工作集=专用内存工作集+共享内存工作集)
        };
        
        struct ZegoPlayQuality
        {
            double fps;              ///< 视频帧率(网络接收)
            double vdjFps;           ///< 视频帧率(dejitter)
            double vdecFps;          ///< 视频帧率(解码)
            double vrndFps;          ///< 视频帧率(渲染)
            double kbps;             ///< 视频码率(kb/s)
            
            double afps;             ///< 音频帧率(网络接收)
            double adjFps;           ///< 音频帧率(dejitter)
            double adecFps;          ///< 音频帧率(解码)
            double arndFps;          ///< 音频帧率(渲染)
            double akbps;            ///< 音频码率(kb/s)
            
            double audioBreakRate;   ///< 音频卡顿次数
            double videoBreakRate;   ///< 视频卡顿次数
            int rtt;                 ///< 延时(ms)
            int pktLostRate;         ///< 丢包率(0~255)
            int peerToPeerDelay;     ///< 端到端延迟
            int peerToPeerPktLostRate; ///< 端到端丢包率(0~255)
            int quality;             ///< 质量(0~3)
            int delay;               ///< 语音延时(ms)
            
            bool isHardwareVdec;     ///< 是否硬解
            int width;               ///< 视频宽度
            int height;              ///< 视频高度

            double totalBytes = 0.; ///< 已接收的总字节数，包括音频、视频及媒体次要信息等
            double audioBytes = 0.; ///< 已接收的音频字节数
            double videoBytes = 0.; ///< 已接收的视频字节数
            
            double cpuAppUsage = 0.;        ///< 当前 APP 的 CPU 使用率
            double cpuTotalUsage = 0.;      ///< 当前系统的 CPU 使用率
            
            double memoryAppUsage = 0.;         ///< 当前 APP 的内存使用率
            double memoryTotalUsage = 0.;       ///< 当前系统的内存使用率
            double memoryAppUsed = 0.;          ///< 当前 APP 的内存使用量, 单位 MB
        };

		enum ZegoMixSysPlayoutPropertyMask
		{
			MIX_PROP_NONE = 0,
			MIX_PROP_ENABLE_AGC_FOR_SYS_PLAYOUT = 1,
		};
    }
}

#endif /* ZegoLiveRoomDefines_h */
