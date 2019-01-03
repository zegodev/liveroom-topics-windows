//
// ZGVideoRender.h
//
// Copyright 2018年 Zego. All rights reserved.
// 

#ifndef ZGVideoRender_h__
#define ZGVideoRender_h__

#include <stdint.h>
#include <memory>
#include <mutex>

class ZGVideoRender
{
public:
    ZGVideoRender();
    ~ZGVideoRender();

    // 初始化要渲染的目标view
    void InitVideoHwnd(HWND hwnd);
    // 更新数据
    void UpdateVideoData(uint8_t *data, uint32_t len, int width, int height);
    // 执行渲染到目标view中
    void OnPaintVideo();

private:

    // 要渲染的目标hwnd
    HWND hwnd_ = nullptr;

    // bitmap信息
    BITMAPINFO bitmap_info_;

    // 图像rgb数据数组
    std::unique_ptr<uint8_t[] > rgb_data_buf_ = nullptr;
    
    // 宽
    int width_ = 0;
    
    // 高
    int height_ = 0;

    // 缓冲锁
    std::mutex buf_mutex_;

    // 视频背景颜色
    double video_bk_color_ = RGB(0, 0, 0);
};

#endif // ZGVideoRender_h__


