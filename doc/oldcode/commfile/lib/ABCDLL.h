#ifndef _GWI_X0_DRV_H_
#define _GWI_X0_DRV_H_

/*���ش�����*/
/*		
  -1			//����������ʧ��
  -2			//��������
  -3			//�豸���ʹ���
  0				//�ɹ�
  0x1001		//��֧�ֽӴ�ʽ�û���
  0x1002		//�Ӵ�ʽ�û���δ�嵽λ
  0x1003		//�Ӵ�ʽ�û������ϵ�
  0x1004		//�Ӵ�ʽ�û���δ�ϵ�
  0x1005		//�Ӵ�ʽ�û����ϵ�ʧ��
  0x1006		//�����Ӵ�ʽ�û��������޻�Ӧ
  0x1007		//�����Ӵ�ʽ�û������ݳ��ִ���
  0x2001		//��֧��PSAM��
  0x2002		//PSAM��δ�嵽λ
  0x2003		//PSAM�����ϵ�
  0x2004		//PSAM��δ�ϵ�
  0x2005		//PSAM���ϵ�ʧ��
  0x2006		//����PSAM�������޻�Ӧ
  0x2007		//����PSAM�����ݳ��ִ���
  0x3001		//��֧�ַǽӴ�ʽ�û���
  0x3002		//δ��⵽�ǽӴ�ʽ�û���
  0x3003		//�ǽӴ�ʽ�û����Ѽ���
  0x3004		//�ǽӴ�ʽ�û���δ����
  0x3005		//�ǽӴ�ʽ�û�������ʧ��
  0x3006		//�����ǽӴ�ʽ�û�����ʱ
  0x3007		//�����ǽӴ�ʽ�û������ݳ��ִ���
  0x3008		//�ǽӴ�ʽ�û���ͣ��ʧ��
  0x3009		//�ж��ſ��ڸ�Ӧ��
  0x4001		//��֧�ִ�����
  0x4006		//����������ʱ
  0x4007		//�����������ݳ��ִ���
*/

