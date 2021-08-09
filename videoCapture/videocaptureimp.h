#pragma once

#include "opencv2/core.hpp"
#include "opencv2/opencv.hpp"
#include "opencv2/highgui.hpp"

#include <functional>

class VideoCaptureImp
{
public:

	VideoCaptureImp();
	~VideoCaptureImp();

	bool open(int camId, const std::pair<int, int>& resolution);
	void close();

	void setCaptureFrameCallBack(const std::function<void(const cv::Mat& frame)>& callBack);

private:
	bool startCapture();
	void threadEntry();

private:
	cv::VideoCapture m_cvVideoCapture;
	cv::Mat m_frame;
	std::thread m_captureThread;
	std::function<void(const cv::Mat& frame)> m_getFrameCallBack = nullptr;
	
	std::atomic<bool> m_stopFlag = false;
};