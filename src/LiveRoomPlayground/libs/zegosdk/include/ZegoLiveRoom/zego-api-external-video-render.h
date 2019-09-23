//
//  zego-api-external-video-render.h
//

#ifndef zego_api_extrnal_video_render_h
#define zego_api_extrnal_video_render_h
#include "video_format.h"
#include "zego-api-defines.h"
#include "video_capture.h"

namespace ZEGO
{
    namespace EXTERNAL_RENDER
    {

        extern ZEGOAVKIT_API const char* kZegoVideoDataMainPublishingStream;
        extern ZEGOAVKIT_API const char* kZegoVideoDataAuxPublishingStream;

        enum VideoRenderType
        {
            /** 默认值, 不开外部渲染 */
            VIDEO_RENDER_TYPE_NONE = 0,
            /** 默认 BGRA32（Android 为 RGBA32）, 外部渲染同时内部不渲染 */
            VIDEO_RENDER_TYPE_RGB = 1,
            /** 默认 I420, 外部渲染同时内部不渲染 */
            VIDEO_RENDER_TYPE_YUV = 2,
            /** 由返回参数指定类型， 外部渲染同时内部不渲染*/
            VIDEO_RENDER_TYPE_ANY = 3,
            /** 外部渲染同时内部渲染，且数据格式为 RGB。默认 BGRA32（Android 为 RGBA32） */
            VIDEO_RENDER_TYPE_EXTERNAL_INTERNAL_RGB = 4,
            /** 外部渲染同时内部渲染，且数据格式为 YUV。默认 I420 */
            VIDEO_RENDER_TYPE_EXTERNAL_INTERNAL_YUV = 5
        };

        class IZegoVideoRenderCallback
        {
        public:
              virtual void OnVideoRenderCallback(unsigned char **pData, int* dataLen, 
                                     const char* pszStreamID, 
                                     int width, int height, int strides[4],                                               
                                     AVE::VideoPixelFormat pixel_format) = 0;

              virtual void SetFlipMode(const char* pszStreamID, int mode) = 0;

              virtual ~IZegoVideoRenderCallback() {};
        };

        class IZegoVideoDecodeCallback
        {
        public:
            virtual void OnVideoDecodeCallback(const unsigned char* data, int length,
                                       const char* pszStreamID, const AVE::VideoCodecConfig& codec_config,
                                       bool b_keyframe, double reference_time_ms) = 0;
            virtual ~IZegoVideoDecodeCallback() {}
        };

        /**
         设置当开启外部视频渲染时要求 SDK 提供的渲染方式（仅外部渲染或者内部外部同时渲染）及数据格式

         @param type 指定 SDK 提供的渲染方式及数据格式
         @note 在初始化 SDK 之前调用有效
         */
        ZEGOAVKIT_API void SetVideoRenderType(VideoRenderType type);

        /**
         设置外部视频渲染回调。App 通过此回调接收 SDK 提供的待渲染视频数据

         @param callback 外部渲染回调
         @note 启动拉流/预览前设置，中途最好不要随便置空
         */
        ZEGOAVKIT_API void SetVideoRenderCallback(IZegoVideoRenderCallback* callback);

        /**
         设置外部视频解码回调。当调用该 API 设置了回调后，App 通过此回调接收 SDK 原始码流。

         @param callback 外部解码回调
         @note 启动拉流前设置，中途不要随便置空；
         @note 当设置了此回调后，内部渲染与外部渲染都将无效，必须由 App 自己实现解码渲染。
         */
        ZEGOAVKIT_API void SetVideoDecodeCallback(IZegoVideoDecodeCallback* callback);

        /**
         设置是否开启/关闭外部视频渲染（拉流）
         
         @param pszStreamID 播放流ID
         @param bEnable true 开启， false 不开启，默认为不开启
         @note 只有当 VideoRenderType 设置为 VIDEO_RENDER_TYPE_EXTERNAL_INTERNAL_RGB 或者 VIDEO_RENDER_TYPE_EXTERNAL_INTERNAL_YUV，该接口才有效
         @note 在初始化 SDK 之后调用有效
         */
        ZEGOAVKIT_API bool EnableVideoRender(bool bEnable, const char *pszStreamID);
        
