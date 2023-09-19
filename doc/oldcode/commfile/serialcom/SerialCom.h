// SerialCom.h: interface for the CSerialCom class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SERIALCOM_H__DAD2F0E4_C99F_4528_81D5_1C5120C14A08__INCLUDED_)
#define AFX_SERIALCOM_H__DAD2F0E4_C99F_4528_81D5_1C5120C14A08__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "com\\cnComm.h"



struct STSCREENSEND
{
	BYTE	cContent[69];
};//ע��STSCREENSEND�ﻹ���Լ������������ԣ��������͡���ͬ����Ӧ���͵�����

typedef int (_stdcall *KEY_CALLBACK_TMP)(BYTE *pBuf);

//typedef int  (_stdcall *MNCARD_EVENTFUN)(char* szParam);
typedef int (_stdcall *MN_CALLBACK) (char *pCardInfo);




class CSerialCom  : public cnComm
{
public:
	CSerialCom(bool fAutoBeginThread=true,DWORD dwIOMode=FILE_FLAG_OVERLAPPED);
	virtual ~CSerialCom();
	
public://����
	virtual void OnReceive();
public://����
	int Send(char* pSend,int iLen);

public:
	int	 m_iUseType;	//1=��ѯС���̣�2=��������4=������������8������������16������

public://����
	int	 m_iComNum;
	
public://������
	char m_cCardInfo[1000];
	int	 m_iCardInfoLen;

	int	 m_iReadCardType;	//����������

	//MNCARD_EVENTFUN  MnCard_EventFun;
	MN_CALLBACK  Mn_CallBack;

public:
	KEY_CALLBACK_TMP Key_CallBack_Tmp;
	
};

#endif // !defined(AFX_SERIALCOM_H__DAD2F0E4_C99F_4528_81D5_1C5120C14A08__INCLUDED_)