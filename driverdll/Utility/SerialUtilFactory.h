#pragma once
#include "ISerialUtil.h"

class UTILITY_API CSerialUtilFactory
{
public:
    static ISerialUtil* CreateSerialUtilImpl(bool fAutoBeginThread, DWORD dwIOMode);
};

