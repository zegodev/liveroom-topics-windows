### Windows-ilivefilter v3.3.9 使用说明
备注：
1，如果使用 ilivefilter 插件美颜；建议关闭AVSDK 内置美颜；避免功能重复，带来不必要的性能开销

最新版本说明
> V3.3.9(2019-3-20)</br>
(1) 修改TILFilter 构造函数，单独线程获取opengl版本号，规避构造函数opengl环境冲突
</br>

###工程配置
<pre>
(1) vs 属性-->链接器-->输入-->添加 ilivefiltersdk.lib 依赖
(2) 将 ilivefiltersdk.dll 拷贝到运行目录
(3) 包含 ilivefiltersdk.h 和 TXCVPComomDefine.h 头文件
</pre>

###使用代码范例：
1，创建预处理类 并且设置参数
<pre>#include "ilivefiltersdk.h"

TILFilter* m_TILFilter = nullptr;

if (nullptr == m_TILFilter) {
        m_TILFilter = new TILFilter();	// 只能处理原始数据（I420）
	
	// m_TILFilter = new TILFilter(true);// new TILFilter(true)，可以处理 纹理textureID 输入

       char szFile[128] = "E:\\green-screen.mp4";
	m_TILFilter->setGreenScreenFile(szFile, strlen(szFile), true);	// 设置绿幕
	m_TILFilter->setBeauty(5);	                                // 设置美颜
	m_TILFilter->setWhite(3);                                       // 设置美白
	m_TILFilter->setSharpness(3);                                   // 设置清晰度
}
</pre>
2，预处理数据
<pre>
	// 处理原始数据（I420）（new TILFilter() 使用）
	if (nullptr != m_TILFilter) {
 		m_TILFilter->processData((unsigned char*)frame->data, frame->length, frame->width, frame->height, VIDEOPROCESS_FORMAT_I420);
 	}

	// 处理纹理数据 textureID（new TILFilter(true) 才能使用）
       if (nullptr != m_TILFilter) {
		frame->textureId = m_TILFilter->processData(frame->textureId, frame->length, frame->width, frame->height, VIDEOPROCESS_FORMAT_I420);	// 处理后的textureID通过返回值返回
	}
</pre>

3，释放预处理类
<pre>
    if (nullptr != m_TILFilter) {
		m_TILFilter->destroyFilter();
		delete m_TILFilter;
		m_TILFilter = nullptr;
	}
</pre>

##sdk接口说明
    /**
	* 构造函数; 默认为无GL环境；只支持原始数据处理
	*/
	TILFilter();
	/**
	* 构造函数
	* @param bGLContext：false: 无GL 环境，只支持原始数据处理   true: 有GL环境；sdk支持纹理输入处理
	*/
	TILFilter(bool bGLContext);
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