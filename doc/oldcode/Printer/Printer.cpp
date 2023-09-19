// Printer.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"

//--------------------------------------------------------------------------------------

//引用公共头文件
#include "../commfile/Log/log.h"

//--------------------------------------------------------------------------------------

//定义全局变量
char g_cLogFile[300] = {0};


struct STCONFIG
{
	//--------------------
	//参数变量
	char arrContent[100][1000];	//实际打印内容
	int  iContentCnt;			//内容条数

	char cPrintFile[300];		//记录打印内容临时文件
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
				//得到日志文件名
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
功能:	打开设备
返回值:	成功返加0，失败返回-1
备注:	串口COM3，即填入3。驱动在内部自行处理串口的波特率、数据位、校验位、停止位等参数。如果为LPT或USB则填入0。
*/
extern "C" int _declspec(dllexport) __stdcall Open(int iPort, char *pMsg)
{
	//sprintf(pMsg, "打开设备[%d]成功", iPort);
	//return 0;

	g_stConfig.iContentCnt = 0;

	sprintf(g_stConfig.cPrintFile, "%s\\printer.prt", LOG_DIR);

	return 0;
}


/*
功能:	填充打印的图片内容
返回值:	成功返加0，失败返回-1
*/
int _declspec(dllexport) __stdcall FillPicInfo(char *pPicFile, char *pMsg)
{
	//sprintf(pMsg, "填充打印的图片内容成功 %s ", pPicFile);
	//return 0;

	sprintf(g_stConfig.arrContent[g_stConfig.iContentCnt++], "%d#%s#%s", 1, pPicFile, "NULL");

	return 0;
}


/*
功能:	填充打印的文字内容
返回值:	成功返加0，失败返回-1
*/
int _declspec(dllexport) __stdcall FillTextInfo(char *pTextInfo, char *pStyle, char *pMsg)
{
	//sprintf(pMsg, "填充打印的文字内容成功 %s - %s", pTextInfo, pStyle);
	//return 0;

	sprintf(g_stConfig.arrContent[g_stConfig.iContentCnt++], "%d#%s#%s", 2, pTextInfo, pStyle);

	return 0;
}

/*
功能:	执行打印
返回值:	成功返加0，失败返回-1
*/
int _declspec(dllexport) __stdcall Print(char *pMsg)
{
	//sprintf(pMsg, "执行打印成功");
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
功能:	关闭设备
返回值:	成功返加0，失败返回-1
*/
int _declspec(dllexport) __stdcall Close(char *pMsg)
{
	sprintf(pMsg, "关闭设备成功");

	return 0;
}

