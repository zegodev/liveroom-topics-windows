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
    public ZEGO::EXTERNAL_RENDER::IZegoExternalRenderCallback2
{
public:
    ZGExternalVideoRenderDemo();
    ~ZGExternalVideoRenderDemo();

    void EnableExternalRender(ZEGO::AV::VideoExternalRenderType type);

    void SetVideoDataCallBack(ExternalVideoRenderDataCallBackType cb_local, ExternalVideoRenderDataCallBackType cb_remote);

protected:

    virtual void OnVideoDataCallback2( unsigned char **pData, int* dataLen, const char* pszStreamID, int width, int height, int strides[4], AVE::VideoPixelFormat pixelFormat) override;

    virtual void OnVideoDataCallback(const unsigned char *pData, int dataLen, const char* pszStreamID, int width, int height, int strides[4]) override;

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

    ZEGO::AV::VideoExternalRenderType cur_external_render_type_ = ZEGO::AV::DECODE_RGB_SERIES;
};


#endif // ZGExternalVideoRenderDemo_h__

