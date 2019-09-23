//
// ZGExternalVideoRenderDemo.h
//
// Copyright 2018Äê Zego. All rights reserved.
// 

#ifndef ZGExternalVideoRenderDemo_h__
#define ZGExternalVideoRenderDemo_h__

#include <memory>
#include <string>
#include <mutex>
#include <functional>

#include "ZegoLiveRoom/zego-api-external-video-render.h"
#include "ZegoLiveRoom/video_capture.h"

using std::string;

typedef std::function<void(string stream_id, const unsigned char *pData, int dataLen, int width, int height)>  ExternalVideoRenderDataCallBackType;

class ZGExternalVideoRenderDemo : 
    public ZEGO::EXTERNAL_RENDER::IZegoVideoRenderCallback,
	public ZEGO::EXTERNAL_RENDER::IZegoVideoDecodeCallback
{
public:
    ZGExternalVideoRenderDemo();
    ~ZGExternalVideoRenderDemo();

    void EnableExternalRender(ZEGO::EXTERNAL_RENDER::VideoRenderType type);

    void SetVideoDataCallBack(ExternalVideoRenderDataCallBackType cb_local, ExternalVideoRenderDataCallBackType cb_remote);

protected:

	virtual void OnVideoRenderCallback(unsigned char **pData, int* dataLen, const char* pszStreamID, int width, int height, int strides[4], AVE::VideoPixelFormat pixel_format) override;
	virtual void SetFlipMode(const char* pszStreamID, int mode) override;
	virtual void OnVideoDecodeCallback(const unsigned char* data, int length, const char* pszStreamID, const AVE::VideoCodecConfig& codec_config, bool b_keyframe, double reference_time_ms) override;


    ExternalVideoRenderDataCallBackType local_video_data_cb_;
    ExternalVideoRenderDataCallBackType remote_video_data_cb_;

    std::unique_ptr<uint8_t[] > local_rgb_data_buf_;
    std::unique_ptr<uint8_t[] > remote_rgb_data_buf_;
    int remote_width_ = 0;
    int remote_height_ = 0;
    int remote_rgb_data_len_ = 0;

    int width_ = 0;
    int height_ =0;
    int rgb_data_len_ = 0;

	ZEGO::EXTERNAL_RENDER::VideoRenderType cur_external_render_type_ = ZEGO::EXTERNAL_RENDER::VIDEO_RENDER_TYPE_RGB;
};


#endif // ZGExternalVideoRenderDemo_h__

