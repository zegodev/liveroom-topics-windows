#include "stdafx.h"
#include "GreyVideoFilter.h"

GreyVideoFilter::GreyVideoFilter()
{

}

GreyVideoFilter::~GreyVideoFilter()
{

}

void GreyVideoFilter::FilterProcessRGBAData(unsigned char * data, int frame_len, int frame_w, int frame_h)
{
	unsigned char* src = data;
    unsigned char* dst = data;

    for (int i = 0; i < frame_h; i++) {
        for (int j = 0; j < frame_w; j++, src += 4, dst += 4) {
            int grey = (*src * 38 + *(src + 1) * 75 + *(src + 2) * 15) >> 7;
            *dst = grey;
            *(dst + 1) = grey;
            *(dst + 2) = grey;
        }
    }
}
