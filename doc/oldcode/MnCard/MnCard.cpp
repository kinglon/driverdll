// MnCard.cpp : Defines the entry point for the DLL application.
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


typedef int (_stdcall *MN_CALLBACK) (char *pCardInfo);

struct STCONFIG
{
	//���б���
	CSerialCom *pSerialCom;



	//��¼����ˢ�����Ļص�����
	MN_CALLBACK Mn_CallBack;

};

STCONFIG g_stConfig;

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
				sprintf(g_cLogFile, "%s\\MN%4.4ld%2.2ld%2.2ld.log", LOG_DIR,  st.wYear, st.wMonth, st.wDay);
											
				//���б���
				g_stConfig.pSerialCom = NULL;

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
	g_stConfig.pSerialCom->m_iUseType = 16;	//���ƴ���ˢ����
	g_stConfig.pSerialCom->m_iReadCardType = 0;	//����������
	
	bool bOpen = g_stConfig.pSerialCom->Open(iPort, "9600,N,8,1");
	if(bOpen)
	{
		iRet = 0;
		sprintf(pMsg, "���豸[%d]�ɹ�", iPort);
	
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
����:	���ûص�����ָ��
����ֵ:	�ɹ�����0��ʧ�ܷ���-1
*/
extern "C" int _declspec(dllexport) __stdcall SetCallBack(void *pCallBack, char *pMsg)
{
	//sprintf(pMsg, "���ûص�����ָ�� 0x%8.8lx ", pCallBack);
	//return 0;

	g_stConfig.Mn_CallBack = (MN_CALLBACK)pCallBack;

	g_stConfig.pSerialCom->Mn_CallBack = (MN_CALLBACK)pCallBack;

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
		//���ٶ���
		g_stConfig.pSerialCom->Close();
		delete g_stConfig.pSerialCom;
		g_stConfig.pSerialCom = NULL;
	}

	return 0;
}
