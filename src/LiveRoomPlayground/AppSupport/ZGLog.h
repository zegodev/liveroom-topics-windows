//
// ZGLog.h
//
// Copyright 2018Äê Zego. All rights reserved.
// 

#ifndef ZGLog_h__
#define ZGLog_h__

void ZGLog(const char * format, ...);

#define ZGENTER_FUN_LOG ZGLog("==>%s",__FUNCTION__);
#define ZGLEAVE_FUN_LOG ZGLog("<==%s",__FUNCTION__);

#endif // ZGLog_h__
