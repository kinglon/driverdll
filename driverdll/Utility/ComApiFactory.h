#pragma once
#include "IComApi.h"

class CComApiFactory
{
public:
    CComApiFactory();
    ~CComApiFactory();

public:
    static IComApi* CreateComApi();
};

