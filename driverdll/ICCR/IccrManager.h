#pragma once
#include "InterfaceSpec.h"

class CIccrManager
{
private:
    CIccrManager();
    ~CIccrManager();

public:
    static CIccrManager* GetInstance();

public:
    PB_RESULT Init();
    PB_RESULT BeginRead();
    PB_RESULT DataAvailable(OUT BOOL *lpAvailable);
    PB_RESULT ReadData(DWORD dwIndex, OUT LPVOID lpBuffer, IN DWORD dwBufferSize, OUT DWORD *lpNumberOfBytesRead);
    PB_RESULT CancelRead(void);
    PB_RESULT GetStatus(PB_STATUS *lpStatus);

private:
    static void ReadThread();   // 旧款读卡器设备读取数据
    static void ReadThreadV2();  // 二合一款读卡器设备读取数据

private:
    bool m_bAlreadyInit = false;
    CCSWrap m_csWrap;    
    std::string m_strCardNumber;
};