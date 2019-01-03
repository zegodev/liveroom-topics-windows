#include "stdafx.h"
#include "ZGVideoRender.h"

#include "AppSupport/ZGLog.h"

ZGVideoRender::ZGVideoRender()
{
}

ZGVideoRender::~ZGVideoRender()
{
}

// 初始化显示的view
void ZGVideoRender::InitVideoHwnd(HWND hwnd)
{
    hwnd_ = hwnd;
}

// 更新视频数据
void ZGVideoRender::UpdateVideoData(uint8_t *data, uint32_t len, int width, int height)
{
    int data_len = width * height * 4;
    if (width != width_ || height_ != height)
    {
        {
            std::lock_guard<std::mutex> lock(buf_mutex_);
            rgb_data_buf_ = std::unique_ptr<uint8_t[] >(new uint8_t[data_len]);
        }

        ZeroMemory(&bitmap_info_, sizeof(bitmap_info_));
        bitmap_info_.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
        bitmap_info_.bmiHeader.biPlanes = 1;
        bitmap_info_.bmiHeader.biBitCount = 32;
        bitmap_info_.bmiHeader.biCompression = BI_RGB;
        bitmap_info_.bmiHeader.biWidth = width;
        bitmap_info_.bmiHeader.biHeight = -height;
        bitmap_info_.bmiHeader.biSizeImage = width * height * 4;
        
        width_ = width;
        height_ = height;
    }

    {
        std::lock_guard<std::mutex> lock(buf_mutex_);
        if (rgb_data_buf_ != nullptr)
        {
            memcpy(rgb_data_buf_.get(), data, data_len);
        }
    }
}

void ZGVideoRender::OnPaintVideo()
{
    //ZGENTER_FUN_LOG;
    if (rgb_data_buf_ == nullptr)
    {
        return;
    }

    CRect rc;
    GetClientRect(hwnd_, &rc);

    HDC hDc = CWnd::FromHandle(hwnd_)->GetWindowDC()->GetSafeHdc();

    // 创建兼容bitmap
    HBITMAP bmp_mem = ::CreateCompatibleBitmap(hDc, rc.right - rc.left, rc.bottom - rc.top);

    // 创建兼容dc
    HDC dc_mem = ::CreateCompatibleDC(hDc);
    ::SetStretchBltMode(dc_mem, HALFTONE);
    // 选择当前兼容bitmap，准备绘制
    HGDIOBJ bmp_old = ::SelectObject(dc_mem, bmp_mem);

    // 绘制背景到兼容内存dc
    HBRUSH brush = ::CreateSolidBrush(video_bk_color_);
     ::FillRect(dc_mem, CRect(0, 0, rc.right - rc.left, rc.bottom - rc.top), brush);
     ::DeleteObject(brush);

    {
         // 绘制图形到内存兼容dc
        std::lock_guard<std::mutex> lock(buf_mutex_);
        StretchDIBits(dc_mem, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, 0, 0, width_, height_, rgb_data_buf_.get(), &bitmap_info_, DIB_RGB_COLORS, SRCCOPY);
    }
    // 从内存兼容dc到dc拷贝绘制
    BitBlt(hDc, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, dc_mem, 0, 0,SRCCOPY);

    // 释放资源
    ::SelectObject(dc_mem, bmp_old);
    ::DeleteObject(bmp_mem);
    ::DeleteDC(dc_mem);
}

