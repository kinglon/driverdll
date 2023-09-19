#include "stdafx.h"
#include <string>
#include "ComApi.h"
#include "LogMacro.h"
#include "StringUtil.h"

using namespace std;

BOOL CComApi::ApiReadFile(HANDLE hFile, LPVOID lpBuffer, DWORD nNumberOfBytesToRead, LPDWORD lpNumberOfBytesRead, LPOVERLAPPED lpOverlapped)
{
    BOOL bResult = ReadFile(hFile, lpBuffer, nNumberOfBytesToRead, lpNumberOfBytesRead, lpOverlapped);
    if (bResult)
    {
        wstring strContent = CStringUtil::ToUnsignedIntString((unsigned char*)lpBuffer, *lpNumberOfBytesRead);
        LOG_DEBUG(L"succeed to read data : %s", strContent.c_str());
    }
    else
    {
        LOG_ERROR(L"failed to read data, error=%d", GetLastError());
    }

    return TRUE;
}

BOOL CComApi::ApiGetOverlappedResult(HANDLE hFile, LPOVERLAPPED lpOverlapped, LPDWORD lpNumberOfBytesTransferred, BOOL bWait)
{
    return GetOverlappedResult(hFile, lpOverlapped, lpNumberOfBytesTransferred, bWait);
}

BOOL CComApi::ApiWriteFile(HANDLE hFile, LPCVOID lpBuffer, DWORD nNumberOfBytesToWrite, LPDWORD lpNumberOfBytesWritten, LPOVERLAPPED lpOverlapped)
{
    BOOL bResult = WriteFile(hFile, lpBuffer, nNumberOfBytesToWrite, lpNumberOfBytesWritten, lpOverlapped);
    wstring strContent = CStringUtil::ToUnsignedIntString((unsigned char*)lpBuffer, nNumberOfBytesToWrite);
    if (bResult || GetLastError() == ERROR_IO_PENDING)
    {
        LOG_DEBUG(L"succeed to send data : %s", strContent.c_str());
    }
    else
    {
        LOG_ERROR(L"failed to send data : %s", strContent.c_str());
    }

    return bResult;
}

HANDLE CComApi::ApiCreateFile(const char* lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile)
{
    HANDLE handle = CreateFileA(lpFileName, dwDesiredAccess, dwShareMode, lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);
    if (handle == INVALID_HANDLE_VALUE)
    {
        LOG_ERROR(L"failed to open serial , file name = %S, error=%d", lpFileName, GetLastError());
    }
    else
    {
        LOG_DEBUG(L"succeed to open serial , file name = %S", lpFileName);
    }

    return handle;
}