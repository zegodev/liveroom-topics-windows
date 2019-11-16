// FuBeautifyFilter.h
//
// Copyright 2019年 Zego. All rights reserved.
// 

#ifndef FUBeautifyFilter_h__
#define FUBeautifyFilter_h__

#include <vector>

#include "VideoFilterProcessBase.h"


// fu美颜接口实现
class FuBeautifyFilter : public VideoFilterProcessBase
{
public:
    FuBeautifyFilter();
    ~FuBeautifyFilter();

    // 初始化美颜滤镜
    virtual bool InitFilter() override;

    // 美颜处理YUV I420数据
    virtual void FilterProcessI420Data(unsigned char * data, int frame_len, int frame_w, int frame_h) override;

    // 更新美颜等级，范围[0-9]
    virtual void UpdateFilterLevel(int level) override;

    virtual void FilterProcessRGBAData(unsigned char * data, int frame_len, int frame_w, int frame_h) override;


    virtual void Release() override;

protected:

    bool InitOpenGL();

    bool InitFuSdk();

    bool LoadFuResource();

    bool inited_ = false;

    int frame_id_ = 0;

    int beauty_handles_ = 0;
    std::vector<int> handles_;

    HGLRC hglrc_ = nullptr;

};

#endif // FUBeautifyFilter_h__


