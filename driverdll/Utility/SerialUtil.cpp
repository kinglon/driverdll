#include "stdafx.h"
#include "SerialUtil.h"
#include <string>
#include "ComApiFactory.h"

using namespace std;

CSerialUtil::CSerialUtil(bool fAutoBeginThread, DWORD dwIOMode)
    : _dwIOMode(dwIOMode), _fAutoBeginThread(fAutoBeginThread)
{
    Init();
}

CSerialUtil::~CSerialUtil()
{
    Close();
    UnInit();
}

bool CSerialUtil::Open(DWORD dwPort)
{
    return Open(dwPort, 9600);
}

bool CSerialUtil::Open(DWORD dwPort, DWORD dwBaudRate)
{
    if (dwPort < 1 || dwPort > 1024)
        return false;

    BindCommPort(dwPort);

    if (!OpenCommPort(GENERIC_READ | GENERIC_WRITE))
        return false;

    if (!SetupPort())
        return false;

    return SetState(dwBaudRate);
}

bool CSerialUtil::Open(DWORD dwPort, char *szSetStr, DWORD dwDesiredAccess)
{
    if (dwPort < 1 || dwPort > 1024)
        return false;

    BindCommPort(dwPort);

    if (!OpenCommPort(dwDesiredAccess))
        return false;

    if (!SetupPort())
        return false;

    return SetState(szSetStr);
}

bool CSerialUtil::IsOpen()
{
    return _hCommHandle != INVALID_HANDLE_VALUE;
}

HANDLE CSerialUtil::GetHandle()
{
    return _hCommHandle;
}

bool CSerialUtil::SetState(char *szSetStr)
{
    if (IsOpen())
    {
        if (!m_pComApi->ApiGetCommState(_hCommHandle, &_DCB))
            return false;
        if (!m_pComApi->ApiBuildCommDCB(szSetStr, &_DCB))//BuildCommDCB
            return false;
        return m_pComApi->ApiSetCommState(_hCommHandle, &_DCB) == TRUE;
    }
    return false;
}

bool CSerialUtil::SetState(DWORD dwBaudRate, DWORD dwByteSize, DWORD dwParity, DWORD dwStopBits)
{
    if (IsOpen())
    {
        if (!m_pComApi->ApiGetCommState(_hCommHandle, &_DCB))
            return false;
        _DCB.BaudRate = dwBaudRate;
        _DCB.ByteSize = (BYTE)dwByteSize;
        _DCB.Parity = (BYTE)dwParity;
        _DCB.StopBits = (BYTE)dwStopBits;
        return m_pComApi->ApiSetCommState(_hCommHandle, &_DCB) == TRUE;
    }
    return false;
}

bool CSerialUtil::SetBufferSize(DWORD dwInputSize, DWORD dwOutputSize)
{
    if (IsOpen())
        return m_pComApi->ApiSetupComm(_hCommHandle, dwInputSize, dwOutputSize);
    return false;
}

DWORD CSerialUtil::Read(char *szBuffer, DWORD dwBufferLength, DWORD dwWaitTime)
{
    if (!IsOpen())
        return 0;

    memset(szBuffer, 0, dwBufferLength);

    COMSTAT  Stat;
    DWORD dwError = 0;

    if (m_pComApi->ApiClearCommError(_hCommHandle, &dwError, &Stat) && dwError > 0)	//清除错误
    {
        m_pComApi->ApiPurgeComm(_hCommHandle, PURGE_RXABORT | PURGE_RXCLEAR); /*清除输入缓冲区*/
        return 0;
    }

    if (!Stat.cbInQue)// 缓冲区无数据
        return 0;

    unsigned long uReadLength = 0;

    dwBufferLength = dwBufferLength - 1 > Stat.cbInQue ? Stat.cbInQue : dwBufferLength - 1;

    if (!m_pComApi->ApiReadFile(_hCommHandle, szBuffer, dwBufferLength, &uReadLength, &_ReadOverlapped)) //2000 下 ReadFile 始终返回 True
    {
        if (::GetLastError() == ERROR_IO_PENDING) // 结束异步I/O
        {
            WaitForSingleObject(_ReadOverlapped.hEvent, dwWaitTime);	//等待20ms
            if (!m_pComApi->ApiGetOverlappedResult(_hCommHandle, &_ReadOverlapped, &uReadLength, false))
            {
                if (::GetLastError() != ERROR_IO_INCOMPLETE)//其他错误
                {
                    uReadLength = 0;
                }
            }
        }
        else
        {
            uReadLength = 0;
        }
    }

    return uReadLength;
}

