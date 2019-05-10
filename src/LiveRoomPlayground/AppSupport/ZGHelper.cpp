#include "stdafx.h"
#include "ZGHelper.h"

#define WIN32_LEAN_AND_MEAN        
#include <windows.h>      
#include <intrin.h>       
#include <iphlpapi.h>     
#include <stdint.h>
#include <fstream>


#include <winsock2.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <httpext.h>
#include <windef.h>
#include <Nb30.h>

#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "netapi32.lib")

#include "AppSupport/ZGUtilTools.h"

static struct VideoPixelFormatInfo {
    string desc;
}kPixFormatInfo[] = {
     "PIXEL_FORMAT_UNKNOWN   ",
     "PIXEL_FORMAT_I420      ",
     "PIXEL_FORMAT_NV12      ",
     "PIXEL_FORMAT_NV21      ",
     "PIXEL_FORMAT_BGRA32    ",
     "PIXEL_FORMAT_RGBA32    ",
     "PIXEL_FORMAT_ARGB32    ",
     "PIXEL_FORMAT_ABGR32    ",
     "PIXEL_FORMAT_I422      ",
     "PIXEL_FORMAT_AVC_AVCC  ",
     "PIXEL_FORMAT_AVC_ANNEXB" };

int getMAC(char * mac)
{
    NCB ncb;
    typedef struct _ASTAT_
    {
        ADAPTER_STATUS   adapt;
        NAME_BUFFER   NameBuff[30];
    }ASTAT, *PASTAT;

    ASTAT Adapter;
    typedef struct _LANA_ENUM
    {
        UCHAR length;
        UCHAR lana[MAX_LANA];
    }LANA_ENUM;
    LANA_ENUM lana_enum;
    UCHAR uRetCode;
    memset(&ncb, 0, sizeof(ncb));
    memset(&lana_enum, 0, sizeof(lana_enum));

    ncb.ncb_command = NCBENUM;
    ncb.ncb_buffer = (unsigned char *)&lana_enum;
    ncb.ncb_length = sizeof(LANA_ENUM);
    uRetCode = Netbios(&ncb);
    if (uRetCode != NRC_GOODRET)
        return uRetCode;

    for (int lana = 0; lana < lana_enum.length; lana++)
    {
        ncb.ncb_command = NCBRESET;
        ncb.ncb_lana_num = lana_enum.lana[lana];
        uRetCode = Netbios(&ncb);
        if (uRetCode == NRC_GOODRET)
            break;
    }
    if (uRetCode != NRC_GOODRET)
        return uRetCode;

    memset(&ncb, 0, sizeof(ncb));
    ncb.ncb_command = NCBASTAT;
    ncb.ncb_lana_num = lana_enum.lana[0];
    strcpy_s((char*)ncb.ncb_callname, 5, "*");
    ncb.ncb_buffer = (unsigned char *)&Adapter;
    ncb.ncb_length = sizeof(Adapter);
    uRetCode = Netbios(&ncb);
    if (uRetCode != NRC_GOODRET)
        return uRetCode;
    sprintf_s(mac, 18, "%02X%02X%02X%02X%02X%02X",
        Adapter.adapt.adapter_address[0],
        Adapter.adapt.adapter_address[1],
        Adapter.adapt.adapter_address[2],
        Adapter.adapt.adapter_address[3],
        Adapter.adapt.adapter_address[4],
        Adapter.adapt.adapter_address[5]
    );
    return 0;
}

uint16_t hashMacAddress(PIP_ADAPTER_INFO info)
{
    uint16_t hash = 0;
    for (int i = 0; i < info->AddressLength; i++)
    {
        hash += (info->Address[i] << ((i & 1) * 8));
    }
    return hash;
}

uint16_t getVolumeHash()
{
    DWORD serialNum = 0;

    // Determine if this volume uses an NTFS file system.      
    GetVolumeInformationA("c:\\", NULL, 0, &serialNum, NULL, NULL, NULL, 0);
    uint16_t hash = (uint16_t)((serialNum + (serialNum >> 16)) & 0xFFFF);

    return hash;
}

uint16_t getCpuHash()
{
    int cpuinfo[4] = { 0, 0, 0, 0 };
    __cpuid(cpuinfo, 0);
    uint16_t hash = 0;
    uint16_t* ptr = (uint16_t*)(&cpuinfo[0]);
    for (uint16_t i = 0; i < 8; i++)
        hash += ptr[i];

    return hash;
}

ZGHelper::ZGHelper()
{
}

ZGHelper::~ZGHelper()
{
}

string ZGHelper::GetDeviceUUID()
{
    char mac_addr[20] = { 0 };
    getMAC(mac_addr);
    std::string str(mac_addr);
    char uuid_buf[512] = { 0 };
    uint16_t cpu_hash = getCpuHash();
    uint16_t volume_hash = getVolumeHash();
    sprintf_s(uuid_buf, "%04X%04X%s", cpu_hash, volume_hash, str.c_str());
    return uuid_buf;
}

string ZGHelper::GetMachineName()
{
    static char computerName[1024];
    DWORD size = 1024;
    GetComputerNameA(computerName, &size);
    return &(computerName[0]);
}

string ZGHelper::GetPixelFormatDesc(AVE::VideoPixelFormat format)
{
    return kPixFormatInfo[format].desc;
}

bool FileExist(string file_path)
{
    std::fstream file;
    file.open(file_path.c_str(), std::ios::in);
    if (!file)
    {
        return false;
    }
    else
    {
        return true;
    }
}