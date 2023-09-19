// WindowScreen.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"

//--------------------------------------------------------------------------------------

//引用公共头文件
#include "../commfile/Log/log.h"
#include "../commfile/Config/Config.h"
#include "../commfile/serialcom/SerialCom.h"
#include "../commfile/screen/screen.h"
#include "../commfile/list/mylist.h"

//--------------------------------------------------------------------------------------

//定义全局变量
char g_cLogFile[300] = {0};


struct STWINDOWSCREEN	//窗口号与屏号对应关系
{
	int iWinNum;			//窗口号
	int iScreenNum;			//屏号
	int iWordCnt;			//条屏字数		//以下为2013_08_15add
	int iFrameKind;			//条屏帧类型
	char cPrefix[100];		//发送内容时的前缀
};


struct STPUBLICSCREEN		//集中屏结构体
{
	int iBlockNum;	//集中屏模块编号
	
	CMyList listPublicScreenNum;	//集中屏屏号序列

	int iWordCnt;			//条屏字数
	int iFrameKind;			//条屏帧类型
	char cPrefix[100];		//发送内容时的前缀
};

struct STCONFIG
{
	//配置变量
	STWINDOWSCREEN	*pStWindowScreen;	//窗口条屏结构体
	int				iWindowScreenCnt;	//窗口条屏总数

	STPUBLICSCREEN	*pStPublicScreen;	//集中屏结构体
	int				iPublicScreenCnt;	//集中屏个数

	//运行变量
	CSerialCom *pSerialCom;

	int iIsThreadExit;		//线程是否需要退出，用于主进程控制线程是否退出
	int iThreadState;		//线程状态：1=正在运行，0=退出运行
};

STCONFIG g_stConfig;

CMyList	 g_listWinNum;	//存放窗口号

CMyList	 g_listContent;	//存放窗口条屏发送内容(含集中屏发送内容)

CMyList	 g_listPublicTextContent;	//集中屏显示文字内容

//--------------------------------------------------------------------------------------


