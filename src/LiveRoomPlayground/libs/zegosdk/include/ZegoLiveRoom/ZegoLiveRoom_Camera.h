#pragma once

#if defined(WIN32)

#include "video_capture.h"
#include "RoomDefines.h"



namespace ZEGO
{
    namespace CAMERA
    {
        ZEGO_API AVE::Camera* CreateCamera();

        ZEGO_API void DestroyCamera(AVE::Camera *pCamera);
    }
}

#endif
