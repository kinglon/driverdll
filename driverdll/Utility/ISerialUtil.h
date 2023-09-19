#pragma once
#include <wtypes.h>
#include "Utility.h"

class UTILITY_API IOnSerialDataArrive
{
public:
    virtual void OnSerialDataArrive(const char* pData, int nDataLength) = 0;
};

class UTILITY_API ISerialUtil
{
public:
    virtual ~ISerialUtil() {}

public:
    /**
    @name �򿪴���
    @param dwPort ���ں�
    @param szSetStr �������ô�����"9600, 8, n, 1" ��9600���������ʡ�n������żУ�顢8��������λ��1����ֹͣλ
    @param dwDesiredAccess ����Ȩ��
    */
    virtual bool Open(DWORD dwPort, char *szSetStr, DWORD dwDesiredAccess) = 0;

    /**
    @name ���ô������ݵ���ص�
    @param pCallback �ص�
    */
    virtual void SetSerialDataArriveCallback(IOnSerialDataArrive* pCallback) = 0;

    /**
    @name ��������
    @param szBuffer ������
    @param dwBufferLength ��������С
    @return ʵ�ʷ��͵��ֽ���
    */
    virtual DWORD Write(char *szBuffer, DWORD dwBufferLength) = 0;

    /**
    @name �رմ���
    */
    virtual void Close() = 0;
};