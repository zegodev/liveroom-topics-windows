#include "stdafx.h"
#include "ZGMediaRecorderDemo.h"

#include "AppSupport/ZGUtilTools.h"


ZGMediaRecorderDemo::ZGMediaRecorderDemo()
{
}

ZGMediaRecorderDemo::~ZGMediaRecorderDemo()
{
}

void ZGMediaRecorderDemo::InitDemoCallBack(ZGMediaRecorderDemoCallBack * cb)
{
    demo_cb_ = cb;
}

void ZGMediaRecorderDemo::StartRecord(ZGMediaRecorderDemoConfig config)
{
    config_ = config;
    
    MEDIA_RECORDER::SetZegoMediaRecordCallback(this);
    bool record_result = MEDIA_RECORDER::StartRecord(MEDIA_RECORDER::ZEGO_MEDIA_RECORD_CHN_MAIN, config_.record_type, config_.record_file_path.c_str(), true, 3000, config_.record_format);
    if (record_result)
    {
        SetCurState(ZGMediaRecordState_StartRecording);
    }
    else {
        StopRecord();
    }
}

void ZGMediaRecorderDemo::StopRecord()
{
    SetCurState(ZGMediaRecordState_Stopping);
    MEDIA_RECORDER::StopRecord(MEDIA_RECORDER::ZEGO_MEDIA_RECORD_CHN_MAIN);
    SetCurState(ZGMediaRecordState_Stopped);
}

string ZGMediaRecorderDemo::DescOfStatus(ZGMediaRecordState status)
{
    string status_str = "";
    switch (status)
    {
    case ZGMediaRecordState_Stopped:
        status_str = "Stoped Record";
        break;
    case ZGMediaRecordState_StartRecording:
        status_str = "Starting Record";
        break;
    case ZGMediaRecordState_Recording:
        status_str = "Recording";
        break;
    case ZGMediaRecordState_Stopping:
        status_str = "Stopping Record";
        break;
    case ZGMediaRecordState_Error_Unknow:
        status_str = "Record Unknow Error";
        break;
    case ZGMediaRecordState_Error_PathTooLong:
        status_str = "Record Error:file path too long";
        break;
    case ZGMediaRecordState_Error_InitFailed:
        status_str = "Record Error:init failed";
        break;
    case ZGMediaRecordState_Error_OpenFileFailed:
        status_str = "Record Error:open file failed";
        break;
    case ZGMediaRecordState_Error_WriteFileFailed:
        status_str = "Record Error:write file failed";
        break;
    default:
        break;
    }
    return status_str;
}

string ZGMediaRecorderDemo::DescRecordFile(const unsigned int duration, const unsigned int file_size)
{
    string desc = StringFormat("file duration:%d , file size:%d", duration, file_size);
    return desc;
}

void ZGMediaRecorderDemo::SetCurState(ZGMediaRecordState s)
{
    cur_state_ = s;
    if (demo_cb_ != nullptr)
    {
        demo_cb_->OnMediaRecordStateUpdate(DescOfStatus(cur_state_));
    }
}

void ZGMediaRecorderDemo::OnMediaRecord(int errCode, MEDIA_RECORDER::ZegoMediaRecordChannelIndex channelIndex, const char* storagePath)
{
    switch (errCode)
    {
    case 0:
        SetCurState(ZGMediaRecordState_Recording);
        break;
    case 1:
        SetCurState(ZGMediaRecordState_Error_PathTooLong);
        break;
    case 2:
        SetCurState(ZGMediaRecordState_Error_InitFailed);
        break;
    case 3:
        SetCurState(ZGMediaRecordState_Error_OpenFileFailed);
        break;
    case 4:
        SetCurState(ZGMediaRecordState_Error_WriteFileFailed);
        break;
    default:
        SetCurState(ZGMediaRecordState_Error_Unknow);
        break;
    }
    
}

void ZGMediaRecorderDemo::OnRecordStatusUpdate(const MEDIA_RECORDER::ZegoMediaRecordChannelIndex channelIndex, const char* storagePath, const unsigned int duration, const unsigned int fileSize)
{
    if (demo_cb_ != nullptr)
    {
        demo_cb_->OnMediaRecorderFileStatus(duration, fileSize);
    }
}
