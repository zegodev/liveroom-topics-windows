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

    // EnableExternalRenderҪ��InitSDK֮ǰ����
    // type ����Ϊ DECODE_RGB_SERIES ʱ����Ƶ�����ڻص� OnVideoDataCallback ��ȡ����ʽΪRGB��ʽϵ��
    // type ����Ϊ DECODE ʱ����Ƶ�����ڻص� OnVideoDataCallback2 ��ȡ������Ԥ����������Ƶ���ݸ�ʽΪRGBϵ�У���������ΪYUV�� PIXEL_FORMAT_I420 ��ʽ
    // type ����Ϊ DECODE_RENDER ʱ���ڲ���Ⱦ��Ч��ͬʱ����ͨ������EnableVideoRender�趨�Ƿ��׳���Ƶ���ݣ�Ĭ�ϲ��׳���Ƶ����
    // 
    //ZEGO::EXTERNAL_RENDER::EnableExternalRender(true, cur_external_render_type_);

    EXTERNAL_RENDER::SetVideoRenderType(EXTERNAL_RENDER::VIDEO_RENDER_TYPE_RGB);

    EXTERNAL_RENDER::SetVideoRenderCallback(this);

    //ZEGO::EXTERNAL_RENDER::SetExternalRenderCallback(this);

    

    // ʹ�ò��Ի�������������ʱ����Ҫ��ϵzego����֧���л�Ϊ��ʽ���������޸�Ϊ SetUseTestEnv(FALSE); ��ʾ������ʽ����
    LIVEROOM::SetUseTestEnv(TRUE);
    ZGManagerInstance()->InitSdk();

    if (cur_external_render_type_ == EXTERNAL_RENDER::VIDEO_RENDER_TYPE_EXTERNAL_INTERNAL_RGB)
    {
        // �����ڲ���Ⱦ��ͬʱ���ص���������Ƶ����
        // ������ǰ����
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

    // ͨ��pszStreamID �ж��� kZegoVideoDataMainPublishingStream ���� kZegoVideoDataAuxPublishingStream 
// kZegoVideoDataMainPublishingStream ��ʾ��ͨ����������Ƶ����
// kZegoVideoDataAuxPublishingStream ��ʾ��ͨ����������Ƶ����
// �����������֣���Ϊ
    if (strcmp(pszStreamID, ZEGO::EXTERNAL_RENDER::kZegoVideoDataMainPublishingStream) == 0)
    {
        // ������ͨ������
        if (pixel_format == AVE::PIXEL_FORMAT_I420 && pData != nullptr && pData[0] != nullptr && pData[1] != nullptr && pData[2] != nullptr)
        {
            // ��ȡyuv��������
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
                // yuvתrgb��Ҫ�Ŀռ����
                local_rgb_data_buf_ = std::unique_ptr<uint8_t[] >(new uint8_t[width_* height_ * 4]);
                rgb_data_len_ = width_ * height_ * 4;
            }

            // yuv תΪ rgb
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
        // ������ͨ������
    }
    else
    {
        if (pixel_format == AVE::PIXEL_FORMAT_I420 && pData != nullptr && pData[0] != nullptr && pData[1] != nullptr && pData[2] != nullptr)
        {
            // ��ȡyuv��������
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
                // yuvתrgb��Ҫ�Ŀռ����
                remote_rgb_data_buf_ = std::unique_ptr<uint8_t[] >(new uint8_t[remote_width_* remote_height_ * 4]);
                remote_rgb_data_len_ = remote_width_ * remote_height_ * 4;
            }

            // yuv תΪ rgb
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
//     // ͨ��pszStreamID �ж��� kZegoVideoDataMainPublishingStream ���� kZegoVideoDataAuxPublishingStream 
// // kZegoVideoDataMainPublishingStream ��ʾ��ͨ����������Ƶ����
// // kZegoVideoDataAuxPublishingStream ��ʾ��ͨ����������Ƶ����
// // �����������֣���Ϊ
//     if (strcmp(pszStreamID, ZEGO::EXTERNAL_RENDER::kZegoVideoDataMainPublishingStream) == 0)
//     {
//         // ������ͨ������
//         if (pixelFormat == AVE::PIXEL_FORMAT_I420 && pData != nullptr && pData[0] != nullptr && pData[1] != nullptr && pData[2] != nullptr)
//         {
//             // ��ȡyuv��������
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
//                 // yuvתrgb��Ҫ�Ŀռ����
//                 local_rgb_data_buf_ = std::unique_ptr<uint8_t[] >(new uint8_t[width_* height_ * 4]);
//                 rgb_data_len_ = width_ * height_ * 4;
//             }
// 
//             // yuv תΪ rgb
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
//         // ������ͨ������
//     }
//     else
//     {
//         if (pixelFormat == AVE::PIXEL_FORMAT_I420 && pData != nullptr && pData[0] != nullptr && pData[1] != nullptr && pData[2] != nullptr )
//         {
//             // ��ȡyuv��������
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
//                 // yuvתrgb��Ҫ�Ŀռ����
//                 remote_rgb_data_buf_ = std::unique_ptr<uint8_t[] >(new uint8_t[remote_width_* remote_height_ * 4]);
//                 remote_rgb_data_len_ = remote_width_ * remote_height_ * 4;
//             }
// 
//             // yuv תΪ rgb
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
//     // ͨ��pszStreamID �ж��� kZegoVideoDataMainPublishingStream ���� kZegoVideoDataAuxPublishingStream 
//     // kZegoVideoDataMainPublishingStream ��ʾ��ͨ����������Ƶ����
//     // kZegoVideoDataAuxPublishingStream ��ʾ��ͨ����������Ƶ����
//     // �����������֣���Ϊ��������
//     if (strcmp(pszStreamID, ZEGO::EXTERNAL_RENDER::kZegoVideoDataMainPublishingStream) == 0)
//     {
//         // ������ͨ������
//         if (local_video_data_cb_)
//         {
//             local_video_data_cb_(pszStreamID, pData, dataLen, width, height);
//         }        
//     }else if (strcmp(pszStreamID, ZEGO::EXTERNAL_RENDER::kZegoVideoDataAuxPublishingStream) == 0)
//     {
//         // ������ͨ������
//     }else
//     {
//         // ��������
//         if (remote_video_data_cb_)
//         {
//             remote_video_data_cb_(pszStreamID, pData, dataLen, width, height);
//         }
//     }
// }

