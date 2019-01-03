//
// ZGImageSource.h
//
// Copyright 2018年 Zego. All rights reserved.
// 

#ifndef ZGImageSource_h__
#define ZGImageSource_h__

#include <string>
#include <mutex>

#include "ZGExternalVideoSource.h"

using std::string;

// 外部采集图片源示例
class ZGImageSource : public ZGExternalVideoSource
{
public:
    ZGImageSource(string img_path);
    ~ZGImageSource();

    virtual void OnGetVideoData(std::shared_ptr<ZGExternalVideoData> & video_data) override;

private:
    // bmp图像路径
    string image_path_;
    // 图像数据buf
    unsigned char * img_buf_ = nullptr;
    int width_ = 0;
    int height_ = 0;
    std::mutex buf_mutex_;
    int pixel_byte_count_ = 3;//  RGB图像数据源一个像素几个字节，例如：数据源 RGB 时为3， 数据源为RGBA时为4
};


#endif // ZGImageSource_h__

