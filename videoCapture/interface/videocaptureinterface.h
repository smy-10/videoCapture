#pragma once

#ifdef DLL_EXPORTS
#define DLL_API _declspec(dllexport)
#else
#define DLL_API _declspec(dllimport)
#endif

#include <iostream>
#include <string>
#include <vector>
#include <functional>
#include "opencv2/opencv.hpp"

struct CameraInfo
{
	std::string cameraName;
	std::vector<std::pair<int, int>> resolutionList;

	CameraInfo()
	{
		resolutionList.clear();
	}
	~CameraInfo()
	{
		resolutionList.clear();
	}
};

class VideoCaptureInterface
{
public:
	virtual bool open(const std::string& name, const std::pair<int ,int>& resolution) = 0;
	virtual void close() = 0;

	virtual std::vector<CameraInfo> getCameraInfoList() = 0;

	virtual void setCaptureFrameCallBack(const std::function<void(const cv::Mat& frame)>& callBack) = 0;
};

extern "C" DLL_API VideoCaptureInterface* CreateVideoCapture();