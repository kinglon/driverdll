#include "stdafx.h"
#include "InterfaceSpec.h"
#include "DeviceSetting.h"
#include "ImCharset.h"
#include "CrdrManager.h"

PB_RESULT PB_API CRDR_Initialize(IN LPCSTR lpszConfigXml)
{
    if (lpszConfigXml == nullptr || strlen(lpszConfigXml) == 0)
    {
        LOG_ERROR(L"CRDR_Initialize lpszConfigXml param invalid");
        return PB_CRDR_INVALID_CONFIG_XML;
    }

    //设置日志级别
    CDeviceSetting devSetting;
    if (!devSetting.Parse(lpszConfigXml))
    {
        LOG_ERROR(L"CRDR_Initialize lpszConfigXml param invalid");
        return PB_CRDR_INVALID_CONFIG_XML;
    }

    if (g_dllLog)
    {
        g_dllLog->SetLogLevel(devSetting.m_logLevel);
        LOG_DEBUG(L"set log level %d", (int)devSetting.m_logLevel);
    }

    //记录输入参数
    LOG_INFO(L"CRDR_Initialize lpszConfigXml is %s", CImCharset::UTF8ToUnicode(lpszConfigXml).c_str());

    int nResult = CCrdrManager::GetInstance()->Init();
    if (nResult == PB_OK)
    {
        LOG_INFO(L"succeed to init crdr");
    }
    else
    {
        LOG_ERROR(L"failed to init crdr, error=%d", nResult);
    }

    return nResult;
}

PB_RESULT PB_API CRDR_BeginRead(IN DWORD dwTrack)
{
    LOG_INFO(L"begin read, track=%d", dwTrack);
    if (dwTrack != PB_CRDR_TRACK2 && dwTrack != PB_CRDR_TRACK3 && dwTrack != PB_CRDR_TRACK2_AND_TRACK3)
    {
        LOG_ERROR(L"begin read, invalid track, track=%d", dwTrack);
        CRDR_ASSERT(false);
        return PB_CRDR_INVALID_CONFIG_XML;
    }

    int nResult = CCrdrManager::GetInstance()->BeginRead(dwTrack);
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

PB_RESULT PB_API CRDR_DataAvailable(OUT BOOL *lpAvailable)
{
    if (lpAvailable == nullptr)
    {
        LOG_ERROR(L"CRDR_DataAvailable invalid param");
        return PB_CRDR_INVALID_CONFIG_XML;
    }

    return CCrdrManager::GetInstance()->DataAvailable(lpAvailable);
}

PB_RESULT PB_API CRDR_ReadData(OUT LPVOID lpBuffer, IN DWORD dwBufferSize, OUT DWORD *lpNumberOfBytesRead)
{ 
    LOG_INFO(L"call CRDR_ReadData");
    return CCrdrManager::GetInstance()->ReadData(lpBuffer, dwBufferSize, lpNumberOfBytesRead);
}

PB_RESULT PB_API CRDR_CancelRead(void)
{
    LOG_INFO(L"call CRDR_CancelRead");
    return CCrdrManager::GetInstance()->CancelRead();
}

PB_RESULT PB_API CRDR_GetStatus(OUT PB_STATUS *lpStatus)
{
    if (lpStatus == nullptr)
    {
        LOG_ERROR(L"CRDR_GetStatus lpStatus param invalid");
        return PB_CRDR_INVALID_CONFIG_XML;  //没有合适的预定义错误返回值
    }

    //记录输入参数
    LOG_INFO(L"CRDR_GetStatus is called");
    return CCrdrManager::GetInstance()->GetStatus(lpStatus);
}