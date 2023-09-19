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
    @name ��
    @param strModuleName ����ģ�����֣���һ��ģ����û��ʼ��ʵ�����ڶ���ģ�����ֻ��¼������
    */
    PB_RESULT Open(const std::wstring& strModuleName, const std::string& strCommandXml);

    //�ڴ�����������ʾ����
    PB_RESULT ShowWindowScreenText(LPCSTR lpszCounterNo, LPCSTR lpszCommandXml);

    //���ۺ�������ʾ����
    PB_RESULT ShowCenterScreenText(IN LPCSTR lpszAddress, IN LPCSTR lpszCommandXml);

    //��ȡ�豸״̬
    PB_RESULT GetStatus(PB_STATUS *lpStatus);

    /**
    @name �ر�
    @param strModuleName ����ģ�����֣����һ��ģ�����ʱ�رմ��ڵ���Դ
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
    std::set<std::wstring> m_setUsingModules;  //��������ʹ�õ�ģ�����
    ISerialUtil* m_pSerialUtil = nullptr;
    std::list<char*> m_listSendData;
    std::vector<std::string> m_vecCompScreenShowText;  //�ۺ�����ʾ���ı�
    HANDLE m_hNotifyHaveNewData = NULL;
    HANDLE m_hNotifyExitSendThread = NULL;
    HANDLE m_hNotifySendThreadAlreadyExit = NULL;
};