#include "stdafx.h"
#include "ZGMediaSideInfoDemo.h"

#include <vector>

#include "AppSupport/ZGLog.h"

ZGMediaSideInfoDemo::ZGMediaSideInfoDemo()
{
}

ZGMediaSideInfoDemo::~ZGMediaSideInfoDemo()
{
}

void ZGMediaSideInfoDemo::SetMediaSideInfoCallBack(MediaSideInfoCallBackFunType cb)
{
    side_info_cb_ = cb;
}

void ZGMediaSideInfoDemo::SetUserDataCallBack(UserDataCallBackFunType cb)
{
    user_data_cb_ = cb;
}

void ZGMediaSideInfoDemo::InitSideInfoForPlay()
{
    // 初始化sdk之后，拉流前调用
    MEDIASIDEINFO::SetMediaSideCallback(this);
}

void ZGMediaSideInfoDemo::InitSideInfoConfig(ZGMediaSideInfoDemoConfig config)
{
    config_ = config;
}

void ZGMediaSideInfoDemo::ActivateMediaSideInfoForPublishChannel(AV::PublishChannelIndex idx /*= AV::PUBLISH_CHN_MAIN*/)
{
    MEDIASIDEINFO::SetMediaSideFlags(true, config_.only_audio_publish, idx);
    MEDIASIDEINFO::SetMediaSideCallback(this);
}

void ZGMediaSideInfoDemo::SendMediaSideInfo(uint8_t * data, int len, AV::PublishChannelIndex idx /*= AV::PUBLISH_CHN_MAIN*/)
{
    ZGENTER_FUN_LOG;
    std::vector<uint8_t> data_to_send_vec;
    if (config_.custom_packet)
    {
        char header_buffer[5] = { 0 };
        // * packet length: 4 bytes
        uint32_t* length = (uint32_t*)header_buffer;
        // * need to convert to network order (big-endian)
        *length = htonl(len + 1);
        // * packet NAL type
        header_buffer[4] = 26; // [26, 31)
        for (int i = 0;i < 5; ++i)
        {
            data_to_send_vec.push_back(header_buffer[i]);
        }
    }

    for (int i = 0; i < len; ++i)
    {
        data_to_send_vec.push_back(data[i]);
    }
    ZGLog("begin send side info, len = %d", data_to_send_vec.size());
    MEDIASIDEINFO::SendMediaSideInfo(data_to_send_vec.data(), data_to_send_vec.size(), config_.custom_packet, idx);

    ZGLEAVE_FUN_LOG;
}

void ZGMediaSideInfoDemo::onRecvMediaSideInfo(const char * pszStreamID, const unsigned char *pBuf, int dataLen)
{
    ZGENTER_FUN_LOG;
    ZGLog("streamid = %s, data buf = %s , datalen = %d", pszStreamID, pBuf, dataLen);
    if (!side_info_cb_ && !user_data_cb_)
    {
        ZGLog("no side info call back, return");
        return;
    }
    uint32_t media_type = ntohl(*(uint32_t*)pBuf);

    if (media_type == 1001)
    {
        // SDK packet
        uint8_t * real_data = (uint8_t *)pBuf + 4;
        if (side_info_cb_)
        {
            side_info_cb_(real_data, dataLen - 4, pszStreamID);
        }
    }else if (media_type == 1002)
    {
        // mix stream user data
        uint8_t * real_data = (uint8_t *)pBuf + 4;
        if (user_data_cb_)
        {
            user_data_cb_(real_data, dataLen - 4, pszStreamID);
        }
	}
	else if (media_type == 1004) {
		// payload type = 5 的信息，即开启发送媒体次要信息开关时 nMediaInfoType 参数设置为 SEI_USER_UNREGISTED
		uint8_t * real_data = (uint8_t *)pBuf + 4;
		if (side_info_cb_)
		{
			side_info_cb_(real_data, dataLen - 4, pszStreamID);
		}
	}
    else {
        // * custom packet (1000)

        /* custom packet format
         * +--------+--------+--------+--------+--------+----------------------+
         * |        |        |        |        |        |                      |
         * |             MediaType             |NALTYPE |       DATA...        |
         * |        |     4 Bytes     |        | 1 Byte |                      |
         * +--------+-----------------+--------+--------+----------------------+
         */
        char nal_type = *((char*)pBuf + 4);
        ZGLog("media tpe:%d, nal:%d", media_type, (int)nal_type);
        uint8_t * real_data = (uint8_t *)pBuf + 5;
        if (side_info_cb_)
        {
            side_info_cb_(real_data, dataLen - 5, pszStreamID);
        }
    }

    ZGLEAVE_FUN_LOG;
}

