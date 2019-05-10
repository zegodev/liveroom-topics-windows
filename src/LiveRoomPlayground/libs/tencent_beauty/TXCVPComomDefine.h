#pragma once
/*
* 该头文件用于定义 sdk 公用的一些结构体 
*/
#include <string.h>

#ifdef TXCVPCOMBINE
#define DLLIMEXPORT __declspec(dllexport)
#else
#define DLLIMEXPORT
#endif // TXCVPCOMBINE

#define VIDEOPROCESS_NO_OUTPUT		-1
#define VIDEOPROCESS_FORMAT_TEXTURE2D  0
#define VIDEOPROCESS_FORMAT_I420  1
#define VIDEOPROCESS_FORMAT_NV12  2
#define VIDEOPROCESS_FORMAT_NV21  3
#define VIDEOPROCESS_FORMAT_RGB   4
#define VIDEOPROCESS_FORMAT_YUY2  5
#define VIDEOPROCESS_FORMAT_RGBA  6
#define VIDEOPROCESS_FORMAT_BGR	  7
#define VIDEOPROCESS_FORMAT_XRGB  8
#define VIDEOPROCESS_FORMAT_BGRA  9

typedef struct CropRect
{
	CropRect() {
		xOffset = -1;
		yOffset = -1;
		cropWidth = -1;
		cropHeight = -1;

		originWidth = -1;
		originHeight = -1;
	}
	int xOffset;
	int yOffset;
	int cropWidth;
	int cropHeight;

	int originWidth;
	int originHeight;
}CropRect;

typedef struct TXCCombineFrame {
	TXCCombineFrame() {
		texture = -1;
		data = nullptr;
		format = VIDEOPROCESS_FORMAT_I420;
		width = 0;
		height = 0;
		rotation = 0;
		memset(&crop, 0, sizeof(crop));
		memset(&posision, 0, sizeof(posision));
	}
	int texture;
	unsigned char* data;
	int format;
	int width;
	int height;
	int rotation;
	CropRect crop;
	CropRect posision;
}TXCCombineFrame;

class DLLIMEXPORT CTXIVideoPreprocessorListener
{
public:
	/**
	* 外部滤镜处理回调
	* @param texture   处理的纹理ID
	* @param width     处理的纹理宽度
	* @param height    处理的纹理高度
	*/
	virtual int willAddWatermark(int texture, int width, int height) {
		return 0;
	}
	/**
	* 处理完一帧图像
	* @param texture   处理完的纹理ID
	* @param width     处理完纹理宽度
	* @param height    处理完纹理高度
	* @param timestamp 处理完时间戳（绝对时间）
	*/
	virtual void didProcessFrame(int texture, int width, int height, unsigned long long timestamp) {

	}

	/**
	* 处理完一帧图像
	* @param data      处理完帧数据
	* @param width     处理完帧数据宽度
	* @param height    处理完帧数据高度
	* @param format    处理完帧数据格式，在TXEFrameFormat中定义
	* @param timestamp 处理完时间戳（绝对时间）
	*/
	virtual void didProcessFrame(unsigned char* data, int width, int height, int format, unsigned long long timestamp) {

	};
};

class CTXINotifyListener {
public:
	/**
	* SurfaceTexture可用
	* @param event 事件id
	* @param param 事件参数
	*/
	virtual void onNotifyEvent(int event, void* param) {
	
	}
};

// 播放错误事件
typedef enum tagPlayEventType
{
	EVENT_ERROR_PLAY = 0,			// 播放失败；1，文件不存在  2，视频流不存在
	EVENT_MEDIA_STOP = 1,          // 非手动播放停止事件; 1,文件播放完  2，视频断流
	EVENT_ERROR_STREAM_FORMAT = 2,		// 不支持流格式		1，不支持音视频格式解码
	EVENT_ERROR_AUDIO_DEVICE = 3,		// 音频设备打开失败	1，需要播放音频轨时，声卡打开失败
	EVENT_ERROR_MEDIE_INFO = 4,	// 获取视频信息错误			1，不支持视频格式

	// SDK 内部错误
	EVENT_ERROR_OPENGL_INIT = 100,	// Opengl 初始化失败
}EventPlayType;

typedef enum TxeFillModeTag
{
	TXE_FILL_MODE_SCALE_TO_FILL = 0,	// 拉伸或压缩填充，全显示（可以看到视频整个画面）
	TXE_FILL_MODE_SCALL_ASPECT_FILL = 1,	// 以视频短边做自适应填充
}TxeFillModeTag;

typedef struct TXCGreenScreenParam {
	TXCGreenScreenParam() {
		bAudioTrack = false;		// 默认忽略音频轨
		iAudioVolume = 100;			// 如果有音频轨，默认音量为 80
		renderMode = TXE_FILL_MODE_SCALE_TO_FILL;	// 默认全显示（绿幕背景可能会变形）
	}
	bool bAudioTrack;	// 1，针对视频水印 2，默认为false；不加载音频轨；3，在视频文件水印设置前有效
	int  iAudioVolume;	// 水印视频声音大小（0~100）

	TxeFillModeTag renderMode;		// 水印视频填充模式
}TXCGreenScreenParam;