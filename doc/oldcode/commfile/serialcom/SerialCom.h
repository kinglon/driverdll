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
};//注：STSCREENSEND里还可以加这样几个属性：条屏类型、不同类型应发送的内容

typedef int (_stdcall *KEY_CALLBACK_TMP)(BYTE *pBuf);

//typedef int  (_stdcall *MNCARD_EVENTFUN)(char* szParam);
typedef int (_stdcall *MN_CALLBACK) (char *pCardInfo);




class CSerialCom  : public cnComm
{
public:
	CSerialCom(bool fAutoBeginThread=true,DWORD dwIOMode=FILE_FLAG_OVERLAPPED);
	virtual ~CSerialCom();
	
public://接收
	virtual void OnReceive();
public://发送
	int Send(char* pSend,int iLen);

public:
	int	 m_iUseType;	//1=轮询小键盘；2=评价器；4=发窗口条屏；8发公共条屏；16读卡器

public://串口
	int	 m_iComNum;
	
public://读卡器
	char m_cCardInfo[1000];
	int	 m_iCardInfoLen;

	int	 m_iReadCardType;	//读卡器类型

	//MNCARD_EVENTFUN  MnCard_EventFun;
	MN_CALLBACK  Mn_CallBack;

public:
	KEY_CALLBACK_TMP Key_CallBack_Tmp;
	
};

#endif // !defined(AFX_SERIALCOM_H__DAD2F0E4_C99F_4528_81D5_1C5120C14A08__INCLUDED_)
