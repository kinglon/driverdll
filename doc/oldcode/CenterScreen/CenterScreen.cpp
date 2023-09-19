// CenterScreen.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"

//--------------------------------------------------------------------------------------

//���ù���ͷ�ļ�
#include "../commfile/Log/log.h"

//--------------------------------------------------------------------------------------

//����ȫ�ֱ���
char g_cLogFile[300] = {0};

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
				sprintf(g_cLogFile, "%s\\CS%4.4ld%2.2ld%2.2ld.log", LOG_DIR,  st.wYear, st.wMonth, st.wDay);
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
int _declspec(dllexport) __stdcall Open(int iPort, char *pMsg)
{
	sprintf(pMsg, "���豸[%d]�ɹ�", iPort);

	return 0;
}


/*
����:	��ʾ����
����ֵ:	�ɹ�����0��ʧ�ܷ���-1
*/
int _declspec(dllexport) __stdcall ShowInfo(int iBlockNum, char *pInfo, char *pStyle, char *pMsg)
{
	sprintf(pMsg, "��ʾ���ݳɹ� %d - %s - %s ", iBlockNum, pInfo, pStyle);	

	try
	{
		char cCsFile[300] = {0};
		sprintf(cCsFile, "%s\\CenterScreen.shw", LOG_DIR);

		FILE *fp = fopen(cCsFile, "wb");
		fprintf(fp, "%d@%s", iBlockNum, pInfo);
		fflush(fp);
		fclose(fp);

		WriteLog(g_cLogFile, pMsg);
	}
	catch(...)
	{
		WriteLog(g_cLogFile, "��������");
	}	

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
