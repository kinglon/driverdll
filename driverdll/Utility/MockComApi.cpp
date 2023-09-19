#include "stdafx.h"
#include <string>
#include "MockComApi.h"
#include "StringUtil.h"
#include "AppSetting.h"

using namespace std;

BOOL CMockComApi::ApiWriteFile(HANDLE hFile, LPCVOID lpBuffer, DWORD nNumberOfBytesToWrite, LPDWORD lpNumberOfBytesWritten, LPOVERLAPPED lpOverlapped)
{
    wstring strContent = CStringUtil::ToUnsignedIntString((unsigned char*)lpBuffer, nNumberOfBytesToWrite);
    LOG_DEBUG(L"succeed to send data : %s", strContent.c_str());
    return TRUE;
}

BOOL CMockComApi::ApiReadFile(HANDLE hFile, LPVOID lpBuffer, DWORD nNumberOfBytesToRead, LPDWORD lpNumberOfBytesRead, LPOVERLAPPED lpOverlapped)
{
    DWORD nNeedLength = CAppSetting::GetInstance()->GetTestSetting()->m_strCrdrMockData.length();
    if (lpBuffer == nullptr || nNumberOfBytesToRead <= 0)
    {
        *lpNumberOfBytesRead = nNeedLength;
        return TRUE;
    }

    if (nNumberOfBytesToRead < nNeedLength)
    {
        return FALSE;
    }
        
    Sleep(5000);  //5秒返回一次，避免太快
    memset(lpBuffer, nNumberOfBytesToRead, 0);
    memcpy(lpBuffer, CAppSetting::GetInstance()->GetTestSetting()->m_strCrdrMockData.c_str(), nNeedLength);
    *lpNumberOfBytesRead = nNeedLength;

    return TRUE;
}

BOOL CMockComApi::ApiWaitCommEvent(HANDLE hFile, LPDWORD lpEvtMask, LPOVERLAPPED lpOverlapped)
{
    *lpEvtMask = EV_RXCHAR;
    return TRUE;
}

BOOL CMockComApi::ApiClearCommError(HANDLE hFile, LPDWORD lpErrors, LPCOMSTAT lpStat)
{
    if (lpErrors)
    {
        *lpErrors = 0;
    }

    if (lpStat)
    {
        lpStat->cbInQue = CAppSetting::GetInstance()->GetTestSetting()->m_strCrdrMockData.length();
    }

    return TRUE;
}