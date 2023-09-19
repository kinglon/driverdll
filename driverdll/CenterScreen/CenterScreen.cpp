#include "stdafx.h"
#include "CenterScreen.h"
#include "InterfaceSpec.h"
#include "DeviceSetting.h"
#include "ImCharset.h"
#include "..\ScreenUtil\ScreenUtil.h"

PB_RESULT COMP_Initialize(IN LPCSTR lpszConfigXml)
{
    if (lpszConfigXml == nullptr || strlen(lpszConfigXml) == 0)
    {
        LOG_ERROR(L"COMP_Initialize lpszConfigXml param invalid");
        return PB_COMP_INVALID_CONFIG_XML;
    }

    //设置日志级别
    CDeviceSetting devSetting;
    if (!devSetting.Parse(lpszConfigXml))
    {
        LOG_ERROR(L"COMP_Initialize lpszConfigXml param invalid");
        return PB_COMP_INVALID_CONFIG_XML;
    }

    if (g_dllLog)
    {
        g_dllLog->SetLogLevel(devSetting.m_logLevel);
        LOG_DEBUG(L"set log level %d", (int)devSetting.m_logLevel);
    }

    //记录输入参数
    LOG_INFO(L"COMP_Initialize lpszConfigXml is %s", CImCharset::UTF8ToUnicode(lpszConfigXml).c_str());

    //打开设备
    PB_RESULT nResult = CScreenUtil::GetInstance()->Open(DEVICE_NAME, lpszConfigXml);
    if (nResult != PB_OK)
    {
        LOG_ERROR(L"COMP_Initialize failed, result=%d", nResult);
    }
    else
    {
        LOG_INFO(L"COMP_Initialize success");
    }

    return nResult;
}

PB_RESULT COMP_Show(IN LPCSTR lpszAddress, IN LPCSTR lpszCommandXml)
{
    if (lpszAddress == nullptr || strlen(lpszAddress) == 0)
    {
        LOG_ERROR(L"COMP_Show lpszAddress param invalid");
        return PB_COMP_INVALID_ADDRESS;
    }

    if (lpszCommandXml == nullptr || strlen(lpszCommandXml) == 0)
    {
        LOG_ERROR(L"COMP_Show lpszCommandXml param invalid");
        return PB_COMP_INVALID_COMMAND_XML;
    }

    //记录输入参数
    std::wstring strCommandXml = CImCharset::AnsiToUnicode(lpszCommandXml).c_str();
    LOG_INFO(L"COMP_Show lpszAddress is %s", CImCharset::UTF8ToUnicode(lpszAddress).c_str());
    LOG_INFO(L"COMP_Show lpszCommandXml is %s", strCommandXml.c_str());

    PB_RESULT nResult = CScreenUtil::GetInstance()->ShowCenterScreenText(lpszAddress, CImCharset::UnicodeToUTF8(strCommandXml.c_str()).c_str());
    if (nResult != PB_OK)
    {
        LOG_ERROR(L"COMP_Show failed, result=%d", nResult);
    }
    else
    {
        LOG_INFO(L"COMP_Show success");
    }

    return nResult;
}

PB_RESULT COMP_GetStatus(OUT PB_STATUS *lpStatus)
{
    if (lpStatus == nullptr)
    {
        LOG_ERROR(L"COMP_GetStatus lpStatus param invalid");
        return PB_COMP_NOT_SUPPORTED;  //没有合适的预定义错误返回值
    }

    //记录输入参数
    LOG_INFO(L"COMP_GetStatus is called");

    PB_RESULT nResult = CScreenUtil::GetInstance()->GetStatus(lpStatus);
    if (nResult != PB_OK)
    {
        LOG_ERROR(L"COMP_GetStatus failed, result=%d", nResult);
    }
    else
    {
        LOG_INFO(L"COMP_GetStatus success");
    }

    return nResult;
}