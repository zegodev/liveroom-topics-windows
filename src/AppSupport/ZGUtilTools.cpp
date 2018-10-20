#include "stdafx.h"
#include "ZGUtilTools.h"


std::string StringFormat(const char *fmt, ...)
{
    va_list args, args1;
    va_start(args, fmt);
    va_copy(args1, args);
    std::string res(1 + vsnprintf(nullptr, 0, fmt, args1), 0);
    va_end(args1);
    vsnprintf(&res[0], res.size(), fmt, args);
    va_end(args);
    return res;
}

std::string GetExePath()
{
    char szFilePath[MAX_PATH + 1];
    GetModuleFileNameA(NULL, szFilePath, MAX_PATH);
    (strrchr(szFilePath, '\\'))[1] = 0;
    return std::string(szFilePath);
}

void SplitString(const std::string& s, std::vector<std::string>& v, const std::string& c)
{
    std::string::size_type pos1, pos2;
    pos2 = s.find(c);
    pos1 = 0;
    while (std::string::npos != pos2)
    {
        v.push_back(s.substr(pos1, pos2 - pos1));

        pos1 = pos2 + c.size();
        pos2 = s.find(c, pos1);
    }
    if (pos1 != s.length())
        v.push_back(s.substr(pos1));
}