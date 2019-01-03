
//
// ZGScreenSource.h
//
// Copyright 2018年 Zego. All rights reserved.
// 

#ifndef ZGScreenSource_h__
#define ZGScreenSource_h__

#include <vector>
#include <atomic>

#include "ZGExternalVideoSource.h"

#include "Topics/ExternalVideoCapture/ScreenCapture/include/zego-screencapture.h"

using std::vector;

class ZGScreenSource : public ZGExternalVideoSource
{
public:
    ZGScreenSource();
    ~ZGScreenSource();

    virtual void OnGetVideoData(std::shared_ptr<ZGExternalVideoData> & video_data) override;

private:

    static void OnScreenCaptureData(const char *data, uint32_t length, const struct ZegoScreenCaptureVideoCaptureFormat *video_frame_format, uint64_t reference_time, uint32_t reference_time_scale, void *user_data);

    static void OnScreenCaptureError(enum ZegoScreenCaptureCaptureError error, void *data);

    // 设置屏幕采集帧率
    int screen_capture_fps_ = 10;

    // 使用缩放输出
    bool scale_ = true;

    // 设置屏幕采集输出分辨率宽
    int width_ = 1920;

    // 设置屏幕采集输出分辨率高
    int height_ = 1080;

    // 数据缓冲
    vector<std::shared_ptr<ZGExternalVideoData> > capture_video_data_vec_;

    // 缓冲区大小设置，环形缓冲
    const int MAX_VIDEO_BUF_LEN = 10;

    // 写索引位置
    std::atomic<int> write_index_ = 0;

    // 读索引位置
    std::atomic<int> read_index_ = 0;

    // 采集是否开启
    bool have_start_captured_ = false;
};


#endif // ZGScreenSource_h__