//当前DLL读取配置文件函数
void Config_Init()
{
	//配置变量
	char cCfgFile[300] = {0};
	sprintf(cCfgFile, "%s", "D:\\QmDriver\\Config\\Config.ini");

	char cValue[300];
	
	ReadConfigFile(cCfgFile, "窗口总数", cValue);
	g_stConfig.iWindowScreenCnt = atoi(cValue);
	g_stConfig.pStWindowScreen = new STWINDOWSCREEN[g_stConfig.iWindowScreenCnt];	
	for(int i=0; i<g_stConfig.iWindowScreenCnt; i++)
	{		
		char cItem[100] = {0};
		sprintf(cItem, "窗口%d", i+1);

		memset(cValue, 0, 300);
		ReadConfigFile(cCfgFile, cItem, cValue);

		char *p = strtok(cValue, ",");
		g_stConfig.pStWindowScreen[i].iWinNum = atoi(p);

		p = strtok(NULL, ",");
		g_stConfig.pStWindowScreen[i].iScreenNum = atoi(p);

		p = strtok(NULL, ",");
		g_stConfig.pStWindowScreen[i].iWordCnt = atoi(p);

		p = strtok(NULL, ",");
		g_stConfig.pStWindowScreen[i].iFrameKind = atoi(p);

		p = strtok(NULL, ",");
		memset(g_stConfig.pStWindowScreen[i].cPrefix, 0, 100);
		if(NULL != p)
		{
			strcpy(g_stConfig.pStWindowScreen[i].cPrefix, p);
		}
	}


	ReadConfigFile(cCfgFile, "集中屏总数", cValue);
	g_stConfig.iPublicScreenCnt = atoi(cValue);
	g_stConfig.pStPublicScreen = new STPUBLICSCREEN[g_stConfig.iPublicScreenCnt];	
	for(i=0; i<g_stConfig.iPublicScreenCnt; i++)
	{		
		char cItem[100] = {0};
		char *p;
/*
集中屏1编号=1
集中屏1序列=122,123,124,125
集中屏1参数=8,3,AA
*/
		sprintf(cItem, "集中屏%d编号", i+1);
		memset(cValue, 0, 300);
		ReadConfigFile(cCfgFile, cItem, cValue);
		g_stConfig.pStPublicScreen[i].iBlockNum = atoi(cValue);

		sprintf(cItem, "集中屏%d序列", i+1);
		memset(cValue, 0, 300);
		ReadConfigFile(cCfgFile, cItem, cValue);
		p = strtok(cValue, ",");
		while( p )
		{
			int iScreenNum = atoi(p);
			g_stConfig.pStPublicScreen[i].listPublicScreenNum.AddTail((void*)iScreenNum);

			p = strtok(NULL, ",");
		}

		sprintf(cItem, "集中屏%d参数", i+1);
		memset(cValue, 0, 300);
		ReadConfigFile(cCfgFile, cItem, cValue);
		p = strtok(cValue, ",");
		g_stConfig.pStPublicScreen[i].iWordCnt = atoi(p);
		p = strtok(NULL, ",");
		g_stConfig.pStPublicScreen[i].iFrameKind = atoi(p);
		p = strtok(NULL, ",");
		memset(g_stConfig.pStPublicScreen[i].cPrefix, 0, 100);
		if(NULL != p)
		{
			strcpy(g_stConfig.pStPublicScreen[i].cPrefix, p);
		}
	}


	//运行变量
	g_stConfig.pSerialCom = NULL;

	//初始化窗口号列表
	g_listWinNum.AddTail("⑴");
	g_listWinNum.AddTail("⑵");
	g_listWinNum.AddTail("⑶");
	g_listWinNum.AddTail("⑷");
	g_listWinNum.AddTail("⑸");
	g_listWinNum.AddTail("⑹");
	g_listWinNum.AddTail("⑺");
	g_listWinNum.AddTail("⑻");
	g_listWinNum.AddTail("⑼");
	g_listWinNum.AddTail("⑽");
	g_listWinNum.AddTail("⑾");
	g_listWinNum.AddTail("⑿");
	g_listWinNum.AddTail("⒀");
	g_listWinNum.AddTail("⒁");
	g_listWinNum.AddTail("⒂");
	g_listWinNum.AddTail("⒃");
	g_listWinNum.AddTail("⒄");
	g_listWinNum.AddTail("⒅");
	g_listWinNum.AddTail("⒆");
	g_listWinNum.AddTail("⒇");
	g_listWinNum.AddTail("21");
	g_listWinNum.AddTail("22");
	g_listWinNum.AddTail("23");
	g_listWinNum.AddTail("24");
	g_listWinNum.AddTail("25");
	g_listWinNum.AddTail("26");
	g_listWinNum.AddTail("27");
	g_listWinNum.AddTail("28");
	g_listWinNum.AddTail("29");
	g_listWinNum.AddTail("30");
	g_listWinNum.AddTail("31");
	g_listWinNum.AddTail("32");
	g_listWinNum.AddTail("33");
	g_listWinNum.AddTail("34");
	g_listWinNum.AddTail("35");
	g_listWinNum.AddTail("36");
	g_listWinNum.AddTail("37");
	g_listWinNum.AddTail("38");
	g_listWinNum.AddTail("39");
	g_listWinNum.AddTail("40");
	g_listWinNum.AddTail("41");
	g_listWinNum.AddTail("42");
	g_listWinNum.AddTail("43");
	g_listWinNum.AddTail("44");
	g_listWinNum.AddTail("45");
	g_listWinNum.AddTail("46");
	g_listWinNum.AddTail("47");
	g_listWinNum.AddTail("48");
	g_listWinNum.AddTail("49");
	g_listWinNum.AddTail("50");
}


//得到窗口条屏参数结构体
STWINDOWSCREEN* GetWindowScreenParam(int iWinNum)
{
	STWINDOWSCREEN *pStWindowScreen = NULL;
		
	for(int i=0; i<g_stConfig.iWindowScreenCnt; i++)
	{
		if(iWinNum == g_stConfig.pStWindowScreen[i].iWinNum)
		{
			pStWindowScreen = &(g_stConfig.pStWindowScreen[i]);
			break;
		}
	}

	return pStWindowScreen;
}

