#include "stdafx.h"
#include <future>
#include "ScreenUtil.h"
#include "AppSetting.h"
#include "StringUtil.h"
#include "Screen.h"
#include "tinyxml\tinyxml.h"
#include "..\Utility\XmlUtil.h"
#include "..\Utility\ImCharset.h"
#include "SerialUtilFactory.h"
#include "DeviceSetting.h"

using namespace std;

#define SEND_DATA_LENGTH  69

CScreenUtil::CScreenUtil()
{
    //
}

CScreenUtil::~CScreenUtil()
{
    Close();
}

CScreenUtil* CScreenUtil::GetInstance()
{
    static CScreenUtil* pInstance = new CScreenUtil();
    return pInstance;
}

PB_RESULT CScreenUtil::Open(const std::wstring& strModuleName, const std::string& strCommandXml)
{
    SCREENUTIL_ASSERT(!strModuleName.empty());
    SCREENUTIL_ASSERT(!strCommandXml.empty());
    if (strModuleName.empty() || strCommandXml.empty())
    {
        LOG_ERROR(L"param is empty");
        return PB_BERR_INVALID_CONFIG_XML;
    }    

    CIcrCriticalSection cs(m_csWrap.GetCS());
    if (m_bOpen) //已经打开
    {
        m_setUsingModules.insert(strModuleName);
        return PB_OK;
    }

    CDeviceSetting devSetting;
    if (!devSetting.Parse(strCommandXml.c_str()))
    {
        LOG_ERROR(L"failed to parse device setting");
        return PB_BERR_INVALID_CONFIG_XML;
    }

    if (g_dllLog)
    {
        g_dllLog->SetLogLevel(devSetting.m_logLevel);
        LOG_DEBUG(L"set log level %d", (int)devSetting.m_logLevel);
    }

    if (CAppSetting::GetInstance()->GetScreenComPort() == INVALID_SERIAL_PORT)
    {
        LOG_ERROR(L"screen com port is not setting");
        return PB_BERR_PORT_OPEN_ERROR;
    }

    m_pSerialUtil = CSerialUtilFactory::CreateSerialUtilImpl(false, FILE_FLAG_OVERLAPPED);
    bool bOpen = m_pSerialUtil->Open(CAppSetting::GetInstance()->GetScreenComPort(), "9600,N,8,1", GENERIC_READ | GENERIC_WRITE);
    if (!bOpen)
    {
        delete m_pSerialUtil;
        m_pSerialUtil = nullptr;
        LOG_ERROR(L"failed to open serial port (%d), error=%d", CAppSetting::GetInstance()->GetScreenComPort(), GetLastError());
        return PB_BERR_PORT_OPEN_ERROR;
    }
    else
    {
        LOG_DEBUG(L"succeed to open serial port");
        m_setUsingModules.insert(strModuleName);
    }

    m_bOpen = true;
    cs.Leave();

    m_hNotifyHaveNewData = CreateEvent(nullptr, TRUE, FALSE, nullptr);
    m_hNotifyExitSendThread = CreateEvent(nullptr, TRUE, FALSE, nullptr);
    m_hNotifySendThreadAlreadyExit = CreateEvent(nullptr, TRUE, FALSE, nullptr);
    async(launch::async, SendThread);

    return PB_OK;
}

