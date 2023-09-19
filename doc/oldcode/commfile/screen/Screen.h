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
	//�ֶ�����	//����		//����				λ��	��ע
	BYTE  a_cHead;			//��ͷ(������)		00
	BYTE  b_cFrameKind;		//������			01		1=��ʾ3��, 3=������ʾ, 4=��ʱ��ʾ
	BYTE  c_cScnCount_l;	//Ļ����λ			02
	BYTE  d_cScnCount_h;	//Ļ�Ÿ�λ			03
	BYTE  e_cWordCount;		//����				04		ȫ�ǺͰ���ַ�֮��
	BYTE  f_cInMode;		//���뷽ʽ			05		�����ٶ�[7:5] ��ʾ��ʽ[4:0]
	BYTE  g_cOutMode;		//������ʽ			06		�����ٶ�[7:5] ��ʾ��ʽ[4:0]
	BYTE  h_cStayMode;		//ͣ����ʽ			07		ͣ��ʱ��[7:5] ��ʾʱ��[4:3] ͣ����ʽ[2:0]
	BYTE  i_cContent[60];	//����				8-67
	BYTE  j_cChecksum;		//�ۼӺ�			68
};
/*
����[�����ٶȡ������ٶ�]--��ֵΪ0-7---��Ӧ�ƶ��ٶ�[��--��]

����[��ʾ��ʽ]------------��ֵΪ0-15--00000----0----�����������չ��
                                      00001----1----�����������չ��
                                      00010----2----����������������м�չ��
                                      00011----3----������м�����������չ��
                                      00100----4----�����������չ��
                                      00101----5----�����������չ��
                                      00110----6----������������ƶ�
                                      00111----7----������������ƶ�
                                      01000----8----������������м��ƶ�
                                      01001----9----������м��������ƶ�
                                      01010---10----������������ƶ�
                                      01011---11----������������ƶ�
                                      01100---12----����������ʾ(����)����ʧ(����)
                                      01101---13----����ָ�����
                                      01110---14----����ָ�����
                                      01111---15----���

����[ͣ��ʱ��]------------��ֵΪ0~7---��Ӧͣ��ʱ��[��--��]

����[��ʾʱ��]------------��ֵΪ0~1---0��ʾ����ʾʱ��,1��ʾ��ʾʱ��

����[ͣ����ʽ]------------��ֵΪ0~7---000----0----ͣ��
                                   ---001----1----������˸
								   ---010----2----������˸
								   ---011----3----���
								   ---100----4----����
								   ---101----5----����
								   ---110----6----����
								   ---111----7----����
*/

class CScreen  
{
public:
	CScreen();
	virtual ~CScreen();

public:
	bool BuildString(char* pSourceString,int iTextNum,char* pTargetString);
	bool BuildFrame(STSCREENFRAME& fs,char* pText,int iTextLen);
	bool BuildFrame(STSCREENFRAME &fs, int iWindowNum, int iWindowNumColor, int iKuohuColor, char *pText, int iTextLen, int iTextColor);

};

#endif // !defined(AFX_SCREEN_H__7D53BD8C_750F_4C6F_AD08_1E69498F5AF1__INCLUDED_)