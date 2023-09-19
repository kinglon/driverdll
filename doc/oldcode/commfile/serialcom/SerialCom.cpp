// SerialCom.cpp: implementation of the CSerialCom class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "SerialCom.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSerialCom::CSerialCom(bool fAutoBeginThread,DWORD dwIOMode):cnComm(fAutoBeginThread,dwIOMode)
{
	m_iUseType = 0;

	memset(m_cCardInfo, 0, 1000);
	m_iCardInfoLen = 0;

	//MnCard_EventFun = NULL;
	Mn_CallBack = NULL;

	Key_CallBack_Tmp = NULL;
}

CSerialCom::~CSerialCom()
{
}


//int	 m_iUseType;	//1=轮询小键盘；2=评价器；4=发窗口条屏；8发公共条屏；16读卡器

void CSerialCom::OnReceive()
{
	//如果是管理键盘和评价器的串口收到消息
	if(0 != (m_iUseType & 3))
	{
		::Sleep(0);

		char Buffer[256] = {0};		
		int iRead = Read(Buffer, 256,255);

		BYTE cCheckSum = Buffer[1] + Buffer[2] + Buffer[3];
		if((iRead >=5) && (cCheckSum == (BYTE)Buffer[4]))
		{
			//立即给小键盘回传一个信息
			char cSend[4];
			cSend[0] = Buffer[0];
			cSend[1] = 0x00;
			cSend[2] = 0x00;
			cSend[3] = Buffer[0];
			Write(cSend,4);

			//调用回调函数			
			Key_CallBack_Tmp((BYTE*)Buffer);

		}
	}

	//如果管理读卡器的串口收到消息
	if(0 != (m_iUseType & 16))
	{
		//0x02
		//0x03
				
		/*
		//中白读卡器均可
		if(0 == this->m_iReadCardType)
		{
			//1 接收串口的信息，存入buf中
			char buf[1000];
			memset(buf, 0, 1000);
			int iRead = Read(buf, 1000);

			for(int k=0; k<iRead; k++)
			{
				//收到结束符(0x1C)
				if(0x1C == buf[k])
				{
					if(0x1B == m_cCardInfo[0])
					{
						m_cCardInfo[m_iCardInfoLen] = buf[k];
						m_iCardInfoLen++;
						break;
					}
				}
				
				//收到起始符(0x1B)
				if(0x1B == buf[k])
				{
					memset(m_cCardInfo, 0, 1000);
					m_iCardInfoLen = 0;
				}
				
				//收到其它符号(含起始符)则保存
				m_cCardInfo[m_iCardInfoLen] = buf[k];
				m_iCardInfoLen++;
			}


			//2 判断此次接收是否完整
			if( m_iCardInfoLen > 2 )
			{
				if( (0x1B == m_cCardInfo[0]) && (0x1C == m_cCardInfo[m_iCardInfoLen-1]))
				{//如果接收信息完整
					//拆分出第二轨和第三轨数据
					char cCardInfo[1000] = {0};
					//char cCardInfo2Way[500] = {0};	//第二轨数据
					//char cCardInfo3Way[500] = {0};	//第三轨数据

					//memcpy(cCardInfo, m_cCardInfo+1, m_iCardInfoLen-2);
					memcpy(cCardInfo, m_cCardInfo+2, m_iCardInfoLen-3);

					TCHAR wCardInfo[500] = {0};

					::MultiByteToWideChar(CP_ACP,0, cCardInfo, strlen(cCardInfo), wCardInfo, 500);
						
						
					MnCard_EventFun(wCardInfo);
					
				}
			}
		}
		*/

		//====================================================================================
		
		//小黑读卡器、大黑读卡器均可
		if(0 == this->m_iReadCardType)
		{
			//1 接收串口的信息，存入buf中
			char buf[1000];
			memset(buf, 0, 1000);
			int iRead = Read(buf, 1000);

			for(int k=0; k<iRead; k++)
			{
				//收到结束符(0x03)
				if(0x03 == buf[k])
				{
					if(0x02 == m_cCardInfo[0])
					{
						m_cCardInfo[m_iCardInfoLen] = buf[k];
						m_iCardInfoLen++;
						break;
					}
				}
				
				//收到起始符(0x02)
				if(0x02 == buf[k])
				{
					memset(m_cCardInfo, 0, 1000);
					m_iCardInfoLen = 0;
				}
				
				//收到其它符号(含起始符)则保存
				m_cCardInfo[m_iCardInfoLen] = buf[k];
				m_iCardInfoLen++;
			}


			//2 判断此次接收是否完整
			if( m_iCardInfoLen > 2 )
			{
				if( (0x02 == m_cCardInfo[0]) && (0x03 == m_cCardInfo[m_iCardInfoLen-1]))
				{//如果接收信息完整
					//拆分出第二轨和第三轨数据
					char cCardInfo[1000] = {0};
			
					memcpy(cCardInfo, m_cCardInfo+2, m_iCardInfoLen-3);

				
					Mn_CallBack(cCardInfo);
					
				}
			}

		}
	



	}
}



int CSerialCom::Send(char *pSend,int iLen)
{
	return Write(pSend,iLen);
}


