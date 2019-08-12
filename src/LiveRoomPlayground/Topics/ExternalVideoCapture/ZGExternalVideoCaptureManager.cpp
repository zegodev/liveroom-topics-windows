#include "stdafx.h"
#include "ZGExternalVideoCaptureManager.h"

#include <chrono>

#include "ZegoLiveRoom/LiveRoom-Publisher.h"

#include "AppSupport/ZGLog.h"
#include "AppSupport/ZGTimer.h"

#include "ZGImageSource.h"
#include "ZGCameraSource.h"
#include "ZGScreenSource.h"

ZGExternalVideoCaptureManager::ZGExternalVideoCaptureManager()
{
}

ZGExternalVideoCaptureManager::~ZGExternalVideoCaptureManager()
{
}

void ZGExternalVideoCaptureManager::SetVideoDataCallBack(VideoCallBackType cb)
{
    video_data_cb_ = cb;
}

void ZGExternalVideoCaptureManager::SetExternalSourceParams(ZGExternalVideoSourceParams params)
{
    source_params_ = params;
}

void ZGExternalVideoCaptureManager::CreateExternalSource(ZGExternalVideoSourceType source_type)
{
    std::lock_guard<std::mutex> lock(external_source_mutex_);
    switch (source_type)
    {
        // 图片源
    case ZGExternalVideoSource_Image:
        external_source_ = std::make_shared<ZGImageSource>(source_params_.image_path);
    	break;
    case ZGExternalVideoSource_Screen:
        external_source_ = std::make_shared<ZGScreenSource>();
        break;
        // 摄像头源
    case ZGExternalVideoSource_Camera:
        external_source_ = std::make_shared<ZGCameraSource>(source_params_.camera_id);
        break;
    default:
        break;
    }
}

void ZGExternalVideoCaptureManager::SetCaptureFps(int fps)
{
    capture_fps_ = fps;
}

void ZGExternalVideoCaptureManager::InitExternalCapture()
{
    // 开启外部采集
    demo_.EnableExternalVideoCapture();
}

void ZGExternalVideoCaptureManager::StartExternalCaptureThread()
{
    if (stoped_)
    {
        stoped_ = false;
        // 创建并启动外部采集线程
        capture_thread_ = std::thread(std::bind(&ZGExternalVideoCaptureManager::CaptureVideoDataThread, this));
    }
}

void ZGExternalVideoCaptureManager::StopExternalCaptureThread()
{
    // 停止采集线程
    if (!stoped_)
    {
        stoped_ = true;
        capture_thread_.join();
    }

}

void ZGExternalVideoCaptureManager::CaptureVideoDataThread()
{
    ZGENTER_FUN_LOG;
    // 设置sdk的帧率fps与外部采集的帧率一致
    bool set_ret = LIVEROOM::SetVideoFPS(capture_fps_);
    if (set_ret)
    {
        ZGLog("set publish fps = %d ", capture_fps_);
    }
    else {
        ZGLog("set publish fps failed");
    }

    // 外部采集的loop，采集到数据推给sdk，并回调输出一份来渲染
    while (true)
    {
        ZGTimer timer;
        if (stoped_)
        {
            break;
        }

        {
            std::lock_guard<std::mutex> lock(external_source_mutex_);
            if (external_source_)
            {
                std::shared_ptr<ZGExternalVideoData> video_data;
                // 获取外部采集的数据
                external_source_->OnGetVideoData(video_data);
                if (video_data != nullptr)
                {
                    unsigned long long reference_time = video_data->reference_time == 0 ? GetTimeStamp() : video_data->reference_time;
                    // 推给sdk
                    demo_.PushExternalVideoData((const char *)video_data->data.get(), video_data->len, video_data->fomat, reference_time);
                    // 回调渲染
                    if (video_data_cb_)
                    {
                        // for render
                        video_data_cb_(video_data);
                    }
                }
            }

        }

        // sleep 控制帧率
        int need_sleep_ms = 1000.0f / capture_fps_ - timer.ElapsedMs();
        std::this_thread::sleep_for(std::chrono::milliseconds(need_sleep_ms));
    }
    ZGLEAVE_FUN_LOG;
}

time_t ZGExternalVideoCaptureManager::GetTimeStamp()
{
    std::chrono::time_point<std::chrono::system_clock, std::chrono::milliseconds> tp = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::system_clock::now());
    auto tmp = std::chrono::duration_cast<std::chrono::milliseconds>(tp.time_since_epoch());
    time_t timestamp = tmp.count();
    return timestamp;
}
