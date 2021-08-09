#include "windows.h"
#include "interface/videocaptureinterface.h"
#include "videocapture.h"

namespace
{
    static VideoCapture* k_videoCapture = nullptr;
}

extern "C" DLL_API VideoCaptureInterface * CreateVideoCapture()
{
    if (nullptr == k_videoCapture)
    {
        k_videoCapture = new VideoCapture();
    }
    return k_videoCapture;
}
