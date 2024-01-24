#include "stdafx.h"
#include "IccrManager.h"
#include <future>
#include "IccrReader.h"
#include "ImCharset.h"
#include "ImPath.h"
#include "StringUtil.h"
#include "AppSetting.h"

using namespace std;

#define PB_ICCR_IDX_ACCOUNT         1
#define PB_ICCR_IDX_NAME                2
#define PB_ICCR_IDX_ID_TYPE             3
#define PB_ICCR_IDX_ID                      4

CIccrManager::CIccrManager()
{
}

CIccrManager::~CIccrManager()
{
}

CIccrManager* CIccrManager::GetInstance()
{
    static CIccrManager* pInstance = new CIccrManager();
    return pInstance;
}

PB_RESULT CIccrManager::Init()
{
    if (m_bAlreadyInit)
    {
        LOG_INFO(L"already init");
        return PB_OK;
    }

    int nIccrType = CAppSetting::GetInstance()->GetIccrType();
    if (nIccrType == ICCR_TYPE_1)
    {
        LOG_INFO(L"it is an old iccr reader");
        async(launch::async, ReadThread);
    }
    else if (nIccrType == ICCR_TYPE_2)
    {
        LOG_INFO(L"it is an iccr reader supporting both ways");
        async(launch::async, ReadThreadV2);
    }
    else
    {
        LOG_ERROR(L"it is not a supported iccr reader, type is %d", nIccrType);
        return PB_ICCR_DEVICE_INIT_ERROR;
    }

    m_bAlreadyInit = true;
    return PB_OK;
}

PB_RESULT CIccrManager::BeginRead()
{
    if (!m_bAlreadyInit)
    {
        LOG_ERROR(L"not init");
        return PB_ICCR_NOT_INITIALIZED;
    }

    CIcrCriticalSection cs(m_csWrap.GetCS());
    m_strCardNumber = "";
    cs.Leave();

    return PB_OK;
}

PB_RESULT CIccrManager::DataAvailable(OUT BOOL *lpAvailable)
{
    if (!m_bAlreadyInit)
    {
        LOG_ERROR(L"not init");
        return PB_ICCR_NOT_INITIALIZED;
    }

    CIcrCriticalSection cs(m_csWrap.GetCS());
    if (m_strCardNumber.length() > 0)
    {
        *lpAvailable = TRUE;
    }
    else
    {
        *lpAvailable = FALSE;
    }
    cs.Leave();

    return PB_OK;
}

PB_RESULT CIccrManager::ReadData(DWORD dwIndex, OUT LPVOID lpBuffer, IN DWORD dwBufferSize, OUT DWORD *lpNumberOfBytesRead)
{
    if (!m_bAlreadyInit)
    {
        LOG_ERROR(L"not init");
        return PB_ICCR_NOT_INITIALIZED;
    }

    LOG_INFO(L"read data, dwIndex=%d", dwIndex);

    CIcrCriticalSection cs(m_csWrap.GetCS());
    string strData;
    if (dwIndex == PB_ICCR_IDX_ACCOUNT)
    {
        strData = m_strCardNumber;
    }
    cs.Leave();

	DWORD dwNeedSize = strData.length();
    if (lpBuffer == nullptr || dwBufferSize == 0)
    {
        *lpNumberOfBytesRead = dwNeedSize;
        return PB_OK;
    }
    
    if (dwBufferSize < dwNeedSize)
    {
        LOG_ERROR(L"buffer is too small, size=%d, need size=%d", dwBufferSize, dwNeedSize);
        return PB_ICCR_INSUFFICIENT_BUFFER;
    }

    memset(lpBuffer, 0, dwBufferSize);
    memcpy(lpBuffer, strData.c_str(), strData.length());
    *lpNumberOfBytesRead = dwNeedSize;

    LOG_INFO(L"read data, dwIndex=%d, data is %S", dwIndex, strData.c_str());

    return PB_OK;
}

PB_RESULT CIccrManager::CancelRead(void)
{
    if (!m_bAlreadyInit)
    {
        LOG_ERROR(L"not init");
        return PB_ICCR_NOT_INITIALIZED;
    }

    return PB_OK;
}

