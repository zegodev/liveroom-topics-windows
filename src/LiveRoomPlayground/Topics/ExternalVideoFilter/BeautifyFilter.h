//
// BeautifyFilter.h
//
// Copyright 2019年 Zego. All rights reserved.
// 

#ifndef BeautifyFilter_h__
#define BeautifyFilter_h__

#include "VideoFilterProcessBase.h"

class TILFilter;

// tencent 美颜接口实现
class BeautifyFilter : public VideoFilterProcessBase
{
public:
    BeautifyFilter();
    ~BeautifyFilter();

    // 初始化美颜滤镜
    virtual bool InitFilter() override;

    // 美颜处理YUV I420数据
    virtual void FilterProcessI420Data(unsigned char * data, int frame_len, int frame_w, int frame_h) override;

    // 更新美颜等级，范围[0-9]
    virtual void UpdateFilterLevel(int level) override;

protected:

    TILFilter* beauty_filter_ = nullptr;
    bool inited_ = false;

};

#endif // BeautifyFilter_h__


