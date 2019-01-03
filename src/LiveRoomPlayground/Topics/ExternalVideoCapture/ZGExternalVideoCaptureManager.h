//
// ZGExternalVideoCaptureManager.h
//
// Copyright 2018年 Zego. All rights reserved.
// 

#ifndef ZGExternalVideoCaptureManager_h__
#define ZGExternalVideoCaptureManager_h__

#include <memory>
#include <string>
#include <thread>
#include <atomic>

#include "ZGExternalVideoSource.h"
#include "ZGExternalVideoCaptureDemo.h"
#include "ZGExternalVideoCatpureDemoHelper.h"

using std::string;

enum ZGExternalVideoSourceType
{
    ZGExternalVideoSource_Image,
    ZGExternalVideoSource_Camera,
    ZGExternalVideoSource_Screen
};

typedef struct ZGExternalVideoSourceParams
{
    string image_path; // for ZGExternalVideoSource_Image type
    string camera_id;  // fro ZGExternalVideoSource_Camera

}ZGExternalVideoSourceParams;

typedef std::function< void(std::shared_ptr<ZGExternalVideoData>)> VideoCallBackType;

class ZGExternalVideoCaptureManager
{
public:
    ZGExternalVideoCaptureManager();
    ~ZGExternalVideoCaptureManager();

    // 设置视频数据回调，采集到的数据的回调，在回调中可以用来渲染，开启外部采集需要自己渲染
    void SetVideoDataCallBack(VideoCallBackType cb);

    // 初始化外部采集，开启
    void InitExternalCapture();

    // 设置外采集源参数
    void SetExternalSourceParams(ZGExternalVideoSourceParams params);

    // 创建外部采集源
    void CreateExternalSource(ZGExternalVideoSourceType source_type);

    // 设置采集fps
    void SetCaptureFps(int fps);

    // 开始外部采集线程
    void StartExternalCaptureThread();

    // 停止外部采集线程
    void StopExternalCaptureThread();
    
private:

    // 外部采集线程函数
    void CaptureVideoDataThread();

    time_t GetTimeStamp();

    std::function< void(std::shared_ptr<ZGExternalVideoData>) > video_data_cb_;

    // 获取外部采集源数据的接口
    std::shared_ptr<ZGExternalVideoSource> external_source_;

    // 外部采集源参数
    ZGExternalVideoSourceParams source_params_;

    ZGExternalVideoCaptureDemo demo_;

    // 外部采集帧率设置和控制
    int capture_fps_ = 25;

    // 外部采集线程
    std::thread capture_thread_;

    // 外部是否停止
    std::atomic<bool> stoped_ = true;
};


#endif // ZGExternalVideoCaptureManager_h__

