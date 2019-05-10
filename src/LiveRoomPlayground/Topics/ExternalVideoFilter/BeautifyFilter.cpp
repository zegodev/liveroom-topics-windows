#include "stdafx.h"
#include "BeautifyFilter.h"

#include <string>
#include "ilivefiltersdk.h"
#include "TXCVPComomDefine.h"

BeautifyFilter::BeautifyFilter()
{
}


BeautifyFilter::~BeautifyFilter()
{
    if (beauty_filter_ != nullptr)
    {
        beauty_filter_->destroyFilter();
        delete beauty_filter_;
        beauty_filter_ = nullptr;
    }

    inited_ = false;
}


bool BeautifyFilter::InitFilter()
{
    if (beauty_filter_ == nullptr)
    {
        beauty_filter_ = new TILFilter(false);

        if (beauty_filter_->getSDKSupport())
        {
            beauty_filter_->setBeauty(5);	                                   // 设置美颜
            beauty_filter_->setWhite(3);                                       // 设置美白
            beauty_filter_->setSharpness(3);                                   // 设置清晰度

            inited_ = true;
        }
        else {
            inited_ = false;
        }
    }

    return inited_;
}

void BeautifyFilter::FilterProcessI420Data(unsigned char * data, int frame_len, int frame_w, int frame_h)
{
    if (beauty_filter_ != nullptr)
    {
        beauty_filter_->processData(data, frame_len, frame_w, frame_h, VIDEOPROCESS_FORMAT_I420);
    }
}

void BeautifyFilter::UpdateFilterLevel(int level)
{
    if (inited_ && beauty_filter_ != nullptr)
    {
        beauty_filter_->setBeauty(level);
        beauty_filter_->setWhite(level);
    }
}

