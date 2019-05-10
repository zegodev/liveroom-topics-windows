#include "stdafx.h"
#include "ZGUtilTools.h"

#include <comutil.h>  
#include <chrono>
#include <locale>
#include <codecvt>
#include <memory>

#pragma comment(lib, "comsuppw.lib")

using std::string;

// codecvt_byname 析构函数为protected，这里做个适配
template <class _InternT, class _ExternT, class _StateT>
struct codecvt_byname_2 : std::codecvt_byname<_InternT, _ExternT, _StateT>
{
    explicit codecvt_byname_2(const string& __nm, size_t __refs = 0)
        : codecvt_byname<_InternT, _ExternT, _StateT>(__nm.c_str(), __refs) {}
    ~codecvt_byname_2()
    {}
};

using WCHAR_GBK = codecvt_byname_2<wchar_t, char, mbstate_t>;
using WCHAR_UTF8 = std::codecvt_utf8<wchar_t>;

#ifdef _WIN32
#define GBK_NAME ".936"
#else
#define GBK_NAME "zh_CN.GBK"
#endif

/*
 *
 * 转换器的使用，gbk 和 utf8 之间转换，都经过unicode的两个转换器
 *
 * to_bytes               cvtGBK                       from_bytes                cvtUTF8                  to_bytes
 *                wstring_convert<WCHAR_GBK>                            wstring_convert<WCHAR_UTF8>
 * gbk(string) --------------------------------- wstring(unicode utf32) -------------------------------- utf8(string)
 *
 **/

std::string GBKToUTF8(const char* str_gbk)
{
    // gbk与unicode之间的转换器
    std::wstring_convert<WCHAR_GBK>  cvtGBK(new WCHAR_GBK(GBK_NAME));
    // utf8与unicode之间的转换器
    std::wstring_convert<WCHAR_UTF8> cvtUTF8;

    std::wstring ustr = cvtGBK.from_bytes(str_gbk);
    string str = cvtUTF8.to_bytes(ustr);
    return str;
}

std::string UTF8ToGBK(const char * str_utf8)
{
    // gbk与unicode之间的转换器
    std::wstring_convert<WCHAR_GBK>  cvtGBK(new WCHAR_GBK(GBK_NAME));
    // utf8与unicode之间的转换器
    std::wstring_convert<WCHAR_UTF8> cvtUTF8;

    std::wstring ustr = cvtUTF8.from_bytes(str_utf8);
    string str = cvtGBK.to_bytes(ustr);
    return str;

}

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

std::vector<uint8_t> AppSignStrToVecData(CString str)
{
    std::string sig_str = CStringA(str).GetBuffer();
    std::vector<std::string> sig_str_vec;
    std::vector<unsigned char> sig_data;
    SplitString(sig_str, sig_str_vec, ",");
    sig_data.clear();
    for (int i = 0; i < sig_str_vec.size(); ++i)
    {
        int data;
        sscanf_s(sig_str_vec[i].c_str(), "%x", &data);
        sig_data.push_back(data);
    }

    return sig_data;
}

bool LoadImageFromResource(IN CImage* pImage, IN UINT nResID, IN LPCWSTR lpTyp)
{
    if (pImage == NULL)
        return false;

    pImage->Destroy();

    // 查找资源
    HRSRC hRsrc = ::FindResource(AfxGetResourceHandle(), MAKEINTRESOURCE(nResID), lpTyp);
    if (hRsrc == NULL)
        return false;

    // 加载资源
    HGLOBAL hImgData = ::LoadResource(AfxGetResourceHandle(), hRsrc);
    if (hImgData == NULL)
    {
        ::FreeResource(hImgData);
        return false;
    }

    // 锁定内存中的指定资源
    LPVOID lpVoid = ::LockResource(hImgData);

    LPSTREAM pStream = NULL;
    DWORD dwSize = ::SizeofResource(AfxGetResourceHandle(), hRsrc);
    HGLOBAL hNew = ::GlobalAlloc(GHND, dwSize);
    LPBYTE lpByte = (LPBYTE)::GlobalLock(hNew);
    ::memcpy(lpByte, lpVoid, dwSize);

    // 解除内存中的指定资源
    ::GlobalUnlock(hNew);

    // 从指定内存创建流对象
    HRESULT ht = ::CreateStreamOnHGlobal(hNew, TRUE, &pStream);
    if (ht != S_OK)
    {
        GlobalFree(hNew);
    }
    else
    {
        // 加载图片
        pImage->Load(pStream);

        GlobalFree(hNew);
    }

    // 释放资源
    ::FreeResource(hImgData);

    return true;
}

void LoadPngToControl(CStatic * cur_control, UINT png_res_id, int pos_x, int pos_y, int w, int h)
{
    CImage image;
    LoadImageFromResource(&image, png_res_id, _T("PNG"));
    HBITMAP hBmp = image.Detach();
    if (hBmp)
    {
        cur_control->SetBitmap(hBmp);
        cur_control->SetWindowPos(NULL,
            pos_x,
            pos_y,
            w,
            h,
            SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOZORDER);
    }
}

