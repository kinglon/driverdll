#pragma once

#include "IPrintApi.h"

class CTxPrinter : public IPrintApi
{
public:
    CTxPrinter();
    ~CTxPrinter();

public:  // implement IPrintApi
    virtual PB_RESULT Init() override;
    virtual void DoFunction(DWORD func, int param1, int param2) override;
    virtual PB_RESULT Write(const CString& strData) override;
    virtual PB_RESULT PrintImage(const std::wstring& strImageFilePath) override;
    virtual PB_RESULT CutPaper() override;

private:
    PB_RESULT GetPrinterErrorCode(DWORD dwPrinterStatus);

private:
    bool m_bAlreadyInit = false;
};

