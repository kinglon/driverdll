#pragma once
#include <windows.h>

class IComApi
{
public:
    virtual ~IComApi() {}

public:
    virtual BOOL ApiGetCommState(HANDLE hFile, LPDCB lpDCB) = 0;
    virtual BOOL ApiBuildCommDCB(const char* lpDef, LPDCB lpDCB) = 0;
    virtual BOOL ApiSetCommState(HANDLE hFile, LPDCB lpDCB) = 0;
    virtual BOOL ApiSetupComm(HANDLE hFile,DWORD dwInQueue,DWORD dwOutQueue) = 0;
    virtual BOOL ApiClearCommError(HANDLE hFile,LPDWORD lpErrors,LPCOMSTAT lpStat) = 0;
    virtual BOOL ApiPurgeComm(HANDLE hFile,DWORD dwFlags) = 0;
    virtual BOOL ApiReadFile(HANDLE hFile,LPVOID lpBuffer,DWORD nNumberOfBytesToRead,LPDWORD lpNumberOfBytesRead,LPOVERLAPPED lpOverlapped) = 0;
    virtual BOOL ApiGetOverlappedResult(HANDLE hFile,LPOVERLAPPED lpOverlapped,LPDWORD lpNumberOfBytesTransferred,BOOL bWait) = 0;
    virtual BOOL ApiWriteFile(HANDLE hFile,LPCVOID lpBuffer,DWORD nNumberOfBytesToWrite,LPDWORD lpNumberOfBytesWritten,LPOVERLAPPED lpOverlapped) = 0;
    virtual BOOL ApiCloseHandle(HANDLE hObject) = 0;
    virtual BOOL ApiSetCommMask(HANDLE hFile,DWORD dwEvtMask) = 0;
    virtual HANDLE ApiCreateFile(const char* lpFileName,DWORD dwDesiredAccess,DWORD dwShareMode,LPSECURITY_ATTRIBUTES lpSecurityAttributes,DWORD dwCreationDisposition,DWORD dwFlagsAndAttributes,HANDLE hTemplateFile) = 0;
    virtual BOOL ApiGetCommTimeouts(HANDLE hFile,LPCOMMTIMEOUTS lpCommTimeouts) = 0;
    virtual BOOL ApiSetCommTimeouts(HANDLE hFile,LPCOMMTIMEOUTS lpCommTimeouts) = 0;
    virtual BOOL ApiWaitCommEvent(HANDLE hFile,LPDWORD lpEvtMask, LPOVERLAPPED lpOverlapped) = 0;
};