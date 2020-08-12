#include "stdafx.h"
#include "ZGScreenCaptureDevice.h"

#include "ZegoLiveRoom/zego-api-external-video-capture.h"
#include "ZegoLiveRoom/zego-api-external-audio-device.h"

#include "AppSupport/ZGManager.h"
#include "AppSupport/ZGLog.h"

using namespace ZEGO;

ZGScreenCaptureDevice::ZGScreenCaptureDevice()
{

}

ZGScreenCaptureDevice::~ZGScreenCaptureDevice()
{

}

void ZGScreenCaptureDevice::AttachDevice()
{
	// 确保在InitSDK 之前调用  **第二路推流通道**
	VCAP::SetVideoCaptureFactory(this, AV::PUBLISH_CHN_AUX);

	// 确保在InitSDK 之前调用 选择辅助推流通道的音频采集源
	int ret = zego_external_audio_device_set_audio_src_for_auxiliary_publish_channel(kZegoAuxPublishChannelAudioSrcTypeSameAsMainPublishChannel);

}

void ZGScreenCaptureDevice::OpenDevice()
{
	ZGENTER_FUN_LOG;

	// 开麦克风
	//LIVEROOM::EnableMic(true);

	StartCapture();

	ZGLEAVE_FUN_LOG;
}

void ZGScreenCaptureDevice::CloseDevice()
{
	ZGENTER_FUN_LOG;

	StopCapture();

	// 开麦克风
	//LIVEROOM::EnableMic(true);

	ZGLEAVE_FUN_LOG;
}

void ZGScreenCaptureDevice::PushExternalVideoData(const char* data, int len, VideoCaptureFormat format, unsigned long long reference_time)
{
	//ZGENTER_FUN_LOG;
	{
		std::lock_guard<std::mutex> lock(capture_mutex_);
		if (capture_started_)
		{
			if (width_ != format.width || height_ != format.height)
			{
				width_ = format.width;
				height_ = format.height;

				ZGLog("SetVideoCaptureResolution and SetVideoEncodeResolution,w=%d h=%d", width_, height_);
				// 设置采集分辨率
				LIVEROOM::SetVideoCaptureResolution(width_, height_, AV::PUBLISH_CHN_AUX);
				// 设置编码分辨率
				LIVEROOM::SetVideoEncodeResolution(width_, height_, AV::PUBLISH_CHN_AUX);
				// 设置码率 
				LIVEROOM::SetVideoBitrate(3000 * 1000, AV::PUBLISH_CHN_AUX);
			}

			//ZGLog("push external video data to sdk , data len = %d", len);
			if (client_ != nullptr)
			{
				client_->OnIncomingCapturedData(data, len, format, reference_time, 1000);
			}
		}
	}
	//ZGLEAVE_FUN_LOG;
}

void ZGScreenCaptureDevice::AllocateAndStart(Client* client)
{
	ZGENTER_FUN_LOG;
	client_ = client;
	ZGLEAVE_FUN_LOG;
}

void ZGScreenCaptureDevice::StopAndDeAllocate()
{
	ZGENTER_FUN_LOG;
	client_->Destroy();
	client_ = nullptr;
	ZGLEAVE_FUN_LOG;
}

int ZGScreenCaptureDevice::StartCapture()
{
	ZGENTER_FUN_LOG;

	std::lock_guard<std::mutex> lock(capture_mutex_);
	capture_started_ = true;
	width_ = 0;
	height_ = 0;

	ZGLEAVE_FUN_LOG;
	return 0;
}

int ZGScreenCaptureDevice::StopCapture()
{
	ZGENTER_FUN_LOG;

	std::lock_guard<std::mutex> lock(capture_mutex_);
	capture_started_ = false;
	ZGLEAVE_FUN_LOG;
	return 0;
}


int ZGScreenCaptureDevice::SetFrameRate(int framerate)
{
	ZGENTER_FUN_LOG;
	return 0;
}

int ZGScreenCaptureDevice::SetResolution(int width, int height)
{
	ZGENTER_FUN_LOG;


	return 0;
}

int ZGScreenCaptureDevice::SetFrontCam(int bFront)
{
	ZGENTER_FUN_LOG;
	return 0;
}

int ZGScreenCaptureDevice::SetView(void *view)
{
	ZGENTER_FUN_LOG;
	return 0;
}

int ZGScreenCaptureDevice::SetViewMode(int nMode)
{
	ZGENTER_FUN_LOG;
	return 0;
}

int ZGScreenCaptureDevice::SetViewRotation(int nRotation)
{
	ZGENTER_FUN_LOG;
	return 0;
}

int ZGScreenCaptureDevice::SetCaptureRotation(int nRotation)
{
	ZGENTER_FUN_LOG;
	return 0;
}

int ZGScreenCaptureDevice::StartPreview()
{
	ZGENTER_FUN_LOG;
	return 0;
}

int ZGScreenCaptureDevice::StopPreview()
{
	ZGENTER_FUN_LOG;
	return 0;
}

int ZGScreenCaptureDevice::EnableTorch(bool bEnable)
{
	ZGENTER_FUN_LOG;
	return 0;
}

int ZGScreenCaptureDevice::TakeSnapshot()
{
	ZGENTER_FUN_LOG;
	return 0;
}

AVE::VideoCaptureDeviceBase* ZGScreenCaptureDevice::Create(const char* device_id)
{
	ZGENTER_FUN_LOG;

	return this;
}

void ZGScreenCaptureDevice::Destroy(VideoCaptureDeviceBase *vc)
{
	ZGENTER_FUN_LOG;

}
