#pragma once
#include <map>
#include "Utility.h"
#include "IcrCriticalSection.h"

#pragma warning(disable:4251)

enum class UTILITY_API ELogLevel
{
    LOG_LEVEL_DEBUG = 1,
    LOG_LEVEL_INFO,
    LOG_LEVEL_ERROR,
    LOG_LEVEL_FATAL
};

class UTILITY_API CLogUtil
{
protected:
    CLogUtil(const wchar_t* szLogFileName);
    ~CLogUtil();

public:
    /**
    @name ��ȡָ����־�ļ�����־���󣬵�һ�ε��û�ʵ������־����
    */
    static CLogUtil* GetLog(const wchar_t* szLogName);

public:
    /**
    @name ������־����
    */
    void SetLogLevel(ELogLevel nLogLevel);

    /**
    @name ��¼��־
    */
    void Log(const char* pFileName, unsigned int nLine, ELogLevel nLogLevel, const wchar_t* szFormat, ...);

private:
    void OpenLog(const wchar_t* szLogFileName);
    void CloseLog();

private:
    static std::map<std::wstring, CLogUtil*> s_mapLogName2LogUtil;
    static CCSWrap s_csLogMapObject;  //���̱߳���m_mapLogName2LogUtil
    ELogLevel m_logLevel = ELogLevel::LOG_LEVEL_DEBUG;
    CCSWrap m_csLog;  //���߳̿�����־��ӡ
    FILE* m_fpFile = nullptr;
};