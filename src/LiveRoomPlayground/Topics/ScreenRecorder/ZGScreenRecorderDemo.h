#pragma once

#include <string>
#include "ZegoLiveRoom/zego-api-media-recorder.h"
#include "ZGScreenCaptureManager.h"

using namespace ZEGO;
using std::string;

typedef struct ZGScreenRecorderDemoConfig
{
	MEDIA_RECORDER::ZegoMediaRecordType   record_type;
	MEDIA_RECORDER::ZegoMediaRecordFormat record_format;
	string record_file_path;

}ZGScreenRecorderDemoConfig;

class ZGScreenRecorderDemoCallBack
{
public:
	virtual ~ZGScreenRecorderDemoCallBack() {}
	virtual void OnScreenRecorderFileStatus(const unsigned int duration, const unsigned int file_size) {};
	virtual void OnScreenRecordStateUpdate(string record_state) {}
};

class ZGScreenRecorderDemo: public MEDIA_RECORDER::IZegoMediaRecordCallback
{
public:
	ZGScreenRecorderDemo();
	~ZGScreenRecorderDemo();

	void SetDemoCallBack(ZGScreenRecorderDemoCallBack * cb);

	void StartRecord(ZGScreenRecorderDemoConfig config);
	void StopRecord();

protected:
	virtual void OnMediaRecord(int errCode, MEDIA_RECORDER::ZegoMediaRecordChannelIndex channelIndex, const char* storagePath) override;
	virtual void OnRecordStatusUpdate(const MEDIA_RECORDER::ZegoMediaRecordChannelIndex channelIndex, const char* storagePath, const unsigned int duration, const unsigned int fileSize) override;

private:
	ZGScreenRecorderDemoConfig config_;
	std::shared_ptr<ZGScreenCaptureManager> screen_capture_manager_;
	ZGScreenRecorderDemoCallBack * demo_cb_ = nullptr;
};
