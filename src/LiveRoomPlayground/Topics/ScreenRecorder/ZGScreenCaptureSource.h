#pragma once

#include <vector>
#include <atomic>
#include <stdint.h>
#include <memory>
#include "ZegoLiveRoom/video_capture.h"
#include "Topics/ScreenRecorder/ScreenCapture/include/zego-screencapture.h"
using namespace AVE;
using std::vector;

// 外部采集的视频数据结构
typedef struct ZGScreenVideoData
{
	// std::unique_ptr<uint8_t[]> data(new uint8_t[10]);
	// this will correctly call delete[]
	std::unique_ptr<uint8_t[] > data;      // 外部采集的video data数组，比如new 10个长度的数组，std::unique_ptr<uint8_t[]> data(new uint8_t[10])，指针首地址data.get()
	uint32_t len = 0;                      // 数据长度
	VideoCaptureFormat fomat;              // 格式 RGBA 还是BGRA等
	unsigned long long reference_time = 0; // 时间戳

}ZGScreenVideoData;

class ZGScreenCaptureSource
{
public:
	ZGScreenCaptureSource();
	~ZGScreenCaptureSource();

	void OpenSource();
	void CloseSource();

	// 获取捕获到的屏幕数据
	void OnGetVideoData(std::shared_ptr<ZGScreenVideoData> & video_data);

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
	vector<std::shared_ptr<ZGScreenVideoData> > capture_video_data_vec_;

	// 缓冲区大小设置，环形缓冲
	const int MAX_VIDEO_BUF_LEN = 10;

	// 写索引位置
	std::atomic<int> write_index_ = 0;

	// 读索引位置
	std::atomic<int> read_index_ = 0;

	// 采集是否开启
	bool have_start_captured_ = false;
};