PB_RESULT CIccrManager::GetStatus(PB_STATUS *lpStatus)
{
    if (m_bAlreadyInit)
    {
        *lpStatus = PB_OK;
    }
    else
    {
        *lpStatus = PB_ICCR_NOT_INITIALIZED;
    }

    return PB_OK;
}

void CIccrManager::ReadThread()
{
    LOG_INFO(L"start read thread");

    IIccrReader* pReader = CIccrReaderFactory::CreateReader();
    while (true)
    {
        if (CAppSetting::GetInstance()->GetTestSetting()->m_bUseMockImpl)
        {
            Sleep(5000);
        }
        else
        {
            Sleep(100);
        }

        char szIDMsg[1024] = { 0 };
        memset(szIDMsg, 0, 1024);
        int nCurMode = 0;
        int nResult = pReader->ApiReadCardInfo(0, 115200, 0x04 | 0x08, szIDMsg, nCurMode, 10.0);
        if (nResult != 0)
        {
            continue;
        }

        if (nCurMode != 0x04 && nCurMode != 0x08)
        {
            LOG_ERROR(L"unsupport card type : %d", nCurMode);
            continue;
        }

        LOG_DEBUG(L"data arrive, lenght=%d", strlen(szIDMsg));
        LOG_DEBUG(L"data arrive, data is %s", CImCharset::UTF8ToUnicode(szIDMsg).c_str());         

        int nTotalLength = strlen(szIDMsg);
        int nCurrentPos = 0;
        while (true)
        {
            if (nCurrentPos + 4 > nTotalLength)
            {
                break;
            }

            char chType = szIDMsg[nCurrentPos];
            int nLength = CStringUtil(&szIDMsg[nCurrentPos + 1], 3).ToInt();
            nCurrentPos += 4;
            if (nCurrentPos + nLength > nTotalLength)
            {
                break;
            }
            string strValue;
            strValue.assign(&szIDMsg[nCurrentPos], nLength);

            if (chType == 'A')
            {                
                LOG_INFO(L"card number : %S", strValue.c_str());
                CIcrCriticalSection cs(CIccrManager::GetInstance()->m_csWrap.GetCS());                
                CIccrManager::GetInstance()->m_strCardNumber = strValue;
                cs.Leave();
            }            

            nCurrentPos += nLength;
        }
    }

    pReader->ApiCarderClose();
    delete pReader;
    pReader = nullptr;

    LOG_INFO(L"read thread exit");
}

