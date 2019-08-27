#include "stdafx.h"
#include "ZGScreenRecorderDemo.h"

#include "AppSupport/ZGLog.h"
#include "AppSupport/ZGUtilTools.h"

ZGScreenRecorderDemo::ZGScreenRecorderDemo()
{
	screen_capture_manager_ = std::make_shared<ZGScreenCaptureManager>();
}


ZGScreenRecorderDemo::~ZGScreenRecorderDemo()
{
}

void ZGScreenRecorderDemo::SetDemoCallBack(ZGScreenRecorderDemoCallBack * cb)
{
	demo_cb_ = cb;
}

void ZGScreenRecorderDemo::StartRecord(ZGScreenRecorderDemoConfig config)
{
	if (config.record_file_path.empty()) {
		if (demo_cb_ != nullptr)
		{
			demo_cb_->OnScreenRecordStateUpdate("please select record file first");
		}
		return;
	}


	screen_capture_manager_->StartCapture();

	config_ = config;
	bool set_cb_result = MEDIA_RECORDER::SetZegoMediaRecordCallback(this);
	bool record_result = MEDIA_RECORDER::StartRecord(MEDIA_RECORDER::ZEGO_MEDIA_RECORD_CHN_AUX, 
		config_.record_type, config_.record_file_path.c_str(), true, 3000, config_.record_format);
	if (record_result)
	{
		ZGLog("StartRecord=> result(bool): %d", record_result);
	}
	else {
		StopRecord();
	}
}

void ZGScreenRecorderDemo::StopRecord()
{
	screen_capture_manager_->StopCapture();
	MEDIA_RECORDER::StopRecord(MEDIA_RECORDER::ZEGO_MEDIA_RECORD_CHN_AUX);
	if (demo_cb_ != nullptr)
	{
		demo_cb_->OnScreenRecordStateUpdate("record stopped");
	}
}

void ZGScreenRecorderDemo::OnMediaRecord(int errCode, MEDIA_RECORDER::ZegoMediaRecordChannelIndex channelIndex, const char* storagePath)
{
	ZGLog("OnMediaRecord=> errCode: %d", errCode);
	if (demo_cb_ != nullptr)
	{
		switch (errCode)
		{
		case 0:
			demo_cb_->OnScreenRecordStateUpdate("recording...");
			break;
		case 1:
			demo_cb_->OnScreenRecordStateUpdate("error path too long");
			break;
		case 2:
			demo_cb_->OnScreenRecordStateUpdate("error init failed");
			break;
		case 3:
			demo_cb_->OnScreenRecordStateUpdate("error open file failed");
			break;
		case 4:
			demo_cb_->OnScreenRecordStateUpdate("error write file failed");
			break;
		default:
			demo_cb_->OnScreenRecordStateUpdate("error: unknown");
			break;
		}
	}
}

void ZGScreenRecorderDemo::OnRecordStatusUpdate(const MEDIA_RECORDER::ZegoMediaRecordChannelIndex channelIndex, const char* storagePath, const unsigned int duration, const unsigned int fileSize)
{
	ZGLog("OnRecordStatusUpdate=> duration:%d; fileSize:%d", duration, fileSize);
	if (demo_cb_ != nullptr)
	{
		demo_cb_->OnScreenRecorderFileStatus(duration, fileSize);
	}
}
