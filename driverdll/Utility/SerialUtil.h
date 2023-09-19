#pragma once

#include <stdio.h>
#include <windows.h>
#include "Utility.h"
#include "ISerialUtil.h"
#include "IComApi.h"

#pragma warning(disable: 4530)
#pragma warning(disable: 4786)
#pragma warning(disable: 4800)
#pragma warning(disable: 4996)

class UTILITY_API CSerialUtil : public ISerialUtil
{
public:
    /**
    @name 构造函数
    @param fAutoBeginThread 标志是否启动监视线程接收串口数据
    @param dwIOMode 0表示同步，FILE_FLAG_OVERLAPPED 表示异步
    */
    CSerialUtil(bool fAutoBeginThread, DWORD dwIOMode);
    virtual ~CSerialUtil();

public:
    /**
    @name 打开串口
    @param dwPort 串口号
    */
    bool Open(DWORD dwPort);

    /**
    @name 打开串口
    @param dwPort 串口号
    @param dwBaudRate 指定波特率
    @param dwDesiredAccess 访问权限
    */
    bool Open(DWORD dwPort, DWORD dwBaudRate);

    /**
    @name 打开串口
    @param dwPort 串口号
    @param szSetStr 串口配置串，如"9600, 8, n, 1" 中9600代表波特率、n代表奇偶校验、8代表数据位、1代表停止位
    */
    virtual bool Open(DWORD dwPort, char *szSetStr, DWORD dwDesiredAccess) override;

    /**
    @name 设置串口数据到达回调
    @param pCallback 回调
    */
    virtual void SetSerialDataArriveCallback(IOnSerialDataArrive* pCallback) override { m_pSerialDataArriveCallback = pCallback; }

    /**
    @name 判断串口是或打开
    */
    bool IsOpen();

    /**
    @name 获得串口句炳
    */
    HANDLE GetHandle();

    /**
    @name 配置串口
    @param szSetStr 串口配置串，如"9600, 8, n, 1" 中9600代表波特率、n代表奇偶校验、8代表数据位、1代表停止位
    */
    bool SetState(char *szSetStr);

    /**
    @name 配置串口
    @param dwBaudRate 波特率
    @param dwByteSize 数据位
    @param dwParity 奇偶校验位
    @param dwStopBits 停止位
    */
    bool SetState(DWORD dwBaudRate, DWORD dwByteSize = 8, DWORD dwParity = NOPARITY, DWORD dwStopBits = ONESTOPBIT);

    /**
    @name 设置串口的I/O缓冲区大小
    @param dwInputSize 接收大小
    @param dwOutputSize 发送大小
    */
    bool SetBufferSize(DWORD dwInputSize, DWORD dwOutputSize);

    /**
    @name 读数据
    @param szBuffer 缓冲区
    @param dwBufferLength 缓冲区大小
    @param dwWaitTime 等待秒数
    @return 实际读到字节数
    */
    DWORD Read(char *szBuffer, DWORD dwBufferLength, DWORD dwWaitTime = 20);

    /**
    @name 发送数据
    @param szBuffer 缓冲区
    @param dwBufferLength 缓冲区大小
    @return 实际发送的字节数
    */
    virtual DWORD Write(char *szBuffer, DWORD dwBufferLength) override;

    /**
    @name 发送数据
    @param szBuffer 缓冲区，\0结尾
    @return 实际发送的字节数
    */
    DWORD Write(char *szBuffer);

    /**
    @name 同步发送数据
    @param szBuffer 缓冲区
    @param dwBufferLength 缓冲区大小
    @return 实际发送的字节数
    */
    DWORD WriteSync(char *szBuffer, DWORD dwBufferLength);

    /**
    @name 关闭串口
    */
    virtual void Close() override;

    /**
    @name 设置读取数据量达多少时通知回调
    @param iNum 读取数据量达多少时通知回调
    */
    void SetNotifyNum(int iNum);

private:
	//辅助线程控制 建监视线程
    bool BeginThread();

	//线程是否运行
    bool IsThreadRunning();

	//终止线程
    bool EndThread(DWORD dwWaitTime = 100);

    //初始化
    void Init();	
    void UnInit();

    //绑定串口
    void BindCommPort(DWORD dwPort);

    //打开串口
    virtual bool OpenCommPort(DWORD dwDesiredAccess);

    //设置串口
    virtual bool SetupPort();

    //监视线程
    static DWORD WINAPI CommThreadProc(LPVOID lpPara);

private:
    IComApi* m_pComApi = nullptr;
    IOnSerialDataArrive* m_pSerialDataArriveCallback = nullptr;
	volatile DWORD _dwPort = 0;	 //串口号
	volatile HANDLE _hCommHandle = INVALID_HANDLE_VALUE;  //串口句柄
	char _szCommStr[20];
	DCB _DCB;			  //波特率，停止位，等
	COMMTIMEOUTS _CO;     //超时结构
    DWORD _dwIOMode = FILE_FLAG_OVERLAPPED;  // 0 同步  默认 FILE_FLAG_OVERLAPPED 重叠I/O 异步
	OVERLAPPED _ReadOverlapped, _WriteOverlapped; //	重叠I/O
	volatile HANDLE _hThreadHandle;	//辅助线程
	volatile DWORD _dwNotifyNum; //接收多少字节发送通知消息
	volatile bool _fRunFlag; //线程运行循环标志
	bool _fAutoBeginThread;//Open() 自动 BeginThread();
	OVERLAPPED _WaitOverlapped; //WaitCommEvent use
};