PB_RESULT CScreenUtil::ShowWindowScreenText(LPCSTR lpszCounterNo, LPCSTR lpszCommandXml)
{
    if (!m_bOpen)
    {
        LOG_ERROR(L"serial com port is not open");
        return PB_BERR_NOT_INITIALIZED;
    }

    PB_RESULT nResult = CheckWindowScreenShowCommand(lpszCounterNo, lpszCommandXml);
    if (nResult != PB_OK)
    {
        return nResult;
    }

    if (m_pSerialUtil == nullptr)
    {
        LOG_ERROR(L"com serial port is not initialized");
        return PB_BERR_NOT_INITIALIZED;
    }

    int nScreenNum = CAppSetting::GetInstance()->GetWindowScreenInfo(CImCharset::UTF8ToUnicode(lpszCounterNo).c_str()).m_nScreenNum;
    CShowCommand showCommand = ParseShowCommand(lpszCommandXml);
    if (showCommand.m_strData == L"暂停服务" || showCommand.m_strData == L"欢迎光临")
    {
        showCommand.m_strData = wstring().append(4, L' ') + showCommand.m_strData;
    }
    LOG_DEBUG(L"show window screen text, screen num=%d, mode=%s, text=%s", nScreenNum, showCommand.m_strMode.c_str(), showCommand.m_strData.c_str());

    int nMaxWordCnt = CAppSetting::GetInstance()->GetWindowScreenInfo(CImCharset::UTF8ToUnicode(lpszCounterNo).c_str()).m_nMaxWordCnt;
    const int nDataLength = 5000;
    char pTargetString[nDataLength];
    memset(pTargetString, 0, nDataLength);

    CScreen screenBuilder;
    screenBuilder.BuildString(CImCharset::UnicodeToGbk(showCommand.m_strData.c_str()).c_str(), nMaxWordCnt, pTargetString);
    memcpy_s(&pTargetString[strlen(pTargetString)], nDataLength / 2, pTargetString, nDataLength / 2);

    //帧结构体参数设定
    STSCREENFRAME stSF;
    memset(&stSF, 0, sizeof(stSF));
    stSF.a_cHead = 255 - nScreenNum;
    stSF.b_cFrameKind = 3;  //3表示代码帧
    stSF.c_cScnCount_l = 0;
    stSF.d_cScnCount_h = 0;
    stSF.e_cWordCount = 0;
    stSF.f_cInMode = 12;
    stSF.g_cOutMode = 12;
    stSF.h_cStayMode = 224;
    
    SCREENUTIL_ASSERT((strlen(pTargetString) % (nMaxWordCnt * 2)) == 0);
    int nFrame = strlen(pTargetString) / (nMaxWordCnt * 2);
    for (int iFrame = 0; iFrame<nFrame; iFrame++)
    {
        //确定这一帧的发送内容
        char cItem[1000] = { 0 };
        memcpy(cItem, pTargetString + iFrame*nMaxWordCnt * 2, nMaxWordCnt * 2);

        //设置发送属性结构体中低位幕号
        stSF.c_cScnCount_l = iFrame;
        stSF.c_cScnCount_l += 10;  //第10帧开始存储，循环播放

        //组帧，处理发送的字符串和计算校验和
        screenBuilder.BuildFrame(stSF, cItem, nMaxWordCnt * 2);

        //将内容加入到发送列表中        
        char* pSendBuffer = new char[SEND_DATA_LENGTH];
        memcpy_s(pSendBuffer, SEND_DATA_LENGTH, (BYTE*)&stSF, sizeof(stSF));

        CIcrCriticalSection cs(m_csWrap.GetCS());
        m_listSendData.insert(m_listSendData.end(), pSendBuffer);        
        SetEvent(m_hNotifyHaveNewData);  //通知有新数据等待发送，也需要放在临界区内作为原子操作
        cs.Leave();        
    }

    return PB_OK;
}

