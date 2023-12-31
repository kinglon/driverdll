#include "stdafx.h"
#include "ImGuid.h"
#include <objbase.h>

std::wstring CImGuid::CreateGuid()
{
    GUID guid;
    CoCreateGuid(&guid);

    wchar_t szBuf[64] = { 0 };
    _snwprintf_s(
        szBuf,
        ARRAYSIZE(szBuf),
        L"%08X-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X",
        guid.Data1, guid.Data2, guid.Data3,
        guid.Data4[0], guid.Data4[1],
        guid.Data4[2], guid.Data4[3],
        guid.Data4[4], guid.Data4[5],
        guid.Data4[6], guid.Data4[7]);

    return szBuf;
}
