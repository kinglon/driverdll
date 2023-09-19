#pragma once

#include <afxwin.h>
#include "InterfaceSpec.h"
#include "TxPrinter/export.h"

class IPrintApi
{
public:
    virtual PB_RESULT Init() = 0;
    virtual void DoFunction(DWORD func, int param1, int param2) = 0;
    virtual PB_RESULT Write(const CString& strData) = 0;
    virtual PB_RESULT PrintImage(const std::wstring& strImageFilePath) = 0;
    virtual PB_RESULT CutPaper() = 0;
};

class CPrintApiFactory
{
public:
    static IPrintApi* GetPrintApi();
};

class CMockPrintApi : public IPrintApi
{
public:
    virtual PB_RESULT Init() override { return PB_OK; }
    virtual void DoFunction(DWORD func, int param1, int param2) override { }
    virtual PB_RESULT Write(const CString& strData) override { return PB_OK; }
    virtual PB_RESULT PrintImage(const std::wstring& strImageFilePath) { return PB_OK; }
    virtual PB_RESULT CutPaper() override { return PB_OK; }
};