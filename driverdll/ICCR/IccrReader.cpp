#include "stdafx.h"
#include "IccrReader.h"
#include "AppSetting.h"
#include "ABCDLL.h"
#include <string>

using namespace std;

IIccrReader* CIccrReaderFactory::CreateReader()
{
    if (CAppSetting::GetInstance()->GetTestSetting()->m_bUseMockImpl)
    {
        return new CMockCrdrReader();
    }
    else
    {
        return new CCrdrReader();
    }
}

CCrdrReader::CCrdrReader()
{
}

CCrdrReader::~CCrdrReader()
{
}

int CCrdrReader::ApiReadCardInfo(int iPort, int iBaud, int iReadMode, char *pucIDMsg, int &iCurMode, double dbOutTime)
{
    return ReadCardInfo(iPort, iBaud, iReadMode, pucIDMsg, iCurMode, dbOutTime);
}

int CCrdrReader::ApiCarderClose()
{
    return CarderClose();
}

int CMockCrdrReader::ApiReadCardInfo(int iPort, int iBaud, int iReadMode, char *pucIDMsg, int &iCurMode, double dbOutTime)
{
    iCurMode = 0x04;
    string strData = "A0196212260200057307593B020                    C00220D018                  E0386212260200057307593D24052207649991511FF024005110000000000764000000I006240531J00201";
    strcpy_s(pucIDMsg, 200, strData.c_str());
    return 0;
}

int CMockCrdrReader::ApiCarderClose()
{
    return 0;
}