//
// ZGExternalVideoSource.h
//
// Copyright 2018年 Zego. All rights reserved.
// 

#ifndef ZGExternalVideoSource_h__
#define ZGExternalVideoSource_h__

#include <stdint.h>
#include <memory>

#include "ZegoLiveRoom/video_capture.h"

using namespace AVE;

// 外部采集的视频数据结构
typedef struct ZGExternalVideoData
{
    // std::unique_ptr<uint8_t[]> data(new uint8_t[10]);
    // this will correctly call delete[]
    std::unique_ptr<uint8_t[] > data;      // 外部采集的video data数组，比如new 10个长度的数组，std::unique_ptr<uint8_t[]> data(new uint8_t[10])，指针首地址data.get()
    uint32_t len = 0;                      // 数据长度
    VideoCaptureFormat fomat;              // 格式 RGBA 还是BGRA等
    unsigned long long reference_time = 0; // 时间戳

}ZGExternalVideoData;

// 外部视频源接口，各种外部采集源的基类
class ZGExternalVideoSource
{
public:
    ZGExternalVideoSource();
    ~ZGExternalVideoSource();
    // 外部采集获取数据的接口
    virtual void OnGetVideoData(std::shared_ptr<ZGExternalVideoData> & video_data) = 0;
};

#endif // ZGExternalVideoSource_h__


