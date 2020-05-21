//
// ZGMediaSideInfoDemo.h
//
// Copyright 2018�� Zego. All rights reserved.
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
    // �Ƿ���Ƶ����,true-����������������Ƶ���ݡ�false - ����Ƶֱ����������Ƶ���ݡ�Ĭ��Ϊfalse
    bool only_audio_publish = true;
    // �Ƿ��Զ�������true-���͵������Ѿ�������ˣ����ҷ��ϴ���淶, false-������ SDK ���,Ĭ��Ϊfalse
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


