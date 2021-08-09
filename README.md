# videoCapture

## 介绍:
Windows 平台下 基于openvc400 的视频捕获模块

### 功能:

1.获取摄像头列表+摄系统支持的分辨率列表

2.线程采集摄系统,视频frame通过callback回传

3.代码量较少接入方便,一个dll+接口


### 编译:
打开videoCapture.sln 通过vs编译,由于git上传文件大小限制,opencv400.dll没有上传,需要自己链接opencv400.dll
