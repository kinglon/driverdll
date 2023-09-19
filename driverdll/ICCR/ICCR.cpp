#include "stdafx.h"
#include "InterfaceSpec.h"
#include "DeviceSetting.h"
#include "ImCharset.h"
#include "IccrManager.h"

PB_RESULT PB_API ICCR_Initialize(IN LPCSTR lpszConfigXml)
{
    if (lpszConfigXml == nullptr || strlen(lpszConfigXml) == 0)
    {
        LOG_ERROR(L"ICCR_Initialize lpszConfigXml param invalid");
        return PB_ICCR_INVALID_CONFIG_XML;
    }

    //设置日志级别
    CDeviceSetting devSetting;
    if (!devSetting.Parse(lpszConfigXml))
    {
        LOG_ERROR(L"ICCR_Initialize lpszConfigXml param invalid");
        return PB_ICCR_INVALID_CONFIG_XML;
    }

    if (g_dllLog)
    {
        g_dllLog->SetLogLevel(devSetting.m_logLevel);
        LOG_DEBUG(L"set log level %d", (int)devSetting.m_logLevel);
    }

    //记录输入参数
    LOG_INFO(L"ICCR_Initialize lpszConfigXml is %s", CImCharset::UTF8ToUnicode(lpszConfigXml).c_str());

    int nResult = CIccrManager::GetInstance()->Init();
    if (nResult == PB_OK)
    {
        LOG_INFO(L"succeed to init iccr");
    }
    else
    {
        LOG_ERROR(L"failed to init iccr, error=%d", nResult);
    }

    return nResult;
}

PB_RESULT PB_API ICCR_BeginRead()
{
    LOG_INFO(L"begin read");  

    int nResult = CIccrManager::GetInstance()->BeginRead();
    if (nResult == PB_OK)
    {
        LOG_INFO(L"succeed to begin read");
    }
    else
    {
        LOG_ERROR(L"failed to begin read, error=%d", nResult);
    }

    return nResult;
}

PB_RESULT PB_API ICCR_DataAvailable(OUT BOOL *lpAvailable)
{
    if (lpAvailable == nullptr)
    {
        LOG_ERROR(L"ICCR_DataAvailable invalid param");
        return PB_ICCR_INVALID_CONFIG_XML;
    }

    return CIccrManager::GetInstance()->DataAvailable(lpAvailable);
}

PB_RESULT PB_API ICCR_ReadData(IN DWORD dwIndex, OUT LPVOID lpBuffer, IN DWORD dwBufferSize, OUT DWORD * lpNumberOfBytesRead)
{
    LOG_INFO(L"call ICCR_ReadData");
    return CIccrManager::GetInstance()->ReadData(dwIndex, lpBuffer, dwBufferSize, lpNumberOfBytesRead);
}

PB_RESULT PB_API ICCR_CancelRead(void)
{
    LOG_INFO(L"call ICCR_CancelRead");
    return CIccrManager::GetInstance()->CancelRead();
}

PB_RESULT PB_API ICCR_GetStatus(OUT PB_STATUS *lpStatus)
{
    if (lpStatus == nullptr)
    {
        LOG_ERROR(L"ICCR_GetStatus lpStatus param invalid");
        return PB_ICCR_INVALID_CONFIG_XML;  //没有合适的预定义错误返回值
    }

    //记录输入参数
    LOG_INFO(L"ICCR_GetStatus is called");
    return CIccrManager::GetInstance()->GetStatus(lpStatus);
}