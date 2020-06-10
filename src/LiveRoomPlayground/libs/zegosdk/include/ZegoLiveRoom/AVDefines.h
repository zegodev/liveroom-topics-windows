//
//  AVDefine.h
//  zegoliveroom
//

#ifndef AVDefine_h
#define AVDefine_h

#include "zego-api-defines.h"

#ifndef ZegoAVCallback_h
namespace ZEGO
{
    namespace AV
    {
        class IZegoAudioRecordCallback
        {
        public:
            /**
            音频录制结果回调

            @param pData SDK 录制的音频数据
            @param data_len 数据长度
            @param sample_rate 采样率，不固定，以当前值为准
            @param num_channels 通道数量，单通道
            @param bit_depth 位深度，16 bit
            @param type 音频类型，参考 ZegoAVAPIAudioRecordMask
            @attention 开启音频录制并设置成功代理对象后，用户调用此 API 获取 SDK 录制的音频数据。用户可自行对数据进行处理，例如：存储等
            @note 存储数据时注意取 sampleRate、numOfChannels、bitDepth 参数写包头信息
            */
            virtual void OnAudioRecordCallback(const unsigned char *pData, int data_len, int sample_rate, int num_channels, int bit_depth, unsigned int type) {}

            /**
            音频录制回调

            @warning Deprecated，请使用带 type 参数的 OnAudioRecordCallback 回调
            */
            virtual void OnAudioRecordCallback(const unsigned char *pData, int data_len, int sample_rate, int num_channels, int bit_depth) {}
        };

        class IZegoLiveEventCallback
        {
        public:
            /**
            AVKit 事件通知

            @param event 事件
            @param pInfo 信息
            */
            virtual void OnAVKitEvent(int event, EventInfo* pInfo) = 0;
        };

        class IZegoDeviceStateCallback
        {
        public:
#if defined(WIN32) || TARGET_OS_OSX
            /**
            音频设备状态变更

            @param deviceType 设备类型
            @param deviceInfo 设备信息
            @param state 状态
            */
            virtual void OnAudioDeviceStateChanged(AudioDeviceType deviceType, DeviceInfo *deviceInfo, DeviceState state) = 0;

            /**
            视频设备状态变更

            @param deviceInfo 设备信息
            @param state 状态
            */
            virtual void OnVideoDeviceStateChanged(DeviceInfo *deviceInfo, DeviceState state) {}

            /**
            音频设备音量变更

            @param deviceType 设备类型
            @param deviceId 设备 Id
            @param volumeType 音量类型
            @param volume 音量值
            @param bMuted 是否静音
            */
            virtual void OnAudioVolumeChanged(AudioDeviceType deviceType, const char *deviceId, VolumeType volumeType, unsigned int volume, bool bMuted) {}
#endif
            /**
            设备错误

            @param deviceName 设备类型名称。返回值 kZegoDeviceCamera 或 kZegoDeviceMicrophone
            @param errorCode 错误码, 返回值参考 ZegoDeviceErrorCode 定义
            
            */
            virtual void OnDeviceError(const char* deviceName, int errorCode) {}

			/**
			设备错误

			@param deviceName 设备名称
			@param errorCode 错误码
			@param deviceID 出错的设备ID 目前只支持win和mac设备
			*/
			virtual void OnDeviceError(const char* deviceName, int errorCode, const char* deviceID) {}

#ifdef WIN32
			/**
			开始重试设备时通知

			@param deviceName 设备名称
			*/
			virtual void OnRetryDevice(const char* deviceName,const char* deviceID) {}
#endif
        };
        
#if TARGET_OS_IPHONE || defined(ANDROID)
        class IZegoAudioRouteCallback
        {
        public:
            virtual ~IZegoAudioRouteCallback(){}
            
            /// \brief 音频路由发生变化的回调
            /// \param
            virtual void OnAudioRouteChange(ZegoAudioRoute audioRoute) = 0;
        };
#endif
        
        class IZegoNetTypeCallback
        {
        public:
            virtual ~IZegoNetTypeCallback(){}
            
            /// \brief 网络类型发生变化的回调
            /// \param netType 网络类型
            virtual void OnNetTypeChange(ZEGONetType netType) = 0;
        };
    }
}

#endif

#endif /* AVDefine_h */
