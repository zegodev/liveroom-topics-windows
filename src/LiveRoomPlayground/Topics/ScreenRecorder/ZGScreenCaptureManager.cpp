#include "stdafx.h"
#include "ZGScreenCaptureManager.h"

#include <chrono>

#include "ZegoLiveRoom/LiveRoom.h"
#include "ZegoLiveRoom/LiveRoom-Publisher.h"
#include "AppSupport/ZGLog.h"
#include "AppSupport/ZGTimer.h"

using namespace ZEGO;

ZGScreenCaptureManager::ZGScreenCaptureManager()
{
	device_ = std::make_shared<ZGScreenCaptureDevice>();
	source_ = std::make_shared<ZGScreenCaptureSource>();

	device_->AttachDevice();
}

ZGScreenCaptureManager::~ZGScreenCaptureManager()
{

}

void ZGScreenCaptureManager::StartCapture()
{
	device_->OpenDevice();
	source_->OpenSource();
	StartScreenCaptureThread();
}

void ZGScreenCaptureManager::StopCapture()
{
	StopScreenCaptureThread();
	source_->CloseSource();
	device_->CloseDevice();
}

void ZGScreenCaptureManager::InitManager()
{
	
}

void ZGScreenCaptureManager::StartScreenCaptureThread()
{
	if (stoped_)
	{
		stoped_ = false;
		// 创建并启动外部采集线程
		capture_thread_ = std::thread(std::bind(&ZGScreenCaptureManager::CaptureVideoDataThread, this));
	}
}

void ZGScreenCaptureManager::StopScreenCaptureThread()
{
	if (!stoped_)
	{
		stoped_ = true;
		capture_thread_.join();
	}
}

void ZGScreenCaptureManager::CaptureVideoDataThread()
{
	ZGENTER_FUN_LOG;
	bool set_ret = LIVEROOM::SetVideoFPS(capture_fps_, AV::PUBLISH_CHN_AUX);
	if (set_ret)
	{
		ZGLog("set publish fps = %d ", capture_fps_);
	}
	else {
		ZGLog("set publish fps failed");
	}

	// 外部采集的loop，采集到数据推给sdk
	while (true)
	{
		ZGTimer timer;
		if (stoped_)
		{
			break;
		}


		{
			std::lock_guard<std::mutex> lock(external_source_mutex_);
			std::shared_ptr<ZGScreenVideoData> video_data;
			// 获取外部采集的数据
			source_->OnGetVideoData(video_data);
			if (video_data != nullptr)
			{
				unsigned long long reference_time = video_data->reference_time == 0 ? GetTimeStamp() : video_data->reference_time;
				// 推给sdk
				device_->PushExternalVideoData((const char *)video_data->data.get(), video_data->len, video_data->fomat, reference_time);
			}
	
		}

		// sleep 控制帧率
		int need_sleep_ms = 1000.0f / capture_fps_ - timer.ElapsedMs();
		std::this_thread::sleep_for(std::chrono::milliseconds(need_sleep_ms));
	}
	ZGLEAVE_FUN_LOG;

}

time_t ZGScreenCaptureManager::GetTimeStamp()
{
	std::chrono::time_point<std::chrono::system_clock, std::chrono::milliseconds> tp = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::system_clock::now());
	auto tmp = std::chrono::duration_cast<std::chrono::milliseconds>(tp.time_since_epoch());
	time_t timestamp = tmp.count();
	return timestamp;
}