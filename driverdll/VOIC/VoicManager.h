#pragma once
#include <list>
#include "InterfaceSpec.h"
#include "XmlUtil.h"

class CVoicManager
{
private:
    class CallNumber
    {
    public:
        std::wstring m_strLanguage;  //����
        wchar_t m_chCustomerType = 'A';  //�˿����ʹ�����ĸ
        std::wstring m_strNumber;  //��
        std::wstring m_strWindow; //���ں�
    };

private:
    CVoicManager();
    ~CVoicManager();

public:
    static CVoicManager* GetInstance();

public:
    PB_RESULT Init();
    PB_RESULT PlayVoice(const char* lpszSoundXml);

private:
    static void PlaySoundThread();
    PB_RESULT Parse(const char* lpszSoundXml, CallNumber& callNumber);
    std::wstring SyntheticAudio(const CallNumber& callNumber);
    std::wstring GetSoundFileWithString(const std::wstring& strLanguage, const std::wstring& strFileName);
    std::wstring GetSoundFileWithChar(const std::wstring& strLanguage, const wchar_t& chFileName);

private:
    bool m_bAlreadyInit = false;
    HANDLE m_hNotifyPlay = INVALID_HANDLE_VALUE;
    std::list<std::wstring> m_listSoundFile;
    CCSWrap m_csWrap; //���̱߳��� m_listSoundFile
};