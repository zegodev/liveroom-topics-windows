//
// ZGTimer.h
//
// Copyright 2018Äê Zego. All rights reserved.
// 

#ifndef ZGTimer_h__
#define ZGTimer_h__

class ZGTimer
{
public:

    ZGTimer()
    {
        QueryPerformanceFrequency(&frequency_);
        QueryPerformanceCounter(&start_count_);
    }

    ~ZGTimer() 
    {

    }

    double ElapsedMs()
    {
        LARGE_INTEGER CurrentCount;
        QueryPerformanceCounter(&CurrentCount);
        return double((CurrentCount.QuadPart - start_count_.QuadPart)*1000.0f) / (double)frequency_.QuadPart;
    }

private:
    LARGE_INTEGER frequency_;
    LARGE_INTEGER start_count_;
};

#endif // ZGTimer_h__


