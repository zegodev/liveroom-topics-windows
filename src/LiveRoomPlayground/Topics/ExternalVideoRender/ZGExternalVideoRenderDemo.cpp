#include "stdafx.h"
#include "ZGExternalVideoRenderDemo.h"

#include "ZegoLiveRoom/zego-api-external-video-render.h"
#include "AppSupport/ZGManager.h"
#include "AppSupport/ZGLog.h"
#include "AppSupport/ZGHelper.h"

#include "../../libs/libyuv/include/libyuv.h"


using namespace AVE;

ZGExternalVideoRenderDemo::ZGExternalVideoRenderDemo()
{
}

ZGExternalVideoRenderDemo::~ZGExternalVideoRenderDemo()
{
}

void ZGExternalVideoRenderDemo::SetEnableExternalRender(bool bEnable)
{
	if(bEnable)
	{ 
	//	EXTERNAL_RENDER::EnableExternalRender(true, AV::DECODE_RGB_SERIES);
		EXTERNAL_RENDER::SetVideoRenderType(EXTERNAL_RENDER::VIDEO_RENDER_TYPE_RGB);
		EXTERNAL_RENDER::SetVideoRenderCallback(this);
	}
	else
	{
	//	EXTERNAL_RENDER::EnableExternalRender(false, AV::DECODE_RGB_SERIES);
		EXTERNAL_RENDER::SetVideoRenderType(EXTERNAL_RENDER::VIDEO_RENDER_TYPE_NONE);
		EXTERNAL_RENDER::SetVideoRenderCallback(nullptr);
	}

}

void ZGExternalVideoRenderDemo ::EnablePlayRender(bool bEnable, string streamId)
{
	EXTERNAL_RENDER::EnableVideoRender(bEnable, streamId.c_str());
}

void ZGExternalVideoRenderDemo::EnableExternalRender(EXTERNAL_RENDER::VideoRenderType type)
{
    ZGENTER_FUN_LOG;

    cur_external_render_type_ = type;

    if (ZGManagerInstance()->SdkIsInited())
    {
        ZGManagerInstance()->UninitSdk();
    }

    // EnableExternalRender要在InitSDK之前调用
    // type 设置为 DECODE_RGB_SERIES 时，视频数据在回调 OnVideoDataCallback 获取，格式为RGB格式系列
    // type 设置为 DECODE 时，视频数据在回调 OnVideoDataCallback2 获取，本地预览推流的视频数据格式为RGB系列，拉流数据为YUV的 PIXEL_FORMAT_I420 格式
    // type 设置为 DECODE_RENDER 时，内部渲染生效的同时可以通过调用EnableVideoRender设定是否抛出视频数据，默认不抛出视频数据
    // 
    //ZEGO::EXTERNAL_RENDER::EnableExternalRender(true, cur_external_render_type_);

    EXTERNAL_RENDER::SetVideoRenderType(EXTERNAL_RENDER::VIDEO_RENDER_TYPE_RGB);

    EXTERNAL_RENDER::SetVideoRenderCallback(this);

    //ZEGO::EXTERNAL_RENDER::SetExternalRenderCallback(this);

    

    // 使用测试环境，生产上线时，需要联系zego技术支持切换为正式环境，并修改为 SetUseTestEnv(FALSE); 表示启用正式环境
    LIVEROOM::SetUseTestEnv(TRUE);
    ZGManagerInstance()->InitSdk();

    if (cur_external_render_type_ == EXTERNAL_RENDER::VIDEO_RENDER_TYPE_EXTERNAL_INTERNAL_RGB)
    {
        // 设置内部渲染的同时，回调推流的视频数据
        // 在推流前调用
        ZEGO::EXTERNAL_RENDER::EnableVideoPreview(true);

    }
}

void ZGExternalVideoRenderDemo::SetVideoDataCallBack(ExternalVideoRenderDataCallBackType cb_local, ExternalVideoRenderDataCallBackType cb_remote)
{
    local_video_data_cb_ = cb_local;
    remote_video_data_cb_ = cb_remote;
}


