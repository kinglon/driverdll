// Screen.h: interface for the CScreen class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SCREEN_H__7D53BD8C_750F_4C6F_AD08_1E69498F5AF1__INCLUDED_)
#define AFX_SCREEN_H__7D53BD8C_750F_4C6F_AD08_1E69498F5AF1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

typedef unsigned char       BYTE;

struct STSCREENFRAME
{
	//字段类型	//变量		//含意				位置	备注
	BYTE  a_cHead;			//桢头(条屏号)		00
	BYTE  b_cFrameKind;		//桢类型			01		1=显示3次, 3=下载显示, 4=临时显示
	BYTE  c_cScnCount_l;	//幕数低位			02
	BYTE  d_cScnCount_h;	//幕号高位			03
	BYTE  e_cWordCount;		//字数				04		全角和半角字符之和
	BYTE  f_cInMode;		//引入方式			05		引入速度[7:5] 显示方式[4:0]
	BYTE  g_cOutMode;		//引出方式			06		引出速度[7:5] 显示方式[4:0]
	BYTE  h_cStayMode;		//停留方式			07		停留时间[7:5] 显示时钟[4:3] 停留方式[2:0]
	BYTE  i_cContent[60];	//内容				8-67
	BYTE  j_cChecksum;		//累加和			68
};
/*
关于[引入速度、引出速度]--数值为0-7---对应移动速度[慢--快]

关于[显示方式]------------数值为0-15--00000----0----画面从左至右展开
                                      00001----1----画面从右至左展开
                                      00010----2----画面从左右两边向中间展开
                                      00011----3----画面从中间向左右两边展开
                                      00100----4----画面从上至下展开
                                      00101----5----画面从下至上展开
                                      00110----6----画面从左至右移动
                                      00111----7----画面从右至左移动
                                      01000----8----画面从两边向中间移动
                                      01001----9----画面从中间向两边移动
                                      01010---10----画面从上至下移动
                                      01011---11----画面从下至上移动
                                      01100---12----画面立即显示(引入)，消失(引出)
                                      01101---13----画面分割左移
                                      01110---14----画面分割右移
                                      01111---15----随机

关于[停留时间]------------数值为0~7---对应停留时间[短--长]

关于[显示时钟]------------数值为0~1---0表示不显示时钟,1表示显示时钟

关于[停留方式]------------数值为0~7---000----0----停留
                                   ---001----1----亮灭闪烁
								   ---010----2----正反闪烁
								   ---011----3----随机
								   ---100----4----连续
								   ---101----5----保留
								   ---110----6----保留
								   ---111----7----保留
*/

class CScreen  
{
public:
	CScreen();
	virtual ~CScreen();

public:
	bool BuildString(const char* pSourceString,int iTextNum,char* pTargetString);
	bool BuildFrame(STSCREENFRAME& fs,char* pText,int iTextLen);
	bool BuildFrame(STSCREENFRAME &fs, int iWindowNum, int iWindowNumColor, int iKuohuColor, char *pText, int iTextLen, int iTextColor);

};

#endif // !defined(AFX_SCREEN_H__7D53BD8C_750F_4C6F_AD08_1E69498F5AF1__INCLUDED_)
