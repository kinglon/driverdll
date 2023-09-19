#include "stdafx.h"
#include "AppSetting.h"
#include "tinyxml\tinyxml.h"
#include "ImPath.h"
#include "ImCharset.h"
#include "XmlUtil.h"

using namespace std;

CAppSetting::CAppSetting()
{
    LoadDeviceSetting();
    LoadTestSetting();
}

CAppSetting::~CAppSetting()
{
}

CAppSetting* CAppSetting::GetInstance()
{
    static CAppSetting* pInstance = new CAppSetting();
    return pInstance;
}

void CAppSetting::LoadTestSetting()
{
    std::wstring strTestConfFile =  L"C:\\testconf.ini";
    if (GetPrivateProfileInt(L"system", L"use_mock_impl", 0, strTestConfFile.c_str()))
    {
        m_testSetting.m_bUseMockImpl = true;
    }

    wchar_t szBuffer[MAX_PATH];
    GetPrivateProfileString(L"crdr", L"crdr_mock_data", L"", szBuffer, MAX_PATH, strTestConfFile.c_str());
    m_testSetting.m_strCrdrMockData = 0x02;
    m_testSetting.m_strCrdrMockData += CImCharset::UnicodeToUTF8(szBuffer);
    m_testSetting.m_strCrdrMockData += 0x03;

    if (GetPrivateProfileInt(L"voic", L"delete_file_after_play", 1, strTestConfFile.c_str()) == 0)
    {
        m_testSetting.m_bDeleteFileAfterPlay = false;
    }
}

void CAppSetting::LoadDeviceSetting()
{
    TiXmlDocument document;
    wstring strSettingFile = CImPath::GetConfPath() + L"config.xml";    
    if (!document.LoadFile(CImCharset::UnicodeToGbk(strSettingFile.c_str()).c_str(), TIXML_ENCODING_UTF8))
    {
        LOG_ERROR(L"failed to load xml, error=%S, path=%s", document.ErrorDesc(), strSettingFile.c_str());
        return;
    }
    
    TiXmlNode* pRootNode = document.RootElement();
    if (pRootNode == nullptr)
    {
        LOG_ERROR(L"root element is not exist");
        return;
    }   

    //´°¿ÚÌõÆÁÅäÖÃ
    TiXmlNode* pBerrNode = pRootNode->FirstChildElement("BERR");
    if (pBerrNode != nullptr)
    {
        m_nScreenComPort = CXmlUtil::GetAttributeValueInt(pBerrNode, "port");
        if (m_nScreenComPort == INVALID_INTEGER)
        {
            UTILITY_ASSERT(false);
            LOG_ERROR(L"invalid berr port setting");
        }

        for (TiXmlElement* pScreenNode = pBerrNode->FirstChildElement("Screen"); pScreenNode != nullptr; pScreenNode = pScreenNode->NextSiblingElement("Screen"))
        {
            CWindowScreenInfo screenInfo;
            screenInfo.m_strWinNum = CXmlUtil::GetAttributeValueString(pScreenNode, "window_number");              
            screenInfo.m_nScreenNum = CXmlUtil::GetAttributeValueInt(pScreenNode, "screen_number");
            screenInfo.m_nMaxWordCnt = CXmlUtil::GetAttributeValueInt(pScreenNode, "max_word_count");
            if (!screenInfo.IsValid())
            {
                UTILITY_ASSERT(false);
                LOG_ERROR(L"invalid berr screen setting");
                continue;
            }
            
            m_mapWinNum2ScreenInfo[screenInfo.m_strWinNum] = screenInfo;
        }
    }    

    //×ÛºÏÆÁÅäÖÃ
    TiXmlNode* pCompNode = pRootNode->FirstChildElement("COMP");
    if (pCompNode != nullptr)
    {
        for (TiXmlElement* pScreenNode = pCompNode->FirstChildElement("Screen"); pScreenNode != nullptr; pScreenNode = pScreenNode->NextSiblingElement("Screen"))
        {
            CCompScreenInfo screenInfo;
            screenInfo.m_nBlockNum = CXmlUtil::GetAttributeValueInt(pScreenNode, "block_number");
            screenInfo.m_nMaxWordCnt = CXmlUtil::GetAttributeValueInt(pScreenNode, "max_word_count");
            screenInfo.m_strPrefix = CXmlUtil::GetAttributeValueString(pScreenNode, "prefix");

            for (TiXmlElement* pScreenNumberNode = pScreenNode->FirstChildElement("Screen_Number"); pScreenNumberNode != nullptr; pScreenNumberNode = pScreenNumberNode->NextSiblingElement("Screen_Number"))
            {
                screenInfo.m_vecScreenNumber.push_back(CXmlUtil::GetAttributeValueInt(pScreenNumberNode, "value"));
            }

            if (!screenInfo.IsValid())
            {
                UTILITY_ASSERT(false);
                LOG_ERROR(L"invalid comp screen setting");
                continue;
            }

            m_mapBlockNum2ScreenInfo[screenInfo.m_nBlockNum] = screenInfo;
        }
    }

    //¶Á¿¨Æ÷ÅäÖÃ
    TiXmlNode* pCrdrNode = pRootNode->FirstChildElement("CRDR");
    if (pCrdrNode != nullptr)
    {
        m_nCrdrComPort = CXmlUtil::GetAttributeValueInt(pCrdrNode, "port");
        if (m_nCrdrComPort == INVALID_INTEGER)
        {
            UTILITY_ASSERT(false);
            LOG_ERROR(L"invalid crdr port setting");
        }
    }

    //´òÓ¡»úÅäÖÃ
    TiXmlNode* pPrintNode = pRootNode->FirstChildElement("PRNT");
    if (pPrintNode != nullptr)
    {
        m_nPrintMaxCharsPerLine = CXmlUtil::GetAttributeValueInt(pPrintNode, "one_line_max_char_num");
        if (m_nPrintMaxCharsPerLine == INVALID_INTEGER)
        {
            UTILITY_ASSERT(false);
            LOG_ERROR(L"invalid print setting");
            m_nPrintMaxCharsPerLine = 28;
        }

        m_nPortType = CXmlUtil::GetAttributeValueInt(pPrintNode, "port_type");
        if (m_nPortType == INVALID_INTEGER)
        {
            UTILITY_ASSERT(false);
            LOG_ERROR(L"invalid print setting");
            m_nPortType = TX_TYPE_NONE;
        }
    }

    // IC¿¨¶Á¿¨Æ÷ÅäÖÃ
    TiXmlNode* pIccrNode = pRootNode->FirstChildElement("ICCR");
    if (pIccrNode != nullptr)
    {
        m_nIccrType = CXmlUtil::GetAttributeValueInt(pIccrNode, "device_type");
        if (m_nIccrType == INVALID_INTEGER)
        {
            UTILITY_ASSERT(false);
            LOG_ERROR(L"invalid iccr setting");
            m_nIccrType = ICCR_TYPE_2;
        }
    }
}

CWindowScreenInfo CAppSetting::GetWindowScreenInfo(const std::wstring& strWinNum)
{
    auto it = m_mapWinNum2ScreenInfo.find(strWinNum);
    if (it != m_mapWinNum2ScreenInfo.end())
    {
        return it->second;
    }
    else
    {
        return CWindowScreenInfo();
    }
}

CCompScreenInfo CAppSetting::GetCompScreenInfo(int nBlockNum)
{
    auto it = m_mapBlockNum2ScreenInfo.find(nBlockNum);
    if (it != m_mapBlockNum2ScreenInfo.end())
    {
        return it->second;
    }
    else
    {
        return CCompScreenInfo();
    }
}