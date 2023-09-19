#pragma once
#include <string>
#include "..\include\InterfaceSpec.h"

typedef PB_RESULT (*PBERRInitialize)(IN LPCSTR lpszConfigXml);
typedef PB_RESULT (*PBERRShow)(IN LPCSTR lpszCounterNo, IN LPCSTR lpszCommandXml);
typedef PB_RESULT (*PBERRGetStatus)(OUT PB_STATUS *lpStatus);

class CWindowScreenSdk
{
protected:
    CWindowScreenSdk();
    ~CWindowScreenSdk();

public:
    static CWindowScreenSdk* GetInstance();
    void Init();

public:
    PB_RESULT BERRShow(const wchar_t* szWindowNumber, const wchar_t* szText);

private:
    void LoadModule();

private:
    bool m_bLoadSuccess = false;
    PBERRInitialize m_pInitializeFunction = nullptr;
    PBERRShow m_pShowFunction = nullptr;
    PBERRGetStatus m_pGetStatusFunction = nullptr;
};