DWORD CSerialUtil::Write(char *szBuffer, DWORD dwBufferLength)
{
    if (!IsOpen())
    {
        return 0;
    }

    DWORD dwError;

    if (m_pComApi->ApiClearCommError(_hCommHandle, &dwError, NULL) && dwError > 0)	//清除错误
        m_pComApi->ApiPurgeComm(_hCommHandle, PURGE_TXABORT | PURGE_TXCLEAR);

    unsigned long uWriteLength = 0;

    if (!m_pComApi->ApiWriteFile(_hCommHandle, szBuffer, dwBufferLength, &uWriteLength, &_WriteOverlapped))
    {
        if (::GetLastError() != ERROR_IO_PENDING)
        {
            uWriteLength = 0;
        }
    }

    return uWriteLength;
}

DWORD CSerialUtil::Write(char *szBuffer)
{
    if (szBuffer == nullptr)
    {
        UTILITY_ASSERT(false);
        return 0;
    }

    return Write(szBuffer, strlen(szBuffer));
}

DWORD CSerialUtil::WriteSync(char *szBuffer, DWORD dwBufferLength)
{
    if (!IsOpen())
        return 0;

    DWORD dwError;
    if (m_pComApi->ApiClearCommError(_hCommHandle, &dwError, NULL) && dwError > 0)	//清除错误
        m_pComApi->ApiPurgeComm(_hCommHandle, PURGE_TXABORT | PURGE_TXCLEAR);

    unsigned long uWriteLength = 0;
    if (!m_pComApi->ApiWriteFile(_hCommHandle, szBuffer, dwBufferLength, &uWriteLength, NULL))
    {
        if (::GetLastError() != ERROR_IO_PENDING)
        {
            uWriteLength = 0;
        }
    }

    return uWriteLength;
}

void CSerialUtil::Close()
{
    if (IsOpen())
    {
        EndThread();
        m_pComApi->ApiCloseHandle(_hCommHandle);
        _hCommHandle = INVALID_HANDLE_VALUE;
    }
}

void CSerialUtil::SetNotifyNum(int iNum)
{
    _dwNotifyNum = iNum;
}

bool CSerialUtil::BeginThread()
{
    if (!IsThreadRunning())
    {
        _fRunFlag = true;
        _hThreadHandle = NULL;
        DWORD id;
        _hThreadHandle = ::CreateThread(NULL, 0, CommThreadProc, this, 0, &id); //兼容98 故使用&ID
        return (_hThreadHandle != NULL); //辅助线程
    }
    return false;
}

bool CSerialUtil::IsThreadRunning()
{
    return _hThreadHandle != NULL;
}

bool CSerialUtil::EndThread(DWORD dwWaitTime)
{
    if (IsThreadRunning())
    {
        _fRunFlag = false;
        m_pComApi->ApiSetCommMask(_hCommHandle, 0);
        ::SetEvent(_WaitOverlapped.hEvent);
        if (::WaitForSingleObject(_hThreadHandle, dwWaitTime) != WAIT_OBJECT_0)
        {
            if (!::TerminateThread(_hThreadHandle, 0))
            {
                return false;
            }
        }

        ::CloseHandle(_hThreadHandle);
        ::ResetEvent(_WaitOverlapped.hEvent);

        _hThreadHandle = NULL;

        return true;
    }
    return false;
}

void CSerialUtil::Init()
{
    m_pComApi = CComApiFactory::CreateComApi();
    memset(_szCommStr, 0, 20);
    memset(&_DCB, 0, sizeof(_DCB));
    _DCB.DCBlength = sizeof(_DCB);
    _hCommHandle = INVALID_HANDLE_VALUE;

    memset(&_ReadOverlapped, 0, sizeof(_ReadOverlapped));
    memset(&_WriteOverlapped, 0, sizeof(_WriteOverlapped));

    _ReadOverlapped.hEvent = ::CreateEvent(NULL, true, false, NULL);
    UTILITY_ASSERT(_ReadOverlapped.hEvent != INVALID_HANDLE_VALUE);

    _WriteOverlapped.hEvent = ::CreateEvent(NULL, true, false, NULL);
    UTILITY_ASSERT(_WriteOverlapped.hEvent != INVALID_HANDLE_VALUE);

    _dwNotifyNum = 0;
    _hThreadHandle = NULL;

    memset(&_WaitOverlapped, 0, sizeof(_WaitOverlapped));
    _WaitOverlapped.hEvent = ::CreateEvent(NULL, true, false, NULL);
    UTILITY_ASSERT(_WaitOverlapped.hEvent != INVALID_HANDLE_VALUE);
}

