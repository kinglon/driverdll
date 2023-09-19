// Key.cpp : Defines the entry point for the DLL application.
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

struct STKEY
{
	int iKeyNum;
	int iWinNum;
};

typedef int (_stdcall *KEY_CALLBACK) (int iWinNum, int iAct);

struct STCONFIG
{
	//配置变量
	STKEY *pStKey;	//键盘结构体
	int    iKeyCnt;	//键盘总数

	//运行变量
	CSerialCom *pSerialCom;

	int iIsThreadExit;		//线程是否需要退出，用于主进程控制线程是否退出
	int iThreadState;		//线程状态：1=正在运行，0=退出运行

	//记录键盘的回调函数
	KEY_CALLBACK Key_CallBack;

};

STCONFIG g_stConfig;

//--------------------------------------------------------------------------------------

//当前DLL读取配置文件函数
void Config_Init()
{
	//配置变量
	char cCfgFile[300] = {0};
	sprintf(cCfgFile, "%s", "D:\\QmDriver\\Config\\Config.ini");

	char cValue[300];
	
	ReadConfigFile(cCfgFile, "键盘总数", cValue);
	g_stConfig.iKeyCnt = atoi(cValue);
	g_stConfig.pStKey = new STKEY[g_stConfig.iKeyCnt];
	for(int i=0; i<g_stConfig.iKeyCnt; i++)
	{
		char cItem[100] = {0};
		sprintf(cItem, "键盘%d", i+1);

		memset(cValue, 0, 300);
		ReadConfigFile(cCfgFile, cItem, cValue);

		char *p = strtok(cValue, ",");
		g_stConfig.pStKey[i].iKeyNum = atoi(p);
		p = strtok(NULL, ",");
		g_stConfig.pStKey[i].iWinNum = atoi(p);
	}

	//运行变量
	g_stConfig.pSerialCom = NULL;
}


//轮询键盘的线程
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

//这个函数被CSerialCom调用，在这里计算窗口号和键盘动作类型
//int Key_CallBack_Tmp(BYTE *pKeyReturn)	//有问题
int _stdcall Key_CallBack_Tmp(BYTE *pKeyReturn)
{
	//得到键盘号
	int iKeyNum = 0xFF - pKeyReturn[0];

	//根据键盘号得到窗口号
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
		sprintf(cLog, "未找到键盘号为[%d]对应的窗口号", iKeyNum);
		WriteLog(g_cLogFile, cLog);
		return -1;
	}

	//得到键盘的动作
	int iAct = 0;

	int iAskType = (int)(pKeyReturn[1]);
	switch(iAskType)
	{
	case 0xCC://呼号
		iAct = 1;
		break;
	case 0x33://重呼
		iAct = 2;
		break;	
	case 0x0F://定呼(里面含登录和退出)
		iAct = 3;
		break;
	case 0xAA://暂停
		iAct = 4;
		break;
	case 0x55://查询
		iAct = 5;
		break;
	case 0xF0://转移
		iAct = 6;
		break;
	}

	g_stConfig.Key_CallBack(iWinNum, iAct);	//调用主程序的回调函数

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
				//得到日志文件名
				SYSTEMTIME st;
				::GetLocalTime(&st);
				sprintf(g_cLogFile, "%s\\KEY%4.4ld%2.2ld%2.2ld.log", LOG_DIR,  st.wYear, st.wMonth, st.wDay);
				
				//读取配置文件
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
	g_stConfig.pSerialCom->m_iUseType = 1;	//控制键盘
	
	bool bOpen = g_stConfig.pSerialCom->Open(iPort, "9600,N,8,1");
	if(bOpen)
	{
		iRet = 0;
		sprintf(pMsg, "打开设备[%d]成功", iPort);

		//创建一个发送线程
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
		sprintf(pMsg, "打开设备[%d]失败", iPort);
	}

	WriteLog(g_cLogFile, pMsg);

	return iRet;
}


/*
功能:	显示内容
返回值:	成功返加0，失败返回-1
*/
int _declspec(dllexport) __stdcall ShowInfo(int iWinNum, char *pInfo, char *pMsg)
{
	//sprintf(pMsg, "显示内容成功 %d - %s ", iWinNum, pInfo);
	//return 0;

	//检查pInfo是否为4位数字
	if(4 != strlen(pInfo))
	{
		sprintf(pMsg, "参数pInfo不是4字节长度");
		WriteLog(g_cLogFile, pMsg);
		return -1;
	}
	if( (pInfo[0]<'0') || ('9'<pInfo[0]) || 
		(pInfo[1]<'0') || ('9'<pInfo[1]) || 
		(pInfo[2]<'0') || ('9'<pInfo[2]) || 
		(pInfo[3]<'0') || ('9'<pInfo[3])  )
	{
		sprintf(pMsg, "参数pInfo不是4位数字");
		WriteLog(g_cLogFile, pMsg);
		return -1;
	}

	//根据窗口号得到键盘号
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
		sprintf(pMsg, "未配置窗口号为[%d]对应的键盘号", iWinNum);
		WriteLog(g_cLogFile, pMsg);
		return -1;
	}

	//向键盘发送内容
	int iInfoNum = atoi(pInfo);

	BYTE cSendToKey[4]={0};

	cSendToKey[0] = 0xFF - iKeyNum;

	int iH2 = iInfoNum/100;
	int iL2 = iInfoNum%100;
	cSendToKey[1]=(BYTE)(((iH2/10)<<4)+(iH2%10));
	cSendToKey[2]=(BYTE)(((iL2/10)<<4)+(iL2%10));
	
	cSendToKey[3] = cSendToKey[0] + cSendToKey[1] + cSendToKey[2];

	g_stConfig.pSerialCom->Write((char*)cSendToKey, 4);

	sprintf(pMsg, "显示内容成功 %d - %s ", iWinNum, pInfo);
	WriteLog(g_cLogFile, pMsg);

	return 0;
}


/*
功能:	设置回调函数指针
返回值:	成功返加0，失败返回-1
*/
int _declspec(dllexport) __stdcall SetCallBack(void *pCallBack, char *pMsg)
{
	//sprintf(pMsg, "设置回调函数指针 0x%8.8lx ", pCallBack);
	//return 0;

	g_stConfig.Key_CallBack = (KEY_CALLBACK)pCallBack;

	g_stConfig.pSerialCom->Key_CallBack_Tmp = (KEY_CALLBACK_TMP)Key_CallBack_Tmp;


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
		//关闭线程
		g_stConfig.iIsThreadExit = 1;
		while(1 == g_stConfig.iThreadState) {::Sleep(1);}

		//销毁对象
		g_stConfig.pSerialCom->Close();
		delete g_stConfig.pSerialCom;
		g_stConfig.pSerialCom = NULL;
	}

	return 0;
}

