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
        // ������Ƶ�ⲿ�˾���IniSDK֮ǰ����
        VIDEOFILTER::SetVideoFilterFactory(this);

    }

    // ʹ�ò��Ի�������������ʱ����Ҫ��ϵzego����֧���л�Ϊ��ʽ���������޸�Ϊ SetUseTestEnv(FALSE); ��ʾ������ʽ����
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

// �˾�������ʼ��
void ZGExternalVideoFilterDemo::AllocateAndStart(Client* client)
{
    //ZGENTER_FUN_LOG;

    is_exit_ = false;
    client_ = client;
    write_index_ = 0;
    read_index_ = 0;
    pending_count_ = 0;

    // �����˾���ʼ��
    filter_process_ = std::shared_ptr<VideoFilterProcessBase>(new FuBeautifyFilter());

    // ���մ����߳�
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

// sdk �ص���1���� ����buffer����
AVE::VideoBufferType ZGExternalVideoFilterDemo::SupportBufferType()
{
    //ZGENTER_FUN_LOG;

    // rgb ֡��С
    // rgb frame_len = w * h * 4
     cal_frame_factor_ = 4;
     return  AVE::BUFFER_TYPE_MEM;

    // yuv֡��С
    // yuv i420 frame_len = w * h * 1.5
    //cal_frame_factor_ = 1.5f;
    //return AVE::BUFFER_TYPE_ASYNC_I420_MEM;
}

// sdk �ص���2���������ȡ�����ӿ�
void* ZGExternalVideoFilterDemo::GetInterface()
{
    //ZGENTER_FUN_LOG;
    return (AVE::VideoBufferPool*)this;
}

// sdk �ص���3������sdkһ�������ţ�sdk��һ��ʹ�����������ȡbuffer��ַ��׼���������ݵ��õ�ַ
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

// sdk �ص���4����sdkͨ��������ȡbuffer��ַ��sdk�´�����ݵ��ṩ�ĵ�ַ��
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

// sdk �ص���5����sdk�Ѿ��������ݵ�ָ����ַ��ϣ������˾������buffer
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


// �˾�������
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

            // �˾�����ͼ������
            if (enable_beautify_ && filter_process_ != nullptr)
            {
                filter_process_->FilterProcessRGBAData(filter_data_list_[read_index_]->data_, filter_data_list_[read_index_]->len_, filter_data_list_[read_index_]->width_, filter_data_list_[read_index_]->height_);
            }


            // �˾�������ϵ���Ƶ����������sdk
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

    have_start_ = false;

    //ZGLEAVE_FUN_LOG;

}

void ZGExternalVideoFilterDemo::Destroy(VideoFilter *vf)
{
    
}
