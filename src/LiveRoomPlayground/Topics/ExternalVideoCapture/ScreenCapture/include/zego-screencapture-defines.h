#ifndef ZEGO_SCREENCAPTURE_DEFINES_H_
#define ZEGO_SCREENCAPTURE_DEFINES_H_

#include <stdint.h>

#ifdef WIN32
    #ifdef SCREENCAPTURE_EXPORTS
        #define SCREENCAPTURE_API __declspec(dllexport)
    #else
        #define SCREENCAPTURE_API __declspec(dllimport)
    #endif
#endif

#ifdef __cplusplus
extern "C" {
#endif

enum ZegoScreenCaptureLogLevel
{
    kZegoLogLevelGrievous = 0,
    kZegoLogLevelError = 1,
    kZegoLogLevelWarning = 2,
    kZegoLogLevelGeneric = 3,    ///< 通常在发布产品中使用
    kZegoLogLevelDebug = 4       ///< 调试阶段使用
};

enum ZegoScreenCaptureCaptureError
{
    kZegoCaptureErrorUnexpected = 1,
    kZegoCaptureErrorTargetUnspecified = 2,		///< 采集目标未指定
    kZegoCaptureErrorTargetInvalid = 3, 			///< 采集目标失效，比如显示器被拔掉、窗口被关闭
    kZegoCaptureErrorCaptureFunctionFailed = 4,
};

/// \brief 采集错误回调
/// \param error 错误代码
/// \param user_data 用户自定义数据
/// \see ZegoScreenCaptureCaptureError
typedef void (*zego_screencapture_capture_error_notify_func)(enum ZegoScreenCaptureCaptureError error, void *data);

struct ZegoScreenCaptureScreenItem
{
    char name[32];
    int is_primary;
};

struct ZegoScreenCaptureWindowItem
{
    void* handle;
    char title[256];
    char image_path[256];
};

/// \brief 采集目标窗口位置、大小发生改变（\see zego_screencapture_start_capture 之后生效）
/// \param handle 窗口句柄（windows）
/// \param left 窗口原点坐标（左上角）横坐标
/// \param top 窗口原点坐标（左上角）纵坐标
/// \param width 窗口宽度
/// \param height 窗口高度
/// \param user_data 用户自定义数据
typedef void (*zego_screencapture_captured_window_moved_notify_func)(void *handle, int left, int top, int width, int height, void *user_data);

enum ZegoScreenCaptureVideoPixelFormat
{
    kZegoPixelFormatUnknown = 0,
    kZegoPixelFormatI420 = 1,
    kZegoPixelFormatNV12 = 2,
    kZegoPixelFormatNV21 = 3,
    kZegoPixelFormatBGRA32 = 4,
    kZegoPixelFormatRGBA32 = 5,
};

struct ZegoScreenCaptureVideoCaptureFormat
{
    int width;
    int height;
    int strides[4];
    int rotation;
    enum ZegoScreenCaptureVideoPixelFormat video_pixel_format;
};

/// \brief 采集数据回调
/// \param data, 每一帧截图元数据
/// \param length, 元数据大小
/// \param video_frame_format，帧信息: format、长宽等
/// \param reference_time，时间戳
/// \param reference_time_scale, 时间戳单位，如果是ms则是1000
/// \param user_data 用户自定义数据
/// \see ZegoScreenCaptureVideoCaptureFormat
typedef void (*zego_screencapture_captured_frame_available_notify_func)(const char *data, uint32_t length, const struct ZegoScreenCaptureVideoCaptureFormat *video_frame_format, uint64_t reference_time, uint32_t reference_time_scale, void *user_data);

#ifdef __cplusplus
}
#endif

#endif