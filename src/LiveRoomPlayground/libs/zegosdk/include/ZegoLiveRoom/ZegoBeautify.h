//
//  ZegoBGeautify.h
//  zegoavkit
//
//  Created by Strong on 2018/2/22.
//  Copyright © 2017年 Zego. All rights reserved.
//

#ifndef ZEGO_BEAUTIFY_H
#define ZEGO_BEAUTIFY_H

#include "zego-api-defines.h"
#include "video_capture.h"

namespace ZEGO
{
	namespace BEAUTIFY
	{
		//beautify
		enum ZEGO_FILTER_BEAUTIFY_FEATURE
		{
			ZEGO_FILTER_BEAUTIFY_NONE = 0,
			ZEGO_FILTER_BEAUTIFY_POLISH = 1,
			ZEGO_FILTER_BEAUTIFY_WHITEN = 1 << 1,
			ZEGO_FILTER_BEAUTIFY_SKINWHITEN = 1 << 2,
			ZEGO_FILTER_BEAUTIFY_SHARPPEN = 1 << 3
		};

		enum ZEGO_FILTER_CUSTOM_TYPE
		{
			ZEGO_FILTER_CUSTOM_NONE = 0,			///< 不使用滤镜
			ZEGO_FILTER_CUSTOM_LOMO = 1,			///< 简洁
			ZEGO_FILTER_CUSTOM_BLACKWHITE = 2,		///< 黑白
			ZEGO_FILTER_CUSTOM_OLDSTYLE = 3,		///< 老化
			ZEGO_FILTER_CUSTOM_GOTHIC = 4,			///< 哥特
			ZEGO_FILTER_CUSTOM_SHARPCOLOR = 5,		///< 锐色
			ZEGO_FILTER_CUSTOM_FADE = 6,			///< 淡雅
			ZEGO_FILTER_CUSTOM_WINE = 7,			///< 酒红
			ZEGO_FILTER_CUSTOM_LIME = 8,			///< 青柠
			ZEGO_FILTER_CUSTOM_ROMANTIC = 9,		///< 浪漫
			ZEGO_FILTER_CUSTOM_HALO = 10,			///< 光晕
			ZEGO_FILTER_CUSTOM_BLUE = 11,			///< 蓝调
			ZEGO_FILTER_CUSTOM_ILLUSION = 12,		///< 梦幻
			ZEGO_FILTER_CUSTOM_DARK = 13			///< 夜色
		};

		class IZegoBeautifyCallback
		{
		public:
			virtual void OnBeautifyResult(
				const char *beautifyData,
				int dataLength,
				const AVE::VideoCaptureFormat &frameFormat) {}
		};

		ZEGOAVKIT_API bool Init();

		ZEGOAVKIT_API void UnInit();

		ZEGOAVKIT_API void SetBeautifyCallback(IZegoBeautifyCallback *pCallback);

		ZEGOAVKIT_API bool EnableBeautifying(int feature);

		ZEGOAVKIT_API bool SetCustomizedFilter(ZEGO_FILTER_CUSTOM_TYPE type);

		ZEGOAVKIT_API bool SetPolishStep(float step);

		ZEGOAVKIT_API bool SetPolishFactor(float factor);

		ZEGOAVKIT_API bool SetWhitenFactor(float factor);

		ZEGOAVKIT_API bool SetSharpenFactor(float factor);

		ZEGOAVKIT_API bool SetSkinWhitenFactor(float factor);

		ZEGOAVKIT_API bool ProcessBeautifyFilter(
			const char *camearData,
			int dataLength,
			const AVE::VideoCaptureFormat &frameFormat
		);
	}
}
#endif