PB_RESULT CScreenUtil::ShowCenterScreenText(IN LPCSTR lpszAddress, IN LPCSTR lpszCommandXml)
{
    if (!m_bOpen)
    {
        LOG_ERROR(L"serial com port is not open");
        return PB_COMP_NOT_INITIALIZED;
    }

    PB_RESULT nResult = CheckCompScreenShowCommand(lpszAddress, lpszCommandXml);
    if (nResult != PB_OK)
    {
        return nResult;
    }

    if (m_pSerialUtil == nullptr)
    {
        LOG_ERROR(L"com serial port is not initialized");
        return PB_COMP_NOT_INITIALIZED;
    }

    CCompScreenInfo screenInfo = CAppSetting::GetInstance()->GetCompScreenInfo(CStringUtil(lpszAddress).ToInt());
    CShowCommand showCommand = ParseShowCommand(lpszCommandXml);
    LOG_DEBUG(L"show comp screen text, block num=%d, mode=%s, text=%s", screenInfo.m_nBlockNum, showCommand.m_strMode.c_str(), showCommand.m_strData.c_str());
    
    const int nDataLength = 5000;
    char pTargetString[nDataLength];
    memset(pTargetString, 0, nDataLength);

    CScreen screenBuilder;
    screenBuilder.BuildString(CImCharset::UnicodeToGbk((screenInfo.m_strPrefix + showCommand.m_strData).c_str()).c_str(), screenInfo.m_nMaxWordCnt, pTargetString);
    memcpy_s(&pTargetString[strlen(pTargetString)], nDataLength / 2, pTargetString, nDataLength / 2);

    m_vecCompScreenShowText.push_back(pTargetString);
    while (m_vecCompScreenShowText.size() > screenInfo.m_vecScreenNumber.size())
    {
        m_vecCompScreenShowText.erase(m_vecCompScreenShowText.begin());  //显示内容行数超过条屏数，将最早显示的内容去除掉
    }
   
    for (unsigned int i = 0; i < m_vecCompScreenShowText.size(); ++i)
    {
        //帧结构体参数设定
        STSCREENFRAME stSF;
        memset(&stSF, 0, sizeof(stSF));
        stSF.a_cHead = 255 - screenInfo.m_vecScreenNumber[i];
        stSF.b_cFrameKind = 3;  //3表示代码帧
        stSF.c_cScnCount_l = 0;
        stSF.d_cScnCount_h = 0;
        stSF.e_cWordCount = 0;
        stSF.f_cInMode = 12;
        stSF.g_cOutMode = 12;
        stSF.h_cStayMode = 224;

        string strTargetString = m_vecCompScreenShowText[i];
        SCREENUTIL_ASSERT((strTargetString.length() % (screenInfo.m_nMaxWordCnt * 2)) == 0);
        int nFrame = strTargetString.length() / (screenInfo.m_nMaxWordCnt * 2);
        for (int iFrame = 0; iFrame < nFrame; iFrame++)
        {
            //确定这一帧的发送内容
            char cItem[1000] = { 0 };
            memcpy(cItem, strTargetString.c_str()+iFrame*screenInfo.m_nMaxWordCnt * 2, screenInfo.m_nMaxWordCnt * 2);

            //设置发送属性结构体中低位幕号
            stSF.c_cScnCount_l = iFrame;
            stSF.c_cScnCount_l += 10;  //第10帧开始存储，循环播放

            //组帧，处理发送的字符串和计算校验和
            screenBuilder.BuildFrame(stSF, cItem, screenInfo.m_nMaxWordCnt * 2);

            //将内容加入到发送列表中        
            char* pSendBuffer = new char[SEND_DATA_LENGTH];
            memcpy_s(pSendBuffer, SEND_DATA_LENGTH, (BYTE*)&stSF, sizeof(stSF));

            CIcrCriticalSection cs(m_csWrap.GetCS());
            m_listSendData.insert(m_listSendData.end(), pSendBuffer);
            SetEvent(m_hNotifyHaveNewData);  //通知有新数据等待发送，也需要放在临界区内作为原子操作
            cs.Leave();
        }
    }    

    return PB_OK;
}

PB_RESULT CScreenUtil::GetStatus(PB_STATUS *lpStatus)
{
    if (m_bOpen)
    {
        *lpStatus = PB_OK;
    }
    else
    {
        *lpStatus = PB_BERR_NOT_INITIALIZED;
    }

    return PB_OK;
}

PB_RESULT CScreenUtil::Close(const std::wstring& strModuleName)
{
    LOG_DEBUG(L"module (%s) request close", strModuleName.c_str());

    CIcrCriticalSection cs(m_csWrap.GetCS());
    m_setUsingModules.erase(strModuleName);
    if (m_setUsingModules.size() <= 0)
    {
        LOG_DEBUG(L"all module request close");
        Close();        
    }

    return PB_OK;
}

