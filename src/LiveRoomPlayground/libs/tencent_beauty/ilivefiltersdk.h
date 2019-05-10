// ilivefiltersdk.h

#pragma once

#ifdef TXCVPCOMBINE
#define DLLIMEXPORT __declspec(dllexport)
#else
#define DLLIMEXPORT
#endif // TXCVPCOMBINE

class CGLDrawThread;

#ifndef VIDEOPROCESS_NO_OUTPUT
#define VIDEOPROCESS_NO_OUTPUT		-1
#endif
#ifndef VIDEOPROCESS_FORMAT_TEXTURE2D
#define VIDEOPROCESS_FORMAT_TEXTURE2D  0
#endif
#ifndef VIDEOPROCESS_FORMAT_I420
#define VIDEOPROCESS_FORMAT_I420  1
#endif
#ifndef VIDEOPROCESS_FORMAT_NV12
#define VIDEOPROCESS_FORMAT_NV12  2
#endif
#ifndef VIDEOPROCESS_FORMAT_NV21
#define VIDEOPROCESS_FORMAT_NV21  3
#endif
#ifndef VIDEOPROCESS_FORMAT_RGB
#define VIDEOPROCESS_FORMAT_RGB   4
#endif
#ifndef VIDEOPROCESS_FORMAT_YUY2
#define VIDEOPROCESS_FORMAT_YUY2  5
#endif
#ifndef VIDEOPROCESS_FORMAT_RGBA
#define VIDEOPROCESS_FORMAT_RGBA  6
#endif
#ifndef VIDEOPROCESS_FORMAT_BGR
#define VIDEOPROCESS_FORMAT_BGR	  7
#endif
#ifndef VIDEOPROCESS_FORMAT_XRGB
#define VIDEOPROCESS_FORMAT_XRGB  8
#endif
#ifndef VIDEOPROCESS_FORMAT_BGRA
#define VIDEOPROCESS_FORMAT_BGRA  9
#endif
class CTXCVPLock;
class CTXINotifyListener;
class CTXMVideoPreprocessor;
struct TXCGreenScreenParam;

class DLLIMEXPORT TILFilter{
public:
	/**
	* 构造函数; 默认为无GL环境；只支持原始数据处理
	*/
	TILFilter();
	/**
	* 构造函数
	* @param bGLContext：false: 无GL 环境，只支持原始数据处理   true: 有GL环境；sdk支持纹理输入处理
	*/
	TILFilter(bool bGLContext);
	~TILFilter();
public:
	/**
	* 单例（同时也支持多实例）
	*/
	static TILFilter* getInstance();
	
public:
	/**
	* 设置滤镜类型（暂未实现）
	*/
	int setFilter(int filter);

	/**
	* 预处理数据
	* @param data：输入/输出 数据
	* @param len：数据长度
	* @param width：数据宽度
	* @param height：数据高度
	* @param type：输入格式(I420数据为：VIDEOPROCESS_FORMAT_I420)
	*/
	int processData(unsigned char* data, int len, int width, int height, int type);

	/**
	* 预处理数据
	* @param textureId：纹理输入/输出 数据
	* @param len：数据长度
	* @param width：数据宽度
	* @param height：数据高度
	* @param type：输入格式（VIDEOPROCESS_FORMAT_TEXTURE2D）
	* @return: 返回值为 sdk处理后的纹理 id
	*/
	int processData(int textureId, int len, int width, int height, int type);

	/**
	* 事件通知回调
	* @param   notify：回调类指针
	*/
	void setNotify(CTXINotifyListener* notify);

	/**
	* 设置美颜程度
	* @param blur： 美颜程度（0~9）建议设置为5
	*/
	void setBeauty(int blur);

	/**
	* 设置美白程度
	* @param white： 美白程度（0~9）建议设置为3
	*/
	void setWhite(int white);
	/**
	* 设置清晰度
	* @param sharpness： 美颜程度（0~9）建议设置为2
	*/
	void setSharpness(int sharpness);
	/**
	* 美颜风格
	* @param style： 0：光滑  1：自然  2：朦胧
	*/
	void setBeautyStyle(int style);
	/**
	* 设置水印
	* @param filePath 水印文件路径；支持 BMP, GIF, JPEG, PNG, TIFF, Exif, WMF, and EMF 格式；如果传 NULL，则代表去掉水印
	* @param xOffset  左上角x轴偏移
	* @param yOffset  左上角y轴偏移
	* @param fWidthRatio 水印占 x轴 的比例
	*/
	void setWaterMark(const char* filePath, float xOffset, float yOffset, float fWidthRatio);
	
	/**
	* 设置绿幕背景
	* @param filePath 绿幕背景文件路径(""代表取消绿幕)；支持 BMP, GIF, JPEG, PNG, TIFF, Exif, WMF, and EMF 格式/mp4格式
	* @param pathLen  路径的长度
	* @param isLoop  是否循环播放视频文件（，暂时不支持视频格式，预留）
	*/
	void setGreenScreenFile(const char* filePath, int pathLen, bool isLoop);

	/**
	* 设置绿幕背景参数（如果不需要改变绿幕的特性，可以不调用）
	* @param   param.bAudioTrack : 是否加载音频轨（true： 加载   false：不加载）
	*		   param.iAudioVolume : 设置绿幕视频声音（前提是加载了音频轨，才有效） 范围：0~100（0代表静音， 100最大音量）
	*		   param.renderMode :	绿幕视频填充模式 （TXE_FILL_MODE_SCALE_TO_FILL：展示所有视频；不考虑变形  TXE_FILL_MODE_SCALL_ASPECT_FILL：自适应填充整个屏幕）
	*/	
	void setGreenScreenParam(TXCGreenScreenParam* param);

	/**
	* 获取 sdk 是否支持预处理（不支持 Opengl 2.0 以下显卡）
	*/
	bool getSDKSupport();
	/**
	* 销毁sdk资源
	*/
	void destroyFilter();

private:
	int m_width;
	int m_height;
	int m_beautyLevel;
	int m_whiteLevel;
	int m_shapnessLevel;
	int m_BeautyStyle;

	// 水印相关
	std::string m_WaterMarkPath;
	float m_xOffset;
	float m_yOffset;
	float m_fWidthRatio;

	// 绿幕相关
	std::string m_GreenScreenPath;
	bool m_bGreenLoop;

	CTXCVPLock* m_lockFilter;
	CGLDrawThread* m_glDrawThread;
	TXCGreenScreenParam* m_TxcGreenParam;
	CTXINotifyListener* mNotifyListen;
	// dua 上报控制变量
	bool m_bReportDua;
	bool m_bSetBautyDua;
	bool m_bSetWhiteDua;
	bool m_bSetSharpDua;

	bool m_bSetGreenScreenDua;

	bool m_bFirstFrame;
	int m_iRealFps;
	long m_startTime;

	bool m_bSDKSupport;

	bool m_bIfGLContext;

	CTXMVideoPreprocessor* mTxcVideoProcessor;
};