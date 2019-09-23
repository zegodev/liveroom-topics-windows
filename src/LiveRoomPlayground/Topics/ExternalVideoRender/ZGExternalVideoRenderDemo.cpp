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

void ZGExternalVideoRenderDemo::EnableExternalRender(ZEGO::EXTERNAL_RENDER::VideoRenderType type)
{
    ZGENTER_FUN_LOG;

    cur_external_render_type_ = type;

    if (ZGManagerInstance()->SdkIsInited())
    {
        ZGManagerInstance()->UninitSdk();
    }

    ZEGO::EXTERNAL_RENDER::SetVideoRenderType(cur_external_render_type_);
	ZEGO::EXTERNAL_RENDER::SetVideoRenderCallback(this);

    // 使用测试环境，生产上线时，需要联系zego技术支持切换为正式环境，并修改为 SetUseTestEnv(FALSE); 表示启用正式环境
    LIVEROOM::SetUseTestEnv(TRUE);
    ZGManagerInstance()->InitSdk();

    if (cur_external_render_type_ >= ZEGO::EXTERNAL_RENDER::VIDEO_RENDER_TYPE_EXTERNAL_INTERNAL_RGB)
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


void ZGExternalVideoRenderDemo::SetFlipMode(const char* pszStreamID, int mode)
{

}

void ZGExternalVideoRenderDemo::OnVideoDecodeCallback(const unsigned char* data, int length, const char* pszStreamID, const AVE::VideoCodecConfig& codec_config, bool b_keyframe, double reference_time_ms)
{

}

void ZGExternalVideoRenderDemo::OnVideoRenderCallback(unsigned char **pData, int* dataLen, const char* pszStreamID, int width, int height, int strides[4], AVE::VideoPixelFormat pixelFormat)
{
	if (pszStreamID == nullptr)
	{
		return;
	}
//    ZGENTER_FUN_LOG;
    ZGLog("ZGExternalVideoRenderDemo::OnVideoRenderCallback , pixelFormat = %s", ZGHelperInstance()->GetPixelFormatDesc(pixelFormat).c_str());

    // 通过pszStreamID 判断是 kZegoVideoDataMainPublishingStream 还是 kZegoVideoDataAuxPublishingStream 
	// kZegoVideoDataMainPublishingStream 表示主通道推流的视频数据 本地渲染
	// kZegoVideoDataAuxPublishingStream 表示辅通道推流的视频数据 本地渲染
	// 不是上述两种，则为拉流渲染
    if (strcmp(pszStreamID, ZEGO::EXTERNAL_RENDER::kZegoVideoDataMainPublishingStream) == 0)
    {
        // 推流主通道数据
        if (pixelFormat == AVE::PIXEL_FORMAT_I420 && pData != nullptr && pData[0] != nullptr && pData[1] != nullptr && pData[2] != nullptr)
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
        }else if (pixelFormat == AVE::PIXEL_FORMAT_BGRA32)
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
        if (pixelFormat == AVE::PIXEL_FORMAT_I420 && pData != nullptr && pData[0] != nullptr && pData[1] != nullptr && pData[2] != nullptr )
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
            libyuv::I420ToARGB(y_data, stride_y, u_data, stride_u, v_data, stride_v, remote_rgb_data_buf_.get(), remote_width_*4, width, height);

            if (remote_video_data_cb_ )
            {
                remote_video_data_cb_(pszStreamID, remote_rgb_data_buf_.get(), remote_rgb_data_len_, remote_width_, remote_height_);
            }
        }
        else {

            if (remote_video_data_cb_)
            {
                remote_video_data_cb_(pszStreamID, remote_rgb_data_buf_.get(), remote_rgb_data_len_, remote_width_, remote_height_);
            }            
        }
    }
}
