#include "stdafx.h"
#include "InterfaceSpec.h"
#include "ScreenUtil.h"
#include "ImCharset.h"
#include "DeviceSetting.h"

PB_RESULT BERR_Initialize(IN LPCSTR lpszConfigXml)
{
    if (lpszConfigXml == nullptr || strlen(lpszConfigXml) == 0)
    {
        LOG_ERROR(L"BERR_Initialize lpszConfigXml param invalid");
        return PB_BERR_INVALID_CONFIG_XML;
    }    

    //设置日志级别
    CDeviceSetting devSetting;
    if (!devSetting.Parse(lpszConfigXml))
    {
        LOG_ERROR(L"BERR_Initialize lpszConfigXml param invalid");
        return PB_BERR_INVALID_CONFIG_XML;
    }

    if (g_dllLog)
    {
        g_dllLog->SetLogLevel(devSetting.m_logLevel);
        LOG_DEBUG(L"set log level %d", (int)devSetting.m_logLevel);
    }

    //记录输入参数
    LOG_INFO(L"BERR_Initialize lpszConfigXml is %s", CImCharset::UTF8ToUnicode(lpszConfigXml).c_str());

    //打开设备
    PB_RESULT nResult = CScreenUtil::GetInstance()->Open(DEVICE_NAME, lpszConfigXml);
    if (nResult != PB_OK)
    {
        LOG_ERROR(L"BERR_Initialize failed, result=%d", nResult);
    }
    else
    {
        LOG_INFO(L"BERR_Initialize success");
    }

    return nResult;
}

PB_RESULT BERR_Show(IN LPCSTR lpszCounterNo, IN LPCSTR lpszCommandXml)
{
    if (lpszCounterNo == nullptr || strlen(lpszCounterNo) == 0)
    {
        LOG_ERROR(L"BERR_Show lpszCounterNo param invalid");
        return PB_BERR_INVALID_ADDRESS;
    }

    if (lpszCommandXml == nullptr || strlen(lpszCommandXml) == 0)
    {
        LOG_ERROR(L"BERR_Show lpszCommandXml param invalid");
        return PB_BERR_INVALID_COMMAND_XML;
    }

    //记录输入参数
    std::wstring strCommandXml = CImCharset::AnsiToUnicode(lpszCommandXml).c_str();
    LOG_INFO(L"BERR_Show lpszCounterNo is %s", CImCharset::UTF8ToUnicode(lpszCounterNo).c_str());
    LOG_INFO(L"BERR_Show lpszCommandXml is %s", strCommandXml.c_str());

    PB_RESULT nResult = CScreenUtil::GetInstance()->ShowWindowScreenText(lpszCounterNo, CImCharset::UnicodeToUTF8(strCommandXml.c_str()).c_str());
    if (nResult != PB_OK)
    {
        LOG_ERROR(L"BERR_Show failed, result=%d", nResult);
    }
    else
    {
        LOG_INFO(L"BERR_Show success");
    }

    return nResult;
}

PB_RESULT BERR_GetStatus(OUT PB_STATUS *lpStatus)
{
    if (lpStatus == nullptr)
    {
        LOG_ERROR(L"BERR_GetStatus lpStatus param invalid");
        return PB_BERR_NOT_SUPPORTED;  //没有合适的预定义错误返回值
    }

    //记录输入参数
    LOG_INFO(L"BERR_GetStatus is called");

    PB_RESULT nResult = CScreenUtil::GetInstance()->GetStatus(lpStatus);
    if (nResult != PB_OK)
    {
        LOG_ERROR(L"BERR_GetStatus failed, result=%d", nResult);
    }
    else
    {
        LOG_INFO(L"BERR_GetStatus success");
    }

    return nResult;
}
