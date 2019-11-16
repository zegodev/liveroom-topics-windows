#include "stdafx.h"
#include "ZGScreenSource.h"

#include "AppSupport/ZGLog.h"

#include "../../libs/libyuv/include/libyuv.h"

#ifdef _X86_
#pragma comment(lib, "./../Topics/ExternalVideoCapture/ScreenCapture/libs/x86/ZegoScreenCapture.lib")
#else
#pragma comment(lib, "./../Topics/ExternalVideoCapture/ScreenCapture/libs/x64/ZegoScreenCapture.lib")
#endif

#ifdef _DEBUG
#pragma comment(lib, "./../libs/libyuv/libs/libyuv_d.lib")
#else 
#pragma comment(lib, "./../libs/libyuv/libs/libyuv.lib")
#endif


ZGScreenSource::ZGScreenSource()
{
    capture_video_data_vec_.resize(MAX_VIDEO_BUF_LEN);

    zego_screencapture_reg_captured_frame_available_notify((zego_screencapture_captured_frame_available_notify_func)&ZGScreenSource::OnScreenCaptureData, this);
    zego_screencapture_reg_capture_error_notify(&ZGScreenSource::OnScreenCaptureError, this);
}

ZGScreenSource::~ZGScreenSource()
{
    if (have_start_captured_)
    {
        zego_screencapture_uninit();
        have_start_captured_ = false;
    }   
}

void ZGScreenSource::OnGetVideoData(std::shared_ptr<ZGExternalVideoData> & video_data)
{
    if (!have_start_captured_)
    {
        zego_screencapture_init();

        zego_screencapture_set_fps(screen_capture_fps_);

        zego_screencapture_set_target_window(nullptr);

        zego_screencapture_start_capture();
        
        have_start_captured_ = true;
        
        video_data = nullptr;
        return;
    }

    if (read_index_ != write_index_)
    {
        // 从采集临时缓冲区读取数据给外部采集
        video_data = capture_video_data_vec_[read_index_];
        read_index_ = (read_index_ + 1) % MAX_VIDEO_BUF_LEN;
    }
}


void ZGScreenSource::OnScreenCaptureData(const char *data, uint32_t length, const struct ZegoScreenCaptureVideoCaptureFormat *video_frame_format, uint64_t reference_time, uint32_t reference_time_scale, void *user_data)
{
    ZGScreenSource * pthis = (ZGScreenSource*)user_data;

    if (video_frame_format->video_pixel_format == kZegoPixelFormatBGRA32)
    {

        std::shared_ptr<ZGExternalVideoData> external_data = std::make_shared<ZGExternalVideoData>();

        int new_width = pthis->width_;
        int new_height = pthis->height_;
        int new_len = new_width * new_height * 4;
        int dst_stride = new_width * 4;

        if (pthis->scale_)
        {
            new_width = pthis->width_;
            new_height = pthis->height_;
            new_len = new_width * new_height * 4;
            dst_stride = new_width * 4;
        }
        else {
            new_width = video_frame_format->width;
            new_height = video_frame_format->height;
            new_len = length;
            dst_stride = video_frame_format->strides[0];
        }

        // 创建缓冲
        external_data->data = std::unique_ptr<uint8_t[] >(new uint8_t[new_len]);

        if (pthis->scale_)
        {
            // 缩放到指定分辨率
            int ret = libyuv::ARGBScale((const uint8_t *)data, video_frame_format->strides[0],
                video_frame_format->width, video_frame_format->height,
                (uint8*)external_data->data.get(), dst_stride, new_width, new_height, libyuv::kFilterBox);
        }
        else {
            // 拷贝屏幕采集的数据
            memcpy(external_data->data.get(), data, length);
        }

        // 设置格式
        external_data->fomat.width = new_width;
        external_data->fomat.height = new_height;
        external_data->fomat.strides[0] = dst_stride;
        external_data->fomat.pixel_format = (VideoPixelFormat)video_frame_format->video_pixel_format;

        // 数据长度
        external_data->len = new_len;

        // 保存到临时缓冲
        pthis->capture_video_data_vec_[pthis->write_index_] = external_data;

        // 写索引增加
        pthis->write_index_ = (pthis->write_index_ + 1) % pthis->MAX_VIDEO_BUF_LEN;
        if (pthis->write_index_ == pthis->read_index_)
        {
            pthis->read_index_ = (pthis->read_index_ + 1) % pthis->MAX_VIDEO_BUF_LEN;
        }
    }
}

void ZGScreenSource::OnScreenCaptureError(enum ZegoScreenCaptureCaptureError error, void *data)
{
    ZGScreenSource * pthis = (ZGScreenSource*)data;

    ZGLog("ZGScreenSource::OnScreenCaptureError, %d", error);
}
