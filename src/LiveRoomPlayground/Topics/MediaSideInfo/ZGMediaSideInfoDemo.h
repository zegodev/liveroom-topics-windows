//
// ZGMediaSideInfoDemo.h
//
// Copyright 2018年 Zego. All rights reserved.
// 

#ifndef ZGMediaSideInfoDemo_h__
#define ZGMediaSideInfoDemo_h__

#include <stdint.h>
#include <functional>
#include <string>

#include <ZegoLiveRoom/zego-api-media-side-info.h>

using std::string;
using namespace ZEGO;

typedef struct ZGMediaSideInfoDemoConfig
{
    // 是否纯音频推流,true-存语音，不传输视频数据。false - 音视频直播，传输视频数据。默认为false
    bool only_audio_publish = false;
    // 是否自定义打包。true-发送的数据已经打包好了，并且符合打包规范, false-数据由 SDK 打包,默认为false
    bool custom_packet = false;

}ZGMediaSideInfoDemoConfig;

typedef std::function<void(uint8_t * data, int len, string stream_id)> MediaSideInfoCallBackFunType;
typedef MediaSideInfoCallBackFunType UserDataCallBackFunType;

class ZGMediaSideInfoDemo : public MEDIASIDEINFO::IZegoMediaSideCallback
{
public:
    ZGMediaSideInfoDemo();
    ~ZGMediaSideInfoDemo();

    void SetMediaSideInfoCallBack(MediaSideInfoCallBackFunType cb);

    void SetUserDataCallBack(UserDataCallBackFunType cb);

    void InitSideInfoForPlay();

    void InitSideInfoConfig(ZGMediaSideInfoDemoConfig config);

    void ActivateMediaSideInfoForPublishChannel(AV::PublishChannelIndex idx = AV::PUBLISH_CHN_MAIN);

    void SendMediaSideInfo(uint8_t * data, int len, AV::PublishChannelIndex idx = AV::PUBLISH_CHN_MAIN);

    virtual void onRecvMediaSideInfo(const char * pszStreamID, const unsigned char *pBuf, int dataLen) override;

protected:
    ZGMediaSideInfoDemoConfig config_;
    MediaSideInfoCallBackFunType side_info_cb_;
    UserDataCallBackFunType user_data_cb_;

};

#endif // ZGMediaSideInfoDemo_h__


