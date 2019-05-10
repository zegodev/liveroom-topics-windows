//
// ZGUIOperator.h
//
// Copyright 2018Äê Zego. All rights reserved.
// 

#ifndef ZGUIOperator_h__
#define ZGUIOperator_h__

#include <functional>

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


#endif // ZGUIOperator_h__


