#include "stdafx.h"
#include "ZGManager.h"

#include "AppSupport/AppIDHelper.h"

ZGManager::ZGManager():
    sdk_have_inited_(false)
{
}

ZGManager::~ZGManager()
{
}

void ZGManager::EnableExternalVideoCapture(AVE::VideoCaptureFactory* factory, LIVEROOM::IZegoVideoRenderCallback* renderer)
{
    if (sdk_have_inited_)
    {
        UninitSdk();
    }

    LIVEROOM::SetVideoCaptureFactory(factory);
    if (renderer != nullptr)
    {
        LIVEROOM::EnableExternalRender(true);
        LIVEROOM::SetExternalRenderCallback(renderer);
    }
    else {
        LIVEROOM::EnableExternalRender(false);
    }
    InitSdk();
}

void ZGManager::EnableMic(bool enableMic)
{
    LIVEROOM::EnableMic(enableMic);
}

bool ZGManager::InitSdk(unsigned int uiAppID, unsigned char * sign, int len)
{
    if (sdk_have_inited_)
    {
        UninitSdk();
    }
    LIVEROOM::SetLogDir("");
    bool ret = LIVEROOM::InitSDK(uiAppID, sign, len);
    sdk_have_inited_ = ret;
    return sdk_have_inited_;
}

bool ZGManager::InitSdk()
{
    unsigned int appid = GetAppId();
    int sign_len = 0;
    unsigned char * sign_data = GetAppSign(sign_len);

    return InitSdk(appid, sign_data, sign_len);
}

void ZGManager::UninitSdk()
{
    LIVEROOM::UnInitSDK();
    sdk_have_inited_ = false;
}

bool ZGManager::SdkIsInited()
{
    return sdk_have_inited_;
}


