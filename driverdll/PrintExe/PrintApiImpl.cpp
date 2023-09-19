#include "stdafx.h"
#include "IPrintApi.h"
#include "AppSetting.h"
#include "TxPrinter.h"

IPrintApi* CPrintApiFactory::GetPrintApi()
{
    static IPrintApi* pInstance = nullptr;
    if (pInstance != nullptr)
    {
        return pInstance;
    }

    if (CAppSetting::GetInstance()->GetTestSetting()->m_bUseMockImpl)
    {
        pInstance = new CMockPrintApi();
    }
    else
    {
        pInstance = new CTxPrinter();
    }

    return pInstance;
}