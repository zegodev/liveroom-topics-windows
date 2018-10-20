#include "stdafx.h"
#include "ZGHelper.h"

#define WIN32_LEAN_AND_MEAN        
#include <windows.h>      
#include <intrin.h>       
#include <iphlpapi.h>     
#include <stdint.h>
#include <fstream>

#include "AppSupport/ZGUtilTools.h"



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

const char* getMachineName()
{
    static char computerName[1024];
    DWORD size = 1024;
    GetComputerNameA(computerName, &size);
    return &(computerName[0]);
}


ZGHelper::ZGHelper()
{
}

ZGHelper::~ZGHelper()
{
}

string ZGHelper::GetDeviceUUID()
{
    char uuid_buf[512] = { 0 };
    sprintf_s(uuid_buf, "%s-%04X-%04X", getMachineName(), getCpuHash(), getVolumeHash());
    return uuid_buf;
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