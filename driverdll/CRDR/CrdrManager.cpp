#include "stdafx.h"
#include "CrdrManager.h"
#include <future>
#include "ImCharset.h"
#include "StringUtil.h"
#include "AppSetting.h"
#include "SerialUtilFactory.h"

using namespace std;

CCrdrManager::CCrdrManager()
{
}

CCrdrManager::~CCrdrManager()
{
}

CCrdrManager* CCrdrManager::GetInstance()
{
    static CCrdrManager* pInstance = new CCrdrManager();
    return pInstance;
}

PB_RESULT CCrdrManager::Init()
{
    if (m_bAlreadyInit)
    {
        LOG_INFO(L"already init");
        return PB_OK;
    }

    if (CAppSetting::GetInstance()->GetCrdrComPort() == INVALID_SERIAL_PORT)
    {
        LOG_ERROR(L"crdr com port is not setting");
        return PB_CRDR_PORT_OPEN_ERROR;
    }

    m_pSerialUtil = CSerialUtilFactory::CreateSerialUtilImpl(true, FILE_FLAG_OVERLAPPED);
    m_pSerialUtil->SetSerialDataArriveCallback(this);
    bool bOpen = m_pSerialUtil->Open(CAppSetting::GetInstance()->GetCrdrComPort(), "9600,N,8,1", GENERIC_READ);
    if (!bOpen)
    {
        delete m_pSerialUtil;
        m_pSerialUtil = nullptr;
        LOG_ERROR(L"failed to open serial port (%d), error=%d", CAppSetting::GetInstance()->GetCrdrComPort(), GetLastError());
        return PB_CRDR_PORT_OPEN_ERROR;
    }
    else
    {
        LOG_DEBUG(L"succeed to open serial port");
        m_bAlreadyInit = true;
        return PB_OK;
    }
}

PB_RESULT CCrdrManager::BeginRead(IN DWORD dwTrack)
{
    if (!m_bAlreadyInit)
    {
        LOG_ERROR(L"not init");
        return PB_CRDR_NOT_INITIALIZED;
    }

    CIcrCriticalSection cs(m_csWrap.GetCS());
    m_dwTrack = dwTrack;
    m_strTrack2 = "";
    m_strTrack3 = "";

    return PB_OK;
}

PB_RESULT CCrdrManager::DataAvailable(OUT BOOL *lpAvailable)
{
    if (!m_bAlreadyInit)
    {
        LOG_ERROR(L"not init");
        return PB_CRDR_NOT_INITIALIZED;
    }    

    if (GetData().length() > 0)
    {
        *lpAvailable = TRUE;
    }
    else
    {
        *lpAvailable = FALSE;
    }

    return PB_OK;
}

PB_RESULT CCrdrManager::ReadData(OUT LPVOID lpBuffer, IN DWORD dwBufferSize, OUT DWORD *lpNumberOfBytesRead)
{
    if (!m_bAlreadyInit)
    {
        LOG_ERROR(L"not init");
        return PB_CRDR_NOT_INITIALIZED;
    }

    string strData = GetData();
    DWORD dwNeedSize = strData.length() + 1;  //含尾部\0
    if (lpBuffer == nullptr || dwBufferSize == 0)
    {
        *lpNumberOfBytesRead = dwNeedSize;
        return PB_OK;
    }
    
    if (dwBufferSize < dwNeedSize)
    {
        LOG_ERROR(L"buffer is too small, size=%d, need size=%d", dwBufferSize, dwNeedSize);
        return PB_CRDR_INSUFFICIENT_BUFFER;
    }

    memset(lpBuffer, 0, dwBufferSize);
    memcpy(lpBuffer, strData.c_str(), strData.length());
    *lpNumberOfBytesRead = dwNeedSize;

    return PB_OK;
}

std::string CCrdrManager::GetData()
{
    CIcrCriticalSection cs(m_csWrap.GetCS());
    if (m_dwTrack == PB_CRDR_TRACK2)
    {
        return m_strTrack2;
    }
    else if (m_dwTrack == PB_CRDR_TRACK3)
    {
        return m_strTrack3;
    }
    else if (m_dwTrack == PB_CRDR_TRACK2_AND_TRACK3)
    {
        if (m_strTrack2.empty() && m_strTrack3.empty())
        {
            return "";
        }
        else
        {
            return m_strTrack2 + "|" + m_strTrack3;
        }
    }
    else
    {
        return "";
    }
}

PB_RESULT CCrdrManager::CancelRead(void)
{
    if (!m_bAlreadyInit)
    {
        LOG_ERROR(L"not init");
        return PB_CRDR_NOT_INITIALIZED;
    }

    m_dwTrack = 0;
    return PB_OK;
}

PB_RESULT CCrdrManager::GetStatus(PB_STATUS *lpStatus)
{
    if (m_bAlreadyInit)
    {
        *lpStatus = PB_OK;
    }
    else
    {
        *lpStatus = PB_CRDR_NOT_INITIALIZED;
    }

    return PB_OK;
}

void CCrdrManager::OnSerialDataArrive(const char* pData, int nDataLength)
{
    for (int k = 0; k < nDataLength; k++)
    {        
        if (0x03 == pData[k])  //收到结束符(0x03)
        {
            if (0x02 == m_strSerialData[0])
            {
                m_strSerialData += pData[k];
                break;
            }
        }       
        else if (0x02 == pData[k])  //收到起始符(0x02)
        {
            m_strSerialData.clear();
        }
        
        m_strSerialData += pData[k];
    }

    //判断数据是否接收完成
    if (m_strSerialData.length() > 2)
    {
        if ((0x02 == m_strSerialData[0]) && (0x03 == m_strSerialData[m_strSerialData.length()-1]))
        {
            m_strSerialData.erase(m_strSerialData.begin());  //去除02
            m_strSerialData = m_strSerialData.substr(0, m_strSerialData.length() - 1); //去除03
            if (m_strSerialData.length() >= 2)
            {
                m_strSerialData.erase(m_strSerialData.begin()); //去除;
                m_strSerialData = m_strSerialData.substr(0, m_strSerialData.length() - 1); //去除校验位
            }

            //按照分隔符拆成第二轨道数据和第三轨道数据
            CIcrCriticalSection cs(m_csWrap.GetCS());
            int nIndex = m_strSerialData.find('|');
            if (nIndex == -1)
            {
                m_strTrack2 = m_strSerialData;
                m_strTrack3 = "";
            }
            else
            {
                m_strTrack2 = m_strSerialData.substr(0, nIndex);
                m_strTrack3 = m_strSerialData.substr(nIndex + 1);
            }
            LOG_INFO(L"track2 data : %S", m_strTrack2.c_str());
            LOG_INFO(L"track3 data : %S", m_strTrack3.c_str());
            cs.Leave();

            m_strSerialData.clear();
        }
    }
}