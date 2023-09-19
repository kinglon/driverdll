#include "stdafx.h"
#include "InterfaceSpec.h"
#include "DeviceSetting.h"
#include "ImCharset.h"
#include "VoicManager.h"

PB_RESULT PB_API VOIC_Initialize(IN LPCSTR lpszConfigXml)
{
    if (lpszConfigXml == nullptr || strlen(lpszConfigXml) == 0)
    {
        LOG_ERROR(L"VOIC_Initialize lpszConfigXml param invalid");
        return PB_VOIC_INVALID_CONFIG_XML;
    }

    //设置日志级别
    CDeviceSetting devSetting;
    if (!devSetting.Parse(lpszConfigXml))
    {
        LOG_ERROR(L"VOIC_Initialize lpszConfigXml param invalid");
        return PB_VOIC_INVALID_CONFIG_XML;
    }

    if (g_dllLog)
    {
        g_dllLog->SetLogLevel(devSetting.m_logLevel);
        LOG_DEBUG(L"set log level %d", (int)devSetting.m_logLevel);
    }

    //记录输入参数
    LOG_INFO(L"VOIC_Initialize lpszConfigXml is %s", CImCharset::UTF8ToUnicode(lpszConfigXml).c_str());

    int nResult = CVoicManager::GetInstance()->Init();
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

PB_RESULT PB_API VOIC_PlaySound(IN LPCSTR lpszSoundXml)
{
    if (lpszSoundXml == nullptr || strlen(lpszSoundXml) == 0)
    {
        LOG_ERROR(L"VOIC_PlaySound lpszSoundXml param invalid");
        return PB_VOIC_INVALID_SOUND_XML;
    }

    //记录输入参数
    LOG_INFO(L"VOIC_PlaySound lpszSoundXml is %s", CImCharset::UTF8ToUnicode(lpszSoundXml).c_str());

    int nResult = CVoicManager::GetInstance()->PlayVoice(lpszSoundXml);
    if (nResult == PB_OK)
    {
        LOG_INFO(L"succeed to play sound");
    }
    else
    {
        LOG_ERROR(L"failed to play sound, error=%d", nResult);
    }

    return nResult;
}

PB_RESULT PB_API VOIC_GetStatus(OUT PB_STATUS *lpStatus)
{
    if (lpStatus == nullptr)
    {
        LOG_ERROR(L"VOIC_GetStatus lpStatus param invalid");
        return PB_VOIC_INVALID_CONFIG_XML;  //没有合适的预定义错误返回值
    }

    //记录输入参数
    LOG_INFO(L"VOIC_GetStatus is called");
    *lpStatus = PB_OK;
    return PB_OK;
}