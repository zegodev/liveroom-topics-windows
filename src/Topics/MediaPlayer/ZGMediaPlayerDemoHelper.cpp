#include "stdafx.h"
#include "ZGMediaPlayerDemoHelper.h"

#include "AppSupport/ZGUtilTools.h"


ZGMediaPlayerDemoHelper::ZGMediaPlayerDemoHelper()
{
    string exe_path = GetExePath();
    MediaFileInfo file;

    file.name = "audio clip(-50% tempo)";
    file.file_type = "mp3";
    file.source_type = "local";
    file.url = exe_path + "/Res/sample_-50_tempo.mp3";
    media_file_list_.push_back(file);

    file.name = "ad";
    file.file_type = "mp4";
    file.source_type = "local";
    file.url = exe_path + "/Res/ad.mp4";
    media_file_list_.push_back(file);

//     file.name = "大海";
//     file.file_type = "mp4";
//     file.source_type = "local";
//     file.url = exe_path + "/Res/dh.mp4";
//     media_file_list_.push_back(file);

    file.name = "audio clip";
    file.file_type = "mp3";
    file.source_type = "online";
    file.url = "http://www.surina.net/soundtouch/sample_orig.mp3";
    media_file_list_.push_back(file);

    file.name = "大海";
    file.file_type = "mp4";
    file.source_type = "online";
    file.url = "http://lvseuiapp.b0.upaiyun.com/201808270915.mp4";
    media_file_list_.push_back(file);


}

ZGMediaPlayerDemoHelper::~ZGMediaPlayerDemoHelper()
{
}

vector<MediaFileInfo> ZGMediaPlayerDemoHelper::GetMediaList()
{
    return media_file_list_;
}

string ZGMediaPlayerDemoHelper::GetTitleForItem(MediaFileInfo media)
{
    return StringFormat("[%s][%s] %s", media.source_type.c_str(), media.file_type.c_str(), media.name.c_str());
}