void ZGExternalVideoRenderDemo::OnVideoRenderCallback(unsigned char **pData, int* dataLen, const char* pszStreamID, int width, int height, int strides[4], AVE::VideoPixelFormat pixel_format)
{
    //    ZGENTER_FUN_LOG;
  //  ZGLog("ZGExternalVideoRenderDemo::OnVideoDataCallback , pszStreamID = %s", pszStreamID);

    // 通过pszStreamID 判断是 kZegoVideoDataMainPublishingStream 还是 kZegoVideoDataAuxPublishingStream 
// kZegoVideoDataMainPublishingStream 表示主通道推流的视频数据
// kZegoVideoDataAuxPublishingStream 表示辅通道推流的视频数据
// 不是上述两种，则为
    if (strcmp(pszStreamID, ZEGO::EXTERNAL_RENDER::kZegoVideoDataMainPublishingStream) == 0)
    {
        // 推流主通道数据
        if (pixel_format == AVE::PIXEL_FORMAT_I420 && pData != nullptr && pData[0] != nullptr && pData[1] != nullptr && pData[2] != nullptr)
        {
            // 获取yuv分量数据
            const uint8* y_data = pData[0];
            const uint8* u_data = pData[1];
            const uint8* v_data = pData[2];

            int stride_y = strides[0];
            int stride_u = strides[1];
            int stride_v = strides[2];

            if (width != width_ || height != height_)
            {
                width_ = width;
                height_ = height;
                // yuv转rgb需要的空间分配
                local_rgb_data_buf_ = std::unique_ptr<uint8_t[] >(new uint8_t[width_* height_ * 4]);
                rgb_data_len_ = width_ * height_ * 4;
            }

            // yuv 转为 rgb
            libyuv::I420ToARGB(y_data, stride_y, u_data, stride_u, v_data, stride_v, local_rgb_data_buf_.get(), width_ * 4, width, height);
            if (local_video_data_cb_)
            {
                local_video_data_cb_(pszStreamID, local_rgb_data_buf_.get(), rgb_data_len_, width_, height_);
            }
        }
        else if (pixel_format == AVE::PIXEL_FORMAT_BGRA32)
        {
            if (local_video_data_cb_ && pData != nullptr && pData[0] != nullptr && dataLen != nullptr)
            {
                local_video_data_cb_(pszStreamID, pData[0], *dataLen, width, height);
            }
        }
    }
    else if (strcmp(pszStreamID, ZEGO::EXTERNAL_RENDER::kZegoVideoDataAuxPublishingStream) == 0)
    {
        // 推流辅通道数据
    }
    else
    {
        if (pixel_format == AVE::PIXEL_FORMAT_I420 && pData != nullptr && pData[0] != nullptr && pData[1] != nullptr && pData[2] != nullptr)
        {
            // 获取yuv分量数据
            const uint8* y_data = pData[0];
            const uint8* u_data = pData[1];
            const uint8* v_data = pData[2];

            int stride_y = strides[0];
            int stride_u = strides[1];
            int stride_v = strides[2];

            if (width != remote_width_ || height != remote_height_)
            {
                remote_width_ = width;
                remote_height_ = height;
                // yuv转rgb需要的空间分配
                remote_rgb_data_buf_ = std::unique_ptr<uint8_t[] >(new uint8_t[remote_width_* remote_height_ * 4]);
                remote_rgb_data_len_ = remote_width_ * remote_height_ * 4;
            }

            // yuv 转为 rgb
            libyuv::I420ToARGB(y_data, stride_y, u_data, stride_u, v_data, stride_v, remote_rgb_data_buf_.get(), remote_width_ * 4, width, height);

            if (remote_video_data_cb_)
            {
                remote_video_data_cb_(pszStreamID, remote_rgb_data_buf_.get(), remote_rgb_data_len_, remote_width_, remote_height_);
            }
        }
        else {

            if (remote_video_data_cb_)
            {
                remote_video_data_cb_(pszStreamID, pData[0], *dataLen, width, height);
            }
        }
    }
}

void ZGExternalVideoRenderDemo::SetFlipMode(const char* pszStreamID, int mode)
{
    
}

