#ifndef ZEGO_SCREENRECORDER_H_
#define ZEGO_SCREENRECORDER_H_

#include <stdint.h>

#ifdef WIN32
#ifdef SCREENRECORDER_EXPORTS
#define SCREENRECORDER_API __declspec(dllexport)
#else
#define SCREENRECORDER_API __declspec(dllimport)
#endif
#endif

#ifdef __cplusplus
extern "C" {
#endif

/// \brief 创建ZegoScreenRecorder进程
/// \param szRecorderPath ZegoScreenRecorder.exe文件全路径
SCREENRECORDER_API void zego_screen_recorder_open(const char *szRecorderPath);

/// \brief 关闭ZegoScreenRecorder进程
SCREENRECORDER_API void zego_screen_recorder_close(void);

/// \brief 设置ZegoScreenRecorder初始化参数
/// \param szBuffer 初始化参数
SCREENRECORDER_API void zego_screen_recorder_init(const char *szBuffer);

/// \brief 设置房间ID
/// \param szRoomId 房间ID
SCREENRECORDER_API void zego_screen_recorder_set_room_id(const char *szRoomId);

/// \brief 设置用户ID
/// \param szUserId 用户ID
SCREENRECORDER_API void zego_screen_recorder_set_user_id(const char *szUserId);

/// \brief 设置要捕获的屏幕名称
/// \param szScreen 要捕获的屏幕名称
SCREENRECORDER_API void zego_screen_recorder_set_target_screen(const char *szScreen);

/// \brief 设置要捕获的窗口句柄
/// \param hWnd 要捕获的窗口句柄
SCREENRECORDER_API void zego_screen_recorder_set_target_window(uintptr_t hWnd);

/// \brief 设置要捕获的矩形范围
/// \param left 要捕获矩形范围顶点横轴坐标
/// \param top 要捕获矩形范围定点纵轴坐标
/// \param width 要捕获矩形范围的宽度
/// \param height 要捕获矩形范围的高度
SCREENRECORDER_API void zego_screen_recorder_set_target_rect(int left, int top, int width, int height);

/// \brief 设置捕获时是否显示鼠标
/// \param visible 非0 捕获时显示鼠标；0，捕获时不显示鼠标
SCREENRECORDER_API void zego_screen_recorder_set_cursor_visible(int visible);

/// \brief 设置是否显示鼠标点击动画
/// \param enable TRUE，允许鼠标显示鼠标点击动画；0，不允许显示鼠标点击动画
SCREENRECORDER_API void zego_screen_recorder_enable_click_animation(int enable);

/// \brief 设置是否开启混入连麦声音
/// \param enable 非0，开启混入连麦声音；0，关闭混入连麦声音
SCREENRECORDER_API void zego_screen_recorder_enable_mix_playout(int enable);

/// \brief 设置推流分辨率
/// \param width 视频宽度
/// \param height 视频高度
SCREENRECORDER_API void zego_screen_recorder_set_video_encode_resolution(int width, int height);

/// \brief 设置推流帧率
/// \param fps 帧率
SCREENRECORDER_API void zego_screen_recorder_set_video_fps(int fps);

/// \brief 设置推流码率
/// \param bitrate 码率
SCREENRECORDER_API void zego_screen_recorder_set_video_bitrate(int bitrate);

/// \brief 设置推流模式
/// \param flag 推流模式
SCREENRECORDER_API void zego_screen_recorder_set_publish_flag(int flag);

/// \brief 开始推流
SCREENRECORDER_API void zego_screen_recorder_start_publish(void);

/// \brief 停止推流
SCREENRECORDER_API void zego_screen_recorder_stop_publish(void);

typedef void(*zego_screen_recorder_capture_error_notify_func)(int error, void *user_data);
/// \brief 注册屏幕捕获错误通知
/// \param notify 屏幕捕获错误通知函数
/// \param user_data 用户自定义数据，通知函数被调用时透传
SCREENRECORDER_API void zego_screen_recorder_reg_capture_error_notify(zego_screen_recorder_capture_error_notify_func notify, void *user_data);

typedef void(*zego_screen_recorder_publish_state_update_notify_func)(int state, const char *rtmp_url, const char *hls_url, void *user_data);
/// \brief 注册流更新通知
/// \param notify 屏幕捕获错误通知函数
/// \param user_data 用户自定义数据，通知函数被调用时透传
SCREENRECORDER_API void zego_screen_recorder_reg_publish_state_update_notify(zego_screen_recorder_publish_state_update_notify_func notify, void *user_data);

#ifdef __cplusplus
}
#endif

#endif