//发送集中显示屏
void SendPublicScreen(int iBlockNum, char *pContent)
{
	//找到集中显示屏模块结构体
	STPUBLICSCREEN *pStPublicScreen = NULL;
	for(int i=0; i<g_stConfig.iPublicScreenCnt; i++)
	{
		if(iBlockNum == g_stConfig.pStPublicScreen[i].iBlockNum)
		{
			pStPublicScreen = &g_stConfig.pStPublicScreen[i];
			break;
		}
	}
	if(NULL == pStPublicScreen)
	{
		return;
	}


	//添加集中显示屏内容列表
	char *pText = new char[100];
	memset(pText, 0, 100);
	strcpy(pText, pContent);
	g_listPublicTextContent.AddTail(pText);
	if(g_listPublicTextContent.GetCount() > pStPublicScreen->listPublicScreenNum.GetCount())
	{
		char *pTemp = (char*)g_listPublicTextContent.GetHead();
		g_listPublicTextContent.RemoveHead();
		delete []pTemp;
		pTemp = NULL;
	}

	//组织集中屏发送内容
	for( i=0; i<g_listPublicTextContent.GetCount(); i++)
	{
		//初始化的参数-----可从配置文件中读取
		int iFrameType = pStPublicScreen->iFrameKind;	// 3;
		int iScreenWord = pStPublicScreen->iWordCnt;	// 8;

		//确定发送内容
		char *pSend = (char*)g_listPublicTextContent.GetAt(i);
		
		char cSend[200]={0};
		sprintf(cSend, "%s%s", pStPublicScreen->cPrefix, pSend);

		
		char* pTargetString = new char[500];
		memset(pTargetString,0,500);

		CScreen s;
		s.BuildString(cSend,iScreenWord,pTargetString);

		strcat(pTargetString, pTargetString);

		//帧结构体参数设定
		STSCREENFRAME stSF;
		memset(&stSF,0,sizeof(stSF));
		stSF.a_cHead		= 255 - 0 - (int)pStPublicScreen->listPublicScreenNum.GetAt(i);// g_stConfig.arrPublicScreen.GetAt(i);//iAddr;
		stSF.b_cFrameKind	= iFrameType;
		stSF.c_cScnCount_l	= 0;
		stSF.d_cScnCount_h	= 0;
		stSF.e_cWordCount	= 0;
		stSF.f_cInMode		= 12;
		stSF.g_cOutMode		= 12;
		stSF.h_cStayMode	= 224;

		//计算帧数
		int nFrame = strlen(pTargetString)/(iScreenWord*2);

		for(int iFrame=0;iFrame<nFrame;iFrame++)
		{
			//确定这一帧的发送内容
			char cItem[50]={0};
			memcpy(cItem,pTargetString+iFrame*iScreenWord*2,iScreenWord*2);

			//设置发送属性结构体中低位幕号
			stSF.c_cScnCount_l = iFrame;			
			if(3 == stSF.b_cFrameKind)
			{
				stSF.c_cScnCount_l += 10;
			}	

			//组帧，处理发送的字符串和计算校验和
			s.BuildFrame(stSF,cItem,iScreenWord*2);

			//将内容加入到发送列表中
			STSCREENSEND *pStScreenSend = new STSCREENSEND;
			memcpy((BYTE*)(pStScreenSend->cContent),(BYTE*)&stSF,sizeof(stSF));

			g_listContent.AddTail((void*)(pStScreenSend));
		}
	
		delete []pTargetString;
		pTargetString = NULL;

	}

}

