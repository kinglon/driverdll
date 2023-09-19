#pragma once
#include "InterfaceSpec.h"

class CPrintManager
{
private:
    CPrintManager();
    ~CPrintManager();

public:
    static CPrintManager* GetInstance();

public:
    PB_RESULT Init();
    PB_RESULT SetUp(const char*  lpszSetupXml);
    PB_RESULT PrintData(const char*  lpszData);
    PB_RESULT Cut(void);
    PB_RESULT GetStatus(PB_STATUS *lpStatus);
    
private:
    PB_RESULT StartPrintService();
    PB_RESULT SendRequest(int nMethodIndex, const char* szMethodParam);
    
private:
    PB_RESULT m_pbStatus = PB_PRNT_NOT_INITIALIZED;
};