void CIccrManager::ReadThreadV2()
{
    LOG_INFO(L"start read thread of v2");

    // get api functions
    typedef int(__stdcall *LPCarderOpen)(int iCom, int iBaud);    
    typedef int(__stdcall *LPGetCardStatus)(unsigned char ucCardNo);
    typedef int(__stdcall *LPCardActive)(unsigned char ucCardNo, unsigned char *ATR, int *nATRLen);
    typedef int(__stdcall *LPCardAPDU)(unsigned char ucCardNo, unsigned char *CApdu, int nCAPDULen, unsigned char *RApdu, int *nRAPDULen, double OutTime);
    typedef int(__stdcall *LPCardDeActive)(unsigned char ucCardNo);
    typedef void(__stdcall *LPCarderClose)();
    std::wstring sdkDllPath = CImPath::GetSoftInstallPath() + L"ABCDllv2.dll";
    HMODULE hModule = ::LoadLibrary(sdkDllPath.c_str());
    if (hModule == NULL)
    {
        LOG_ERROR(L"failed to load the reader sdk dll, path is %s, error is %d", sdkDllPath.c_str(), GetLastError());
        return;
    }
    LPCarderOpen lpCarderOpen = (LPCarderOpen)::GetProcAddress(hModule, "CarderOpen");
    LPGetCardStatus lpGetCardStatus = (LPGetCardStatus)::GetProcAddress(hModule, "GetCardStatus");
    LPCardActive lpCardActive = (LPCardActive)::GetProcAddress(hModule, "CardActive");
    LPCardAPDU lpCardAPDU = (LPCardAPDU)::GetProcAddress(hModule, "CardAPDU");
    LPCardDeActive lpCardDeActive = (LPCardDeActive)::GetProcAddress(hModule, "CardDeActive");    
    LPCarderClose lpCarderClose = (LPCarderClose)::GetProcAddress(hModule, "CarderClose");
    if (lpCarderOpen == nullptr || lpGetCardStatus == nullptr || lpCardActive == nullptr || \
        lpCardAPDU == nullptr || lpCardDeActive == nullptr || lpCarderClose == nullptr)
    {
        LOG_ERROR(L"failed to load get reader functions");
        return;
    }
    
    // open the card reader with the USB interface type 
    int nResult = lpCarderOpen(0, 9600);
    if (nResult == -1) {
        LOG_ERROR(L"failed to open the card reader");
        return;
    }

    while (true) {
        Sleep(100);

        {
            CIcrCriticalSection cs(CIccrManager::GetInstance()->m_csWrap.GetCS());
            bool bEmpty = CIccrManager::GetInstance()->m_strCardNumber.empty();
            cs.Leave();
            if (!bEmpty) {
                // wait to read until the application layer request
                continue;
            }
        }             
 
        // active card
        const int MAX_LENGTH = 10240;
        unsigned char data[MAX_LENGTH];
        int nDataLen = MAX_LENGTH;
        memset(data, 0, MAX_LENGTH);
        unsigned char cardNo = 0xFF;  // 射频卡（贴片）
        nResult = lpCardActive(cardNo, data, &nDataLen);
        if (nResult != 0) {
            LOG_DEBUG(L"failed to activate the card of %x, error is %d", cardNo, nResult);
            nDataLen = MAX_LENGTH;
            memset(data, 0, MAX_LENGTH);
            cardNo = 0x00;  // CPU卡（插片）
            nResult = lpCardActive(cardNo, data, &nDataLen);
            if (nResult != 0) {
                LOG_DEBUG(L"failed to activate the card of %x, error is %d", cardNo, nResult);
                continue;
            }
        }

        // APDU选择应用A000000333010101
        nDataLen = MAX_LENGTH;
        memset(data, 0, MAX_LENGTH);
        std::string strCmd = CStringUtil::ToAsciiChar(L"00A4040008A000000333010101");        
        nResult = lpCardAPDU(cardNo, (unsigned char*)strCmd.c_str(), strCmd.length(), data, &nDataLen, 48);
        if (nResult != 0) {
            LOG_ERROR(L"failed to select ADF, the card of %x, error is %d", cardNo, nResult);
            continue;
        }

        // 读取磁条2数据，位于记录号1，SFI为1
        nDataLen = MAX_LENGTH;
        memset(data, 0, MAX_LENGTH);
        strCmd = CStringUtil::ToAsciiChar(L"00B2010C00");
        nResult = lpCardAPDU(cardNo, (unsigned char*)strCmd.c_str(), strCmd.length(), data, &nDataLen, 48);
        if (nResult != 0) {
            LOG_ERROR(L"failed to read the data of the track 2, the card of %x, error is %d", cardNo, nResult);
            continue;
        }

        // 解析出卡号，详查磁道2数据格式定义
        std::wstring strTrack2Data = CStringUtil::ToHexChar(data, nDataLen);
        int firstIndex = strTrack2Data.find(L"57") + 4;
        int secondIndex = strTrack2Data.find(L'D');
        if (secondIndex == -1) {
            secondIndex = strTrack2Data.find(L'd');
        }
        if (firstIndex == -1 || secondIndex == -1) {
            LOG_ERROR(L"failed to parse the data of the track 2, the data is %s", strTrack2Data.c_str());
            continue;
        }

        std::wstring strCardNo = strTrack2Data.substr(firstIndex, secondIndex - firstIndex);
		strCardNo = CStringUtil(strCardNo.c_str()).TrimInvisibleChar(); //有些卡号尾部会有乱码
        if (strCardNo.empty())
        {
            LOG_ERROR(L"failed to parse the data of the track 2, the data is %s", strTrack2Data.c_str());
            continue;
        }

        LOG_INFO(L"card number : %s", strCardNo.c_str());
        {
            CIcrCriticalSection cs(CIccrManager::GetInstance()->m_csWrap.GetCS());
            CIccrManager::GetInstance()->m_strCardNumber.assign(strCardNo.begin(), strCardNo.end());
            cs.Leave();
        }

        // deactive
        lpCardDeActive(cardNo);
    }

    LOG_INFO(L"read thread exit");
}