void ZGExternalVideoRenderDemo::SetRotation(const char* pszStreamID, int rotation)
{
    
}
// 
// void ZGExternalVideoRenderDemo::OnVideoDataCallback2(unsigned char **pData, int* dataLen, const char* pszStreamID, int width, int height, int strides[4], AVE::VideoPixelFormat pixelFormat)
// {
// //    ZGENTER_FUN_LOG;
//     ZGLog("ZGExternalVideoRenderDemo::OnVideoDataCallback2 , pixelFormat = %s", ZGHelperInstance()->GetPixelFormatDesc(pixelFormat).c_str());
// 
//     // 通过pszStreamID 判断是 kZegoVideoDataMainPublishingStream 还是 kZegoVideoDataAuxPublishingStream 
// // kZegoVideoDataMainPublishingStream 表示主通道推流的视频数据
// // kZegoVideoDataAuxPublishingStream 表示辅通道推流的视频数据
// // 不是上述两种，则为
//     if (strcmp(pszStreamID, ZEGO::EXTERNAL_RENDER::kZegoVideoDataMainPublishingStream) == 0)
//     {
//         // 推流主通道数据
//         if (pixelFormat == AVE::PIXEL_FORMAT_I420 && pData != nullptr && pData[0] != nullptr && pData[1] != nullptr && pData[2] != nullptr)
//         {
//             // 获取yuv分量数据
//             const uint8* y_data = pData[0];
//             const uint8* u_data = pData[1];
//             const uint8* v_data = pData[2];
// 
//             int stride_y = strides[0];
//             int stride_u = strides[1];
//             int stride_v = strides[2];
// 
//             if (width != width_ || height != height_)
//             {
//                 width_ = width;
//                 height_ = height;
//                 // yuv转rgb需要的空间分配
//                 local_rgb_data_buf_ = std::unique_ptr<uint8_t[] >(new uint8_t[width_* height_ * 4]);
//                 rgb_data_len_ = width_ * height_ * 4;
//             }
// 
//             // yuv 转为 rgb
//             libyuv::I420ToARGB(y_data, stride_y, u_data, stride_u, v_data, stride_v, local_rgb_data_buf_.get(), width_ * 4, width, height);
//             if (local_video_data_cb_)
//             {
//                 local_video_data_cb_(pszStreamID, local_rgb_data_buf_.get(), rgb_data_len_, width_, height_);
//             }
//         }else if (pixelFormat == AVE::PIXEL_FORMAT_BGRA32)
//         {
//             if (local_video_data_cb_ && pData != nullptr && pData[0] != nullptr && dataLen != nullptr)
//             {
//                 local_video_data_cb_(pszStreamID, pData[0], *dataLen, width, height);
//             }
//         }
//     }
//     else if (strcmp(pszStreamID, ZEGO::EXTERNAL_RENDER::kZegoVideoDataAuxPublishingStream) == 0)
//     {
//         // 推流辅通道数据
//     }
//     else
//     {
//         if (pixelFormat == AVE::PIXEL_FORMAT_I420 && pData != nullptr && pData[0] != nullptr && pData[1] != nullptr && pData[2] != nullptr )
//         {
//             // 获取yuv分量数据
//             const uint8* y_data = pData[0];
//             const uint8* u_data = pData[1];
//             const uint8* v_data = pData[2];
//             
//             int stride_y = strides[0];
//             int stride_u = strides[1];
//             int stride_v = strides[2];
// 
//             if (width != remote_width_ || height != remote_height_)
//             {
//                 remote_width_ = width;
//                 remote_height_ = height;
//                 // yuv转rgb需要的空间分配
//                 remote_rgb_data_buf_ = std::unique_ptr<uint8_t[] >(new uint8_t[remote_width_* remote_height_ * 4]);
//                 remote_rgb_data_len_ = remote_width_ * remote_height_ * 4;
//             }
// 
//             // yuv 转为 rgb
//             libyuv::I420ToARGB(y_data, stride_y, u_data, stride_u, v_data, stride_v, remote_rgb_data_buf_.get(), remote_width_*4, width, height);
// 
//             if (remote_video_data_cb_ )
//             {
//                 remote_video_data_cb_(pszStreamID, remote_rgb_data_buf_.get(), remote_rgb_data_len_, remote_width_, remote_height_);
//             }
//         }
//         else {
// 
//             if (remote_video_data_cb_)
//             {
//                 remote_video_data_cb_(pszStreamID, remote_rgb_data_buf_.get(), remote_rgb_data_len_, remote_width_, remote_height_);
//             }            
//         }
//     }
// }
// 
// void ZGExternalVideoRenderDemo::OnVideoDataCallback(const unsigned char *pData, int dataLen, const char* pszStreamID, int width, int height, int strides[4])
// {
// //    ZGENTER_FUN_LOG;
//     // 通过pszStreamID 判断是 kZegoVideoDataMainPublishingStream 还是 kZegoVideoDataAuxPublishingStream 
//     // kZegoVideoDataMainPublishingStream 表示主通道推流的视频数据
//     // kZegoVideoDataAuxPublishingStream 表示辅通道推流的视频数据
//     // 不是上述两种，则为拉流数据
//     if (strcmp(pszStreamID, ZEGO::EXTERNAL_RENDER::kZegoVideoDataMainPublishingStream) == 0)
//     {
//         // 推流主通道数据
//         if (local_video_data_cb_)
//         {
//             local_video_data_cb_(pszStreamID, pData, dataLen, width, height);
//         }        
//     }else if (strcmp(pszStreamID, ZEGO::EXTERNAL_RENDER::kZegoVideoDataAuxPublishingStream) == 0)
//     {
//         // 推流辅通道数据
//     }else
//     {
//         // 拉流数据
//         if (remote_video_data_cb_)
//         {
//             remote_video_data_cb_(pszStreamID, pData, dataLen, width, height);
//         }
//     }
// }

