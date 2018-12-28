//
// ZGUtilTools.h
//
// Copyright 2018Äê Zego. All rights reserved.
// 

#ifndef ZGUtilTools_h__
#define ZGUtilTools_h__

#include <cstdio>
#include <cstdarg>
#include <cstring>
#include <memory>
#include <string>
#include <vector>

std::string StringFormat(const char *fmt, ...);
std::string GetExePath();
void SplitString(const std::string& s, std::vector<std::string>& v, const std::string& c);

#endif // ZGUtilTools_h__

