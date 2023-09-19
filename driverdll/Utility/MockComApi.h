#pragma once
#include "IComApi.h"

class CMockComApi : public IComApi
{
public:
    virtual BOOL ApiGetCommState(HANDLE hFile, LPDCB lpDCB) { return TRUE; }
    virtual BOOL ApiBuildCommDCB(const char* lpDef, LPDCB lpDCB) { return TRUE; }
    virtual BOOL ApiSetCommState(HANDLE hFile, LPDCB lpDCB) { return TRUE; }
    virtual BOOL ApiSetupComm(HANDLE hFile, DWORD dwInQueue, DWORD dwOutQueue) { return TRUE; }
    virtual BOOL ApiClearCommError(HANDLE hFile, LPDWORD lpErrors, LPCOMSTAT lpStat);
    virtual BOOL ApiPurgeComm(HANDLE hFile, DWORD dwFlags) { return TRUE; }
    virtual BOOL ApiReadFile(HANDLE hFile, LPVOID lpBuffer, DWORD nNumberOfBytesToRead, LPDWORD lpNumberOfBytesRead, LPOVERLAPPED lpOverlapped);
    virtual BOOL ApiGetOverlappedResult(HANDLE hFile, LPOVERLAPPED lpOverlapped, LPDWORD lpNumberOfBytesTransferred, BOOL bWait) { return TRUE; }
    virtual BOOL ApiWriteFile(HANDLE hFile, LPCVOID lpBuffer, DWORD nNumberOfBytesToWrite, LPDWORD lpNumberOfBytesWritten, LPOVERLAPPED lpOverlapped);
    virtual BOOL ApiCloseHandle(HANDLE hObject) { return TRUE; }
    virtual BOOL ApiSetCommMask(HANDLE hFile, DWORD dwEvtMask) { return TRUE; }
    virtual HANDLE ApiCreateFile(const char* lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile) { return (HANDLE)1L; }
    virtual BOOL ApiGetCommTimeouts(HANDLE hFile, LPCOMMTIMEOUTS lpCommTimeouts) { return TRUE; }
    virtual BOOL ApiSetCommTimeouts(HANDLE hFile, LPCOMMTIMEOUTS lpCommTimeouts) { return TRUE; }
    virtual BOOL ApiWaitCommEvent(HANDLE hFile, LPDWORD lpEvtMask, LPOVERLAPPED lpOverlapped);
};