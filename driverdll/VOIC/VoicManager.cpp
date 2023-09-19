#include "stdafx.h"
#include "VoicManager.h"
#include <future>
#include "ImCharset.h"
#include "StringUtil.h"
#include "AppSetting.h"
#include "ImPath.h"
#include "ImGuid.h"
#include "WaveFile.h"
#include <mmsystem.h>

using namespace std;

CVoicManager::CVoicManager()
{
    m_hNotifyPlay = CreateEvent(nullptr, TRUE, FALSE, nullptr);
}

CVoicManager::~CVoicManager()
{
    if (m_hNotifyPlay)
    {
        CloseHandle(m_hNotifyPlay);
    }
}

CVoicManager* CVoicManager::GetInstance()
{
    static CVoicManager* pInstance = new CVoicManager();
    return pInstance;
}

PB_RESULT CVoicManager::Init()
{
    if (m_bAlreadyInit)
    {
        LOG_INFO(L"already init");
        return PB_OK;
    }

    async(launch::async, PlaySoundThread);

    m_bAlreadyInit = true;
    return PB_OK;
}

PB_RESULT CVoicManager::PlayVoice(const char* lpszSoundXml)
{
    if (!m_bAlreadyInit)
    {
        LOG_ERROR(L"not init");
        return PB_VOIC_NOT_INITIALIZED;
    }

    CallNumber callNumber;
    PB_RESULT nResult = Parse(lpszSoundXml, callNumber);
    if (nResult != PB_OK)
    {
        return nResult;
    }

    wstring strSoundFile = SyntheticAudio(callNumber);
    if (strSoundFile.empty())
    {
        return PB_VOIC_INTERNAL_ERROR;
    }

    return PB_OK;
}

void CVoicManager::PlaySoundThread()
{
    while (true)
    {
        if (WaitForSingleObject(CVoicManager::GetInstance()->m_hNotifyPlay, INFINITE) == WAIT_OBJECT_0)
        {
            wstring strSoundFile;
            CIcrCriticalSection cs(CVoicManager::GetInstance()->m_csWrap.GetCS());
            if (CVoicManager::GetInstance()->m_listSoundFile.size() > 0)
            {
                strSoundFile = *CVoicManager::GetInstance()->m_listSoundFile.begin();
                CVoicManager::GetInstance()->m_listSoundFile.erase(CVoicManager::GetInstance()->m_listSoundFile.begin());
            }
            else
            {
                ResetEvent(CVoicManager::GetInstance()->m_hNotifyPlay);
            }
            cs.Leave();

            if (!strSoundFile.empty())
            {
                LOG_DEBUG(L"start to play sound : %s", strSoundFile.c_str());
                ::sndPlaySound(strSoundFile.c_str(), SND_SYNC);
                if (CAppSetting::GetInstance()->GetTestSetting()->m_bDeleteFileAfterPlay)
                {
                    ::DeleteFile(strSoundFile.c_str());
                }
            }
        }
        else
        {
            LOG_ERROR(L"failed to wait notify play event, error=%d", GetLastError());
            break;
        }
    }
}

PB_RESULT CVoicManager::Parse(const char* lpszSoundXml, CallNumber& callNumber)
{
    TiXmlDocument document;
    document.Parse(lpszSoundXml, nullptr, TIXML_ENCODING_UTF8);
    TiXmlNode* pSoundNode = document.RootElement();
    if (pSoundNode == nullptr)
    {
        LOG_ERROR(L"failed to parse xml, error=%S, path=%S", document.ErrorDesc(), lpszSoundXml);
        return PB_VOIC_INVALID_SOUND_XML;
    }

    TiXmlNode* pModeNode = pSoundNode->FirstChildElement("Mode");
    wstring strMode = CStringUtil(CXmlUtil::GetNodeText(pModeNode).c_str()).TrimSpace();
    if (strMode != L"NUMBER")
    {
        LOG_ERROR(L"unsupport mode : %s", strMode.c_str());
        return PB_VOIC_INVALID_SOUND_XML;
    }

    TiXmlNode* pLanguageNode = pSoundNode->FirstChildElement("Language");
    wstring strLanguage = CStringUtil(CXmlUtil::GetNodeText(pLanguageNode).c_str()).TrimSpace();
    if (_wcsicmp(strLanguage.c_str(), L"Cantonese") != 0 && _wcsicmp(strLanguage.c_str(), L"English") != 0 && _wcsicmp(strLanguage.c_str(), L"Mandarin") != 0)
    {
        LOG_ERROR(L"unsupport language : %s", strLanguage.c_str());
        return PB_VOIC_INVALID_SOUND_XML;
    }
    else
    {
        callNumber.m_strLanguage = strLanguage;
    }

    TiXmlNode* pDataNode = pSoundNode->FirstChildElement("Data");
    wstring strData = CStringUtil(CXmlUtil::GetNodeText(pDataNode).c_str()).TrimSpace();
    if (strData.empty() || !isalpha(strData[0]))
    {
        LOG_ERROR(L"invalid data : %s", strData.c_str());
        return PB_VOIC_INVALID_SOUND_XML;
    }
    else
    {
        callNumber.m_chCustomerType = strData[0];
    }
    
    int nIndex = strData.find(L'|');
    if (nIndex == string::npos)
    {
        LOG_ERROR(L"invalid data : %s", strData.c_str());
        return PB_VOIC_INVALID_SOUND_XML;
    }

    wstring strNumber = CStringUtil(strData.substr(1, nIndex-1).c_str()).TrimSpace();
    wstring strWindow = CStringUtil(strData.substr(nIndex + 1).c_str()).TrimSpace();
    if (strNumber.empty() || strWindow.empty())
    {
        LOG_ERROR(L"invalid data : %s", strData.c_str());
        return PB_VOIC_INVALID_SOUND_XML;
    }
    else
    {
        callNumber.m_strNumber = strNumber;
        callNumber.m_strWindow = strWindow;
    }

    return PB_OK;
}

