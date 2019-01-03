//
// ZGCameraSource.h
//
// Copyright 2018年 Zego. All rights reserved.
// 

#ifndef ZGCameraSource_h__
#define ZGCameraSource_h__

#include <string>
#include <mutex>
#include <vector>
#include <atomic>

#include "ZGExternalVideoSource.h"
#include "ZegoCamera.h"

using std::string;
using std::vector;

// 摄像头数据作为外部采集源的示例
class ZGCameraSource : public ZGExternalVideoSource
{
public:
    ZGCameraSource(string camera_id);
    ~ZGCameraSource();

    virtual void OnGetVideoData(std::shared_ptr<ZGExternalVideoData> & video_data) override;

private:

    // 摄像头采集到数据的回调，在这里放入到缓冲
    void OnVideoData(const char* data,
        int length,
        const AVE::VideoCaptureFormat& frame_format,
        unsigned long long reference_time,
        unsigned int reference_time_scale);

    // 摄像头错误回调
    void OnCameraError(const char *deviceId, const char *reason);

    // 摄像头采集模块
    ZegoVideoCapExternal zego_camera_capture_;

    // 摄像头id
    string camera_id_;

    // 摄像头数据缓冲
    vector<std::shared_ptr<ZGExternalVideoData> > capture_video_data_vec_;

    // 缓冲区大小设置，环形缓冲
    const int MAX_VIDEO_BUF_LEN = 10;

    // 写索引位置
    std::atomic<int> write_index_ = 0;
    
    // 读索引位置
    std::atomic<int> read_index_ = 0;
    
    // 摄像头采集是否开启
    bool have_start_captured_ = false;
    // 设置摄像头模块的采集帧率
    int capture_fps_ = 25;

};


#endif // ZGCameraSource_h__


