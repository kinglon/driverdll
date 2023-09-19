#pragma once

class IIccrReader
{
public:
    virtual int ApiReadCardInfo(int iPort, int iBaud, int iReadMode, char *pucIDMsg, int &iCurMode, double dbOutTime) = 0;
    virtual int ApiCarderClose() = 0;
};

class CIccrReaderFactory
{
public:
    static IIccrReader* CreateReader();
};

class CCrdrReader : public IIccrReader
{
public:
    CCrdrReader();
    ~CCrdrReader();

public:
    virtual int ApiReadCardInfo(int iPort, int iBaud, int iReadMode, char *pucIDMsg, int &iCurMode, double dbOutTime) override;
    virtual int ApiCarderClose() override;
};

class CMockCrdrReader : public IIccrReader
{
public:
    virtual int ApiReadCardInfo(int iPort, int iBaud, int iReadMode, char *pucIDMsg, int &iCurMode, double dbOutTime) override;
    virtual int ApiCarderClose() override;
};