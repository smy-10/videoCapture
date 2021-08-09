#pragma once

#include "interface/videocaptureinterface.h"

class VideoCaptureImp;
class CameraInfoCollector;
class VideoCapture : public VideoCaptureInterface
{
public:
	VideoCapture();
	~VideoCapture();

	virtual bool open(const std::string& name, const std::pair<int, int>& resolution) override;
	virtual void close() override;
	virtual std::vector<CameraInfo> getCameraInfoList() override;
	virtual void setCaptureFrameCallBack(const std::function<void(const cv::Mat& frame)>& callBack) override;

private:
	CameraInfoCollector* m_cameraInfoCollector = nullptr;
	VideoCaptureImp* m_videoCaptureImp = nullptr;
	std::vector<CameraInfo> m_cameraInfoVec;
};

