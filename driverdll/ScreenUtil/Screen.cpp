// Screen.cpp: implementation of the CScreen class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Screen.h"
#include <Strsafe.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


CScreen::CScreen()
{

}

CScreen::~CScreen()
{

}

bool CScreen::BuildString(const char *pSourceString, int iTextNum, char *pTargetString)
{
	int iSourceLen = strlen(pSourceString);	//pSourceString的长度
	int iTargetLen = 0;//pTargetString的现有长度
	int iFrameTextLen = iTextNum*2;	//一帧字符串长度

	int i = 0;
	while(i<iSourceLen)
	{
		if(pSourceString[i]<0 || pSourceString[i]>127) //全角字符
		{
			if(iTargetLen%iFrameTextLen+2 > iFrameTextLen)  //帧尾部填空格
			{
				memcpy(pTargetString+iTargetLen," ",1);
				iTargetLen++;
			}
			else
			{
				memcpy(pTargetString+iTargetLen,pSourceString+i,2);
				iTargetLen += 2;
				i += 2;
			}
		}
		else //半角字符
		{
			memcpy(pTargetString+iTargetLen,pSourceString+i,1);
			iTargetLen++;
			i++;
		}
	}

	while(iTargetLen%iFrameTextLen != 0)  //还未达到一帧的数字，尾部填充0
	{
		memcpy(pTargetString+iTargetLen," ",1);
		iTargetLen++;
	}

	return true;
}

/*
函数名： BuildFrame
功能：组帧
*/
bool CScreen::BuildFrame(STSCREENFRAME &fs, char *pText, int iTextLen)
{
	int iTextPos = 0;
	int iFsContentPos = 0;
	int iWordCount = 0;

	while(iTextPos<iTextLen)
	{
		if(pText[iTextPos]<0 || pText[iTextPos]>127) //全角字符
		{
			fs.i_cContent[iFsContentPos++] = 32; //全角字符，黑底，红字
			memcpy(fs.i_cContent+iFsContentPos,pText+iTextPos,2);
			iFsContentPos +=2;
			iTextPos += 2;
		}
		else	
		{
			fs.i_cContent[iFsContentPos++] = 33; //半角字符，黑底，红字
			fs.i_cContent[iFsContentPos++] = pText[iTextPos++];
		}

		iWordCount++;
	}

	fs.e_cWordCount = iWordCount;

	int iCheckSum = 0;
	iCheckSum += fs.b_cFrameKind+fs.c_cScnCount_l+fs.d_cScnCount_h+fs.e_cWordCount+fs.f_cInMode;
	iCheckSum += fs.g_cOutMode+fs.h_cStayMode;
	for(int i=0;i<60;i++)
	{
		iCheckSum += fs.i_cContent[i];
	}
	fs.j_cChecksum = iCheckSum & 0xFF;

	return true;
}

/*
函数名： BuildFrame
功能：组帧
*/
bool CScreen::BuildFrame(STSCREENFRAME &fs, int iWindowNum, int iWindowNumColor, int iKuohuColor, char *pText, int iTextLen, int iTextColor)
{
	int iFsContentPos = 0;

	int iWordCount = 0;

	if(iKuohuColor)
	{
		fs.i_cContent[iFsContentPos++] = 32*iKuohuColor+1;
		fs.i_cContent[iFsContentPos++] = '(';	
	}

	if(iWindowNum!=0)
	{
		char cWindowNum[5]={0};
        sprintf_s(cWindowNum, 5, "%d", iWindowNum);
		int iWindowNumPos = 0;
		while((unsigned int)iWindowNumPos < strlen(cWindowNum))
		{
			fs.i_cContent[iFsContentPos++] = 32*iWindowNumColor+1;
			fs.i_cContent[iFsContentPos++] = cWindowNum[iWindowNumPos++];
		}
	}

	if(iKuohuColor)
	{
		fs.i_cContent[iFsContentPos++] = 32*iKuohuColor+1;
		fs.i_cContent[iFsContentPos++] = ')';	
	}


	int iTextPos = 0;

	while(iTextPos<iTextLen)
	{
		if(pText[iTextPos]<0 || pText[iTextPos]>127)
		{
			fs.i_cContent[iFsContentPos++] = 32*iTextColor;
			memcpy(fs.i_cContent+iFsContentPos,pText+iTextPos,2);
			iFsContentPos +=2;
			iTextPos += 2;
		}
		else		
		{
			fs.i_cContent[iFsContentPos++] = 32*iTextColor+1;
			fs.i_cContent[iFsContentPos++] = pText[iTextPos++];
		}

		iWordCount++;
	}

	fs.e_cWordCount = iWordCount;


	
	int iCheckSum = 0;
	iCheckSum += fs.b_cFrameKind+fs.c_cScnCount_l+fs.d_cScnCount_h+fs.e_cWordCount+fs.f_cInMode;
	iCheckSum += fs.g_cOutMode+fs.h_cStayMode;
	for(int i=0;i<60;i++)
	{
		iCheckSum += fs.i_cContent[i];
	}
	fs.j_cChecksum = iCheckSum & 0xFF;

	

	return true;
}
