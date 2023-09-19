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
    @name 获取指定日志文件的日志对象，第一次调用会实例化日志对象
    */
    static CLogUtil* GetLog(const wchar_t* szLogName);

public:
    /**
    @name 设置日志级别
    */
    void SetLogLevel(ELogLevel nLogLevel);

    /**
    @name 记录日志
    */
    void Log(const char* pFileName, unsigned int nLine, ELogLevel nLogLevel, const wchar_t* szFormat, ...);

private:
    void OpenLog(const wchar_t* szLogFileName);
    void CloseLog();

private:
    static std::map<std::wstring, CLogUtil*> s_mapLogName2LogUtil;
    static CCSWrap s_csLogMapObject;  //多线程保护m_mapLogName2LogUtil
    ELogLevel m_logLevel = ELogLevel::LOG_LEVEL_DEBUG;
    CCSWrap m_csLog;  //多线程控制日志打印
    FILE* m_fpFile = nullptr;
};