//发送条屏内容线程
DWORD WINAPI Thread_SendScreen(void *ptr)
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

			for(;g_listContent.GetCount()>0;)
			{
				STSCREENSEND *pStScreenSend = (STSCREENSEND*)(g_listContent.GetHead());
				g_stConfig.pSerialCom->Write((char*)(pStScreenSend->cContent),69);

				::Sleep(300);

				g_listContent.RemoveHead();

				delete pStScreenSend;
				pStScreenSend = NULL;
			}

			::Sleep(10);

			//-----------------------------
			try
			{//读取公共屏的内容
				char cCsFile[300] = {0};
				sprintf(cCsFile, "%s\\CenterScreen.shw", LOG_DIR);
				
				char cLine[1000] = {0};
				FILE *fp = fopen(cCsFile, "rb");
				if(fp)
				{
					fgets(cLine, 1000, fp);
					fclose(fp);

					::DeleteFile(cCsFile);

					StrTrim(cLine);
				}

				if(0 < strlen(cLine))
				{
					int iBlockNum = 0;
					char cPublicScreenContent[1000] = {0};
					char *p = strtok(cLine, "@");
					if(p)
					{
						iBlockNum = atoi(p);

						p = strtok(NULL, "@");
						if(p)
						{
							strcpy(cPublicScreenContent, p);

							SendPublicScreen(iBlockNum, cPublicScreenContent);
						}						
					}
				}
			}
			catch(...)
			{
				WriteLog(g_cLogFile, "操作错误");
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
				sprintf(g_cLogFile, "%s\\WS%4.4ld%2.2ld%2.2ld.log", LOG_DIR,  st.wYear, st.wMonth, st.wDay);
				
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
	g_stConfig.pSerialCom->m_iUseType = 4;	//控制窗口条屏
	
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
		hThread=::CreateThread(NULL,0,(unsigned long(_stdcall*)(void*))Thread_SendScreen,NULL,0,&ThreadId);
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
extern "C" int _declspec(dllexport) __stdcall ShowInfo(int iWinNum, char *pInfo, char *pStyle, char *pMsg)
{
	//sprintf(pMsg, "显示内容成功 %d - %s - %s ", iWinNum, pInfo, pStyle);
	//return 0;


	//根据窗口号得到条屏号
	STWINDOWSCREEN *pStWindowScreen = GetWindowScreenParam(iWinNum);
	if(NULL == pStWindowScreen)
	{
		sprintf(pMsg, "未找到窗口号[%d]对应的屏号", iWinNum);
		return -1;
	}

	//初始化的参数-----可从配置文件中读取
	int iFrameType	= pStWindowScreen->iFrameKind;	// 3;
	int iScreenWord = pStWindowScreen->iWordCnt;	// 8;

	//确定发送内容----内容居中,且拼凑两幕
	char cSend[200]={0};

	/*
	char cPart1[100] = {0};
	char cPart2[100] = {0};
	
	//sprintf(cPart1, "%s%s", pStWindowScreen->cPrefix, (char*)g_listWinNum.GetAt(iWinNum-1));
	sprintf(cPart1, "%s", pStWindowScreen->cPrefix);

	int k1 = strlen(cPart1);
	int k2 = strlen(pInfo);
	int iPart2SpaceLen = (pStWindowScreen->iWordCnt*2 - (int)strlen(cPart1) - (int)strlen(pInfo)) / 2;
	while(0 < iPart2SpaceLen)
	{
		strcat(cPart2, " ");
		iPart2SpaceLen--;
	}
	strcat(cPart2, pInfo);

	sprintf(cSend, "%s%s", cPart1, cPart2);
	while((int)strlen(cSend) < pStWindowScreen->iWordCnt*2)
	{
		strcat(cSend, " ");
	}
	*/

	sprintf(cSend, "%s", pInfo);


	char* pTargetString = new char[200];
	memset(pTargetString,0,200);

	CScreen s;
	s.BuildString(cSend, iScreenWord, pTargetString);

	strcat(pTargetString, pTargetString);

	//帧结构体参数设定
	STSCREENFRAME stSF;
	memset(&stSF,0,sizeof(stSF));
	stSF.a_cHead		= 255 - 0 - pStWindowScreen->iScreenNum;
	stSF.b_cFrameKind	= iFrameType;
	stSF.c_cScnCount_l	= 0;
	stSF.d_cScnCount_h	= 0;
	stSF.e_cWordCount	= 0;
	stSF.f_cInMode		= 12;
	stSF.g_cOutMode		= 12;
	stSF.h_cStayMode	= 224;

	//计算帧数
	int nFrame = strlen(pTargetString)/(iScreenWord*2);

	for(int iFrame=0;iFrame<nFrame;iFrame++)
	{
		//确定这一帧的发送内容
		char cItem[50]={0};
		memcpy(cItem,pTargetString+iFrame*iScreenWord*2,iScreenWord*2);

		//设置发送属性结构体中低位幕号
		stSF.c_cScnCount_l = iFrame;			
		if(3 == stSF.b_cFrameKind)
		{
			stSF.c_cScnCount_l += 10;
		}	

		//组帧，处理发送的字符串和计算校验和
		s.BuildFrame(stSF,cItem,iScreenWord*2);

		//将内容加入到发送列表中
		STSCREENSEND *pStScreenSend = new STSCREENSEND;
		memcpy((BYTE*)(pStScreenSend->cContent),(BYTE*)&stSF,sizeof(stSF));

		g_listContent.AddTail((void*)(pStScreenSend));
	}
	
	delete []pTargetString;
	pTargetString = NULL;

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

