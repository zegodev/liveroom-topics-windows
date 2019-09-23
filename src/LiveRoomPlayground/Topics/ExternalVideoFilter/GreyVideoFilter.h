//
// GreyVideoFilter.h
//
// Copyright 2019年 Zego. All rights reserved.
// 

#ifndef GreyVideoFilter_h__
#define GreyVideoFilter_h__

#include "VideoFilterProcessBase.h"

// 视频滤镜处理类
class GreyVideoFilter: public VideoFilterProcessBase
{
public:
    GreyVideoFilter();
    virtual ~GreyVideoFilter();

    // 初始化滤镜
    virtual bool InitFilter(){ return true;};

    // 滤镜处理RGB数据
	virtual void FilterProcessRGBAData(unsigned char * data, int frame_len, int frame_w, int frame_h);

    // 刷新滤镜处理等级，例如美颜等级
    virtual void UpdateFilterLevel(int level){}

};

#endif // GreyVideoFilter_h__
