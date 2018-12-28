//
//  ZegoChromaKey.h
//  zegoavkit
//
//  Created by Strong on 2018/1/24.
//  Copyright © 2017年 Zego. All rights reserved.
//

#ifndef ZEGO_CHROMA_KEY_H
#define ZEGO_CHROMA_KEY_H

#include "zego-api-defines.h"
#include "video_capture.h"

namespace ZEGO
{
	namespace CHROMA
	{
		struct ChromaKeyParameter {
			unsigned int similarity = 400;
			unsigned int smoothness = 80;
			unsigned int spill = 100;

			unsigned int opacity = 100;

			float contrast = 0.0;
			float brightness = 0.0;
			float gamma = 0.0;

			//green
			int keyColor = 0x00FF00;  
		};

		struct ChromaKeyRect {
			int x;
			int y;

			int width;
			int height;
		};

		enum ChromaKeyViewMode {
			ViewModeScaleAspectFit  = 0,	/**< 等比缩放，可能有黑边 */
			ViewModeScaleAspectFill = 1,    /**< 等比缩放填充整View，可能有部分被裁减 */
			ViewModeScaleToFill		= 2		/**< 填充整个View */
		};

		class IZegoChromaKeyCallback
		{
		public:
			virtual void OnFilterResult(
				const char *chromaKeyData,
				int dataLength,
				const AVE::VideoCaptureFormat &frameFormat) {}
		};

		ZEGOAVKIT_API bool Init();

		ZEGOAVKIT_API void UnInit();

		ZEGOAVKIT_API void SetChromaKeyCalback(IZegoChromaKeyCallback *callback);

		ZEGOAVKIT_API void UpdateChromaKeyParameter(struct ChromaKeyParameter *parameter);

		ZEGOAVKIT_API bool SetOutputDataSize(unsigned int width, unsigned int height);

		ZEGOAVKIT_API bool ProcessChromaKeyFilter(
			const char *chromaData, 
			int chromaLength, 
			const AVE::VideoCaptureFormat &chromaFormat,
			const char *backData,
			int backLength,
			const AVE::VideoCaptureFormat &backFormat);

		ZEGOAVKIT_API bool ProcessChopFilter(
			const char *chopData,
			int chopLength,
			const AVE::VideoCaptureFormat &chopFormat);

		ZEGOAVKIT_API void SetPictureInPictureViewMode(
			ChromaKeyViewMode mode);
		
		ZEGOAVKIT_API bool ProcessPictureInPicture(
			const char *frontData,
			int frontLength,
			const AVE::VideoCaptureFormat &frontFormat,
			struct ChromaKeyRect frontRect,
			const char *backData,
			int backLength,
			const AVE::VideoCaptureFormat &backFormat,
			struct ChromaKeyRect backRect);

		ZEGOAVKIT_API void ClearCacheData();
	}
}

#endif