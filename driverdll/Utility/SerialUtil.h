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
    @name ���캯��
    @param fAutoBeginThread ��־�Ƿ����������߳̽��մ�������
    @param dwIOMode 0��ʾͬ����FILE_FLAG_OVERLAPPED ��ʾ�첽
    */
    CSerialUtil(bool fAutoBeginThread, DWORD dwIOMode);
    virtual ~CSerialUtil();

public:
    /**
    @name �򿪴���
    @param dwPort ���ں�
    */
    bool Open(DWORD dwPort);

    /**
    @name �򿪴���
    @param dwPort ���ں�
    @param dwBaudRate ָ��������
    @param dwDesiredAccess ����Ȩ��
    */
    bool Open(DWORD dwPort, DWORD dwBaudRate);

    /**
    @name �򿪴���
    @param dwPort ���ں�
    @param szSetStr �������ô�����"9600, 8, n, 1" ��9600���������ʡ�n������żУ�顢8��������λ��1����ֹͣλ
    */
    virtual bool Open(DWORD dwPort, char *szSetStr, DWORD dwDesiredAccess) override;

    /**
    @name ���ô������ݵ���ص�
    @param pCallback �ص�
    */
    virtual void SetSerialDataArriveCallback(IOnSerialDataArrive* pCallback) override { m_pSerialDataArriveCallback = pCallback; }

    /**
    @name �жϴ����ǻ��
    */
    bool IsOpen();

    /**
    @name ��ô��ھ��
    */
    HANDLE GetHandle();

    /**
    @name ���ô���
    @param szSetStr �������ô�����"9600, 8, n, 1" ��9600���������ʡ�n������żУ�顢8��������λ��1����ֹͣλ
    */
    bool SetState(char *szSetStr);

    /**
    @name ���ô���
    @param dwBaudRate ������
    @param dwByteSize ����λ
    @param dwParity ��żУ��λ
    @param dwStopBits ֹͣλ
    */
    bool SetState(DWORD dwBaudRate, DWORD dwByteSize = 8, DWORD dwParity = NOPARITY, DWORD dwStopBits = ONESTOPBIT);

    /**
    @name ���ô��ڵ�I/O��������С
    @param dwInputSize ���մ�С
    @param dwOutputSize ���ʹ�С
    */
    bool SetBufferSize(DWORD dwInputSize, DWORD dwOutputSize);

    /**
    @name ������
    @param szBuffer ������
    @param dwBufferLength ��������С
    @param dwWaitTime �ȴ�����
    @return ʵ�ʶ����ֽ���
    */
    DWORD Read(char *szBuffer, DWORD dwBufferLength, DWORD dwWaitTime = 20);

    /**
    @name ��������
    @param szBuffer ������
    @param dwBufferLength ��������С
    @return ʵ�ʷ��͵��ֽ���
    */
    virtual DWORD Write(char *szBuffer, DWORD dwBufferLength) override;

    /**
    @name ��������
    @param szBuffer ��������\0��β
    @return ʵ�ʷ��͵��ֽ���
    */
    DWORD Write(char *szBuffer);

    /**
    @name ͬ����������
    @param szBuffer ������
    @param dwBufferLength ��������С
    @return ʵ�ʷ��͵��ֽ���
    */
    DWORD WriteSync(char *szBuffer, DWORD dwBufferLength);

    /**
    @name �رմ���
    */
    virtual void Close() override;

    /**
    @name ���ö�ȡ�����������ʱ֪ͨ�ص�
    @param iNum ��ȡ�����������ʱ֪ͨ�ص�
    */
    void SetNotifyNum(int iNum);

private:
	//�����߳̿��� �������߳�
    bool BeginThread();

	//�߳��Ƿ�����
    bool IsThreadRunning();

	//��ֹ�߳�
    bool EndThread(DWORD dwWaitTime = 100);

    //��ʼ��
    void Init();	
    void UnInit();

    //�󶨴���
    void BindCommPort(DWORD dwPort);

    //�򿪴���
    virtual bool OpenCommPort(DWORD dwDesiredAccess);

    //���ô���
    virtual bool SetupPort();

    //�����߳�
    static DWORD WINAPI CommThreadProc(LPVOID lpPara);

private:
    IComApi* m_pComApi = nullptr;
    IOnSerialDataArrive* m_pSerialDataArriveCallback = nullptr;
	volatile DWORD _dwPort = 0;	 //���ں�
	volatile HANDLE _hCommHandle = INVALID_HANDLE_VALUE;  //���ھ��
	char _szCommStr[20];
	DCB _DCB;			  //�����ʣ�ֹͣλ����
	COMMTIMEOUTS _CO;     //��ʱ�ṹ
    DWORD _dwIOMode = FILE_FLAG_OVERLAPPED;  // 0 ͬ��  Ĭ�� FILE_FLAG_OVERLAPPED �ص�I/O �첽
	OVERLAPPED _ReadOverlapped, _WriteOverlapped; //	�ص�I/O
	volatile HANDLE _hThreadHandle;	//�����߳�
	volatile DWORD _dwNotifyNum; //���ն����ֽڷ���֪ͨ��Ϣ
	volatile bool _fRunFlag; //�߳�����ѭ����־
	bool _fAutoBeginThread;//Open() �Զ� BeginThread();
	OVERLAPPED _WaitOverlapped; //WaitCommEvent use
};