        /**
        设置是否开启/关闭外部视频渲染（拉流）

        @param nPlayChannel 播放通道
        @param bEnable true 开启， false 不开启，默认为不开启
        @note 只有当 VideoRenderType 设置为 VIDEO_RENDER_TYPE_EXTERNAL_INTERNAL_RGB 或者 VIDEO_RENDER_TYPE_EXTERNAL_INTERNAL_YUV，该接口才有效
        @note 在初始化 SDK 之后调用有效
        */
        ZEGOAVKIT_API bool EnableVideoRender(bool bEnable, int nPlayChannel);

        /**
        设置是否需要 SDK 将推流预览的数据抛出。当置为 true 时，SDK 通过 SetVideoRenderCallback 设置的接口将预览数据返回给 App

        @param bEnable true 开启， false 不开启，默认为不开启
        @param nPublishChannel 推流通道，默认为主通道
        @note 在初始化 SDK 之后调用有效
        */
        ZEGOAVKIT_API bool EnableVideoPreview(bool bEnable, AV::PublishChannelIndex nPublishChannel = AV::PUBLISH_CHN_MAIN);

        /**
         @warning Deprecated 请根据实际情况使用 IZegoVideoRenderCallback or IZegoVideoDecodeCallback 替换
         */
        class IZegoExternalRenderCallback2
        {
        public:
        
            /**
             视频帧数据回调
             
             @param pData 视频数据起始地址，windows 平台上回调数据格式为 RGBA32
             @param dataLen 视频数据长度
             @param pszStreamID 流 ID，如果是本地预览数据，值为 kZegoVideoDataMainPublishingStream 或者 kZegoVideoDataAuxPublishingStream
             @param width 视频宽度
             @param height 视频高度
             @param strides 每个平面一行字节数（RGBA 只需考虑 strides[0]）
             */
            virtual void OnVideoDataCallback(const unsigned char *pData, int dataLen, const char* pszStreamID, int width, int height, int strides[4]) {};
            
            /**
             视频帧数据回调2
             @param pData 视频数据起始地址
             @param dataLen 视频数据长度起始地址
             @param pszStreamID 流 ID，如果是本地预览数据，值为 kZegoVideoDataMainPublishingStream 或者 kZegoVideoDataAuxPublishingStream
             @param width 视频帧宽度
             @param height 视频帧高度
             @param strides 视频帧每个平面一行字节数
             @param pixel_format 视频帧数据格式
             */
            virtual void OnVideoDataCallback2(unsigned char **pData, int* dataLen, const char* pszStreamID, int width, int height, int strides[4], AVE::VideoPixelFormat pixelFormat) {}

            /**
             通知即将接收的帧数据是否需要翻转
             @param pszStreamID 流 ID，如果是本地预览数据，值为 kZegoVideoDataMainPublishingStream 或者 kZegoVideoDataAuxPublishingStream
             @param mode 翻转类型，参见 VideoFlipMode 定义
             @note 仅本地预览的外部渲染会回调。此处的 mode 是基于推流图像计算出来的，和 SetVideoMirrorMode 不一定一致，请基于 SetFlipMode 的参数决定是否翻转
             */
            virtual void SetFlipMode(const char* pszStreamID, int mode) {}
            
            virtual ~IZegoExternalRenderCallback2() {}
        };

        /**
         设置外部渲染回调，SDK 将通过此回调接口返回待渲染数据

         @warning Deprecated 请 SetVideoRenderType 替代

         @param callback 回调接口实例
         */
        ZEGOAVKIT_API bool SetExternalRenderCallback(IZegoExternalRenderCallback2* callback);
       
		/**
		设置是否使用外部视频渲染

        @warning Deprecated 请根据实际情况使用 SetVideoRenderCallback or SetVideoDecodeCallback 替换

		@param externalRender true 开启，false 不开启，默认为不开启
		@param type 外部渲染类型，详见类型 VideoExternalRenderType
		@note 在InitSDK之前调用。
		*/
        ZEGOAVKIT_API bool EnableExternalRender(bool externalRender, AV::VideoExternalRenderType type);
    }
}

#endif /* zego_api_extrnal_video_render_h */
