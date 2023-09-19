#pragma once
#include <wtypes.h>
#include "Utility.h"

class UTILITY_API IOnSerialDataArrive
{
public:
    virtual void OnSerialDataArrive(const char* pData, int nDataLength) = 0;
};

class UTILITY_API ISerialUtil
{
public:
    virtual ~ISerialUtil() {}

public:
    /**
    @name 打开串口
    @param dwPort 串口号
    @param szSetStr 串口配置串，如"9600, 8, n, 1" 中9600代表波特率、n代表奇偶校验、8代表数据位、1代表停止位
    @param dwDesiredAccess 访问权限
    */
    virtual bool Open(DWORD dwPort, char *szSetStr, DWORD dwDesiredAccess) = 0;

    /**
    @name 设置串口数据到达回调
    @param pCallback 回调
    */
    virtual void SetSerialDataArriveCallback(IOnSerialDataArrive* pCallback) = 0;

    /**
    @name 发送数据
    @param szBuffer 缓冲区
    @param dwBufferLength 缓冲区大小
    @return 实际发送的字节数
    */
    virtual DWORD Write(char *szBuffer, DWORD dwBufferLength) = 0;

    /**
    @name 关闭串口
    */
    virtual void Close() = 0;
};