void CScreenUtil::Close()
{
    if (!m_bOpen)
    {
        return;
    }

    //退出发送线程
    SetEvent(m_hNotifyExitSendThread);
    if (WaitForSingleObject(m_hNotifySendThreadAlreadyExit, 3000) != WAIT_OBJECT_0)
    {
        LOG_ERROR(L"failed to wait send thread to exit, error=%d", GetLastError());
    }

    m_listSendData.clear();
    if (m_pSerialUtil)
    {
        m_pSerialUtil->Close();
        delete m_pSerialUtil;
        m_pSerialUtil = nullptr;
    }

    CloseHandle(m_hNotifyHaveNewData);
    m_hNotifyHaveNewData = NULL;
    CloseHandle(m_hNotifyExitSendThread);
    m_hNotifyExitSendThread = NULL;
    CloseHandle(m_hNotifySendThreadAlreadyExit);
    m_hNotifySendThreadAlreadyExit = NULL;

    m_bOpen = false;
}

void CScreenUtil::SendThread()
{
    LOG_DEBUG(L"send thread start");
    HANDLE waitObjects[2] = {CScreenUtil::GetInstance()->m_hNotifyHaveNewData, CScreenUtil::GetInstance()->m_hNotifyExitSendThread};
    while (true)
    {
        DWORD dwWaitResult = WaitForMultipleObjects(2, waitObjects, FALSE, INFINITE);
        if (dwWaitResult == WAIT_OBJECT_0)  //有新数据等待发送
        {
            CIcrCriticalSection cs(CScreenUtil::GetInstance()->m_csWrap.GetCS());
            if (CScreenUtil::GetInstance()->m_listSendData.size() <= 0)
            {
                ResetEvent(CScreenUtil::GetInstance()->m_hNotifyHaveNewData);
                continue;
            }

            auto it = CScreenUtil::GetInstance()->m_listSendData.begin();
            if (CScreenUtil::GetInstance()->m_pSerialUtil)
            {
                CScreenUtil::GetInstance()->m_pSerialUtil->Write(*it, SEND_DATA_LENGTH);
                ::Sleep(300);  //保留原有代码的实现
            }

            delete[](*it);
            CScreenUtil::GetInstance()->m_listSendData.erase(it);

            cs.Leave();
        }
        else if (dwWaitResult == WAIT_OBJECT_0 + 1)  //通知退出
        {
            break;
        }
        else
        {
            SCREENUTIL_ASSERT(false);
            LOG_ERROR(L"failed to wait object, error=%d", GetLastError());
            break;
        }
    }

    SetEvent(CScreenUtil::GetInstance()->m_hNotifySendThreadAlreadyExit);
    LOG_DEBUG(L"send thread end");
}

