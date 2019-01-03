#include "stdafx.h"
#include "ZGImageSource.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

// 外部采集输入源，宽高要保证为4的倍数
#define WIDTH_BYTES(bits) (((bits) + 31) / 32 * 4) 
#define HEIGHT_BYTES WIDTH_BYTES 

ZGImageSource::ZGImageSource(string img_path)
{
    image_path_ = img_path;
}

ZGImageSource::~ZGImageSource()
{
    std::lock_guard<std::mutex> lock(buf_mutex_);
    if (img_buf_ != nullptr)
    {
        // 释放图片数据
        stbi_image_free(img_buf_);
        img_buf_ = nullptr;
    }
}

void ZGImageSource::OnGetVideoData(std::shared_ptr<ZGExternalVideoData> & video_data)
{
    std::lock_guard<std::mutex> lock(buf_mutex_);

    if (img_buf_ == nullptr)
    {
        // 加载图像数据
        // 加载bmp图像到内存img_buf_中
        img_buf_ = stbi_load(image_path_.c_str(), &width_, &height_, &pixel_byte_count_, 0);
        // 加载rgb rgba图像
        assert(pixel_byte_count_ == 3 || pixel_byte_count_ == 4);

        if (width_ == 0 || height_ == 0)
        {
            if (img_buf_ != nullptr)
            {
                stbi_image_free(img_buf_);
            }
            img_buf_ = nullptr;
        }
    }
    
    if (img_buf_ != nullptr)
    {
        // 作为外部采集源的宽，必须是4的倍数，不是4的倍数时，边缘填充至满足
        int source_width = WIDTH_BYTES(width_ * 8);
        // 作为外部采集源的高，必须是4的倍数，不是4的倍数时，边缘填充至满足
        int source_height = HEIGHT_BYTES(height_ * 8);
        // 外部采集数据的每一行占的字节数，输入源设置为BGRA格式，每个像素点占4个字节
        int source_per_width_bytes = source_width * 4;
        // 外部采集源的数据长度
        int len = source_height * source_per_width_bytes;

        // 读取的源图像文件每行占的字节数
        int img_per_width_bytes = width_ * pixel_byte_count_;

        // 创建外部采集源的缓冲区
        std::shared_ptr<ZGExternalVideoData> external_data = std::make_shared<ZGExternalVideoData>();

        external_data->data = std::unique_ptr<uint8_t[] >(new uint8_t[len]);

        memset(external_data->data.get(), 0, len);
        
        int icount = 0;

        // 填充外部采集的数据
        for (int h = 0; h < source_height; ++h)
        {
            int buf_i_count = 0;
            for (int w = 0; w < source_per_width_bytes; w+=4)
            {
                // 按照BGRA拷贝每一行图像数据
                if (buf_i_count < width_*pixel_byte_count_ && h < height_)
                {
                    external_data->data[h*source_per_width_bytes + w] = img_buf_[h*img_per_width_bytes + buf_i_count + 2]; // B
                    external_data->data[h*source_per_width_bytes + w + 1] = img_buf_[h*img_per_width_bytes + buf_i_count + 1];// G
                    external_data->data[h*source_per_width_bytes + w + 2] = img_buf_[h*img_per_width_bytes + buf_i_count]; // R
                    external_data->data[h*source_per_width_bytes + w + 3] = 255; // A
                    buf_i_count += pixel_byte_count_;
                }
                else {
                    // 不是4的倍数时，边缘的填充， 用 B = 0, G = 0 , R = 0 填充（即黑边填充），白边填充时都设置为255
                    external_data->data[h*source_per_width_bytes + w] = 0; // B
                    external_data->data[h*source_per_width_bytes + w + 1] = 0;// G
                    external_data->data[h*source_per_width_bytes + w + 2] = 0; // R
                    external_data->data[h*source_per_width_bytes + w + 3] = 255; // A
                }
            }
        }

        // 填写外部采集的数据格式
        external_data->fomat.width = source_width;
        external_data->fomat.height = source_height;
        external_data->len = len;
        external_data->fomat.strides[0] = source_width * 4;
        external_data->fomat.pixel_format = PIXEL_FORMAT_BGRA32;// 设置数据源格式为BGRA

        video_data = external_data;
    }
}
