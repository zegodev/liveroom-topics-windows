//
// ZGUIOperator.h
//
// Copyright 2018年 Zego. All rights reserved.
// 

#ifndef ZGUIOperator_h__
#define ZGUIOperator_h__

#include <functional>

/************************************************************************

// 切UI线程封装

// 在Dialog类里，实现自定义消息UI_CALLBACK_MSG的处理函数

afx_msg LRESULT OnUICallbackMsg(WPARAM wParam, LPARAM lParam)
{
    PostUIData * pdata = (PostUIData *)wParam;
    if (pdata != nullptr && pdata->cb_in_ui_fun)
    {
        pdata->cb_in_ui_fun();
    }
    return 0;
}


// 在需要切线程的地方
// 定义一个包装结构体，从工作线程到UI线程投递数据
typedef struct UIDataStruct
{
    // 自定义要传递的数据项
} UIDataStruct;

UIDataStruct ui_data;
// 在线程函数中，拷贝保存数据项ui_data

PostUIData * pdata = CreateUIData();
// 定义UI线程上的回调函数
pdata->cb_in_ui_fun = [ui_data, pdata, this]()->void
{
    // 在这里写更新UI的代码

    // 释放资源
    DestroyUIData(pdata);
};

// 投递消息到UI线程，让UI线程回调执行lampda表达式
// 在Dialog类里，通过PostMsgDataToUI，投递，在非dialog类里，可以拿到窗口句柄通过GlobalPostMsgDataToUI投递

PostMsgDataToUI(pdata);


************************************************************************/

#define UI_CALLBACK_MSG  WM_USER + 888

typedef struct PostUIData
{
    std::function<void()>  cb_in_ui_fun;

}PostUIData;

#define CreateUIData() new PostUIData()

#define DestroyUIData(ptr)\
if(ptr != nullptr)\
{\
    PostUIData *pd = (PostUIData *)ptr;\
    delete ptr;\
}\


#define PostMsgDataToUI(p)\
if (!PostMessage(UI_CALLBACK_MSG, (WPARAM)pdata, 0))\
{\
    DestroyUIData(p);\
}\

#define GlobalPostMsgDataToUI(hwnd, p)\
if (!::PostMessage(hwnd, UI_CALLBACK_MSG, (WPARAM)pdata, 0))\
{\
    DestroyUIData(p);\
}\


#endif // ZGUIOperator_h__


