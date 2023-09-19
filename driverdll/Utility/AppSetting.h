#pragma once
#include <string>
#include <map>
#include <vector>
#include "Utility.h"
#include "XmlUtil.h"

#define INVALID_SCREEN_NUMBER -1   //��Ч��Ļ��
#define INVALID_SERIAL_PORT      0xffffffff   //��Ч���ڶ˿�
#define INVALID_BLOCK_NUMBER      -1   //��Ч�ۺ������

/** TxOpenPrinter�ĵ�һ������ */
#define TX_TYPE_NONE       0
#define TX_TYPE_USB        1
#define TX_TYPE_LPT        2
#define TX_TYPE_COM        3
#define TX_TYPE_MEM        4
#define TX_TYPE_FD         5
#define TX_TYPE_NET        6

// ��ȡIC���������豸����
#define ICCR_TYPE_1  1  // �ɿ�
#define ICCR_TYPE_2  2  // ����һ�����Ƭ��ˢоƬ��

#pragma warning(disable:4251)

class UTILITY_API CTestSetting
{
public:
    bool m_bUseMockImpl = false;  //ʹ��ģ��Ĵ��ڹ���ʵ��
    std::string m_strCrdrMockData;
    bool m_bDeleteFileAfterPlay = true;
};

class UTILITY_API CWindowScreenInfo
{
public:
    std::wstring m_strWinNum;			                                    //���ں�
    int m_nScreenNum = INVALID_SCREEN_NUMBER;			//����
    int m_nMaxWordCnt = 0;			                                        //������ʾ��������

public:
    bool IsValid()
    {
        if (m_strWinNum.empty() || m_nScreenNum == INVALID_INTEGER || m_nMaxWordCnt == INVALID_INTEGER)
        {
            return false;
        }
        else
        {
            return true;
        }
    }
};

class UTILITY_API CCompScreenInfo
{
public:
    int m_nBlockNum = INVALID_BLOCK_NUMBER;			    //���
    int m_nMaxWordCnt = 0;			                                        //������ʾ��������
    std::wstring m_strPrefix;                                                    //������ʾǰ׺
    std::vector<int> m_vecScreenNumber;                             //����������Ļ��

public:
    bool IsValid()
    {
        if (m_nBlockNum == INVALID_INTEGER || m_nMaxWordCnt == INVALID_INTEGER)
        {
            return false;
        }

        if (m_vecScreenNumber.size() <= 0)
        {
            return false;
        }
        
        for (auto nScreenNumber : m_vecScreenNumber)
        {
            if (nScreenNumber == INVALID_INTEGER)
            {
                return false;
            }
        }

        return true;
    }
};

class UTILITY_API CAppSetting
{
protected:
    CAppSetting();
    ~CAppSetting();

public:
    static CAppSetting* GetInstance();
    void Init() {}

public:
    /**
    @name ��������
    */
    void LoadDeviceSetting();
    void LoadTestSetting();

    /**
    @name ��ȡ��������
    */
    const CTestSetting* GetTestSetting() { return &m_testSetting; }

    /**
    @name ��ȡ��Ļ���������ڶ˿�
    */
    int GetScreenComPort() { return m_nScreenComPort; }

    /**
    @name ��ȡ����������Ϣ
    */
    CWindowScreenInfo GetWindowScreenInfo(const std::wstring& strWinNum);

    /**
    @name ��ȡ�ۺ�����Ϣ
    */
    CCompScreenInfo GetCompScreenInfo(int nBlockNum);

    /**
    @name ��ȡ���������ڶ˿�
    */
    int GetCrdrComPort() { return m_nCrdrComPort; }

    /**
    @name ��ȡ��ӡ��ÿ����ʾ����ַ���
    */
    int GetPrintMaxCharsPerLine() { return m_nPrintMaxCharsPerLine; }

    /**
    @name ��ȡ��ӡ���˿����� TX_TYPE_*
    */
    int GetPrinterPortType() { return m_nPortType; }

    /**
    @name ��ȡIC���������豸���� ICCR_TYPE_*
    */
    int GetIccrType() { return m_nIccrType; }

private:
    CTestSetting m_testSetting;
    int m_nScreenComPort = INVALID_SERIAL_PORT;
    std::map<std::wstring, CWindowScreenInfo> m_mapWinNum2ScreenInfo;
    std::map<int, CCompScreenInfo> m_mapBlockNum2ScreenInfo;
    int m_nCrdrComPort = INVALID_SERIAL_PORT;
    int m_nPrintMaxCharsPerLine = 28;
    int m_nPortType = TX_TYPE_NONE;
    int m_nIccrType = ICCR_TYPE_2;
};