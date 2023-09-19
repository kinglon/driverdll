#include "stdafx.h"
#include "InterfaceSpec.h"
#include "DeviceSetting.h"
#include "ImCharset.h"
#include "PrintManager.h"

using namespace std;

PB_RESULT PB_API PRNT_Initialize(const char*  lpszConfigXml)
{
    static bool bFirstCall = true;
    if (bFirstCall)
    {
        bFirstCall = false;
        CoInitialize(nullptr);
        AfxEnableControlContainer(nullptr);     
    }
        
    if (lpszConfigXml == nullptr || strlen(lpszConfigXml) == 0)
    {
        LOG_ERROR(L"PRNT_Initialize lpszConfigXml param invalid");
        return PB_PRNT_INVALID_CONFIG_XML;
    }

    //测试程序有BUG，没有在字符串尾部加0，会导致访问越界异常
    string strConfigXml;
    int nIndex = 0;
    while (true)
    {
        if (lpszConfigXml[nIndex] == '\0' || lpszConfigXml[nIndex] < ' ' || lpszConfigXml[nIndex] > '~') //不是ascii可见字符，就认为结束
        {
            strConfigXml.append(1, 0);  //尾部加\0
            break;
        }
        else
        {
            strConfigXml.append(1, lpszConfigXml[nIndex]);
        }

        nIndex++;
    }

    //设置日志级别
    CDeviceSetting devSetting;
    if (!devSetting.Parse(strConfigXml.c_str()))
    {
        LOG_ERROR(L"PRNT_Initialize lpszConfigXml param invalid");
        return PB_PRNT_INVALID_CONFIG_XML;
    }

    if (g_dllLog)
    {
        g_dllLog->SetLogLevel(devSetting.m_logLevel);
        LOG_DEBUG(L"set log level %d", (int)devSetting.m_logLevel);
    }

    //记录输入参数
    LOG_INFO(L"PRNT_Initialize lpszConfigXml is %s", CImCharset::UTF8ToUnicode(strConfigXml.c_str()).c_str());

    int nResult = PB_PRNT_INTERNAL_ERROR;
    TRY 
    {
        nResult = CPrintManager::GetInstance()->Init();
    }
    CATCH(COleDispatchException, e)
    {
        LOG_ERROR(L"print throw exception, code=%d, error msg=%s", e->m_wCode, (LPCTSTR)e->m_strDescription);
    }
    CATCH(COleException, e)
    {
        wchar_t szCause[MAX_PATH];
        e->GetErrorMessage(szCause, MAX_PATH);
        LOG_ERROR(L"print throw exception, code=%d, error msg=%s", e->m_sc, szCause);
    }
    CATCH(CException, e)
    {
        wchar_t szCause[MAX_PATH];
        e->GetErrorMessage(szCause, MAX_PATH);
        LOG_ERROR(L"print throw exception, error msg=%s", szCause);
    }
    END_CATCH    
    if (nResult == PB_OK)
    {
        LOG_INFO(L"succeed to init print");
    }
    else
    {
        LOG_ERROR(L"failed to init print, error=%d", nResult);
    }

    return nResult;
}

PB_RESULT PB_API PRNT_Setup(const char*  lpszSetupXml)
{
    if (lpszSetupXml == nullptr || strlen(lpszSetupXml) == 0)
    {
        LOG_ERROR(L"call PRNT_Setup with invalid param");
        return PB_PRNT_INVALID_SETUP_XML;
    }

    LOG_INFO(L"call PRNT_Setup, lpszSetupXml=%S", lpszSetupXml);

    int nResult = CPrintManager::GetInstance()->SetUp(lpszSetupXml);
    if (nResult == PB_OK)
    {
        LOG_INFO(L"succeed to setup printer");
    }
    else
    {
        LOG_ERROR(L"failed to setup printer, error=%d", nResult);
    }

    return nResult;
}

PB_RESULT PB_API PRNT_PrintData(const char* lpszData)
{
    if (lpszData == nullptr || strlen(lpszData) == 0)
    {
        LOG_ERROR(L"call PRNT_PrintData with invalid param");
        return PB_PRNT_DATA_FORMAT_ERROR;
    }

    LOG_INFO(L"call PRNT_PrintData, lpszData=%s", CImCharset::AnsiToUnicode(lpszData).c_str());

    int nResult = CPrintManager::GetInstance()->PrintData(lpszData);
    if (nResult == PB_OK)
    {
        LOG_INFO(L"succeed to print data");
    }
    else
    {
        LOG_ERROR(L"failed to print data, error=%d", nResult);
    }

    return nResult;
}

PB_RESULT PB_API PRNT_Cut(void)
{
    LOG_INFO(L"call PRNT_Cut");

    int nResult = CPrintManager::GetInstance()->Cut();
    if (nResult == PB_OK)
    {
        LOG_INFO(L"succeed to cut");
    }
    else
    {
        LOG_ERROR(L"failed to cut, error=%d", nResult);
    }

    return nResult;
}

PB_RESULT PB_API PRNT_GetStatus(OUT PB_STATUS *lpStatus)
{
    if (lpStatus == nullptr)
    {
        LOG_ERROR(L"PRNT_GetStatus lpStatus param invalid");
        return PB_PRNT_INVALID_CONFIG_XML;  //没有合适的预定义错误返回值
    }

    //记录输入参数
    LOG_INFO(L"PRNT_GetStatus is called");
    return CPrintManager::GetInstance()->GetStatus(lpStatus);
}