#include "stdafx.h"
#include "ZGManager.h"



ZGManager::ZGManager():
    sdk_have_inited_(false)
{
}

ZGManager::~ZGManager()
{
}

void ZGManager::EnableExternalVideoCapture(AVE::VideoCaptureFactory* factory, LIVEROOM::IZegoVideoRenderCallback* renderer)
{
    LIVEROOM::SetVideoCaptureFactory(factory);
    if (renderer != nullptr)
    {
        //LIVEROOM::EnableExternalRender(true);
        LIVEROOM::SetExternalRenderCallback(renderer);
    }
    else {
        //LIVEROOM::EnableExternalRender(false);
    }

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
    bool ret = LIVEROOM::InitSDK(uiAppID, sign, len);
    sdk_have_inited_ = ret;
    return sdk_have_inited_;
}

void ZGManager::UninitSdk()
{
    LIVEROOM::UnInitSDK();
}

