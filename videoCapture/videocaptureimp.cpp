#include "videocaptureimp.h"

namespace
{
	const int k_waitMsTime = 30;
	const int k_mjpgFourccCode = cv::VideoWriter::fourcc('M', 'J', 'P', 'G');
}

VideoCaptureImp::VideoCaptureImp()
{

}

VideoCaptureImp::~VideoCaptureImp()
{
	close();

	m_stopFlag = true;

	if (m_captureThread.joinable())
	{
		m_captureThread.join();
	}

	if (m_getFrameCallBack)
	{
		m_getFrameCallBack = nullptr;
	}

}

bool VideoCaptureImp::open(int camId, const std::pair<int, int>& resolution)
{
	if (m_cvVideoCapture.isOpened())
	{
		close();
	}

	if (m_cvVideoCapture.open(cv::CAP_DSHOW + camId))
	{
		m_cvVideoCapture.set(cv::CAP_PROP_FOURCC, k_mjpgFourccCode);
		m_cvVideoCapture.set(cv::CAP_PROP_FRAME_WIDTH, resolution.first);
		m_cvVideoCapture.set(cv::CAP_PROP_FRAME_HEIGHT, resolution.second);
		m_cvVideoCapture.set(cv::CAP_PROP_AUTOFOCUS, 0);
	}

	return startCapture();
}

void VideoCaptureImp::close()
{
	if (m_cvVideoCapture.isOpened())
	{
		m_cvVideoCapture.release();
	}
}

void VideoCaptureImp::setCaptureFrameCallBack(const std::function<void(const cv::Mat& frame)>& callBack)
{
	m_getFrameCallBack = callBack;
}

bool VideoCaptureImp::startCapture()
{
	bool ret = false;

	if (false == m_cvVideoCapture.isOpened())
	{
		return ret;
	}

	if (false == m_captureThread.joinable())
	{
		m_captureThread = std::thread(&VideoCaptureImp::threadEntry, this);
	}

	return true;
}

void VideoCaptureImp::threadEntry()
{
	while (false == m_stopFlag)
	{
		if (m_cvVideoCapture.grab() && m_cvVideoCapture.isOpened())
		{
			m_cvVideoCapture.retrieve(m_frame);

			if (m_getFrameCallBack)
			{
				m_getFrameCallBack(m_frame);
			}
		}
		cv::waitKey(k_waitMsTime);
	}
}
