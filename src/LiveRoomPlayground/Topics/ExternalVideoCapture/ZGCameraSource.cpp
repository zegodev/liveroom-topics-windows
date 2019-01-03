#include "stdafx.h"
#include "ZGCameraSource.h"

#include "AppSupport/ZGLog.h"

using namespace std::placeholders;
ZGCameraSource::ZGCameraSource(string camera_id):
    camera_id_(camera_id)
{
    // 设置缓冲大小
    capture_video_data_vec_.resize(MAX_VIDEO_BUF_LEN);

    // 设置摄像头模块数据回调
    zego_camera_capture_.SetVideoFrameCallBack(std::bind(&ZGCameraSource::OnVideoData, this, _1, _2, _3, _4, _5));
    // 设置摄像头模块错误回调
    zego_camera_capture_.SetCaptureDeviceErrorCallBack(std::bind(&ZGCameraSource::OnCameraError, this, _1, _2));

}

ZGCameraSource::~ZGCameraSource()
{
    // 停止摄像头模块采集
    zego_camera_capture_.StopCapture();
}

void ZGCameraSource::OnGetVideoData(std::shared_ptr<ZGExternalVideoData> & video_data)
{
    if (!have_start_captured_)
    {
        have_start_captured_ = true;

        // 设置摄像头id
        zego_camera_capture_.SetCamera(camera_id_);

        // 设置摄像头模块的分辨率
        zego_camera_capture_.SetCameraResolution(640, 480);

        // 设置摄像头模块的帧率
        zego_camera_capture_.SetFrameRate(capture_fps_);

        // 开始采集
        zego_camera_capture_.StartCapture();

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

// 摄像头采集到数据的回调，在这里放入到缓冲，等外部采集来消费
void ZGCameraSource::OnVideoData(const char* data, int length, const AVE::VideoCaptureFormat& frame_format, unsigned long long reference_time, unsigned int reference_time_scale)
{
    std::shared_ptr<ZGExternalVideoData> external_data = std::make_shared<ZGExternalVideoData>();

    // 创建缓冲
    external_data->data = std::unique_ptr<uint8_t[] >(new uint8_t[length]);

    // 拷贝摄像头采集的数据
    memcpy(external_data->data.get(), data, length);

    // 设置格式
    external_data->fomat = frame_format;

    // 数据长度
    external_data->len = length;

    // 时间戳
    external_data->reference_time = reference_time;

    // 保存到临时缓冲
    capture_video_data_vec_[write_index_] = external_data;

    // 写索引增加
    write_index_ = (write_index_ + 1) % MAX_VIDEO_BUF_LEN;
    if (write_index_ == read_index_)
    {
        read_index_ = (read_index_ + 1) % MAX_VIDEO_BUF_LEN;
    }
}

void ZGCameraSource::OnCameraError(const char *deviceId, const char *reason)
{
    ZGLog("OnCameraError %s , %s", deviceId, reason);
}
