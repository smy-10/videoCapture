#include "videocapture.h"
#include "camerainfocollector.h"
#include "videocaptureimp.h"

VideoCapture::VideoCapture()
{
	m_cameraInfoCollector = new CameraInfoCollector();
	m_videoCaptureImp = new VideoCaptureImp();
}

VideoCapture::~VideoCapture()
{
	if (m_cameraInfoCollector)
	{
		delete m_cameraInfoCollector;
		m_cameraInfoCollector = nullptr;
	}

	if(m_videoCaptureImp)
	{
		delete m_videoCaptureImp;
		m_videoCaptureImp = nullptr;
	}

	m_cameraInfoVec.clear();
}

bool VideoCapture::open(const std::string& name, const std::pair<int, int>& resolution)
{
	bool ret = false;

	if (name.empty() || resolution.first == 0 || resolution.second == 0)
	{
		return ret;
	}

	if (m_videoCaptureImp)
	{
		int camId = 0;
		for (int i = 0; i < m_cameraInfoVec.size(); i++)
		{
			if (name == m_cameraInfoVec[i].cameraName)
			{
				camId = i;
				break;
			}
		}
		
		ret = m_videoCaptureImp->open(camId, resolution);
	}

	return ret;
}

void VideoCapture::close()
{
	if (m_videoCaptureImp)
	{
		m_videoCaptureImp->close();
	}
}

std::vector<CameraInfo> VideoCapture::getCameraInfoList()
{
	m_cameraInfoVec.clear();

	if (m_cameraInfoCollector)
	{
		m_cameraInfoVec = std::move(m_cameraInfoCollector->getCameraInfoList());
	}

	return m_cameraInfoVec;
}

void VideoCapture::setCaptureFrameCallBack(const std::function<void(const cv::Mat& frame)>& callBack)
{
	if (m_videoCaptureImp)
	{
		m_videoCaptureImp->setCaptureFrameCallBack(callBack);
	}
}
