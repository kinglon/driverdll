// Printer.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"

//--------------------------------------------------------------------------------------

//���ù���ͷ�ļ�
#include "../commfile/Log/log.h"

//--------------------------------------------------------------------------------------

//����ȫ�ֱ���
char g_cLogFile[300] = {0};


struct STCONFIG
{
	//--------------------
	//��������
	char arrContent[100][1000];	//ʵ�ʴ�ӡ����
	int  iContentCnt;			//��������

	char cPrintFile[300];		//��¼��ӡ������ʱ�ļ�
	//--------------------
};

STCONFIG g_stConfig;

#define	WM_PRINTTICKET	WM_USER+101

//--------------------------------------------------------------------------------------

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
				sprintf(g_cLogFile, "%s\\PRT%4.4ld%2.2ld%2.2ld.log", LOG_DIR,  st.wYear, st.wMonth, st.wDay);
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
��ע:	����COM3��������3���������ڲ����д������ڵĲ����ʡ�����λ��У��λ��ֹͣλ�Ȳ��������ΪLPT��USB������0��
*/
extern "C" int _declspec(dllexport) __stdcall Open(int iPort, char *pMsg)
{
	//sprintf(pMsg, "���豸[%d]�ɹ�", iPort);
	//return 0;

	g_stConfig.iContentCnt = 0;

	sprintf(g_stConfig.cPrintFile, "%s\\printer.prt", LOG_DIR);

	return 0;
}


/*
����:	����ӡ��ͼƬ����
����ֵ:	�ɹ�����0��ʧ�ܷ���-1
*/
int _declspec(dllexport) __stdcall FillPicInfo(char *pPicFile, char *pMsg)
{
	//sprintf(pMsg, "����ӡ��ͼƬ���ݳɹ� %s ", pPicFile);
	//return 0;

	sprintf(g_stConfig.arrContent[g_stConfig.iContentCnt++], "%d#%s#%s", 1, pPicFile, "NULL");

	return 0;
}


/*
����:	����ӡ����������
����ֵ:	�ɹ�����0��ʧ�ܷ���-1
*/
int _declspec(dllexport) __stdcall FillTextInfo(char *pTextInfo, char *pStyle, char *pMsg)
{
	//sprintf(pMsg, "����ӡ���������ݳɹ� %s - %s", pTextInfo, pStyle);
	//return 0;

	sprintf(g_stConfig.arrContent[g_stConfig.iContentCnt++], "%d#%s#%s", 2, pTextInfo, pStyle);

	return 0;
}

/*
����:	ִ�д�ӡ
����ֵ:	�ɹ�����0��ʧ�ܷ���-1
*/
int _declspec(dllexport) __stdcall Print(char *pMsg)
{
	//sprintf(pMsg, "ִ�д�ӡ�ɹ�");
	//return 0;

	
	try
	{
		FILE *fp = fopen(g_stConfig.cPrintFile, "wb");
		for(int i=0; i<g_stConfig.iContentCnt; i++)
		{
			fprintf(fp, "%s\r\n", g_stConfig.arrContent[i]);
		}
		fflush(fp);
		fclose(fp);
	}
	catch(...)
	{
		return -1;
	}

	//-------------------------------------------

	g_stConfig.iContentCnt = 0;

	//-------------------------------------------
	
	HWND hWnd = ::FindWindowA(NULL, "PrinterSample");
	if(hWnd)
	{
		::PostMessage(hWnd, WM_PRINTTICKET, 0, 0);
	}
	
	//-------------------------------------------

	return 0;
}


/*
����:	�ر��豸
����ֵ:	�ɹ�����0��ʧ�ܷ���-1
*/
int _declspec(dllexport) __stdcall Close(char *pMsg)
{
	sprintf(pMsg, "�ر��豸�ɹ�");

	return 0;
}
