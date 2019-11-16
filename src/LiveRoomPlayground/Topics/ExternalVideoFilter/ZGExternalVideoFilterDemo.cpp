#include "stdafx.h"
#include "ZGExternalVideoFilterDemo.h"

#include "AppSupport/ZGLog.h"
#include "AppSupport/ZGManager.h"

#include "BeautifyFilter.h"
#include "FuBeautifyFilter.h"

ZGExternalVideoFilterDemo::ZGExternalVideoFilterDemo()
{
    for (int i = 0; i < MAX_FILTER_FRAME_COUNT; ++i)
    {
        std::unique_ptr<VideoFilterBuffer> fb(new VideoFilterBuffer());
        fb->width_ = 0;
        fb->height_ = 0;
        fb->stride_ = 0;
        fb->data_ = nullptr;
        filter_data_list_.push_back(std::move(fb));
    }
}


ZGExternalVideoFilterDemo::~ZGExternalVideoFilterDemo()
{
}


void ZGExternalVideoFilterDemo::EnableVideoFilter(bool enable)
{
    //ZGENTER_FUN_LOG;
    if (ZGManagerInstance()->SdkIsInited())
    {
        ZGManagerInstance()->UninitSdk();
    }

    if (enable)
    {
        // 开启视频外部滤镜，IniSDK之前调用
        LIVEROOM::SetVideoFilterFactory(this);

    }

    // 使用测试环境，生产上线时，需要联系zego技术支持切换为正式环境，并修改为 SetUseTestEnv(FALSE); 表示启用正式环境
    LIVEROOM::SetUseTestEnv(TRUE);
    ZGManagerInstance()->InitSdk();
}

void ZGExternalVideoFilterDemo::EnableBeauty(bool enable)
{
    enable_beautify_ = enable;
}

void ZGExternalVideoFilterDemo::UpdateBeautyLevel(int level)
{
    if (filter_process_ != nullptr)
    {
        filter_process_->UpdateFilterLevel(level);
    }
}

AVE::VideoFilter* ZGExternalVideoFilterDemo::Create()
{
    //ZGENTER_FUN_LOG;
    return this;
}

// 滤镜启动初始化
void ZGExternalVideoFilterDemo::AllocateAndStart(Client* client)
{
    //ZGENTER_FUN_LOG;

    is_exit_ = false;
    client_ = client;
    write_index_ = 0;
    read_index_ = 0;
    pending_count_ = 0;

    // 美颜滤镜初始化
    filter_process_ = std::shared_ptr<VideoFilterProcessBase>(new FuBeautifyFilter());

    // 美颜处理线程
    process_thread_ = std::thread(&ZGExternalVideoFilterDemo::BeautifyProcess, this);


}

void ZGExternalVideoFilterDemo::StopAndDeAllocate()
{
    ZGENTER_FUN_LOG;
    is_exit_ = true;

    process_thread_.join();

    if (client_ != nullptr)
    {
        client_->Destroy();
        client_ = nullptr;
    }
}

// sdk 回调第1步， 请求buffer类型
AVE::VideoBufferType ZGExternalVideoFilterDemo::SupportBufferType()
{
    //ZGENTER_FUN_LOG;

    // rgb 帧大小
    // rgb frame_len = w * h * 4
     cal_frame_factor_ = 4;
     return  AVE::BUFFER_TYPE_MEM;

    // yuv帧大小
    // yuv i420 frame_len = w * h * 1.5
    //cal_frame_factor_ = 1.5f;
    //return AVE::BUFFER_TYPE_ASYNC_I420_MEM;
}

// sdk 回调第2步，请求获取操作接口
void* ZGExternalVideoFilterDemo::GetInterface()
{
    //ZGENTER_FUN_LOG;
    return (AVE::VideoBufferPool*)this;
}

