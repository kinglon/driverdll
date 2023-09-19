#include "stdafx.h"
#include "TxPrinter.h"
#include "TxPrinter/export.h"
#include "AppSetting.h"
#include "ImCharset.h"

CTxPrinter::CTxPrinter()
{
}

CTxPrinter::~CTxPrinter()
{
}

PB_RESULT CTxPrinter::Init()
{
    if (m_bAlreadyInit)
    {
        LOG_INFO(L"the printer has been initialized");
        return PB_OK;
    }

    int nPortType = CAppSetting::GetInstance()->GetPrinterPortType();
    if (TX_TYPE_NET == nPortType ? TxOpenPrinter(nPortType, (DWORD_PTR)"127.0.0.1") : TxOpenPrinter(nPortType, 0))
    {
        if (TX_TYPE_COM == nPortType)
        {
            TxSetupSerial(TX_SER_BAUD38400 | TX_SER_DATA_8BITS | TX_SER_PARITY_NONE | TX_SER_STOP_1BITS | TX_SER_FLOW_HARD);
        }

        DWORD status = TxGetStatus();
        LOG_INFO(L"the current status of the printer is 0x%x", status);

        TxInit();
        m_bAlreadyInit = true;
        return PB_OK;
    }
    else
    {
        LOG_ERROR(L"failed to open printer, port type is %d", nPortType);
        return PB_PRNT_PORT_OPEN_ERROR;
    }
}

void CTxPrinter::DoFunction(DWORD func, int param1, int param2)
{
    if (!m_bAlreadyInit)
    {
        LOG_ERROR(L"the printer needs to be initialized first");
        return;
    }

    LOG_INFO(L"send control command to the printer, id is %d, param1 is %d, param2 is %d", func, param1, param2);
    TxDoFunction(func, param1, param2);
}

PB_RESULT CTxPrinter::Write(const CString& strData)
{
    if (!m_bAlreadyInit)
    {
        LOG_ERROR(L"the printer needs to be initialized first");
        return PB_PRNT_NOT_INITIALIZED;
    }

    std::string data = CImCharset::UnicodeToGbk(strData);
    if (TxWritePrinter(data.c_str(), data.length()))
    {
        return PB_OK;
    }
    else
    {
        DWORD status = TxGetStatus();
        LOG_ERROR(L"failed to write data to the printer, current status is 0x%x", status);
        return GetPrinterErrorCode(status);
    }
}

PB_RESULT CTxPrinter::PrintImage(const std::wstring& strImageFilePath)
{
    if (!m_bAlreadyInit)
    {
        LOG_ERROR(L"the printer needs to be initialized first");
        return PB_PRNT_NOT_INITIALIZED;
    }

    if (TxPrintImage(CImCharset::UnicodeToGbk(strImageFilePath.c_str()).c_str()))
    {
        return PB_OK;
    }
    else
    {
        DWORD status = TxGetStatus();
        LOG_ERROR(L"failed to print image, current status is 0x%x", status);
        return GetPrinterErrorCode(status);
    }
}

PB_RESULT CTxPrinter::CutPaper()
{
    if (!m_bAlreadyInit)
    {
        LOG_ERROR(L"the printer needs to be initialized first");
        return PB_PRNT_NOT_INITIALIZED;
    }
    
    TxDoFunction(TX_CUT, TX_CUT_FULL, 0);

    // log the status if there is an error
    DWORD status = TxGetStatus();
    if (status & TX_STAT_ERROR)
    {
        LOG_ERROR(L"the current status of printer is 0x%x", status);
    }

    return PB_OK;  // always return success, since TxDoFunction does not tell it
}

PB_RESULT CTxPrinter::GetPrinterErrorCode(DWORD dwPrinterStatus)
{
    if (dwPrinterStatus & TX_STAT_PAPEREND)
    {
        return PB_PRNT_PAPER_OUT;
    }
    else if (dwPrinterStatus & TX_STAT_BUSY)
    {
        return PB_PRNT_BUSY;
    }
    else if (dwPrinterStatus & TX_STAT_ERROR)
    {
        return PB_PRNT_INTERNAL_ERROR;
    }
    else
    {
        return PB_OK;
    }
}