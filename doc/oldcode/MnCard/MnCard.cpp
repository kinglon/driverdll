// MnCard.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"

//--------------------------------------------------------------------------------------

//引用公共头文件
#include "../commfile/Log/log.h"
#include "../commfile/Config/Config.h"
#include "../commfile/serialcom/SerialCom.h"


//--------------------------------------------------------------------------------------

//定义全局变量
char g_cLogFile[300] = {0};


typedef int (_stdcall *MN_CALLBACK) (char *pCardInfo);

struct STCONFIG
{
	//运行变量
	CSerialCom *pSerialCom;



	//记录磁条刷卡器的回调函数
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
				//得到日志文件名
				SYSTEMTIME st;
				::GetLocalTime(&st);
				sprintf(g_cLogFile, "%s\\MN%4.4ld%2.2ld%2.2ld.log", LOG_DIR,  st.wYear, st.wMonth, st.wDay);
											
				//运行变量
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
功能:	打开设备
返回值:	成功返加0，失败返回-1
*/
extern "C" int _declspec(dllexport) __stdcall Open(int iPort, char *pMsg)
{
	//sprintf(pMsg, "打开设备[%d]成功", iPort);
	//return 0;
	
	int iRet = 0;
	
	if(NULL != g_stConfig.pSerialCom)
	{
		g_stConfig.pSerialCom->Close();
		delete g_stConfig.pSerialCom;
		g_stConfig.pSerialCom = NULL;
	}
	
	g_stConfig.pSerialCom = new CSerialCom;
	g_stConfig.pSerialCom->m_iUseType = 16;	//控制磁条刷卡器
	g_stConfig.pSerialCom->m_iReadCardType = 0;	//读卡器类型
	
	bool bOpen = g_stConfig.pSerialCom->Open(iPort, "9600,N,8,1");
	if(bOpen)
	{
		iRet = 0;
		sprintf(pMsg, "打开设备[%d]成功", iPort);
	
	}
	else
	{
		iRet = -1;
		sprintf(pMsg, "打开设备[%d]失败", iPort);
	}

	WriteLog(g_cLogFile, pMsg);

	return iRet;

}

/*
功能:	设置回调函数指针
返回值:	成功返加0，失败返回-1
*/
extern "C" int _declspec(dllexport) __stdcall SetCallBack(void *pCallBack, char *pMsg)
{
	//sprintf(pMsg, "设置回调函数指针 0x%8.8lx ", pCallBack);
	//return 0;

	g_stConfig.Mn_CallBack = (MN_CALLBACK)pCallBack;

	g_stConfig.pSerialCom->Mn_CallBack = (MN_CALLBACK)pCallBack;

	sprintf(pMsg, "设置回调函数指针 0x%8.8lx ", pCallBack);

	return 0;
}


/*
功能:	关闭设备
返回值:	成功返加0，失败返回-1
*/
extern "C" int _declspec(dllexport) __stdcall Close(char *pMsg)
{
	//sprintf(pMsg, "关闭设备成功");
	//return 0;

	if(NULL != g_stConfig.pSerialCom)
	{
		//销毁对象
		g_stConfig.pSerialCom->Close();
		delete g_stConfig.pSerialCom;
		g_stConfig.pSerialCom = NULL;
	}

	return 0;
}

