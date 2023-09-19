#include "stdafx.h"
#include "PrintManager.h"
#include "tinyxml\tinyxml.h"
#include "XmlUtil.h"
#include "ImCharset.h"
#include "StringUtil.h"
#include "PrintRequest.h"
#include "ImPath.h"

using namespace std;

#define EVENT_NAME_PRNT_DLL L"7a8b533f-546b-4dea-83f7-9858a77488f2"

CPrintManager::CPrintManager()
{
}

CPrintManager::~CPrintManager()
{
}

CPrintManager* CPrintManager::GetInstance()
{
    static CPrintManager* pInstance = new CPrintManager();
    return pInstance;
}

PB_RESULT CPrintManager::Init()
{
    if (m_pbStatus == PB_OK)
    {
        LOG_DEBUG(L"already init");
        return PB_OK;
    }
    
    if (StartPrintService() != PB_OK)
    {
        return PB_PRNT_INTERNAL_ERROR;
    }
    
    m_pbStatus = SendRequest(METHODINDEX_INIT, "");
    return m_pbStatus;
}

PB_RESULT CPrintManager::SetUp(const char*  lpszSetupXml)
{
    if (m_pbStatus != PB_OK)
    {
        LOG_ERROR(L"not init");
        return PB_PRNT_NOT_INITIALIZED;
    }
    
    return SendRequest(METHODINDEX_SETUP, lpszSetupXml);
}

PB_RESULT CPrintManager::PrintData(const char*  lpszData)
{
    if (m_pbStatus != PB_OK)
    {
        LOG_ERROR(L"not init");
        return PB_PRNT_NOT_INITIALIZED;
    }
    
    return SendRequest(METHODINDEX_PRINT, lpszData);
}

PB_RESULT CPrintManager::Cut(void)
{
    if (m_pbStatus != PB_OK)
    {
        LOG_ERROR(L"not init");
        return PB_PRNT_NOT_INITIALIZED;
    }
    
    return SendRequest(METHODINDEX_CUT, "");
}

PB_RESULT CPrintManager::GetStatus(PB_STATUS *lpStatus)
{
    *lpStatus = m_pbStatus;
    return PB_OK;
}

PB_RESULT CPrintManager::SendRequest(int nMethodIndex, const char* szMethodParam)
{
    static HWND hReceiveRequestWnd = NULL;
    if (hReceiveRequestWnd == NULL)
    {
        hReceiveRequestWnd = FindWindow(L"driversddd", L"driversddd");
        if (hReceiveRequestWnd == NULL)
        {
            LOG_ERROR(L"failed to find receive request window, error=%d", GetLastError());
            return PB_PRNT_INTERNAL_ERROR;
        }
    }
    
    COPYDATASTRUCT copyDataStruct;
    copyDataStruct.dwData = nMethodIndex;
    copyDataStruct.cbData = strlen(szMethodParam) +1;
    copyDataStruct.lpData = new char[copyDataStruct.cbData];
    memset(copyDataStruct.lpData, 0, copyDataStruct.cbData);
    memcpy(copyDataStruct.lpData, szMethodParam, strlen(szMethodParam));
    LOG_DEBUG(L"start to send request : %d", nMethodIndex);
    LRESULT result = SendMessage(hReceiveRequestWnd, WM_COPYDATA, 0, (LPARAM)&copyDataStruct);    
    delete copyDataStruct.lpData;
    copyDataStruct.lpData = nullptr;
    LOG_DEBUG(L"succeed to process request : %d, result=%d", nMethodIndex, result);

    return result;
}

PB_RESULT CPrintManager::StartPrintService()
{
    static bool bStartSuccess = false;
    if (bStartSuccess)
    {
        LOG_DEBUG(L"print service is running");
        return PB_OK;
    }
    
	wstring strExePath = CImPath::GetSoftInstallPath() + L"PRNTEXE.exe";
    LOG_DEBUG(L"start print service, exe path = %s", strExePath.c_str());
    
    //如果服务启动成功，服务定时通过判断该事件是否存在来确认DLL所在的进程是否运行，如果没运行自动退出
    HANDLE hEvent = CreateEvent(nullptr, TRUE, FALSE, EVENT_NAME_PRNT_DLL);  
    if (hEvent == NULL)
    {
        LOG_ERROR(L"failed to create event, error=%d", GetLastError());
        return PB_PRNT_INTERNAL_ERROR;
    }
    ResetEvent(hEvent);  //有可能多次进来，显示的设置事件为无信号
    
    STARTUPINFO si;
	PROCESS_INFORMATION pi;
	ZeroMemory(&pi, sizeof(pi));
	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	si.dwFlags = STARTF_USESHOWWINDOW;
    si.wShowWindow = SW_HIDE;
    BOOL bResult = CreateProcess(NULL, (wchar_t*)strExePath.c_str(), NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);
    if (bResult)
	{
        CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);
        if (WaitForSingleObject(hEvent, 30000) == WAIT_OBJECT_0)
        {
            bStartSuccess = true;
            LOG_DEBUG(L"succeed to start print service");
            return PB_OK;
        }
        else
        {		
            CloseHandle(hEvent);
            LOG_ERROR(L"failed to create process, path=%s", strExePath.c_str());
            return PB_PRNT_INTERNAL_ERROR;
        }
	}
	else
	{		
        CloseHandle(hEvent);
		LOG_ERROR(L"failed to create process, path=%s", strExePath.c_str());
		return PB_PRNT_INTERNAL_ERROR;
	}
}