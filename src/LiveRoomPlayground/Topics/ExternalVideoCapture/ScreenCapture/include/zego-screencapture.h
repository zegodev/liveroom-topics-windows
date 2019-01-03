#ifndef ZEGO_SCREENCAPTURE_H_
#define ZEGO_SCREENCAPTURE_H_

#include "zego-screencapture-defines.h"

#ifdef __cplusplus
extern "C" {
#endif

/// \brief 初始化屏幕采集模块,非线程安全，与UnInit配对使用
SCREENCAPTURE_API void zego_screencapture_init(void);

/// \brief 反初始化屏幕采集模块，非线程安全，与Init配对使用
SCREENCAPTURE_API void zego_screencapture_uninit(void);

/// \brief 设置log等级和路径
/// \param log_level log等级
/// \param szLogDir log保存路径
/// \see ZegoScreenCaptureLogLevel
SCREENCAPTURE_API void zego_screencapture_set_log_level(enum ZegoScreenCaptureLogLevel log_level, const char *szLogDir);

/// \brief 开始采集指定屏幕内容
SCREENCAPTURE_API void zego_screencapture_start_capture(void);

/// \brief 停止采集指定屏幕内容
SCREENCAPTURE_API void zego_screencapture_stop_capture(void);

/// \brief 是否正在采集
/// \return 正在采集返回非0值，否则返回0
SCREENCAPTURE_API int zego_screencapture_is_capturing(void);

/// \brief 设置采集回调的帧率
/// \param nFPS 有效范围5~30，默认为15
SCREENCAPTURE_API void zego_screencapture_set_fps(int nFPS);

/// \brief 设置时间戳偏移
/// \param uOffset 外部提供的时间戳偏移，默认为0
SCREENCAPTURE_API void zego_screencapture_set_timestamp_offset(uint64_t uOffset);

/// \brief 设置采集目标窗口
/// \param handle Windows平台为窗口句柄，可通过 \see zego_screencapture_enum_window_list 获取，如传入NULL/nullptr则采集主屏幕全屏
/// \return 设置成功返回非0值， 否则返回0
/// \note 当目标窗口关闭时，将收到 \see zego_screencapture_capture_error_notify_func 回调
SCREENCAPTURE_API int zego_screencapture_set_target_window(void *handle);

/// \brief 设置采集目标区域，单位为像素，支持高DPI及多屏坐标
/// \param left 目标区域左上角横坐标
/// \param top 目标区域左上角纵坐标
/// \param width 目标区域宽
/// \param height 目标区域高
/// \return 设置成功返回非0值， 否则返回0
SCREENCAPTURE_API int zego_screencapture_set_target_rect(int left, int top, int width, int height);

/// \brief 设置采集目标屏幕
/// \param szScreenName 屏幕名，可通过 \see zego_screencapture_enum_screen_list 获取，如传入NULL/nullptr则采集虚拟桌面全屏
/// \return 设置成功返回非0值， 否则返回0
/// \note 当目标屏幕失去连接时，将收到 \see zego_screencapture_capture_error_notify_func 回调
SCREENCAPTURE_API int zego_screencapture_set_target_screen(const char *szScreenName);

/// \brief 设置是否显示光标
/// \param visible 是否显示光标。非0值，显示光标；否则不显示光标。
SCREENCAPTURE_API void zego_screencapture_set_cursor_visible(int visible);

/// \brief 设置是否显示鼠标点击动画
/// \param enable 是否显示鼠标点击动画。非0值，显示鼠标点击动画；否则不显示鼠标点击动画。
SCREENCAPTURE_API void zego_screencapture_enable_click_animation(int enable);

/// \brief 获取虚拟桌面大小，单位为像素，支持高DPI及多屏坐标
/// \param left 虚拟桌面左上角横坐标
/// \param top 虚拟桌面左上角纵坐标
/// \param width 虚拟桌面宽
/// \param height 虚拟桌面高
SCREENCAPTURE_API void zego_screencapture_get_virtual_desktop_rect(int *left, int *top, int *width, int *height);

/// \brief 同步枚举屏幕列表，包括屏幕名，是否主屏
/// \param uCount 屏幕数量
/// \return 屏幕列表首地址，需配对调用 \see zego_screencapture_free_screen_list 释放
SCREENCAPTURE_API const struct ZegoScreenCaptureScreenItem* zego_screencapture_enum_screen_list(uint32_t *uCount);

/// \brief 同步释放枚举到的屏幕列表
/// \param pScreenList 枚举到的屏幕列表首地址
SCREENCAPTURE_API void zego_screencapture_free_screen_list(const struct ZegoScreenCaptureScreenItem *pScreenList);

/// \brief 同步枚举窗口列表，包括窗口标题、窗口对应可执行文件名
/// \param isIncludeIconic 枚举时是否包括最小化的窗口。非0表示包括最小化窗口；否则不包括最小化窗口。
/// \param uCount 窗口数量
/// \return 窗口列表首地址，需配对调用 \see zego_screencapture_free_window_list 释放
SCREENCAPTURE_API const struct ZegoScreenCaptureWindowItem* zego_screencapture_enum_window_list(int isIncludeIconic, uint32_t *uCount);

/// \brief 同步释放枚举到的屏幕列表
/// \param pWindowList 枚举到的屏幕列表首地址
SCREENCAPTURE_API void zego_screencapture_free_window_list(const struct ZegoScreenCaptureWindowItem *pWindowList);

/// \brief 注册采集数据通知
/// \param notify 通知函数
/// \param data 用户自定义数据，回调时透传
SCREENCAPTURE_API void zego_screencapture_reg_captured_frame_available_notify(zego_screencapture_captured_frame_available_notify_func notify, void *data);

/// \brief 注册采集窗口移动通知
/// \param notify 通知函数
/// \param data 用户自定义数据，回调时透传
SCREENCAPTURE_API void zego_screencapture_reg_captured_window_moved_notify(zego_screencapture_captured_window_moved_notify_func notify, void *data);

/// \brief 注册采集异常通知
/// \param notify 通知函数
/// \param data 用户自定义数据，回调时透传
SCREENCAPTURE_API void zego_screencapture_reg_capture_error_notify(zego_screencapture_capture_error_notify_func notify, void *data);

#ifdef __cplusplus
}
#endif

#endif