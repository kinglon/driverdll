// Key.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"

//--------------------------------------------------------------------------------------

//���ù���ͷ�ļ�
#include "../commfile/Log/log.h"
#include "../commfile/Config/Config.h"
#include "../commfile/serialcom/SerialCom.h"


//--------------------------------------------------------------------------------------

//����ȫ�ֱ���
char g_cLogFile[300] = {0};

struct STKEY
{
	int iKeyNum;
	int iWinNum;
};

typedef int (_stdcall *KEY_CALLBACK) (int iWinNum, int iAct);

struct STCONFIG
{
	//���ñ���
	STKEY *pStKey;	//���̽ṹ��
	int    iKeyCnt;	//��������

	//���б���
	CSerialCom *pSerialCom;

	int iIsThreadExit;		//�߳��Ƿ���Ҫ�˳������������̿����߳��Ƿ��˳�
	int iThreadState;		//�߳�״̬��1=�������У�0=�˳�����

	//��¼���̵Ļص�����
	KEY_CALLBACK Key_CallBack;

};

STCONFIG g_stConfig;

//--------------------------------------------------------------------------------------

//��ǰDLL��ȡ�����ļ�����
void Config_Init()
{
	//���ñ���
	char cCfgFile[300] = {0};
	sprintf(cCfgFile, "%s", "D:\\QmDriver\\Config\\Config.ini");

	char cValue[300];
	
	ReadConfigFile(cCfgFile, "��������", cValue);
	g_stConfig.iKeyCnt = atoi(cValue);
	g_stConfig.pStKey = new STKEY[g_stConfig.iKeyCnt];
	for(int i=0; i<g_stConfig.iKeyCnt; i++)
	{
		char cItem[100] = {0};
		sprintf(cItem, "����%d", i+1);

		memset(cValue, 0, 300);
		ReadConfigFile(cCfgFile, cItem, cValue);

		char *p = strtok(cValue, ",");
		g_stConfig.pStKey[i].iKeyNum = atoi(p);
		p = strtok(NULL, ",");
		g_stConfig.pStKey[i].iWinNum = atoi(p);
	}

	//���б���
	g_stConfig.pSerialCom = NULL;
}


//��ѯ���̵��߳�
DWORD WINAPI Thread_QueryKey(void *ptr)
{
	try
	{
		::Sleep(1000);

		for(;;)
		{
			if(1 == g_stConfig.iIsThreadExit)
			{
				break;
			}

			int iSleepMS = 500 / g_stConfig.iKeyCnt; 

			for(int i=0; i<g_stConfig.iKeyCnt; i++)
			{
				char cKeyNum = 0xFF - g_stConfig.pStKey[i].iKeyNum;
				g_stConfig.pSerialCom->Write(&cKeyNum, 1);
				
				::Sleep(iSleepMS);
			}
		}
	}
	catch(...)
	{
		exit(0);
	}

	g_stConfig.iThreadState = 0;

	return 0;
}

//���������CSerialCom���ã���������㴰�ںźͼ��̶�������
//int Key_CallBack_Tmp(BYTE *pKeyReturn)	//������
int _stdcall Key_CallBack_Tmp(BYTE *pKeyReturn)
{
	//�õ����̺�
	int iKeyNum = 0xFF - pKeyReturn[0];

	//���ݼ��̺ŵõ����ں�
	int iWinNum = -1;
	for(int i=0; i<g_stConfig.iKeyCnt; i++)
	{
		if(iKeyNum == g_stConfig.pStKey[i].iKeyNum)
		{
			iWinNum = g_stConfig.pStKey[i].iWinNum;
			break;
		}
	}

	if(-1 == iWinNum)
	{
		char cLog[200] = {0};
		sprintf(cLog, "δ�ҵ����̺�Ϊ[%d]��Ӧ�Ĵ��ں�", iKeyNum);
		WriteLog(g_cLogFile, cLog);
		return -1;
	}

	//�õ����̵Ķ���
	int iAct = 0;

	int iAskType = (int)(pKeyReturn[1]);
	switch(iAskType)
	{
	case 0xCC://����
		iAct = 1;
		break;
	case 0x33://�غ�
		iAct = 2;
		break;	
	case 0x0F://����(���溬��¼���˳�)
		iAct = 3;
		break;
	case 0xAA://��ͣ
		iAct = 4;
		break;
	case 0x55://��ѯ
		iAct = 5;
		break;
	case 0xF0://ת��
		iAct = 6;
		break;
	}

	g_stConfig.Key_CallBack(iWinNum, iAct);	//����������Ļص�����

	return 0;
}
//--------------------------------------------------------------------------------------

BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{	
    switch (ul_reason_for_call)
	{
		case DLL_PROCESS_ATTACH:
			{
				//�õ���־�ļ���
				SYSTEMTIME st;
				::GetLocalTime(&st);
				sprintf(g_cLogFile, "%s\\KEY%4.4ld%2.2ld%2.2ld.log", LOG_DIR,  st.wYear, st.wMonth, st.wDay);
				
				//��ȡ�����ļ�
				Config_Init();
			}
			break;
		case DLL_THREAD_ATTACH:
			{
				int a = 3;
			}
			break;
		case DLL_THREAD_DETACH:
			{
				int a = 3;
			}
			break;
		case DLL_PROCESS_DETACH:
			{
				int a = 3;
			}
			break;
    }

    return TRUE;
}



