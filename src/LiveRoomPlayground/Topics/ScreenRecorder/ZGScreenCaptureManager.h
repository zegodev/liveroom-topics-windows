#pragma once

#include <memory>
#include <string>
#include <thread>
#include <atomic>
#include <mutex>

#include "ZGScreenCaptureDevice.h"
#include "ZGScreenCaptureSource.h"

using std::string;


class ZGScreenCaptureManager
{
public:
	ZGScreenCaptureManager();
	~ZGScreenCaptureManager();

	void StartCapture();
	void StopCapture();

protected:
	// 初始化
	void InitManager();

	// 开始从采集源中读取数据给设备
	void StartScreenCaptureThread();
	// 停止从采集源中读取数据给设备
	void StopScreenCaptureThread();
	// 外部采集线程函数
	void CaptureVideoDataThread();

	time_t GetTimeStamp();

private:
	// 外部采集线程
	std::thread capture_thread_;

	// 外部是否停止
	std::atomic<bool> stoped_ = true;

	std::shared_ptr<ZGScreenCaptureDevice> device_;
	std::shared_ptr<ZGScreenCaptureSource> source_;

	// 外部采集帧率设置和控制
	int capture_fps_ = 10;
	std::mutex external_source_mutex_;

};