//
// ZGHelper.h
//
// Copyright 2018Äê Zego. All rights reserved.
// 

#ifndef ZGHelper_h__
#define ZGHelper_h__

#include <string>

using std::string;

class ZGHelper
{
public:
    static ZGHelper * Instance()
    {
        static ZGHelper z;
        return &z;
    }

    string GetDeviceUUID();

private:
    ZGHelper();
    ~ZGHelper();
};

#define ZGHelperInstance ZGHelper::Instance

#endif // ZGHelper_h__

