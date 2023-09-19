// WindowScreen.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"

//--------------------------------------------------------------------------------------

//���ù���ͷ�ļ�
#include "../commfile/Log/log.h"
#include "../commfile/Config/Config.h"
#include "../commfile/serialcom/SerialCom.h"
#include "../commfile/screen/screen.h"
#include "../commfile/list/mylist.h"

//--------------------------------------------------------------------------------------

//����ȫ�ֱ���
char g_cLogFile[300] = {0};


struct STWINDOWSCREEN	//���ں������Ŷ�Ӧ��ϵ
{
	int iWinNum;			//���ں�
	int iScreenNum;			//����
	int iWordCnt;			//��������		//����Ϊ2013_08_15add
	int iFrameKind;			//����֡����
	char cPrefix[100];		//��������ʱ��ǰ׺
};


struct STPUBLICSCREEN		//�������ṹ��
{
	int iBlockNum;	//������ģ����
	
	CMyList listPublicScreenNum;	//��������������

	int iWordCnt;			//��������
	int iFrameKind;			//����֡����
	char cPrefix[100];		//��������ʱ��ǰ׺
};

struct STCONFIG
{
	//���ñ���
	STWINDOWSCREEN	*pStWindowScreen;	//���������ṹ��
	int				iWindowScreenCnt;	//������������

	STPUBLICSCREEN	*pStPublicScreen;	//�������ṹ��
	int				iPublicScreenCnt;	//����������

	//���б���
	CSerialCom *pSerialCom;

	int iIsThreadExit;		//�߳��Ƿ���Ҫ�˳������������̿����߳��Ƿ��˳�
	int iThreadState;		//�߳�״̬��1=�������У�0=�˳�����
};

STCONFIG g_stConfig;

CMyList	 g_listWinNum;	//��Ŵ��ں�

CMyList	 g_listContent;	//��Ŵ���������������(����������������)

CMyList	 g_listPublicTextContent;	//��������ʾ��������

//--------------------------------------------------------------------------------------


