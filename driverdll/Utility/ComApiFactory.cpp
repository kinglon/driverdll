#include "stdafx.h"
#include "ComApiFactory.h"
#include "AppSetting.h"
#include "MockComApi.h"
#include "ComApi.h"

CComApiFactory::CComApiFactory()
{
}

CComApiFactory::~CComApiFactory()
{
}

IComApi* CComApiFactory::CreateComApi()
{
    if (CAppSetting::GetInstance()->GetTestSetting()->m_bUseMockImpl)
    {
        return new CMockComApi();
    }
    else
    {
        return new CComApi();
    }
}
