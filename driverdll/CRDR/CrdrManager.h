#pragma once
#include "InterfaceSpec.h"
#include "ISerialUtil.h"

class CCrdrManager : public IOnSerialDataArrive
{
private:
    CCrdrManager();
    ~CCrdrManager();

public:
    static CCrdrManager* GetInstance();

public:  //implement IOnSerialDataArrive
    virtual void OnSerialDataArrive(const char* pData, int nDataLength) override;

public:
    PB_RESULT Init();
    PB_RESULT BeginRead(IN DWORD dwTrack);
    PB_RESULT DataAvailable(OUT BOOL *lpAvailable);
    PB_RESULT ReadData(OUT LPVOID lpBuffer, IN DWORD dwBufferSize, OUT DWORD *lpNumberOfBytesRead);
    PB_RESULT CancelRead(void);
    PB_RESULT GetStatus(PB_STATUS *lpStatus);

private:
    std::string GetData();

private:
    bool m_bAlreadyInit = false;
    ISerialUtil* m_pSerialUtil = nullptr;
    CCSWrap m_csWrap;
    DWORD m_dwTrack = 0;
    std::string m_strTrack2;
    std::string m_strTrack3;
    std::string m_strSerialData;
};