// sdk 回调第3步，给sdk一个索引号，sdk下一步使用这个索引来取buffer地址，准备拷贝数据到该地址
int ZGExternalVideoFilterDemo::DequeueInputBuffer(int width, int height, int stride)
{
    //ZGENTER_FUN_LOG;

    if (!have_start_)
    {
        return -1;
    }

    if (pending_count_ >= MAX_FILTER_FRAME_COUNT)
    {
        ZGLog("pending_count_ >= MAX_FILTER_FRAME_COUNT, return");
        return -1;
    }

    //ZGLog("width = %d, height = %d, stride = %d ", width, height, stride);

    if (filter_data_list_[write_index_]->width_ != width
        || filter_data_list_[write_index_]->height_ != height
        || filter_data_list_[write_index_]->stride_ != stride)
    {
        ZGLog("reset buffer , index = %d", write_index_);
        std::unique_ptr<VideoFilterBuffer> fb(new VideoFilterBuffer());
        fb->width_ = width;
        fb->height_ = height;
        fb->stride_ = stride;
        fb->data_ = new unsigned char[width * height * cal_frame_factor_];
        ZGLog("filter_data_list_ size = %d ", filter_data_list_.size());
        if (write_index_ < filter_data_list_.size())
        {
            filter_data_list_[write_index_] = std::move(fb);
        }
        else {
            ZGLog("write_index_ error");
        }
    }

    return write_index_;
}

// sdk 回调第4步，sdk通过索引，取buffer地址，sdk会拷贝数据到提供的地址上
void* ZGExternalVideoFilterDemo::GetInputBuffer(int index)
{
    //ZGENTER_FUN_LOG;

    if (index < 0 || index >= MAX_FILTER_FRAME_COUNT)
    {
        ZGLog("index invalid !!!");
        return nullptr;
    }

    //ZGLog("input buffer index %d, data = %d", index, filter_data_list_[write_index_]->data_);

    return filter_data_list_[write_index_]->data_;
}

// sdk 回调第5步，sdk已经拷贝数据到指定地址完毕，可以滤镜处理该buffer
void ZGExternalVideoFilterDemo::QueueInputBuffer(int index, int width, int height, int stride, unsigned long long timestamp_100n)
{
    //ZGENTER_FUN_LOG;

    if (write_index_ != index)
    {
        ZGLog("write_index_ != index, return");
        return;
    }

    //ZGLog("index=%d, width=%d, height=%d, stride=%d, timestamp=%ld", index, width, height, stride, timestamp_100n);

    filter_data_list_[write_index_]->width_ = width;
    filter_data_list_[write_index_]->height_ = height;
    filter_data_list_[write_index_]->timestamp_100n_ = timestamp_100n;
    filter_data_list_[write_index_]->stride_  = stride;
    filter_data_list_[write_index_]->len_ = width * height * cal_frame_factor_;
    write_index_ = (write_index_ + 1) % MAX_FILTER_FRAME_COUNT;

    pending_count_++;
}


// 滤镜处理函数
void ZGExternalVideoFilterDemo::BeautifyProcess()
{
    //ZGENTER_FUN_LOG;

    filter_process_->InitFilter();

    have_start_ = true;

    while (!is_exit_)
    {
        while (pending_count_ > 0)
        {
            unsigned char* src_data = filter_data_list_[read_index_]->data_;

            // 滤镜处理图像数据
            if (enable_beautify_ && filter_process_ != nullptr)
            {
                filter_process_->FilterProcessRGBAData(filter_data_list_[read_index_]->data_, filter_data_list_[read_index_]->len_, filter_data_list_[read_index_]->width_, filter_data_list_[read_index_]->height_);
            }


            // 滤镜处理完毕的视频数据再塞到sdk
            {
                VideoBufferPool* pool = (VideoBufferPool*)client_->GetInterface();
                int index = pool->DequeueInputBuffer(filter_data_list_[read_index_]->width_, filter_data_list_[read_index_]->height_, filter_data_list_[read_index_]->stride_);

                if (index >= 0)
                {
                    unsigned char* dst = (unsigned char*)pool->GetInputBuffer(index);

                    memcpy(dst, src_data, filter_data_list_[read_index_]->width_*filter_data_list_[read_index_]->height_ * cal_frame_factor_);

                    pool->QueueInputBuffer(index, filter_data_list_[read_index_]->width_, filter_data_list_[read_index_]->height_, filter_data_list_[read_index_]->stride_, filter_data_list_[read_index_]->timestamp_100n_);
                }

            }

            read_index_ = (read_index_ + 1) % MAX_FILTER_FRAME_COUNT;
            pending_count_--;

            if (is_exit_)
            {
                break;
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(TICK_PERIOD_MS));
    }

    filter_process_->Release();

    //ZGLEAVE_FUN_LOG;

}

void ZGExternalVideoFilterDemo::Destroy(VideoFilter *vf)
{
    
}