/*
����:	���豸
����ֵ:	�ɹ�����0��ʧ�ܷ���-1
*/
extern "C" int _declspec(dllexport) __stdcall Open(int iPort, char *pMsg)
{
	//sprintf(pMsg, "���豸[%d]�ɹ�", iPort);
	//return 0;

	int iRet = 0;
	
	if(NULL != g_stConfig.pSerialCom)
	{
		g_stConfig.pSerialCom->Close();
		delete g_stConfig.pSerialCom;
		g_stConfig.pSerialCom = NULL;
	}
	
	g_stConfig.pSerialCom = new CSerialCom;
	g_stConfig.pSerialCom->m_iUseType = 1;	//���Ƽ���
	
	bool bOpen = g_stConfig.pSerialCom->Open(iPort, "9600,N,8,1");
	if(bOpen)
	{
		iRet = 0;
		sprintf(pMsg, "���豸[%d]�ɹ�", iPort);

		//����һ�������߳�
		g_stConfig.iIsThreadExit = 0;
		g_stConfig.iThreadState = 1;

		HANDLE hThread;
		DWORD ThreadId;	
		hThread=::CreateThread(NULL,0,(unsigned long(_stdcall*)(void*))Thread_QueryKey,NULL,0,&ThreadId);
		::CloseHandle(hThread);
	}
	else
	{
		iRet = -1;
		sprintf(pMsg, "���豸[%d]ʧ��", iPort);
	}

	WriteLog(g_cLogFile, pMsg);

	return iRet;
}


/*
����:	��ʾ����
����ֵ:	�ɹ�����0��ʧ�ܷ���-1
*/
int _declspec(dllexport) __stdcall ShowInfo(int iWinNum, char *pInfo, char *pMsg)
{
	//sprintf(pMsg, "��ʾ���ݳɹ� %d - %s ", iWinNum, pInfo);
	//return 0;

	//���pInfo�Ƿ�Ϊ4λ����
	if(4 != strlen(pInfo))
	{
		sprintf(pMsg, "����pInfo����4�ֽڳ���");
		WriteLog(g_cLogFile, pMsg);
		return -1;
	}
	if( (pInfo[0]<'0') || ('9'<pInfo[0]) || 
		(pInfo[1]<'0') || ('9'<pInfo[1]) || 
		(pInfo[2]<'0') || ('9'<pInfo[2]) || 
		(pInfo[3]<'0') || ('9'<pInfo[3])  )
	{
		sprintf(pMsg, "����pInfo����4λ����");
		WriteLog(g_cLogFile, pMsg);
		return -1;
	}

	//���ݴ��ںŵõ����̺�
	int iKeyNum = -1;
	for(int i=0; i<g_stConfig.iKeyCnt; i++)
	{
		if(iWinNum == g_stConfig.pStKey[i].iWinNum)
		{
			iKeyNum = g_stConfig.pStKey[i].iKeyNum;
			break;
		}
	}
	if(-1 == iKeyNum)
	{
		sprintf(pMsg, "δ���ô��ں�Ϊ[%d]��Ӧ�ļ��̺�", iWinNum);
		WriteLog(g_cLogFile, pMsg);
		return -1;
	}

	//����̷�������
	int iInfoNum = atoi(pInfo);

	BYTE cSendToKey[4]={0};

	cSendToKey[0] = 0xFF - iKeyNum;

	int iH2 = iInfoNum/100;
	int iL2 = iInfoNum%100;
	cSendToKey[1]=(BYTE)(((iH2/10)<<4)+(iH2%10));
	cSendToKey[2]=(BYTE)(((iL2/10)<<4)+(iL2%10));
	
	cSendToKey[3] = cSendToKey[0] + cSendToKey[1] + cSendToKey[2];

	g_stConfig.pSerialCom->Write((char*)cSendToKey, 4);

	sprintf(pMsg, "��ʾ���ݳɹ� %d - %s ", iWinNum, pInfo);
	WriteLog(g_cLogFile, pMsg);

	return 0;
}


/*
����:	���ûص�����ָ��
����ֵ:	�ɹ�����0��ʧ�ܷ���-1
*/
int _declspec(dllexport) __stdcall SetCallBack(void *pCallBack, char *pMsg)
{
	//sprintf(pMsg, "���ûص�����ָ�� 0x%8.8lx ", pCallBack);
	//return 0;

	g_stConfig.Key_CallBack = (KEY_CALLBACK)pCallBack;

	g_stConfig.pSerialCom->Key_CallBack_Tmp = (KEY_CALLBACK_TMP)Key_CallBack_Tmp;


	sprintf(pMsg, "���ûص�����ָ�� 0x%8.8lx ", pCallBack);

	return 0;
}


/*
����:	�ر��豸
����ֵ:	�ɹ�����0��ʧ�ܷ���-1
*/
extern "C" int _declspec(dllexport) __stdcall Close(char *pMsg)
{
	//sprintf(pMsg, "�ر��豸�ɹ�");
	//return 0;

	if(NULL != g_stConfig.pSerialCom)
	{
		//�ر��߳�
		g_stConfig.iIsThreadExit = 1;
		while(1 == g_stConfig.iThreadState) {::Sleep(1);}

		//���ٶ���
		g_stConfig.pSerialCom->Close();
		delete g_stConfig.pSerialCom;
		g_stConfig.pSerialCom = NULL;
	}

	return 0;
}
