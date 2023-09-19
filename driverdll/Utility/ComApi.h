#pragma once

#include "IComApi.h"

class CComApi : public IComApi
{
    virtual BOOL ApiGetCommState(HANDLE hFile, LPDCB lpDCB) { return GetCommState(hFile, lpDCB); }
    virtual BOOL ApiBuildCommDCB(const char* lpDef, LPDCB lpDCB) { return BuildCommDCBA(lpDef, lpDCB); }
    virtual BOOL ApiSetCommState(HANDLE hFile, LPDCB lpDCB) { return SetCommState(hFile, lpDCB); }
    virtual BOOL ApiSetupComm(HANDLE hFile, DWORD dwInQueue, DWORD dwOutQueue) { return SetupComm(hFile, dwInQueue, dwOutQueue); }
    virtual BOOL ApiClearCommError(HANDLE hFile, LPDWORD lpErrors, LPCOMSTAT lpStat) { return ClearCommError(hFile, lpErrors, lpStat); }
    virtual BOOL ApiPurgeComm(HANDLE hFile, DWORD dwFlags) { return PurgeComm(hFile, dwFlags); }
    virtual BOOL ApiReadFile(HANDLE hFile, LPVOID lpBuffer, DWORD nNumberOfBytesToRead, LPDWORD lpNumberOfBytesRead, LPOVERLAPPED lpOverlapped);
    virtual BOOL ApiGetOverlappedResult(HANDLE hFile, LPOVERLAPPED lpOverlapped, LPDWORD lpNumberOfBytesTransferred, BOOL bWait);
    virtual BOOL ApiWriteFile(HANDLE hFile, LPCVOID lpBuffer, DWORD nNumberOfBytesToWrite, LPDWORD lpNumberOfBytesWritten, LPOVERLAPPED lpOverlapped);
    virtual BOOL ApiCloseHandle(HANDLE hObject) { return CloseHandle(hObject); }
    virtual BOOL ApiSetCommMask(HANDLE hFile, DWORD dwEvtMask) { return SetCommMask(hFile, dwEvtMask); }
    virtual HANDLE ApiCreateFile(const char* lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile);
    virtual BOOL ApiGetCommTimeouts(HANDLE hFile, LPCOMMTIMEOUTS lpCommTimeouts) { return GetCommTimeouts(hFile, lpCommTimeouts); }
    virtual BOOL ApiSetCommTimeouts(HANDLE hFile, LPCOMMTIMEOUTS lpCommTimeouts) { return SetCommTimeouts(hFile, lpCommTimeouts); }
    virtual BOOL ApiWaitCommEvent(HANDLE hFile, LPDWORD lpEvtMask, LPOVERLAPPED lpOverlapped) { return WaitCommEvent(hFile, lpEvtMask, lpOverlapped); }
};