PB_RESULT CScreenUtil::CheckWindowScreenShowCommand(LPCSTR lpszCounterNo, LPCSTR lpszCommandXml)
{
    SCREENUTIL_ASSERT(lpszCounterNo && strlen(lpszCounterNo) != 0);    
    if (lpszCounterNo == nullptr || strlen(lpszCounterNo) == 0)
    {
        LOG_ERROR(L"lpszCounterNo param invalid");
        return PB_BERR_INVALID_ADDRESS;
    }

    int nWindowNum = CStringUtil(lpszCounterNo).ToInt();
    if (nWindowNum <= 0 || CAppSetting::GetInstance()->GetWindowScreenInfo(CImCharset::UTF8ToUnicode(lpszCounterNo).c_str()).m_nScreenNum == INVALID_SCREEN_NUMBER)
    {
        LOG_ERROR(L"invalid lpszCounterNo, window num=%d", nWindowNum);
        return PB_BERR_INVALID_ADDRESS;
    }

    SCREENUTIL_ASSERT(lpszCommandXml && strlen(lpszCommandXml) != 0);
    if (lpszCommandXml == nullptr || strlen(lpszCommandXml) == 0)
    {
        LOG_ERROR(L"lpszCommandXml param invalid");
        return PB_BERR_INVALID_COMMAND_XML;
    }    

    CShowCommand showCommand = ParseShowCommand(lpszCommandXml);
    if (showCommand.m_strMode.empty())
    {
        LOG_ERROR(L"mode param invalid");
        return PB_BERR_INVALID_COMMAND_XML;
    }

    if (showCommand.m_strData.empty())
    {
        LOG_ERROR(L"data param invalid");
        return PB_BERR_INVALID_COMMAND_XML;
    }

    if (showCommand.m_strMode != L"NUMBER" && showCommand.m_strMode != L"TEXT" && showCommand.m_strMode != L"INIT")
    {
        LOG_ERROR(L"unsupport mode : %s", showCommand.m_strMode.c_str());
        return PB_BERR_NOT_SUPPORTED;
    }

    return PB_OK;
}

PB_RESULT CScreenUtil::CheckCompScreenShowCommand(LPCSTR lpszAddress, LPCSTR lpszCommandXml)
{
    SCREENUTIL_ASSERT(lpszAddress && strlen(lpszAddress) != 0);
    if (lpszAddress == nullptr || strlen(lpszAddress) == 0)
    {
        LOG_ERROR(L"lpszAddress param invalid");
        return PB_COMP_INVALID_ADDRESS;
    }

    int nBlockNum = CStringUtil(lpszAddress).ToInt();
    if (nBlockNum <= 0 || CAppSetting::GetInstance()->GetCompScreenInfo(nBlockNum).m_nBlockNum == INVALID_SCREEN_NUMBER) //没找到
    {
        LOG_ERROR(L"invalid lpszAddress, block num=%d", nBlockNum);
        return PB_COMP_INVALID_ADDRESS;
    }

    SCREENUTIL_ASSERT(lpszCommandXml && strlen(lpszCommandXml) != 0);
    if (lpszCommandXml == nullptr || strlen(lpszCommandXml) == 0)
    {
        LOG_ERROR(L"lpszCommandXml param invalid");
        return PB_COMP_INVALID_COMMAND_XML;
    }

    CShowCommand showCommand = ParseShowCommand(lpszCommandXml);
    if (showCommand.m_strMode.empty())
    {
        LOG_ERROR(L"mode param invalid");
        return PB_COMP_INVALID_COMMAND_XML;
    }

    if (showCommand.m_strData.empty())
    {
        LOG_ERROR(L"data param invalid");
        return PB_COMP_INVALID_COMMAND_XML;
    }

    if (showCommand.m_strMode != L"NUMBER" && showCommand.m_strMode != L"TEXT" && showCommand.m_strMode != L"INIT")
    {
        LOG_ERROR(L"unsupport mode : %s", showCommand.m_strMode.c_str());
        return PB_COMP_NOT_SUPPORTED;
    }

    return PB_OK;
}

CShowCommand CScreenUtil::ParseShowCommand(LPCSTR lpszCommandXml)
{
    CShowCommand showCommand;
    TiXmlDocument document;
    document.Parse(lpszCommandXml, nullptr, TIXML_ENCODING_UTF8);

    TiXmlNode* pCommandNode = document.RootElement();
    if (pCommandNode == nullptr)
    {
        LOG_ERROR(L"failed to parse xml, error=%S, path=%S", document.ErrorDesc(), lpszCommandXml);
        return showCommand;
    }

    TiXmlNode* pModeNode = pCommandNode->FirstChildElement("Mode");
    showCommand.m_strMode = CXmlUtil::GetNodeText(pModeNode);

    TiXmlNode* pDataNode = pCommandNode->FirstChildElement("Data");
    showCommand.m_strData = CXmlUtil::GetNodeText(pDataNode);

    return showCommand;
}