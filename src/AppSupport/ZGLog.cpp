#include "stdafx.h"
#include "ZGLog.h"
#include <string>

void ZGLog(const char * format, ...)
{
    char log_buf[1024] = { 0 };
    va_list la;
    va_start(la, format);
    vsprintf_s(log_buf, format, la);
    va_end(la);

    std::string log_str = std::string(log_buf) + "\r\n";

    TRACE(log_str.c_str());
}
