#pragma once

#include <string>
#include "InterfaceSpec.h"

#define METHODINDEX_INVALID  0
#define METHODINDEX_INIT        1
#define METHODINDEX_SETUP   2
#define METHODINDEX_PRINT   3
#define METHODINDEX_CUT     4

class CRequest
{
public:
    DWORD m_dwMethodIndex = METHODINDEX_INVALID;
    std::string m_strMethodParam;
    PB_RESULT m_pbResult = PB_PRNT_INTERNAL_ERROR;
    HANDLE m_hSyncEvent = NULL;
};