extern "C"
{
/*--------------------------------------����Ϊ�豸ͨѶ��������API----------------------------------------*/
/*
��������	int CarderOpen(int iCom) 
��������	���豸����������
��������	iCom�����������1-COM1 , 2-COM2 , ...��ѡ1~16
����ֵ		0 �ɹ���-1ʧ��
*/
int __stdcall CarderOpen(int iCom);


/*
��������	CarderClose( )
��������	�ر��豸
��������	��
����ֵ		0 �ɹ���-1ʧ��
*/
int __stdcall CarderClose();

/*
��������	int GetModuleVer( int nModule, unsigned char *ucModuleVer)
��������	ȡ�豸�汾
��������	nModule�����������=0 �Ӵ�ʽIC��ģ�� =1 �ǽӴ�ʽ��ģ��
			ucModuleVer���������������ģ��汾
����ֵ		0 �ɹ���<0ʧ��
*/
int  __stdcall GetModuleVer( int nModule, unsigned char *ucModuleVer);

/*
��������	int GetCarderVer(unsigned char *ucCarderVer)
��������	ȡ�������汾
��������	ucCarderVer��������������ض������汾
����ֵ		0 �ɹ���<0ʧ��
*/
int  __stdcall GetCarderVer(unsigned char *ucCarderVer);

/*
��������	int CarderBeeping(unsigned char ucSustainedTime, unsigned char ucIntervalTime, unsigned char ucTimes)
��������	����������������
��������	ucSustainedTime������������������г���ʱ��, ��λ100����
			ucIntervalTime��������������м��ʱ��, ��λ100����
			ucTimes��������������������д���
����ֵ		0 �ɹ���!=0 �����
*/
int  __stdcall CarderBeeping(unsigned char ucSustainedTime, unsigned char ucIntervalTime, unsigned char ucTimes);
/*--------------------------------------����ΪCPU��API-----------------------------------------*/
/*
��������	int  GetCardStatus(unsigned char ucCardNo) 
��������	ȡ��Ƭ״̬
��������	ucCardNo��������������ۺ� =0xFF �ǽӴ�ʽ�� =0x00��0x0F  �Ӵ�ʽCPU��1����16 =0x10��0x1F  SAM��1����16
����ֵ		0 �ɹ���-1ʧ�� >0 ״̬�Ż�����
*/
int  __stdcall GetCardStatus(unsigned char ucCardNo);

/*
��������	int  CardActive(unsigned char ucCardNo, 
							unsigned char *ATR, 
							int *nATRLen) 
��������	CPU������
��������	ucCardNo��������������ۺ� =0xFF, �ǽӴ�ʽ�� =0x00��0x0F  �Ӵ�ʽCPU��1����16 =0x10��0x1F  SAM��1����16
			ATR�����������	����ǰ����Ϊ�Ӵ�ʽ����SAM����ƬʱΪ��ƬATR����  
							Ϊ�ǽӿ�ʱΪ ������(1Byte = 0x0A TYPE A, = 0x0B TYPE A) + ��ƬUID����(1Byte) + ��ƬUID + ��ƬATR����
			nATRLen�����������ATR���ݳ���
����ֵ		0 �ɹ���-1ͨѶʧ�� >0�����
*/
int  __stdcall CardActive(	unsigned char ucCardNo, 
							unsigned char *ATR, 
							int *nATRLen);
/*
��������	int  CardAPDU(	unsigned char ucCardNo, 
							unsigned char *CAPDU, 
							int nCAPDULen, 
							unsigned char *RAPDU, 
							int *nRAPDULen) 
��������	CPU��APDU����
��������	ucCardNo��������������ۺ� =0xFF, �ǽӴ�ʽ�� =0x00��0x0F  �Ӵ�ʽCPU��1����16 =0x10��0x1F  SAM��1����16
			CAPDU�����������APDU����
			nCAPDULen�����������APDU���ݳ���
			RAPDU�������������Ƭ��Ӧ����
			nRAPDULen�������������Ƭ��Ӧ���ݳ���
����ֵ		0 �ɹ���-1ͨѶʧ�� >0�����
*/
int  __stdcall CardAPDU(unsigned char ucCardNo, 
						unsigned char *CAPDU, 
						int nCAPDULen, 
						unsigned char *RAPDU, 
						int *nRAPDULen);

/*
��������	int  CardDeActive(unsigned char ucCardNo)
��������	CPU��ͣ��
��������	ucCardNo��������������ۺ� =0xFF, �ǽӴ�ʽ�� =0x00��0x0F  �Ӵ�ʽCPU��1����16 =0x10��0x1F  SAM��1����16
����ֵ		0 �ɹ���-1ͨѶʧ�� >0�����
*/
int  __stdcall CardDeActive(unsigned char ucCardNo);

/*--------------------------------------����Ϊ�ǽӴ�ʽ�洢��API------------------------------------------*/
/*��������	int PiccMemGetPort(int *piCom) 
��������	ȡ��ǰ�豸���ڴ��ڣ���������
��������	*piCom���������������1-COM1 , 2-COM2 , ...��ѡ1~16 ע�����벻Ϊ���ʾ���ȼ�鵱ǰ����ָ���˿ڣ����Զ������豸��
����ֵ		0 �ɹ���-1ʧ��
*/
int __stdcall PiccMemGetPort(int *piCom);
/*
��������	int  PiccMemActive(unsigned short usDelayTimes, 
								unsigned char *ucCardType, 
								unsigned char *UID) 
��������	����ǽӴ�ʽ�洢��
��������	usDelayTimes������������ȴ��������Ӧ��ʱ�䣨��λ�����룩��0:����ȴ����޿�ֱ�ӷ���, 0xffff:һֱ�ȴ�
			ucCardType����������������1�ֽڣ�0AH  A ��;  0BH  B��
			UID�������������UID��4�ֽ�
����ֵ		0 �ɹ���-1ʧ�� >0�����
*/
int  __stdcall PiccMemActive(unsigned short usDelayTimes, 
			   unsigned char *ucCardType,
			   unsigned char *UID);

/*
��������	int  PiccMemActiveHex(unsigned short usDelayTimes, 
								unsigned char *ucCardType, 
								unsigned char *UID) 
��������	����ǽӴ�ʽ�洢��
��������	usDelayTimes������������ȴ��������Ӧ��ʱ�䣨��λ�����룩��0:����ȴ����޿�ֱ�ӷ���, 0xffff:һֱ�ȴ�
			ucCardType����������������1�ֽڣ�'A'  TYPE A ��;  'B'  TYPE B��
			UID�������������UID��8�ֽڲ�ֺ��UID����0xAA ��� "AA"
����ֵ		0 �ɹ���-1ͨѶʧ�� >0�����
*/
int  __stdcall PiccMemActiveHex(unsigned short usDelayTimes, 
			   unsigned char *ucCardType,
			   unsigned char *UID);

/*
��������	int  PiccMemAuthSector(unsigned char ucSectorNo, 
								unsigned char ucKeyType,
								unsigned char *Key) 
��������	�ǽӴ�ʽ�洢����֤����
��������	ucSectorNo�����������������
			ucKeyType�������������Կ���� 0x60 A��Կ 0x61 B��Կ
			Key�������������Կ��6�ֽ�
����ֵ		0 �ɹ���-1ͨѶʧ�� >0�����
*/
int  __stdcall PiccMemAuthSector(unsigned char ucSectorNo, 
								unsigned char ucKeyType,
								unsigned char *Key);

/*
��������	int  PiccMemAuthSectorHex(unsigned char ucSectorNo, 
								unsigned char ucKeyType,
								unsigned char *Key) 
��������	�ǽӴ�ʽ�洢����֤����
��������	ucSectorNo�����������������
			ucKeyType�������������Կ���� 'A' A��Կ 'B' B��Կ
			Key�������������Կ��12�ֽڲ�ֺ����Կ
����ֵ		0 �ɹ���-1ͨѶʧ�� >0�����
*/
int  __stdcall PiccMemAuthSectorHex(unsigned char ucSectorNo, 
								unsigned char ucKeyType,
								unsigned char *Key);
								
/*
��������	int  PiccMemReadBlock(unsigned char ucBlockNo, 
								unsigned char *BlockData) 
��������	�ǽӴ�ʽ�洢������
��������	ucBlockNo��������������
			BlockData��������������ݣ�16�ֽ�
����ֵ		0 �ɹ���-1ʧ�� >0�����
*/
int  __stdcall PiccMemReadBlock(unsigned char ucBlockNo, 
								unsigned char *BlockData);
										

/*
��������	int  PiccMemReadBlockHex(unsigned char ucBlockNo, 
								unsigned char *BlockData) 
��������	�ǽӴ�ʽ�洢������
��������	ucBlockNo�������������� ��0��ʼ
			BlockData��������������ݣ�32�ֽڲ�ֺ�Ŀ�����
����ֵ		0 �ɹ���-1ͨѶʧ�� >0�����
*/
int  __stdcall PiccMemReadBlockHex(unsigned char ucBlockNo, 
								unsigned char *BlockData);

/*
��������	int  PiccMemWriteBlock(unsigned char ucBlockNo, 
								unsigned char *BlockData) 
��������	�ǽӴ�ʽ�洢��д��
��������	ucBlockNo��������������
			BlockData��������������ݣ�16�ֽ�
����ֵ		0 �ɹ���-1ʧ�� >0�����
*/
int  __stdcall PiccMemWriteBlock(unsigned char ucBlockNo, 
								unsigned char *BlockData);


/*
��������	int  PiccMemWriteBlockHex(unsigned char ucBlockNo, 
								unsigned char *BlockData) 
��������	�ǽӴ�ʽ�洢��д��
��������	ucBlockNo��������������
			BlockData��������������ݣ�32�ֽڲ�ֺ�Ŀ�����
����ֵ		0 �ɹ���-1ͨѶʧ�� >0�����
*/
int  __stdcall PiccMemWriteBlockHex(unsigned char ucBlockNo, 
								unsigned char *BlockData);

/*
��������	int  PiccMemReadValue(unsigned char ucBlockNo, 
								unsigned char *Value) 
��������	�ǽӴ�ʽ�洢����ֵ��
��������	ucBlockNo��������������
			Value��������������ݣ�4�ֽ�
����ֵ		0 �ɹ���-1ʧ�� >0�����
*/
int  __stdcall PiccMemReadValue(unsigned char ucBlockNo, 
								unsigned char *Value);


/*
��������	int  PiccMemReadValueInt(unsigned char ucBlockNo, 
								unsigned long *Value) 
��������	�ǽӴ�ʽ�洢����ֵ��
��������	ucBlockNo��������������
			Value�����������ֵ���޷�������
����ֵ		0 �ɹ���-1ͨѶʧ�� >0�����
*/
int  __stdcall PiccMemReadValueInt(unsigned char ucBlockNo, 
								unsigned long *Value);

/*
��������	int  PiccMemWriteValue(unsigned char ucBlockNo, 
								unsigned char *Value) 
��������	�ǽӴ�ʽ�洢��д��ֵ
��������	ucBlockNo��������������
			Value��������������ݣ�4�ֽ�
����ֵ		0 �ɹ���-1ʧ�� >0�����
*/
int  __stdcall PiccMemWriteValue(unsigned char ucBlockNo, 
								unsigned char *Value);


/*
��������	int  PiccMemWriteValueInt(unsigned char ucBlockNo, 
								unsigned long *Value) 
��������	�ǽӴ�ʽ�洢��дֵ��
��������	ucBlockNo��������������
			Value�����������ֵ���޷�������
����ֵ		0 �ɹ���-1ͨѶʧ�� >0�����
*/
int  __stdcall PiccMemWriteValueInt(unsigned char ucBlockNo, 
								unsigned long *Value);

/*
��������	int  PiccMemIncValue(unsigned char ucBlockNo, 
								unsigned char *Value)
��������	�ǽӴ�ʽ�洢����ֵ
��������	ucBlockNo��������������
			Value��������������ݣ�4�ֽ�
����ֵ		0 �ɹ���-1ʧ�� >0�����
*/
int  __stdcall PiccMemIncValue(unsigned char ucBlockNo, 
								unsigned char *Value);


/*
��������	int  __stdcall PiccMemIncValueInt(unsigned char ucBlockNo, 
								unsigned long *Value)
��������	�ǽӴ�ʽ�洢����ֵ 
��������	ucBlockNo��������������
			Value�����������ֵ���޷�������
����ֵ		0 �ɹ���-1ͨѶʧ�� >0�����
*/
int  __stdcall PiccMemIncValueInt(unsigned char ucBlockNo, 
								unsigned long *Value);

/*
��������	int  PiccMemDecValue(unsigned char ucBlockNo, 
								unsigned char *Value)
��������	�ǽӴ�ʽ�洢����ֵ
��������	ucBlockNo��������������
			Value��������������ݣ�4�ֽ�
����ֵ		0 �ɹ���-1ʧ�� >0�����
*/
int  __stdcall PiccMemDecValue(unsigned char ucBlockNo, 
								unsigned char *Value);


/*
��������	int  __stdcall PiccMemDecValueInt(unsigned char ucBlockNo, 
								unsigned long *Value)
��������	�ǽӴ�ʽ�洢����ֵ
��������	ucBlockNo��������������
			Value�����������ֵ���޷�������
����ֵ		0 �ɹ���-1ͨѶʧ�� >0�����
*/
int  __stdcall PiccMemDecValueInt(unsigned char ucBlockNo, 
								unsigned long *Value);

/*
��������	int  PiccMemWriteKey(unsigned char ucSectorNo, 
								unsigned char ucKeyType,
								unsigned char *Key)
��������	�ǽӴ�ʽ�洢��д����Կ
��������	ucSectorNo�����������������
			ucKeyType�������������Կ���� 
			Key�������������Կ��6�ֽ�
����ֵ		0 �ɹ���-1ʧ�� >0�����
*/
int  __stdcall PiccMemWriteKey(unsigned char ucSectorNo, 
								unsigned char ucKeyType,
								unsigned char *Key);

/*
��������	int  __stdcall PiccMemWriteKeyHex(unsigned char ucSectorNo, 
								unsigned char ucKeyType,
								unsigned char *Key)
��������	�ǽӴ�ʽ�洢��д����Կ
��������	ucSectorNo�����������������
			ucKeyType�������������Կ���� 'A' A��Կ 'B' B��Կ
			Key�������������Կ��12�ֽڲ�ֺ����Կ
����ֵ		0 �ɹ���-1ͨѶʧ�� >0�����
*/
int  __stdcall PiccMemWriteKeyHex(unsigned char ucSectorNo, 
								unsigned char ucKeyType,
								unsigned char *Key);
/*
��������	int  PiccMemHalt(void)
��������	�ǽӴ�ʽ�洢��ͣ��
��������	������� ����
����ֵ		0 �ɹ���-1ʧ�� >0�����
*/
int  __stdcall PiccMemHalt(void);

/*--------------------------------------����Ϊ����ҽ��S50��ר�ö�д����API------------------------------------------*/
/*
ע����ʹ����Щ����ǰ����������S50������һ���������ȼ�S50������֤Ҫ��д������
*/
/*
��������	int  PiccMemReadRecord(unsigned char ucBlockNo, 
								unsigned char *RecordData,
								int ReadLen) 
��������	�ǽӴ�ʽ�洢������¼
��������	ucBlockNo��������������
			RecordData��������������ݣ��32�ֽ�
			ReadLen: ���������Ҫ��ȡ�ĳ���
����ֵ		0 �ɹ���-1ʧ�� >0�����
*/
int  __stdcall PiccMemReadRecord(unsigned char ucBlockNo, 
								unsigned char *RecordData,
								int ReadLen);

/*
��������	int  PiccMemWriteRecord(unsigned char ucBlockNo, 
								unsigned char *RecordData,
								int ReadLen) 
��������	�ǽӴ�ʽ�洢��д��¼
��������	ucBlockNo��������������
			RecordData��������������ݣ��32�ֽ�
			WriteLen: ���������Ҫд��ĳ���
����ֵ		0 �ɹ���-1ʧ�� >0�����
*/
int  __stdcall PiccMemWriteRecord(unsigned char ucBlockNo, 
								unsigned char *RecordData,
								int WriteLen);
/*--------------------------------------����Ϊ�㽭ά��ָ��API------------------------------------------*/
int __stdcall FPIGetVersion (int nPort, unsigned char *psOutversion, int *lpLength);
int __stdcall FPIDevDetect(void);
int __stdcall FPITemplate(int nPort, unsigned char *psMB, int *lpLength);
int __stdcall FPIFeature(int nPort, unsigned char *psTZ, int *lpLength);
int __stdcall FPIMatch(unsigned char *psMB, unsigned char *psTZ, int iLevel);
int __stdcall FPIGetImageData(int nPort, int *lpImageWidth, int *lpImageHeight, unsigned char *psImage);
int __stdcall FPIImg2Bmp(char *psImgData, char *psBmpData);//(int nPort, char *psImgPath);
int __stdcall FPIBmp2Feature(int nPort, char *psImgPath, unsigned char *psTZ, int *lpLength); 
int __stdcall FPIBmp2Template(int nPort, char *psImgPath1, char *psImgPath2, char *psImgPath3, unsigned char *psMB, int *lpLength);
int __stdcall FPIExtract(char *psImgData, unsigned char *psTZ, int *lpLength);
int __stdcall FPIEnroll(char *psImg1, char *psImg2, char *psImg3, unsigned char *psMB, int *lpLength);
int __stdcall FPICheckFinger(int nPort);
/*--------------------------------------����Ϊ����֤API------------------------------------------*/
/**********************************************************
 ********************** �˿���API *************************
 **********************************************************/
/*
int SDT_GetCOMBaud (int iComID,unsigned int *puiBaud);
��������	�鿴SAM_V��ǰ������(�ú���ֻ����SAM_V����RS232���ڵ����Σ��������USB�ӿ���֧�ָ�API)��
��������    iComID���������,��������ʾ�˿ںš��˴��˿ںű���Ϊ1-16����ʾ���ڡ�
			puiBaudRate������������޷�������ָ�룬ָ����ͨ���ڵ�ǰ������, Ĭ�������Ϊ115200��
����ֵ		0x90 �ɹ�
			0x01 �˿ڴ�ʧ��/�˿ںŲ��Ϸ�
			0x05 �޷���ø�SAM_V�Ĳ����ʣ���SAM_V���ڲ����á�
*/
int __stdcall SDT_GetCOMBaud(int iComID,unsigned int *puiBaud);

/*
int SDT_SetCOMBaud(int iComID,unsigned int  uiCurrBaud,unsigned int  uiSetBaud);
��������	����SAM_V�Ĵ��ڵĲ�����(�ú���ֻ����SAM_V����RS232���ڵ����Σ��������USB�ӿ���֧�ָ�API)��
			���óɹ����ڸ�SAM_V������ע����ж���¼���ú�Ĳ����ʣ���֤��SAM_V���������͸���API�����µ�
			��ʱ�������ú�Ĳ����ʡ��ú������óɹ�����Ҫ��ʱ5���룬Ȼ����ܼ�����SAM_Vͨ�š�
��������    iComID���������,��������ʾ�˿ںš��˴��˿ںű���Ϊ1-16����ʾ���ڡ�
			uiCurrBaud������������޷������������ø�APIǰ�����õ�ҵ���ն���SAM_Vͨ�ŵĲ�����(SAM_V����ʱĬ��,
						ҵ���ն���SAM_Vͨ�ŵĲ�����Ϊ115200).ҵ���ն��Ըò�������SAM_Vͨ��,��������SAM_V�²�
						���ʵ�����.��uiCurrBaudֻ��Ϊ������ֵ֮һ��115200��57600��38400��19200��9600.
						���uiCurrBaud��ֵ������Щֵ֮һ����������0x21;��������õĲ�������uiCurrBaud��һ��, 
						��������0x02,��ʾ��������,����API���ɹ���
			uiSetBaud������������޷�����������Ҫ���õ�SAM_V��ҵ���ն�ͨ�Ų����ʡ�uiSetBaudֻ��ȡ����ֵ֮һ��
						115200��57600��38400��19200��9600���������uiSetBaud����������Щ��ֵ֮һ,����������0x21,
						���ò��ɹ�,����ԭ���Ĳ����ʲ��䡣
����ֵ		0x90 �ɹ�
			0x01 �˿ڴ�ʧ��/�˿ںŲ��Ϸ���
			0x02 ��ʱ�����ò��ɹ���
			0x21 uiCurrBaud ��uiSetBaud���������ֵ����.��
*/
int __stdcall SDT_SetCOMBaud(int iComID,unsigned int  uiCurrBaud,unsigned int  uiSetBaud);
int __stdcall SDT_OpenPort(int iPortID);
int __stdcall SDT_ClosePort(int iPortID);

/**********************************************************
 ********************** SAM��API **************************
 **********************************************************/
int __stdcall SDT_GetSAMStatus(int iPortID,int iIfOpen);
int __stdcall SDT_ResetSAM(int iPortID,int iIfOpen);
int __stdcall SDT_SetMaxRFByte(int iPortID,unsigned char ucByte,int bIfOpen);
int __stdcall SDT_GetSAMID(int iPortID,unsigned char *pucSAMID,int iIfOpen);
int __stdcall SDT_GetSAMIDToStr(int iPortID,char *pcSAMID,int iIfOpen);

/**********************************************************
 ******************* ����֤����API ************************
 **********************************************************/
int __stdcall SDT_StartFindIDCard(int iPortID,unsigned char *pucManaInfo,int iIfOpen);
int __stdcall SDT_SelectIDCard(int iPortID,unsigned char *pucManaMsg,int iIfOpen);
int __stdcall SDT_ReadBaseMsg(int iPortID,unsigned char * pucCHMsg,unsigned int *	puiCHMsgLen,unsigned char * pucPHMsg,unsigned int  *puiPHMsgLen,int iIfOpen);
int __stdcall SDT_ReadMngInfo(int iPortID,unsigned char * pucManageMsg,int iIfOpen);
int __stdcall SDT_ReadNewAppMsg(int iPortID,unsigned char * pucAppMsg,unsigned int *	puiAppMsgLen,int iIfOpen);

/*--------------------------------------����Ϊ������API------------------------------------------*/
/*
��������	int  ReadCardTrack(int ReadMode,unsigned char *Track2Data,unsigned char *Track3Data, double OutTime = 48) 
��������	������������
��������	ReadMode�������������ģʽ =1, �����ŵ�  =2, �����ŵ�  =3, �������ŵ�
			Track2Data��������������ŵ����ݣ��37�ֽڣ����ܣ�107�ֽڣ����ܣ�
			Track3Data: ������������ŵ����ݣ��107�ֽ�
			OutTime: �����������ѡ�� ������ʱʱ�䣬��λΪ�� ȱʡΪ48��
����ֵ		0 �ɹ���-1ʧ�� >0�����
*/
int __stdcall ReadCardTrack(int ReadMode,unsigned char *Track2Data,unsigned char *Track3Data, double OutTime);

/*
��������	int  WriteCardTrack(int WriteMode, unsigned char *Track2Data, unsigned char *Track3Data, double OutTime = 48) 
��������	д����������
��������	WriteMode�����������дģʽ =1, д���ŵ�  =2, д���ŵ�  =3, д�����ŵ�
			Track2Data��������������ŵ����ݣ��37�ֽڣ����ܣ�107�ֽڣ����ܣ�
			Track3Data: ������������ŵ����ݣ��107�ֽ�
			OutTime: �����������ѡ�� ������ʱʱ�䣬��λΪ�� ȱʡΪ48��
����ֵ		0 �ɹ���-1ʧ�� >0�����
*/
int __stdcall WriteCardTrack(int WriteMode, unsigned char *Track2Data, unsigned char *Track3Data, double OutTime);

//����Ϊ��ҽӦ�ýӿ�
/*
��������	int BeginReadCardNo(int ReadMode)
��������	�����ҽ�ƿ�����ģʽ
��������	ReadMode�������������ģʽ =1, ������  =2, ��M1�� =3��������M1��
����ֵ		0 �ɹ���-1ʧ�� >0�����
*/
int __stdcall BeginReadCardNo(int ReadMode);

/*
��������	int EndReadCardNo(int ReadMode)
��������	�˳���ҽ�ƿ�����ģʽ
��������	ReadMode�������������ģʽ =1, ������  =2, ��M1�� =3��������M1��
����ֵ		0 �ɹ���-1ʧ�� >0�����
*/
int __stdcall EndReadCardNo(int ReadMode);

/*
��������	int GetHospitalCardNo(int ReadMode, int MsrTrk, int MemBlock, unsigned char *MemPass, unsigned char MemPassType, unsigned char *CardNo, unsigned char *CardUID, int &CurMode, double OutTime = 48)
��������	��ҽ�ƿ�����
��������	ReadMode�������������ģʽ =1, ������  =2, ��M1�� =3��������M1��
			MsrTrk�����������������ģʽ =1, �����ŵ�  =2, �����ŵ�
			MemBlock�����������M1����ţ���0��ʼ
			MemPass�����������M1����������
			MemPassType�����������M1�������������� ='A' A��Կ, ='B' A��Կ
			CardNo: ������������ؿ���
			CurMode: ������������ؿ���ģʽ =1, ����  =2, M1��
			OutTime: �����������ѡ�� ������ʱʱ�䣬��λΪ�� ȱʡΪ48��
����ֵ		0 �ɹ���-1ʧ�� >0�����
*/
int __stdcall GetHospitalCardNo(int ReadMode, int MsrTrk, int MemBlock, unsigned char *MemPass, unsigned char MemPassType, unsigned char *CardNo, unsigned char *CardUID, int &CurMode, double OutTime = 48);

/*
����:    ��ȡ�ͻ���Ϣ
���������
         port:	ʹ�ô��ڵĶ˿ں�,int��,����int�͵�ֵ����,��:1
	   baud:	������,int��,һ����9600
	   extport:	��չ��,���Լ��豸����,û�о���"0"��NULL,��;"A"
         outtime:	��ʱ,int��,�ַ���ȡ�������ʱ��
	   icflag:	������,int��,�ֶ������Ͳ࿨��,��������1,����Ϊ�࿨��
  	   aidlist:	Ӧ��ѡ���б�,��ǿ�Ϊ"A000000333010101",���ǿ�Ϊ"A000000333010102"
         taglist:	��ǩ�б�����"ABCDEFGHIJKL",
			����Ԫ		����IC��������	��ǩ
			�ʺ�		19λ ans��ʽ	0x41
			����		STRING		0x42
			֤������	HEX     	0x43
					ֵ:
					00������֤
					01������֤
					02������
					03���뾳֤      
					04����ʱ����֤
					05������	
			֤������	Ans		0x44
			���ŵ���Ϣ����ѡ��Ans		0x45
			һ�ŵ���Ϣ����ѡ��Ans		0x46
			���		Ans,����С����	0x47
			�����ֽ��������		0x48
			Ӧ��ʧЧ����			0x49
			Ӧ�����ʺ�(PAN)ϵ�к�		0x4A
				     
���������
	  userinfo:	���еĿͻ���Ϣ,���������ǩ����(tag+3�ֽڳ���+���ݣ�����A001xB000C003123)
	  ictype:       ���ؿ�Ƭ����,1�Ƕ�����,����Ϊ�࿨��
����ֵ��
	  0:   	�ɹ�
	  -4:  	�޶�ӦĿ¼
   	  -7:	��ʱ��_ERR_NT_OUTTIME��
	  -16:	_ERR_NT_DATA
	  -100:	�򿪴���ʧ��
	   ������ִ��IC���ű�����ʱIC�����صĴ�����

����userinfo
A0196217995510000011675B00511675C00201D018520112198212152540E0386217995510000011675D49122201230600000FG012000000015469H012000000100000I006301231J00201
*/
int WINAPI IC_GetInfo(int port, int baud, char *extport, int outtime, int icflag,char* aidlist,
					  char* taglist, char* userinfo, int *ictype);

/*
��������	int ReadCardInfo(int iPort, int iBaud, int iReadMode, char *pucIDMsg, int &iCurMode, double dbOutTime)
��������	����Ƭ��Ϣ
��������	iReadMode�������������ģʽ��
						0x01: ����
						0x02: M1�����ݲ�֧�֣�������
						0x04: �Ӵ�ʽ���ڿ�
						0x08: �ǽӴ�ʽ���ڿ�
						0x10: ��������֤
			pucIDMsg: ������������ؿ�Ƭ��Ϣ���ɶ��ֿ�Ƭ��Ϣ��ɣ�������Ϣ��Դ��ͬ�������ʽ���£�
						1���������ݸ�ʽ��
							���ŵ����� + ��A�� + ���ŵ�����
						2��M1�����ݣ�
							16�ֽڿ�����
						3���Ӵ�ʽ���ڿ��ͷǽӴ�ʽ���ڿ���
							��ǩ(0x41) + ���ֽ�ʮ���Ƴ��ȣ��磺��100����ʾ����Ϊ100��+ �ʺ� + ��ǩ(0x42) + 
										���� + ��ǩ(0x4A) + ���ֽ�ʮ���Ƴ��ȣ��磺��100����ʾ����Ϊ100��+ Ӧ�����ʺ�(PAN)ϵ�к�
							��ǩ�Ķ������£�
								����Ԫ				����IC��������	��ǩ
								�ʺ�				19λ ans��ʽ	0x41A
								����				STRING		0x42B
								֤������			HEX     	0x43C
												ֵ:
												00������֤
												01������֤
												02������
												03���뾳֤      
												04����ʱ����֤
												05������	
								֤������			Ans		0x44D
								���ŵ���Ϣ����ѡ��	Ans			0x45E
								һ�ŵ���Ϣ����ѡ��	Ans			0x46F
								���				Ans,����С����	0x47G
								�����ֽ��������				0x48H
								Ӧ��ʧЧ����					0x49I
								Ӧ�����ʺ�(PAN)ϵ�к�				0x4AJ
						4����������֤��
							��ǩ(0x41) + ���ֽ�ʮ���Ƴ��ȣ��磺��100����ʾ����Ϊ100��+ ���� + ��ǩ(0x42) + 
										���� + ��ǩ(0x4A) + ���ֽ�ʮ���Ƴ��ȣ��磺��100����ʾ����Ϊ100��+ �¼�ͥסַ
							��ǩ�Ķ������£�
								����Ԫ					��ǩ
								����					0x41
								�Ա�					0x42
								����					0x43													
								��������				0x44
								��ͥסַ				0x45
								����֤����				0x46
								��֤����				0x47
								��Ч����ʼ����				0x48
								��Ч�ڽ�������				0x49
								�¼�ͥסַ				0x4A
			iCurMode: ������������ص�ǰ��Ƭ��Ϣ��Դ:
						0x01: ����
						0x02: M1�����ݲ�֧�֣�������
						0x04: �Ӵ�ʽ���ڿ�
						0x08: �ǽӴ�ʽ���ڿ�
						0x10: ��������֤
			dbOutTime: �����������ѡ�� ������ʱʱ�䣬��λΪ�� ȱʡΪ48��
����ֵ		0 �ɹ���-1ʧ�� >0�����
*/
int __stdcall ReadCardInfo(int iPort, int iBaud, int iReadMode, char *pucIDMsg, int &iCurMode, double dbOutTime);
}
#endif