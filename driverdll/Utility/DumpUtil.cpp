#include "stdafx.h"
#include <dbghelp.h>
#include "DumpUtil.h"
#include <time.h>

using namespace std;

#pragma comment(lib, "Dbghelp.lib")

wstring CDumpUtil::m_strDumpFilePath;

void CDumpUtil::SetDumpFilePath(const wchar_t* szDumpFilePath)
{
    m_strDumpFilePath = szDumpFilePath;
}

long __stdcall CDumpUtil::UnhandledExceptionFilter(struct _EXCEPTION_POINTERS* pExceptionInfo)
{
    wstring strDumpFile = m_strDumpFilePath;
    time_t currentTime;
    time(&currentTime);
    wchar_t szFileName[100];
    memset(szFileName, 0, sizeof(szFileName));
    struct tm *tmCurrent = nullptr;
    localtime_s(tmCurrent, &currentTime);
    if (tmCurrent == nullptr)
    {
        strDumpFile += L"driverdll.dmp";
    }
    else
    {
        tmCurrent->tm_year = tmCurrent->tm_year + 1900;
        tmCurrent->tm_mon = tmCurrent->tm_mon + 1;
        _snwprintf_s(szFileName, ARRAYSIZE(szFileName), L"%04d%02d%02d_%02d%02d%02d",
            tmCurrent->tm_year, tmCurrent->tm_mon, tmCurrent->tm_mday, tmCurrent->tm_hour, tmCurrent->tm_min, tmCurrent->tm_sec);
        strDumpFile += szFileName;
    }

    HANDLE hFile = CreateFile(strDumpFile.c_str(), GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile == INVALID_HANDLE_VALUE)
    {
        exit(1);
        return EXCEPTION_EXECUTE_HANDLER;
    }

    MINIDUMP_EXCEPTION_INFORMATION exceptionInfo;
    exceptionInfo.ThreadId = GetCurrentThreadId();
    exceptionInfo.ExceptionPointers = pExceptionInfo;
    exceptionInfo.ClientPointers = TRUE;

    MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), hFile, MiniDumpNormal, &exceptionInfo, nullptr, nullptr);
    CloseHandle(hFile);

    exit(1);
    return EXCEPTION_EXECUTE_HANDLER;
}

void myInvalidParameterHandler(const wchar_t* expression,
    const wchar_t* function,
    const wchar_t* file,
    unsigned int line,
    uintptr_t pReserved)
{
    throw 1;
}

void myPurecallHandler(void)
{
    throw 1;
}

void CDumpUtil::Enable(bool bEnable)
{
    if (bEnable)
    {
        ::SetUnhandledExceptionFilter(UnhandledExceptionFilter);

        /*
        * Ϊ�˰�ȫ�������һ��ĳ�����͵��쳣û�б����Ȱ�װ�Ĵ��������ˣ�UEF�ᱻ���ǣ���
        * �����HOOK��Kernel32.SetUnhandledExceptionFilter,�Ӷ���ֹ�κο��ܵĸ��ǡ�
        */
        _set_abort_behavior(0, _CALL_REPORTFAULT);
        _set_invalid_parameter_handler(myInvalidParameterHandler);
        _set_purecall_handler(myPurecallHandler);

        void *pSetUnhandledExceptionFilterAddr = (void*)GetProcAddress(LoadLibrary(L"kernel32.dll"), "SetUnhandledExceptionFilter");
        if (pSetUnhandledExceptionFilterAddr)
        {            
            unsigned char code[16];
            int size = 0;
            code[size++] = 0x33;
            code[size++] = 0xC0;
            code[size++] = 0xC2;
            code[size++] = 0x04;
            code[size++] = 0x00;

            DWORD dwOldFlag, dwTempFlag;
            //win8.1���ðٶ�������PAGE_READWRITE������ʵ�ʲ�������debugȨ�ޣ�SetProcessDEPPolicy(1);����Ч���ĳ�PAGE_EXECUTE_READWRITE��Ч
            //VirtualProtect(addr, size, PAGE_READWRITE, &dwOldFlag);
            VirtualProtect(pSetUnhandledExceptionFilterAddr, size, PAGE_EXECUTE_READWRITE, &dwOldFlag);
            WriteProcessMemory(GetCurrentProcess(), pSetUnhandledExceptionFilterAddr, code, size, NULL);
            VirtualProtect(pSetUnhandledExceptionFilterAddr, size, dwOldFlag, &dwTempFlag);
        }
    }
    else
    {
        ::SetUnhandledExceptionFilter(nullptr);
    }
}