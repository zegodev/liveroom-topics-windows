//
// ZGMediaPlayerDemoHelper.h
//
// Copyright 2018Äê Zego. All rights reserved.
// 

#ifndef ZGMediaPlayerDemoHelper_h__
#define ZGMediaPlayerDemoHelper_h__

#include <string>
#include <vector>

using std::string;
using std::vector;

typedef struct MediaFileInfo {
    string name;
    string file_type;
    string source_type;//local or online
    string url;

}MediaFileInfo;

class ZGMediaPlayerDemoHelper
{
public:
    ZGMediaPlayerDemoHelper();
    ~ZGMediaPlayerDemoHelper();

    vector<MediaFileInfo> GetMediaList();
    string GetTitleForItem(MediaFileInfo media);

private:
    vector<MediaFileInfo> media_file_list_;
};


#endif // ZGMediaPlayerDemoHelper_h__

