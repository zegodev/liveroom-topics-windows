//
// ZGUIOperator.h
//
// Copyright 2018Äê Zego. All rights reserved.
// 

#ifndef ZGUIOperator_h__
#define ZGUIOperator_h__

#define UI_CALLBACK_MSG  WM_USER + 888

typedef struct CBFunInfoStruct
{
    void * cb_fun;
    //int line;
    //wstring file;
} CBFunInfoStruct;


#define UI_CB_SNIPPETS() \
CBFunInfoStruct *cb_info = new CBFunInfoStruct();\
std::function< void(void*) > *func = new std::function< void(void*) >();\
cb_info->cb_fun = (void*)func;\
*func = [&](void * p)\

//#define UI_CB_SNIPPETS() \
//CBFunInfoStruct *cb_info = new CBFunInfoStruct();\
//static std::function< void(void*) > func; \
//cb_info->cb_fun = (void*)&func;\
//cb_info->line = __LINE__; \
//cb_info->file = wstring(__FILET__); \
//func = [&](void * p)->void


#define SET_MSG_DATA(msg_data) \
    void * data_to_send_or_post = (void *)msg_data;

#define UI_CB_BY_POST() if(::IsWindow(GetHWND())) PostMessage(UI_CALLBACK_MSG, (WPARAM)(void*)cb_info, (LPARAM)(void*)data_to_send_or_post);
#define UI_CB_BY_SEND() if(::IsWindow(GetHWND())) SendMessage(UI_CALLBACK_MSG, (WPARAM)(void*)cb_info, (LPARAM)(void*)data_to_send_or_post);

#define UI_CB_BY_POST_TO(TO_HWND) if(::IsWindow(TO_HWND)) ::PostMessage(TO_HWND, UI_CALLBACK_MSG, (WPARAM)(void*)cb_info, (LPARAM)(void*)data_to_send_or_post);
#define UI_CB_BY_SEND_TO(TO_HWND) if(::IsWindow(TO_HWND)) ::SendMessage(TO_HWND, UI_CALLBACK_MSG, (WPARAM)(void*)cb_info, (LPARAM)(void*)data_to_send_or_post);


#endif // ZGUIOperator_h__


