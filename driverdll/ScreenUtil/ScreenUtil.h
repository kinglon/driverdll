#pragma once
#include <string>
#include <set>
#include <list>
#include <vector>
#include "InterfaceSpec.h"
#include "IcrCriticalSection.h"
#include "SerialUtil.h"

#ifdef SCREENUTIL_EXPORTS
#define SCREENUTIL_API __declspec(dllexport)
#else
#define SCREENUTIL_API __declspec(dllimport)
#endif

#pragma warning(disable:4251)

class CShowCommand
{
public:
    std::wstring m_strMode;
    std::wstring m_strData;
};

class SCREENUTIL_API CScreenUtil
{
protected:
    CScreenUtil();
    ~CScreenUtil();

public:
    static CScreenUtil* GetInstance();

public:
    /**
    @name 打开
    @param strModuleName 调用模块名字，第一个模块调用会初始化实例，第二个模块调用只记录调用者
    */
    PB_RESULT Open(const std::wstring& strModuleName, const std::string& strCommandXml);

    //在窗口条屏上显示内容
    PB_RESULT ShowWindowScreenText(LPCSTR lpszCounterNo, LPCSTR lpszCommandXml);

    //在综合屏上显示内容
    PB_RESULT ShowCenterScreenText(IN LPCSTR lpszAddress, IN LPCSTR lpszCommandXml);

    //获取设备状态
    PB_RESULT GetStatus(PB_STATUS *lpStatus);

    /**
    @name 关闭
    @param strModuleName 调用模块名字，最后一个模块调用时关闭串口等资源
    */
    PB_RESULT Close(const std::wstring& strModuleName);

private:
    static void SendThread();
    PB_RESULT CheckWindowScreenShowCommand(LPCSTR lpszCounterNo, LPCSTR lpszCommandXml);
    PB_RESULT CheckCompScreenShowCommand(LPCSTR lpszAddress, LPCSTR lpszCommandXml);
    CShowCommand ParseShowCommand(LPCSTR lpszCommandXml);
    void Close();

private:
    CCSWrap m_csWrap;
    bool m_bOpen = false;
    std::set<std::wstring> m_setUsingModules;  //保存正在使用的模块对象
    ISerialUtil* m_pSerialUtil = nullptr;
    std::list<char*> m_listSendData;
    std::vector<std::string> m_vecCompScreenShowText;  //综合屏显示的文本
    HANDLE m_hNotifyHaveNewData = NULL;
    HANDLE m_hNotifyExitSendThread = NULL;
    HANDLE m_hNotifySendThreadAlreadyExit = NULL;
};