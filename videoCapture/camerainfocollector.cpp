#include "camerainfocollector.h"

#include <uuids.h>
#include <amvideo.h>

namespace
{
	std::string wcharToString(LPWSTR lpcwszStr)
	{
		std::string str;
		size_t len = WideCharToMultiByte(CP_ACP, 0, lpcwszStr, wcslen(lpcwszStr), NULL, 0, NULL, NULL);
		char* m_char = new char[len + 1];
		WideCharToMultiByte(CP_ACP, 0, lpcwszStr, wcslen(lpcwszStr), m_char, len, NULL, NULL);
		m_char[len] = '\0';
		str = m_char;
		delete[] m_char;
		return str;
	}
}

CameraInfoCollector::CameraInfoCollector()
{

}

CameraInfoCollector::~CameraInfoCollector()
{

}

std::vector<CameraInfo> CameraInfoCollector::getCameraInfoList()
{
	std::vector<CameraInfo> nameList;
	HRESULT hr;

	ICreateDevEnum* pSysDevEnum = NULL;
	hr = CoCreateInstance(CLSID_SystemDeviceEnum, NULL, CLSCTX_INPROC_SERVER,
		IID_ICreateDevEnum, (void**)&pSysDevEnum);

	if (FAILED(hr))
	{
		pSysDevEnum->Release();
		return nameList;
	}

	IEnumMoniker* pEnumCat = NULL;
	hr = pSysDevEnum->CreateClassEnumerator(CLSID_VideoInputDeviceCategory, &pEnumCat, 0);

	if (FAILED(hr))
	{
		pSysDevEnum->Release();
		return nameList;
	}

	IMoniker* pMoniker = NULL;
	ULONG cFetched;
	auto index = 0;
	while (pEnumCat->Next(1, &pMoniker, &cFetched) == S_OK)
	{
		IPropertyBag* pPropBag;
		hr = pMoniker->BindToStorage(0, 0, IID_IPropertyBag, (void**)&pPropBag);
		if (SUCCEEDED(hr))
		{
			IBaseFilter* pFilter;
			hr = pMoniker->BindToObject(NULL, NULL, IID_IBaseFilter, (void**)&pFilter);
			if (!pFilter)
			{
				pMoniker->Release();
				break;
			}

			VARIANT varName;
			VariantInit(&varName);
			hr = pPropBag->Read(L"FriendlyName", &varName, 0);
			if (SUCCEEDED(hr))
			{
				CameraInfo info;
				info.resolutionList = getCameraSupportResolutions(pFilter);
				info.cameraName = wcharToString(varName.bstrVal);

				nameList.push_back(info);
			}
			VariantClear(&varName);
			pFilter->Release();
			pPropBag->Release();
		}
		pMoniker->Release();
	}
	pEnumCat->Release();

	return nameList;
}

std::vector<std::pair<int, int>> CameraInfoCollector::getCameraSupportResolutions(IBaseFilter* pBaseFilter)
{
	HRESULT hr = 0;
	std::vector<IPin*> pins;
	IEnumPins* EnumPins;
	pBaseFilter->EnumPins(&EnumPins);
	pins.clear();

	std::vector<std::pair<int, int>> result;

	for (;;)
	{
		IPin* pin;
		hr = EnumPins->Next(1, &pin, NULL);
		if (hr != S_OK)
		{
			break;
		}
		pins.push_back(pin);
		pin->Release();
	}

	EnumPins->Release();

	PIN_INFO pInfo;
	for (int i = 0; i < pins.size(); i++)
	{
		if (nullptr == pins[i])
		{
			break;
		}
		pins[i]->QueryPinInfo(&pInfo);

		IEnumMediaTypes* emt = NULL;
		pins[i]->EnumMediaTypes(&emt);
		AM_MEDIA_TYPE* pmt;

		for (;;)
		{
			hr = emt->Next(1, &pmt, NULL);
			if (hr != S_OK)
			{
				break;
			}
			if ((pmt->formattype == FORMAT_VideoInfo)
				//&& (pmt->subtype == MEDIASUBTYPE_RGB24)
				&& (pmt->cbFormat >= sizeof(VIDEOINFOHEADER))
				&& (pmt->pbFormat != NULL)) {

				VIDEOINFOHEADER* pVIH = (VIDEOINFOHEADER*)pmt->pbFormat;

				auto insertParam = std::pair<int, int>{ pVIH->bmiHeader.biWidth, pVIH->bmiHeader.biHeight };
				bool isSet = false;

				for (auto param : result)
				{
					if (param.first == insertParam.first && param.second == insertParam.second)
					{
						isSet = true;
						break;
					}
				}

				if (!isSet)
				{
					result.push_back(insertParam);
				}
			}

			if (pmt->cbFormat != 0)
			{
				CoTaskMemFree((PVOID)pmt->pbFormat);
				pmt->cbFormat = 0;
				pmt->pbFormat = NULL;
			}
			if (pmt->pUnk != NULL)
			{
				// pUnk should not be used.
				pmt->pUnk->Release();
				pmt->pUnk = NULL;
			}
		}
		break;
		emt->Release();
	}

	return result;
}