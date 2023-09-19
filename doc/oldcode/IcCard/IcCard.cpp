// IcCard.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"

//--------------------------------------------------------------------------------------

//���ù���ͷ�ļ�
#include "../commfile/Log/log.h"
#include "../commfile/Config/Config.h"
#include "../commfile/lib/ABCDLL.h"


//--------------------------------------------------------------------------------------

//����ȫ�ֱ���
char g_cLogFile[300] = {0};


typedef int (_stdcall *IC_CALLBACK) (char *pCardInfo);

struct STCONFIG
{
	//���б���
	int iIsThreadExit;		//�߳��Ƿ���Ҫ�˳������������̿����߳��Ƿ��˳�
	int iThreadState;		//�߳�״̬��1=�������У�0=�˳�����

	//��¼���̵Ļص�����
	IC_CALLBACK Ic_CallBack;
};

STCONFIG g_stConfig;

//--------------------------------------------------------------------------------------


DWORD WINAPI Thread_ReadCard(void *ptr)
{
	int iRet;

	try
	{
		::Sleep(1000);

		for(;;)
		{
			if(1 == g_stConfig.iIsThreadExit)
			{
				break;
			}

			//----------------------------------------------------------
			char cDebugMsg[1000];

			char pucIDMsg[1024] = {0};
			int iCurMode;

			int iStrLen = 0;
			int iStrPos = 0;
			char cItemLen[10];
			int iItemLen = 0;

			memset(pucIDMsg, 0, 1024);
			iRet = ReadCardInfo(0, 115200, 0x04 | 0x08, pucIDMsg, iCurMode, 10.0);

			if(0 == iRet)
			{
				sprintf(cDebugMsg, "ReadCardInfo() = %d (%s)", iRet, pucIDMsg);
				WriteLog(g_cLogFile, cDebugMsg);

				switch(iCurMode)
				{
				case 0x04:	//�Ӵ�ʽIC��
				case 0x08:	//�ǽӴ�ʽIC��
					{
						char cTrack1[100] = {0};
						char cTrack2[100] = {0};

						//������ȡ����Ϣ
						iStrLen = strlen(pucIDMsg);
						iStrPos = 0;

						while(iStrPos < iStrLen)
						{
							switch(pucIDMsg[iStrPos])
							{
							case 'E':	//�ڶ��ŵ�
								{
									memset(cItemLen, 0, 10);
									memcpy(cItemLen, &pucIDMsg[iStrPos+1], 3);
									iItemLen = atoi(cItemLen);
									memcpy(cTrack2, &pucIDMsg[iStrPos+1+3], iItemLen);
									iStrPos += 1+3+iItemLen;

									//���ڶ��ŵ��еġ�D�����Ÿ�Ϊ��=��
									for(int i=0; i<(int)strlen(cTrack2); i++)
									{
										if('D' == cTrack2[i])
										{
											cTrack2[i] = '=';
										}
									}
								}
								break;
							case 'F':	//��һ�ŵ�
								{
									memset(cItemLen, 0, 10);
									memcpy(cItemLen, &pucIDMsg[iStrPos+1], 3);
									iItemLen = atoi(cItemLen);
									memcpy(cTrack1, &pucIDMsg[iStrPos+1+3], iItemLen);
									iStrPos += 1+3+iItemLen;
								}
								break;
							default:
								{
									memset(cItemLen, 0, 10);
									memcpy(cItemLen, &pucIDMsg[iStrPos+1], 3);
									iItemLen = atoi(cItemLen);
									iStrPos += 1+3+iItemLen;
								}
							}
						}

						try
						{
							char cCardInfo[1000] = {0};
							sprintf(cCardInfo, "%s+%s", cTrack2, cTrack1);
							g_stConfig.Ic_CallBack(cCardInfo);
						}
						catch(...)
						{
						}

						sprintf(cDebugMsg, "IC��:(%s+%s)", cTrack2, cTrack1);
						WriteLog(g_cLogFile, cDebugMsg);

					}
					break;
				}
			}
			else
			{
				sprintf(cDebugMsg, "ReadCardInfo() = %d", iRet);
				WriteLog(g_cLogFile, cDebugMsg);
			}

			//----------------------------------------------------------

			::Sleep(100);
		}
	}
	catch(...)
	{
		g_stConfig.iThreadState = 0;
		exit(0);
	}

	g_stConfig.iThreadState = 0;

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
				sprintf(g_cLogFile, "%s\\IC%4.4ld%2.2ld%2.2ld.log", LOG_DIR,  st.wYear, st.wMonth, st.wDay);
				
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
	//sprintf(pMsg, "���豸[%d]�ɹ�", iPort);
	//return 0;

	g_stConfig.iIsThreadExit = 0;	//����Ҫ�˳��߳�
	g_stConfig.iThreadState = 1;	//�߳���������
	
	//����һ�������߳�
	HANDLE hThread;
	DWORD ThreadId;	
	hThread=::CreateThread(NULL,0,(unsigned long(_stdcall*)(void*))Thread_ReadCard,NULL,0,&ThreadId);
	::CloseHandle(hThread);

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

	g_stConfig.Ic_CallBack = (IC_CALLBACK)pCallBack;
	sprintf(pMsg, "���ûص�����ָ�� 0x%8.8lx ", pCallBack);

	return 0;
}


/*
����:	�ر��豸
����ֵ:	�ɹ�����0��ʧ�ܷ���-1
*/
int _declspec(dllexport) __stdcall Close(char *pMsg)
{
	//sprintf(pMsg, "�ر��豸�ɹ�");
	//return 0;

	//�ر��߳�
	g_stConfig.iIsThreadExit = 1;
	while(1 == g_stConfig.iThreadState) {::Sleep(1);}
	
	g_stConfig.iIsThreadExit = 0;
	
	//���ٶ���
	CarderClose();


	return 0;
}
