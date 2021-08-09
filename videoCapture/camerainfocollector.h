#pragma once

#include "interface/videocaptureinterface.h"
#include <vector>
#include <strmif.h>

class CameraInfoCollector
{
public:
	CameraInfoCollector();
	~CameraInfoCollector();

	std::vector<CameraInfo> getCameraInfoList();
	std::vector<std::pair<int, int>> getCameraSupportResolutions(IBaseFilter* pBaseFilter);
};