void CSerialUtil::UnInit()
{
    if (_ReadOverlapped.hEvent != INVALID_HANDLE_VALUE)
        CloseHandle(_ReadOverlapped.hEvent);

    if (_WriteOverlapped.hEvent != INVALID_HANDLE_VALUE)
        CloseHandle(_WriteOverlapped.hEvent);

    if (_WaitOverlapped.hEvent != INVALID_HANDLE_VALUE)
        CloseHandle(_WaitOverlapped.hEvent);

    delete m_pComApi;
    m_pComApi = nullptr;
}

void CSerialUtil::BindCommPort(DWORD dwPort)
{
    if (dwPort < 1 || dwPort > 1024)
    {
        UTILITY_ASSERT(false);
        return;
    }

    _dwPort = dwPort;
    strcpy(_szCommStr, "\\\\.\\COM");
    to_string(dwPort);
    strcat(_szCommStr, to_string(dwPort).c_str());
}

bool CSerialUtil::OpenCommPort(DWORD dwDesiredAccess)
{
    if (IsOpen())
    {
        Close();
    }

    _hCommHandle = m_pComApi->ApiCreateFile(	//CreateFile
        _szCommStr,
        dwDesiredAccess,
        0,
        NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL | _dwIOMode,
        NULL
        );

    if (_fAutoBeginThread)
    {
        if (IsOpen() && BeginThread())
        {
            return true;
        }
        else
        {
            Close(); //创建线程失败
            return false;
        }
    }
    return IsOpen();//检测串口是否成功打开
}

bool CSerialUtil::SetupPort()
{
    if (!IsOpen())
        return false;

    //设置推荐缓冲区
    if (!m_pComApi->ApiSetupComm(_hCommHandle, 4096, 4096))
        return false;

    //设置超时时间
    if (!m_pComApi->ApiGetCommTimeouts(_hCommHandle, &_CO))
        return false;
    _CO.ReadIntervalTimeout = 0xFFFFFFFF;
    _CO.ReadTotalTimeoutMultiplier = 0;
    _CO.ReadTotalTimeoutConstant = 0;
    _CO.WriteTotalTimeoutMultiplier = 0;
    _CO.WriteTotalTimeoutConstant = 2000;
    if (!m_pComApi->ApiSetCommTimeouts(_hCommHandle, &_CO))
        return false;

    //清空串口缓冲区
    if (!m_pComApi->ApiPurgeComm(_hCommHandle, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR))
        return false;

    return true;
}

DWORD WINAPI CSerialUtil::CommThreadProc(LPVOID lpPara)
{
    CSerialUtil *pComm = (CSerialUtil *)lpPara;
    if (!pComm->m_pComApi->ApiSetCommMask(pComm->_hCommHandle, EV_RXCHAR | EV_ERR))
    {
        return 1;
    }

    COMSTAT Stat;
    DWORD dwError;
    for (DWORD dwLength, dwMask = 0; pComm->_fRunFlag && pComm->IsOpen(); dwMask = 0)
    {
        if (!pComm->m_pComApi->ApiWaitCommEvent(pComm->_hCommHandle, &dwMask, &pComm->_WaitOverlapped))
        {
            if (::GetLastError() == ERROR_IO_PENDING)
            {
                pComm->m_pComApi->ApiGetOverlappedResult(pComm->_hCommHandle, &pComm->_WaitOverlapped, &dwLength, TRUE);
            }
        }

        if (dwMask & EV_ERR) // == EV_ERR
            pComm->m_pComApi->ApiClearCommError(pComm->_hCommHandle, &dwError, &Stat);

        if (dwMask & EV_RXCHAR) // == EV_RXCHAR
        {
            pComm->m_pComApi->ApiClearCommError(pComm->_hCommHandle, &dwError, &Stat);
            if (Stat.cbInQue > pComm->_dwNotifyNum)
            {
                char pData[1000];
                memset(pData, 0, 1000);
                int nReadLength = pComm->Read(pData, 1000);
                if (nReadLength > 0 && pComm->m_pSerialDataArriveCallback)
                {
                    pComm->m_pSerialDataArriveCallback->OnSerialDataArrive(pData, nReadLength);
                }
            }
        }
    }

    return 0;
}