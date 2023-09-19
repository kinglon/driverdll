#pragma once
#include <string>
#include <map>
#include <vector>
#include "Utility.h"
#include "XmlUtil.h"

#define INVALID_SCREEN_NUMBER -1   //无效屏幕号
#define INVALID_SERIAL_PORT      0xffffffff   //无效串口端口
#define INVALID_BLOCK_NUMBER      -1   //无效综合屏块号

/** TxOpenPrinter的第一个参数 */
#define TX_TYPE_NONE       0
#define TX_TYPE_USB        1
#define TX_TYPE_LPT        2
#define TX_TYPE_COM        3
#define TX_TYPE_MEM        4
#define TX_TYPE_FD         5
#define TX_TYPE_NET        6

// 获取IC卡读卡器设备类型
#define ICCR_TYPE_1  1  // 旧款
#define ICCR_TYPE_2  2  // 二合一款，可贴片可刷芯片卡

#pragma warning(disable:4251)

class UTILITY_API CTestSetting
{
public:
    bool m_bUseMockImpl = false;  //使用模拟的串口工具实现
    std::string m_strCrdrMockData;
    bool m_bDeleteFileAfterPlay = true;
};

class UTILITY_API CWindowScreenInfo
{
public:
    std::wstring m_strWinNum;			                                    //窗口号
    int m_nScreenNum = INVALID_SCREEN_NUMBER;			//屏号
    int m_nMaxWordCnt = 0;			                                        //条屏显示最大的字数

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
    int m_nBlockNum = INVALID_BLOCK_NUMBER;			    //块号
    int m_nMaxWordCnt = 0;			                                        //条屏显示最大的字数
    std::wstring m_strPrefix;                                                    //内容显示前缀
    std::vector<int> m_vecScreenNumber;                             //条屏窗口屏幕号

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
    @name 加载设置
    */
    void LoadDeviceSetting();
    void LoadTestSetting();

    /**
    @name 获取测试设置
    */
    const CTestSetting* GetTestSetting() { return &m_testSetting; }

    /**
    @name 获取屏幕发送器串口端口
    */
    int GetScreenComPort() { return m_nScreenComPort; }

    /**
    @name 获取窗口条屏信息
    */
    CWindowScreenInfo GetWindowScreenInfo(const std::wstring& strWinNum);

    /**
    @name 获取综合屏信息
    */
    CCompScreenInfo GetCompScreenInfo(int nBlockNum);

    /**
    @name 获取读卡器串口端口
    */
    int GetCrdrComPort() { return m_nCrdrComPort; }

    /**
    @name 获取打印机每行显示最大字符数
    */
    int GetPrintMaxCharsPerLine() { return m_nPrintMaxCharsPerLine; }

    /**
    @name 获取打印机端口类型 TX_TYPE_*
    */
    int GetPrinterPortType() { return m_nPortType; }

    /**
    @name 获取IC卡读卡器设备类型 ICCR_TYPE_*
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