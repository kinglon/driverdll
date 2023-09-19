// CenterScreen.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"

//--------------------------------------------------------------------------------------

//引用公共头文件
#include "../commfile/Log/log.h"

//--------------------------------------------------------------------------------------

//定义全局变量
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
				//得到日志文件名
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
功能:	打开设备
返回值:	成功返加0，失败返回-1
*/
int _declspec(dllexport) __stdcall Open(int iPort, char *pMsg)
{
	sprintf(pMsg, "打开设备[%d]成功", iPort);

	return 0;
}


/*
功能:	显示内容
返回值:	成功返加0，失败返回-1
*/
int _declspec(dllexport) __stdcall ShowInfo(int iBlockNum, char *pInfo, char *pStyle, char *pMsg)
{
	sprintf(pMsg, "显示内容成功 %d - %s - %s ", iBlockNum, pInfo, pStyle);	

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
		WriteLog(g_cLogFile, "操作错误");
	}	

	return 0;
}


/*
功能:	关闭设备
返回值:	成功返加0，失败返回-1
*/
int _declspec(dllexport) __stdcall Close(char *pMsg)
{
	sprintf(pMsg, "关闭设备成功");

	return 0;
}

