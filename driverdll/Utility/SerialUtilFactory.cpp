#include "stdafx.h"
#include "SerialUtilFactory.h"
#include "AppSetting.h"
#include "SerialUtil.h"

ISerialUtil* CSerialUtilFactory::CreateSerialUtilImpl(bool fAutoBeginThread, DWORD dwIOMode)
{    
    return new CSerialUtil(fAutoBeginThread, dwIOMode);
}
