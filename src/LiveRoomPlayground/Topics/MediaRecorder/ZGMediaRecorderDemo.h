//
// MediaRecorderDemo.h
//
// Copyright 2018Äê Zego. All rights reserved.
// 

#ifndef MediaRecorderDemo_h__
#define MediaRecorderDemo_h__

#include <string>

#include "ZegoLiveRoom/zego-api-media-recorder.h"

using namespace ZEGO;
using std::string;

enum ZGMediaRecordState
{
    ZGMediaRecordState_Stopped = 0,
    ZGMediaRecordState_StartRecording,
    ZGMediaRecordState_Recording,
    ZGMediaRecordState_Stopping,
    ZGMediaRecordState_Error_Unknow,
    ZGMediaRecordState_Error_PathTooLong,
    ZGMediaRecordState_Error_InitFailed,
    ZGMediaRecordState_Error_OpenFileFailed,
    ZGMediaRecordState_Error_WriteFileFailed,
};

typedef struct ZGMediaRecorderDemoConfig
{
    MEDIA_RECORDER::ZegoMediaRecordType   record_type;
    MEDIA_RECORDER::ZegoMediaRecordFormat record_format;
    string record_file_path;

}ZGMediaRecorderDemoConfig;

class ZGMediaRecorderDemoCallBack
{
public:
    virtual ~ZGMediaRecorderDemoCallBack() {}
    virtual void OnMediaRecorderFileStatus(const unsigned int duration, const unsigned int file_size) {};
    virtual void OnMediaRecordStateUpdate(string record_state) {}
};

class ZGMediaRecorderDemo : public MEDIA_RECORDER::IZegoMediaRecordCallback
{
public:
    
    ZGMediaRecorderDemo();

    ~ZGMediaRecorderDemo();

    void InitDemoCallBack(ZGMediaRecorderDemoCallBack * cb);

    void StartRecord(ZGMediaRecorderDemoConfig config);

    void StopRecord();

    string DescRecordFile(const unsigned int duration, const unsigned int file_size);

protected:

    string DescOfStatus(ZGMediaRecordState status);

    void SetCurState(ZGMediaRecordState s);

    virtual void OnMediaRecord(int errCode, MEDIA_RECORDER::ZegoMediaRecordChannelIndex channelIndex, const char* storagePath) override;
    virtual void OnRecordStatusUpdate(const MEDIA_RECORDER::ZegoMediaRecordChannelIndex channelIndex, const char* storagePath, const unsigned int duration, const unsigned int fileSize) override;

private:

    ZGMediaRecorderDemoConfig config_;
    ZGMediaRecorderDemoCallBack * demo_cb_ = nullptr;

    ZGMediaRecordState cur_state_ = ZGMediaRecordState_Stopped;
};


#endif // MediaRecorderDemo_h__