//��ǰDLL��ȡ�����ļ�����
void Config_Init()
{
	//���ñ���
	char cCfgFile[300] = {0};
	sprintf(cCfgFile, "%s", "D:\\QmDriver\\Config\\Config.ini");

	char cValue[300];
	
	ReadConfigFile(cCfgFile, "��������", cValue);
	g_stConfig.iWindowScreenCnt = atoi(cValue);
	g_stConfig.pStWindowScreen = new STWINDOWSCREEN[g_stConfig.iWindowScreenCnt];	
	for(int i=0; i<g_stConfig.iWindowScreenCnt; i++)
	{		
		char cItem[100] = {0};
		sprintf(cItem, "����%d", i+1);

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


	ReadConfigFile(cCfgFile, "����������", cValue);
	g_stConfig.iPublicScreenCnt = atoi(cValue);
	g_stConfig.pStPublicScreen = new STPUBLICSCREEN[g_stConfig.iPublicScreenCnt];	
	for(i=0; i<g_stConfig.iPublicScreenCnt; i++)
	{		
		char cItem[100] = {0};
		char *p;
/*
������1���=1
������1����=122,123,124,125
������1����=8,3,AA
*/
		sprintf(cItem, "������%d���", i+1);
		memset(cValue, 0, 300);
		ReadConfigFile(cCfgFile, cItem, cValue);
		g_stConfig.pStPublicScreen[i].iBlockNum = atoi(cValue);

		sprintf(cItem, "������%d����", i+1);
		memset(cValue, 0, 300);
		ReadConfigFile(cCfgFile, cItem, cValue);
		p = strtok(cValue, ",");
		while( p )
		{
			int iScreenNum = atoi(p);
			g_stConfig.pStPublicScreen[i].listPublicScreenNum.AddTail((void*)iScreenNum);

			p = strtok(NULL, ",");
		}

		sprintf(cItem, "������%d����", i+1);
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


	//���б���
	g_stConfig.pSerialCom = NULL;

	//��ʼ�����ں��б�
	g_listWinNum.AddTail("��");
	g_listWinNum.AddTail("��");
	g_listWinNum.AddTail("��");
	g_listWinNum.AddTail("��");
	g_listWinNum.AddTail("��");
	g_listWinNum.AddTail("��");
	g_listWinNum.AddTail("��");
	g_listWinNum.AddTail("��");
	g_listWinNum.AddTail("��");
	g_listWinNum.AddTail("��");
	g_listWinNum.AddTail("��");
	g_listWinNum.AddTail("��");
	g_listWinNum.AddTail("��");
	g_listWinNum.AddTail("��");
	g_listWinNum.AddTail("��");
	g_listWinNum.AddTail("��");
	g_listWinNum.AddTail("��");
	g_listWinNum.AddTail("��");
	g_listWinNum.AddTail("��");
	g_listWinNum.AddTail("��");
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


//�õ��������������ṹ��
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

//���ͼ�����ʾ��
void SendPublicScreen(int iBlockNum, char *pContent)
{
	//�ҵ�������ʾ��ģ��ṹ��
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


	//���Ӽ�����ʾ�������б�
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

	//��֯��������������
	for( i=0; i<g_listPublicTextContent.GetCount(); i++)
	{
		//��ʼ���Ĳ���-----�ɴ������ļ��ж�ȡ
		int iFrameType = pStPublicScreen->iFrameKind;	// 3;
		int iScreenWord = pStPublicScreen->iWordCnt;	// 8;

		//ȷ����������
		char *pSend = (char*)g_listPublicTextContent.GetAt(i);
		
		char cSend[200]={0};
		sprintf(cSend, "%s%s", pStPublicScreen->cPrefix, pSend);

		
		char* pTargetString = new char[500];
		memset(pTargetString,0,500);

		CScreen s;
		s.BuildString(cSend,iScreenWord,pTargetString);

		strcat(pTargetString, pTargetString);

		//֡�ṹ������趨
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

		//����֡��
		int nFrame = strlen(pTargetString)/(iScreenWord*2);

		for(int iFrame=0;iFrame<nFrame;iFrame++)
		{
			//ȷ����һ֡�ķ�������
			char cItem[50]={0};
			memcpy(cItem,pTargetString+iFrame*iScreenWord*2,iScreenWord*2);

			//���÷������Խṹ���е�λĻ��
			stSF.c_cScnCount_l = iFrame;			
			if(3 == stSF.b_cFrameKind)
			{
				stSF.c_cScnCount_l += 10;
			}	

			//��֡���������͵��ַ����ͼ���У���
			s.BuildFrame(stSF,cItem,iScreenWord*2);

			//�����ݼ��뵽�����б���
			STSCREENSEND *pStScreenSend = new STSCREENSEND;
			memcpy((BYTE*)(pStScreenSend->cContent),(BYTE*)&stSF,sizeof(stSF));

			g_listContent.AddTail((void*)(pStScreenSend));
		}
	
		delete []pTargetString;
		pTargetString = NULL;

	}

}

//�������������߳�
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
			{//��ȡ������������
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
				WriteLog(g_cLogFile, "��������");
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
				//�õ���־�ļ���
				SYSTEMTIME st;
				::GetLocalTime(&st);
				sprintf(g_cLogFile, "%s\\WS%4.4ld%2.2ld%2.2ld.log", LOG_DIR,  st.wYear, st.wMonth, st.wDay);
				
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
	g_stConfig.pSerialCom->m_iUseType = 4;	//���ƴ�������
	
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
		hThread=::CreateThread(NULL,0,(unsigned long(_stdcall*)(void*))Thread_SendScreen,NULL,0,&ThreadId);
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
extern "C" int _declspec(dllexport) __stdcall ShowInfo(int iWinNum, char *pInfo, char *pStyle, char *pMsg)
{
	//sprintf(pMsg, "��ʾ���ݳɹ� %d - %s - %s ", iWinNum, pInfo, pStyle);
	//return 0;


	//���ݴ��ںŵõ�������
	STWINDOWSCREEN *pStWindowScreen = GetWindowScreenParam(iWinNum);
	if(NULL == pStWindowScreen)
	{
		sprintf(pMsg, "δ�ҵ����ں�[%d]��Ӧ������", iWinNum);
		return -1;
	}

	//��ʼ���Ĳ���-----�ɴ������ļ��ж�ȡ
	int iFrameType	= pStWindowScreen->iFrameKind;	// 3;
	int iScreenWord = pStWindowScreen->iWordCnt;	// 8;

	//ȷ����������----���ݾ���,��ƴ����Ļ
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

	//֡�ṹ������趨
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

	//����֡��
	int nFrame = strlen(pTargetString)/(iScreenWord*2);

	for(int iFrame=0;iFrame<nFrame;iFrame++)
	{
		//ȷ����һ֡�ķ�������
		char cItem[50]={0};
		memcpy(cItem,pTargetString+iFrame*iScreenWord*2,iScreenWord*2);

		//���÷������Խṹ���е�λĻ��
		stSF.c_cScnCount_l = iFrame;			
		if(3 == stSF.b_cFrameKind)
		{
			stSF.c_cScnCount_l += 10;
		}	

		//��֡���������͵��ַ����ͼ���У���
		s.BuildFrame(stSF,cItem,iScreenWord*2);

		//�����ݼ��뵽�����б���
		STSCREENSEND *pStScreenSend = new STSCREENSEND;
		memcpy((BYTE*)(pStScreenSend->cContent),(BYTE*)&stSF,sizeof(stSF));

		g_listContent.AddTail((void*)(pStScreenSend));
	}
	
	delete []pTargetString;
	pTargetString = NULL;

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