std::wstring CVoicManager::SyntheticAudio(const CallNumber& callNumber)
{
    /**
    合成模板：请A5023到3号窗口
    */
    wstring strSoundFile = CImPath::GetCachePath() + CImGuid::CreateGuid() + L".wav";
    LOG_DEBUG(L"synthetic audio file : %s", strSoundFile.c_str());

    wstring strBindFile = GetSoundFileWithString(callNumber.m_strLanguage, L"please");
    if (!CWaveFile::BindFile(strSoundFile, strBindFile))
    {
        return L"";
    }

    strBindFile = GetSoundFileWithChar(callNumber.m_strLanguage, callNumber.m_chCustomerType);
    if (!CWaveFile::BindFile(strSoundFile, strBindFile))
    {
        return L"";
    }

    for (auto ch : callNumber.m_strNumber)
    {
        strBindFile = GetSoundFileWithChar(callNumber.m_strLanguage, ch);
        if (!CWaveFile::BindFile(strSoundFile, strBindFile))
        {
            return L"";
        }
    }

    if (_wcsicmp(callNumber.m_strLanguage.c_str(), L"English") == 0)
    {
        strBindFile = GetSoundFileWithString(callNumber.m_strLanguage, L"numwindow");
        if (!CWaveFile::BindFile(strSoundFile, strBindFile))
        {
            return L"";
        }

        for (auto ch : callNumber.m_strWindow)
        {
            strBindFile = GetSoundFileWithChar(callNumber.m_strLanguage, ch);
            if (!CWaveFile::BindFile(strSoundFile, strBindFile))
            {
                return L"";
            }
        }
    }
    else
    {
        strBindFile = GetSoundFileWithString(callNumber.m_strLanguage, L"numgoto");
        if (!CWaveFile::BindFile(strSoundFile, strBindFile))
        {
            return L"";
        }

        for (auto ch : callNumber.m_strWindow)
        {
            strBindFile = GetSoundFileWithChar(callNumber.m_strLanguage, ch);
            if (!CWaveFile::BindFile(strSoundFile, strBindFile))
            {
                return L"";
            }
        }

        strBindFile = GetSoundFileWithString(callNumber.m_strLanguage, L"numwindow");
        if (!CWaveFile::BindFile(strSoundFile, strBindFile))
        {
            return L"";
        }
    }    

    CIcrCriticalSection cs(m_csWrap.GetCS());
    m_listSoundFile.push_back(strSoundFile);
    SetEvent(m_hNotifyPlay);
    cs.Leave();

    return strSoundFile;
}

std::wstring CVoicManager::GetSoundFileWithString(const std::wstring& strLanguage, const std::wstring& strFileName)
{
    wstring strSoundFile = CImPath::GetResourcePath() + L"SOUND\\" + strLanguage + L"\\" + strFileName + L".wav";
    return strSoundFile;
}

std::wstring CVoicManager::GetSoundFileWithChar(const std::wstring& strLanguage, const wchar_t& chFileName)
{
    wstring strFileName;
    if (_wcsicmp(strLanguage.c_str(), L"Mandarin") == 0 && chFileName == L'1')
    {
        strFileName.append(L"1yi");
    }
    else
    {
        strFileName.append(1, chFileName);
    }    
    return GetSoundFileWithString(strLanguage, strFileName);
}