#include "stdafx.h"
#include "ZGManager.h"

#include <vector>
#include <functional>

#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>

#include "AppSupport/AppIDHelper.h"
#include "AppSupport/ZGHttpClient.h"

using namespace ZEGO::APPSUPPORT;

ZGManager::ZGManager():
    sdk_have_inited_(false)
{
}

ZGManager::~ZGManager()
{
}

void ZGManager::EnableExternalVideoCapture(AVE::VideoCaptureFactory* factory, EXTERNAL_RENDER::IZegoVideoRenderCallback* renderer)
{
    if (sdk_have_inited_)
    {
        UninitSdk();
    }

    VCAP::SetVideoCaptureFactory(factory);
    if (renderer != nullptr)
    {
        EXTERNAL_RENDER::EnableExternalRender(true, AV::VideoExternalRenderType::DECODE_RGB_SERIES);
        EXTERNAL_RENDER::SetVideoRenderCallback(renderer);
    }
    else {
        EXTERNAL_RENDER::EnableExternalRender(false, AV::VideoExternalRenderType::DECODE_RGB_SERIES);
    }
    InitSdk();
}

void ZGManager::EnableMic(bool enableMic)
{
    LIVEROOM::EnableMic(enableMic);
}

bool ZGManager::InitSdk(unsigned int uiAppID, unsigned char * sign, int len)
{
    if (sdk_have_inited_)
    {
        UninitSdk();
    }
    LIVEROOM::SetLogDir("");
    bool ret = LIVEROOM::InitSDK(uiAppID, sign, len);
    sdk_have_inited_ = ret;
    return sdk_have_inited_;
}

bool ZGManager::InitSdk()
{
    unsigned int appid = GetAppId();
    int sign_len = 0;
    unsigned char * sign_data = GetAppSign(sign_len);

    return InitSdk(appid, sign_data, sign_len);
}

void ZGManager::UninitSdk()
{
    LIVEROOM::UnInitSDK();
    sdk_have_inited_ = false;
}

bool ZGManager::SdkIsInited()
{
    return sdk_have_inited_;
}


// 获取appid下的所有房间列表
std::vector<RoomInfo> ZGManager::GetRoomList(unsigned int app_id, bool test_env)
{
    std::string request_url;
    // 根据测试环境还是正式环境的appid，请求房间列表信息
    if (test_env)
    {
        char sz_request_url[1000] = { 0 };
        sprintf_s(sz_request_url, "https://test2-liveroom-api.zego.im/demo/roomlist?appid=%u", app_id);
        request_url = sz_request_url;
    }
    else {
        char sz_request_url[1000] = { 0 };
        sprintf_s(sz_request_url, "https://liveroom%u-api.zego.im/demo/roomlist?appid=%u", app_id, app_id);
        request_url = sz_request_url;
    }

    string response_str = "";
    int rsp_code = 0;
    int time_out = 20;
    HttpClient client;
    if (client.SendHttpRequest(request_url, nullptr, &response_str, &rsp_code, "GET"))
    {
        do
        {
            rapidjson::Document d;
            if (d.Parse(response_str.c_str()).HasParseError() || !d.IsObject())
            {
                break;
            }
            if (d.HasMember("code"))
            {
                if (d["code"].GetInt() == 0)
                {
                    if (d.HasMember("data") && d["data"].HasMember("room_list"))
                    {
                        rapidjson::Value& room_list = d["data"]["room_list"];
                        if (room_list.IsArray()) {

                            std::vector<RoomInfo> ret_vec;
                            for (int i = 0; i < room_list.Size(); i++) {

                                const rapidjson::Value& room_item = room_list[i];

                                string room_id = room_item["room_id"].GetString();
                                string room_name = room_item["room_name"].GetString();

                                string anchor_id = room_item["anchor_id_name"].GetString();
                                string anchor_name = room_item["anchor_nick_name"].GetString();
                                RoomInfo new_item;
                                new_item.room_id = room_id;
                                new_item.room_name = room_name;
                                new_item.anchor_id = anchor_id;
                                new_item.anchor_name = anchor_name;

                                ret_vec.push_back(new_item);

                            }

                            return ret_vec;
                        }
                    }
                }
            }

        } while (false);

    }

    return std::vector<RoomInfo>();
}

// 异步获取所有房间列表
void ZGManager::GetRoomList(bool is_test_env, std::function<void(std::vector<RoomInfo> room_list)> room_list_cb)
{
    async_task_queue_.AsyncRun([=]() {
        if (room_list_cb)
        {
            std::vector<RoomInfo> room_vec = GetRoomList(GetAppId(), is_test_env);
            room_list_cb(room_vec);
        }
    });
}