#ifndef ZEGO_SCREENCAPTURE_H_
#define ZEGO_SCREENCAPTURE_H_

#include "zego-screencapture-defines.h"

#ifdef __cplusplus
extern "C" {
#endif

/// \brief 初始化屏幕采集模块,非线程安全,与UnInit配对使用
SCREENCAPTURE_API void zego_screencapture_init(void);

/// \brief 反初始化屏幕采集模块,非线程安全,与Init配对使用
SCREENCAPTURE_API void zego_screencapture_uninit(void);

/// \brief 设置log等级和路径
/// \param log_level log等级
/// \param szLogDir log保存路径
/// \see ZegoScreenCaptureLogLevel
SCREENCAPTURE_API void zego_screencapture_set_log_level(enum ZegoScreenCaptureLogLevel log_level, const char *szLogDir);

/// \brief 开始采集指定的目标
SCREENCAPTURE_API void zego_screencapture_start_capture(void);

/// \brief 停止对目标的采集
SCREENCAPTURE_API void zego_screencapture_stop_capture(void);

/// \brief 是否正在采集
/// \return 正在采集返回非0值,否则返回0
SCREENCAPTURE_API int zego_screencapture_is_capturing(void);

/// \brief 设置采集回调的帧率
/// \param fps 有效范围5~30,默认为15
SCREENCAPTURE_API void zego_screencapture_set_fps(int fps);

/// \brief 指定窗口,在采集屏幕时将这些窗口过滤,不在画面中显示
/// \param handle_list 待过滤的窗口句柄列表,win7下指定了layered属性的窗口才能被过滤
/// \param count 窗口数量
/// \param add true 添加过滤窗口,false 移除过滤窗口
/// \note macOS下设置window setSharingType:NSWindowSharingNone 则会被过滤
SCREENCAPTURE_API void zego_screencapture_set_excluded_windows(ZegoWindowHandle* handle_list,int count,bool add);

/// \brief 设置时间戳偏移
/// \param offset 外部提供的时间戳偏移，默认为0
SCREENCAPTURE_API void zego_screencapture_set_timestamp_offset(uint64_t offset);

/// \brief 在采集目标为窗口的情况下,初次采集时,设置是否激活窗口提升至前台显示
/// \param active 是否激活窗口。非0值,激活窗口,否则不激活窗口,默认激活
SCREENCAPTURE_API void zego_screencapture_activate_window_when_capturing(int active);

/// \brief 指定采集目标为单个窗口
/// \param handle 窗口句柄,参考zego_screencapture_enum_window_list,如传入NULL/nullptr则采集主屏幕全屏
/// \return 设置成功返回非0值,否则返回0
/// \note 当目标窗口关闭时,将收到zego_screencapture_capture_error_notify_func回调
/// \see zego_screencapture_capture_error_notify_func
/// \see zego_screencapture_enum_window_list
SCREENCAPTURE_API int zego_screencapture_set_target_window(ZegoWindowHandle handle);

/// \brief 在采集目标为单个窗口的模式下,设置目标采集模式,比如是全部窗口内容还是窗口客户区内容
/// \param mode 窗口采集模式,默认采集整个窗口
/// \return 设置成功返回非0值,否则返回0
/// \note 在zego_screencapture_set_target_window后调用本接口进行设置,kZegoScreenCaptureWindowModelClient仅支持有限窗口
/// \see zego_screencapture_set_target_window
/// \see ZegoScreenCaptureWindowMode
SCREENCAPTURE_API int zego_screencapture_set_target_window_mode(enum ZegoScreenCaptureWindowMode mode);

/// \brief 获取指定窗口所在屏幕(macOS没有实现)
/// \param handle Windows平台为窗口句柄,可通过zego_screencapture_enum_window_list获取,如传入NULL/nullptr则返回主屏幕
/// \return 成功返回屏幕名称,失败返回NULL
/// \see zego_screencapture_enum_window_list
SCREENCAPTURE_API const char* zego_screencapture_get_window_screen(ZegoWindowHandle handle);

/// \brief 获取指定窗口相对于屏幕坐标
/// \param handle Windows平台为窗口句柄,可通过zego_screencapture_enum_window_list获取
/// \param rect 坐标矩形
/// \return 窗口无效或者窗口被最小化隐藏等坐标无效时返回false,获取到实际坐标返回true
SCREENCAPTURE_API bool zego_screencapture_get_window_rect(ZegoWindowHandle handle, ZegoRect* rect);

/// \brief 设置采集目标区域,单位为像素,支持高DPI及多屏坐标，左上角为坐标原点
/// \param screen 屏幕ID,在windows未使用,在mac_osx中为CGDirectDisplayID
/// \param left 相对左上角的横坐标
/// \param top 相对左上角纵坐标
/// \param width 目标区域宽
/// \param height 目标区域高
/// \return 设置成功返回非0值,否则返回0
SCREENCAPTURE_API int zego_screencapture_set_target_rect(ZegoScreenID screen, int left, int top, int width, int height);

/// \brief 设置采集目标为指定屏幕
/// \param screen 屏幕名,可通过zego_screencapture_enum_screen_list获取，如传入NULL/nullptr则采集虚拟桌面全屏
/// \return 设置成功返回非0值,否则返回0
/// \note 当目标屏幕失去连接时,将收到zego_screencapture_capture_error_notify_func回调
/// \see zego_screencapture_enum_screen_list
/// \see zego_screencapture_capture_error_notify_func
SCREENCAPTURE_API int zego_screencapture_set_target_screen(ZegoScreenID screen);

/// \brief 设置是否显示光标
/// \param visible 是否显示光标。非0值显示光标,否则不显示光标。
SCREENCAPTURE_API void zego_screencapture_set_cursor_visible(int visible);

/// \brief 设置是否显示鼠标点击动画
/// \param enable 是否显示鼠标点击动画。非0值显示鼠标点击动画,否则不显示鼠标点击动画。
SCREENCAPTURE_API void zego_screencapture_enable_click_animation(int enable);

/// \brief 获取虚拟桌面尺寸（多个屏幕合并后计算）,单位为像素,支持高DPI及多屏坐标,原点坐标为主屏左上角
/// \param left 虚拟桌面左上角横坐标
/// \param top 虚拟桌面左上角纵坐标
/// \param width 虚拟桌面宽
/// \param height 虚拟桌面高
SCREENCAPTURE_API void zego_screencapture_get_virtual_desktop_rect(int *left, int *top, int *width, int *height);

/// \brief 同步枚举屏幕列表,包括屏幕名,是否主屏
/// \param count 屏幕数量
/// \return 屏幕列表首地址,需配对调用zego_screencapture_free_screen_list释放
/// \see zego_screencapture_free_screen_list
SCREENCAPTURE_API const struct ZegoScreenCaptureScreenItem* zego_screencapture_enum_screen_list(uint32_t *count);

/// \brief 同步释放枚举到的屏幕列表
/// \param screen_list 枚举到的屏幕列表首地址
SCREENCAPTURE_API void zego_screencapture_free_screen_list(const struct ZegoScreenCaptureScreenItem *screen_list);

/// \brief 同步枚举窗口列表，包括窗口标题、窗口对应可执行文件名
/// \param isIncludeIconic 枚举时是否包括最小化的窗口。非0表示包括最小化窗口,否则不包括最小化窗口。
/// \param count 窗口数量
/// \return 窗口列表首地址,需配对调用zego_screencapture_free_window_list释放
/// \note 可以通过缩略图相关接口获取需要共享的窗口
/// \see zego_screencapture_free_window_list
SCREENCAPTURE_API const struct ZegoScreenCaptureWindowItem* zego_screencapture_enum_window_list(int isIncludeIconic, uint32_t *count);

/// \brief 同步释放枚举到的屏幕列表
/// \param window_list 枚举到的屏幕列表首地址
SCREENCAPTURE_API void zego_screencapture_free_window_list(const struct ZegoScreenCaptureWindowItem *window_list);

/// \brief 设置视频采集的格式
/// \param format 视频采集的格式
/// \see ZegoScreenCaptureVideoPixelFormat
SCREENCAPTURE_API void zego_screencapture_set_capture_video_pixel_format(enum ZegoScreenCaptureVideoPixelFormat format);

/// \brief 注册采集数据通知
/// \param notify 通知函数
/// \param data 用户自定义数据,回调时透传
SCREENCAPTURE_API void zego_screencapture_reg_captured_frame_available_notify(zego_screencapture_captured_frame_available_notify_func notify, void *data);

/// \brief 注册采集窗口移动通知(废弃 参考zego_screencapture_reg_captured_window_status_change_notify)
/// \param notify 通知函数
/// \param data 用户自定义数据,回调时透传
/// \note 采集目标窗口位置、大小发生改变（当采集目标为窗口时,zego_screencapture_start_capture之后生效）
/// \see zego_screencapture_start_capture
/// \see zego_screencapture_reg_captured_window_status_change_notify
SCREENCAPTURE_API void zego_screencapture_reg_captured_window_moved_notify(zego_screencapture_captured_window_moved_notify_func notify, void *data);

/// \brief 注册采集窗口状态变化通知
/// \param notify 通知函数
/// \param data 用户自定义数据,回调时透传
/// \note 采集目标窗口状态发生改变（当采集目标为窗口时,zego_screencapture_start_capture之后生效）
/// \see zego_screencapture_start_capture
SCREENCAPTURE_API void zego_screencapture_reg_captured_window_status_change_notify(zego_screencapture_captured_window_status_change_notify_func notify, void *data);

/// \brief 注册采集异常通知
/// \param notify 通知函数
/// \param data 用户自定义数据,回调时透传
SCREENCAPTURE_API void zego_screencapture_reg_capture_error_notify(zego_screencapture_capture_error_notify_func notify, void *data);

/// \brief 注册系统桌面组合开关通知(win7)
/// \param notify 通知函数
/// \param data 用户自定义数据,回调时透传
SCREENCAPTURE_API void zego_screencapture_reg_capture_dwm_composition_change_notify(zego_screencapture_capture_dwm_composition_change_notify_func notify, void *data);
#ifdef __cplusplus
}
#endif

#endif