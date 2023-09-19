#ifndef _GWI_X0_DRV_H_
#define _GWI_X0_DRV_H_

/*返回错误定义*/
/*		
  -1			//读卡器联接失败
  -2			//参数错误
  -3			//设备类型错误
  0				//成功
  0x1001		//不支持接触式用户卡
  0x1002		//接触式用户卡未插到位
  0x1003		//接触式用户卡已上电
  0x1004		//接触式用户卡未上电
  0x1005		//接触式用户卡上电失败
  0x1006		//操作接触式用户卡数据无回应
  0x1007		//操作接触式用户卡数据出现错误
  0x2001		//不支持PSAM卡
  0x2002		//PSAM卡未插到位
  0x2003		//PSAM卡已上电
  0x2004		//PSAM卡未上电
  0x2005		//PSAM卡上电失败
  0x2006		//操作PSAM卡数据无回应
  0x2007		//操作PSAM卡数据出现错误
  0x3001		//不支持非接触式用户卡
  0x3002		//未检测到非接触式用户卡
  0x3003		//非接触式用户卡已激活
  0x3004		//非接触式用户卡未激活
  0x3005		//非接触式用户卡激活失败
  0x3006		//操作非接触式用户卡超时
  0x3007		//操作非接触式用户卡数据出现错误
  0x3008		//非接触式用户卡停活失败
  0x3009		//有多张卡在感应区
  0x4001		//不支持磁条卡
  0x4006		//操作磁条超时
  0x4007		//操作磁条数据出现错误
*/

extern "C"
{
/*--------------------------------------以下为设备通讯基本函数API----------------------------------------*/
/*
函数声明	int CarderOpen(int iCom) 
功能描述	打开设备（带参数）
参数含义	iCom：输入参数，1-COM1 , 2-COM2 , ...可选1~16
返回值		0 成功　-1失败
*/
int __stdcall CarderOpen(int iCom);


/*
函数声明	CarderClose( )
功能描述	关闭设备
参数含义	无
返回值		0 成功　-1失败
*/
int __stdcall CarderClose();

/*
函数声明	int GetModuleVer( int nModule, unsigned char *ucModuleVer)
功能描述	取设备版本
参数含义	nModule：输入参数，=0 接触式IC卡模块 =1 非接触式卡模块
			ucModuleVer：输出参数，返回模块版本
返回值		0 成功　<0失败
*/
int  __stdcall GetModuleVer( int nModule, unsigned char *ucModuleVer);

/*
函数声明	int GetCarderVer(unsigned char *ucCarderVer)
功能描述	取读卡器版本
参数含义	ucCarderVer：输出参数，返回读卡器版本
返回值		0 成功　<0失败
*/
int  __stdcall GetCarderVer(unsigned char *ucCarderVer);

/*
函数声明	int CarderBeeping(unsigned char ucSustainedTime, unsigned char ucIntervalTime, unsigned char ucTimes)
功能描述	读卡器蜂鸣器控制
参数含义	ucSustainedTime：输入参数，单次鸣叫持续时间, 单位100毫秒
			ucIntervalTime：输入参数，鸣叫间隔时间, 单位100毫秒
			ucTimes：输入参数，蜂鸣器鸣叫次数
返回值		0 成功　!=0 错误号
*/
int  __stdcall CarderBeeping(unsigned char ucSustainedTime, unsigned char ucIntervalTime, unsigned char ucTimes);
/*--------------------------------------以下为CPU卡API-----------------------------------------*/
/*
函数声明	int  GetCardStatus(unsigned char ucCardNo) 
功能描述	取卡片状态
参数含义	ucCardNo：输入参数，卡槽号 =0xFF 非接触式卡 =0x00～0x0F  接触式CPU卡1～卡16 =0x10～0x1F  SAM卡1～卡16
返回值		0 成功　-1失败 >0 状态号或错误号
*/
int  __stdcall GetCardStatus(unsigned char ucCardNo);

/*
函数声明	int  CardActive(unsigned char ucCardNo, 
							unsigned char *ATR, 
							int *nATRLen) 
功能描述	CPU卡激活
参数含义	ucCardNo：输入参数，卡槽号 =0xFF, 非接触式卡 =0x00～0x0F  接触式CPU卡1～卡16 =0x10～0x1F  SAM卡1～卡16
			ATR：输出参数，	当卡前卡槽为接触式卡或SAM卡卡片时为卡片ATR数据  
							为非接卡时为 卡类型(1Byte = 0x0A TYPE A, = 0x0B TYPE A) + 卡片UID长度(1Byte) + 卡片UID + 卡片ATR数据
			nATRLen：输出参数，ATR数据长度
返回值		0 成功　-1通讯失败 >0错误号
*/
int  __stdcall CardActive(	unsigned char ucCardNo, 
							unsigned char *ATR, 
							int *nATRLen);
/*
函数声明	int  CardAPDU(	unsigned char ucCardNo, 
							unsigned char *CAPDU, 
							int nCAPDULen, 
							unsigned char *RAPDU, 
							int *nRAPDULen) 
功能描述	CPU卡APDU交互
参数含义	ucCardNo：输入参数，卡槽号 =0xFF, 非接触式卡 =0x00～0x0F  接触式CPU卡1～卡16 =0x10～0x1F  SAM卡1～卡16
			CAPDU：输入参数，APDU数据
			nCAPDULen：输入参数，APDU数据长度
			RAPDU：输出参数，卡片响应数据
			nRAPDULen：输出参数，卡片响应数据长度
返回值		0 成功　-1通讯失败 >0错误号
*/
int  __stdcall CardAPDU(unsigned char ucCardNo, 
						unsigned char *CAPDU, 
						int nCAPDULen, 
						unsigned char *RAPDU, 
						int *nRAPDULen);

/*
函数声明	int  CardDeActive(unsigned char ucCardNo)
功能描述	CPU卡停活
参数含义	ucCardNo：输入参数，卡槽号 =0xFF, 非接触式卡 =0x00～0x0F  接触式CPU卡1～卡16 =0x10～0x1F  SAM卡1～卡16
返回值		0 成功　-1通讯失败 >0错误号
*/
int  __stdcall CardDeActive(unsigned char ucCardNo);

/*--------------------------------------以下为非接触式存储卡API------------------------------------------*/
/*函数声明	int PiccMemGetPort(int *piCom) 
功能描述	取当前设备所在串口（带参数）
参数含义	*piCom：输入输出参数，1-COM1 , 2-COM2 , ...可选1~16 注：输入不为零表示优先检查当前参数指定端口，再自动查找设备。
返回值		0 成功　-1失败
*/
int __stdcall PiccMemGetPort(int *piCom);
/*
函数声明	int  PiccMemActive(unsigned short usDelayTimes, 
								unsigned char *ucCardType, 
								unsigned char *UID) 
功能描述	激活非接触式存储卡
参数含义	usDelayTimes：输入参数，等待卡进入感应区时间（单位：毫秒），0:无需等待，无卡直接返回, 0xffff:一直等待
			ucCardType：输出参数，卡类别，1字节，0AH  A 卡;  0BH  B卡
			UID：输出参数，卡UID，4字节
返回值		0 成功　-1失败 >0错误号
*/
int  __stdcall PiccMemActive(unsigned short usDelayTimes, 
			   unsigned char *ucCardType,
			   unsigned char *UID);

/*
函数声明	int  PiccMemActiveHex(unsigned short usDelayTimes, 
								unsigned char *ucCardType, 
								unsigned char *UID) 
功能描述	激活非接触式存储卡
参数含义	usDelayTimes：输入参数，等待卡进入感应区时间（单位：毫秒），0:无需等待，无卡直接返回, 0xffff:一直等待
			ucCardType：输出参数，卡类别，1字节，'A'  TYPE A 卡;  'B'  TYPE B卡
			UID：输出参数，卡UID，8字节拆分后的UID，如0xAA 拆成 "AA"
返回值		0 成功　-1通讯失败 >0错误号
*/
int  __stdcall PiccMemActiveHex(unsigned short usDelayTimes, 
			   unsigned char *ucCardType,
			   unsigned char *UID);

/*
函数声明	int  PiccMemAuthSector(unsigned char ucSectorNo, 
								unsigned char ucKeyType,
								unsigned char *Key) 
功能描述	非接触式存储卡认证扇区
参数含义	ucSectorNo：输入参数，扇区号
			ucKeyType：输入参数，密钥类型 0x60 A密钥 0x61 B密钥
			Key：输入参数，密钥，6字节
返回值		0 成功　-1通讯失败 >0错误号
*/
int  __stdcall PiccMemAuthSector(unsigned char ucSectorNo, 
								unsigned char ucKeyType,
								unsigned char *Key);

/*
函数声明	int  PiccMemAuthSectorHex(unsigned char ucSectorNo, 
								unsigned char ucKeyType,
								unsigned char *Key) 
功能描述	非接触式存储卡认证扇区
参数含义	ucSectorNo：输入参数，扇区号
			ucKeyType：输入参数，密钥类型 'A' A密钥 'B' B密钥
			Key：输入参数，密钥，12字节拆分后的密钥
返回值		0 成功　-1通讯失败 >0错误号
*/
int  __stdcall PiccMemAuthSectorHex(unsigned char ucSectorNo, 
								unsigned char ucKeyType,
								unsigned char *Key);
								
/*
函数声明	int  PiccMemReadBlock(unsigned char ucBlockNo, 
								unsigned char *BlockData) 
功能描述	非接触式存储卡读块
参数含义	ucBlockNo：输入参数，块号
			BlockData：输出参数，数据，16字节
返回值		0 成功　-1失败 >0错误号
*/
int  __stdcall PiccMemReadBlock(unsigned char ucBlockNo, 
								unsigned char *BlockData);
										

/*
函数声明	int  PiccMemReadBlockHex(unsigned char ucBlockNo, 
								unsigned char *BlockData) 
功能描述	非接触式存储卡读块
参数含义	ucBlockNo：输入参数，块号 从0开始
			BlockData：输出参数，数据，32字节拆分后的块数据
返回值		0 成功　-1通讯失败 >0错误号
*/
int  __stdcall PiccMemReadBlockHex(unsigned char ucBlockNo, 
								unsigned char *BlockData);

/*
函数声明	int  PiccMemWriteBlock(unsigned char ucBlockNo, 
								unsigned char *BlockData) 
功能描述	非接触式存储卡写块
参数含义	ucBlockNo：输入参数，块号
			BlockData：输入参数，数据，16字节
返回值		0 成功　-1失败 >0错误号
*/
int  __stdcall PiccMemWriteBlock(unsigned char ucBlockNo, 
								unsigned char *BlockData);


/*
函数声明	int  PiccMemWriteBlockHex(unsigned char ucBlockNo, 
								unsigned char *BlockData) 
功能描述	非接触式存储卡写块
参数含义	ucBlockNo：输入参数，块号
			BlockData：输入参数，数据，32字节拆分后的块数据
返回值		0 成功　-1通讯失败 >0错误号
*/
int  __stdcall PiccMemWriteBlockHex(unsigned char ucBlockNo, 
								unsigned char *BlockData);

/*
函数声明	int  PiccMemReadValue(unsigned char ucBlockNo, 
								unsigned char *Value) 
功能描述	非接触式存储卡读值块
参数含义	ucBlockNo：输入参数，块号
			Value：输出参数，数据，4字节
返回值		0 成功　-1失败 >0错误号
*/
int  __stdcall PiccMemReadValue(unsigned char ucBlockNo, 
								unsigned char *Value);


/*
函数声明	int  PiccMemReadValueInt(unsigned char ucBlockNo, 
								unsigned long *Value) 
功能描述	非接触式存储卡读值块
参数含义	ucBlockNo：输入参数，块号
			Value：输出参数，值，无符号整型
返回值		0 成功　-1通讯失败 >0错误号
*/
int  __stdcall PiccMemReadValueInt(unsigned char ucBlockNo, 
								unsigned long *Value);

/*
函数声明	int  PiccMemWriteValue(unsigned char ucBlockNo, 
								unsigned char *Value) 
功能描述	非接触式存储卡写块值
参数含义	ucBlockNo：输入参数，块号
			Value：输入参数，数据，4字节
返回值		0 成功　-1失败 >0错误号
*/
int  __stdcall PiccMemWriteValue(unsigned char ucBlockNo, 
								unsigned char *Value);


/*
函数声明	int  PiccMemWriteValueInt(unsigned char ucBlockNo, 
								unsigned long *Value) 
功能描述	非接触式存储卡写值块
参数含义	ucBlockNo：输入参数，块号
			Value：输入参数，值，无符号整型
返回值		0 成功　-1通讯失败 >0错误号
*/
int  __stdcall PiccMemWriteValueInt(unsigned char ucBlockNo, 
								unsigned long *Value);

/*
函数声明	int  PiccMemIncValue(unsigned char ucBlockNo, 
								unsigned char *Value)
功能描述	非接触式存储卡加值
参数含义	ucBlockNo：输入参数，块号
			Value：输入参数，数据，4字节
返回值		0 成功　-1失败 >0错误号
*/
int  __stdcall PiccMemIncValue(unsigned char ucBlockNo, 
								unsigned char *Value);


/*
函数声明	int  __stdcall PiccMemIncValueInt(unsigned char ucBlockNo, 
								unsigned long *Value)
功能描述	非接触式存储卡加值 
参数含义	ucBlockNo：输入参数，块号
			Value：输入参数，值，无符号整型
返回值		0 成功　-1通讯失败 >0错误号
*/
int  __stdcall PiccMemIncValueInt(unsigned char ucBlockNo, 
								unsigned long *Value);

/*
函数声明	int  PiccMemDecValue(unsigned char ucBlockNo, 
								unsigned char *Value)
功能描述	非接触式存储卡减值
参数含义	ucBlockNo：输入参数，块号
			Value：输入参数，数据，4字节
返回值		0 成功　-1失败 >0错误号
*/
int  __stdcall PiccMemDecValue(unsigned char ucBlockNo, 
								unsigned char *Value);


/*
函数声明	int  __stdcall PiccMemDecValueInt(unsigned char ucBlockNo, 
								unsigned long *Value)
功能描述	非接触式存储卡减值
参数含义	ucBlockNo：输入参数，块号
			Value：输入参数，值，无符号整型
返回值		0 成功　-1通讯失败 >0错误号
*/
int  __stdcall PiccMemDecValueInt(unsigned char ucBlockNo, 
								unsigned long *Value);

/*
函数声明	int  PiccMemWriteKey(unsigned char ucSectorNo, 
								unsigned char ucKeyType,
								unsigned char *Key)
功能描述	非接触式存储卡写主密钥
参数含义	ucSectorNo：输入参数，扇区号
			ucKeyType：输入参数，密钥类型 
			Key：输入参数，密钥，6字节
返回值		0 成功　-1失败 >0错误号
*/
int  __stdcall PiccMemWriteKey(unsigned char ucSectorNo, 
								unsigned char ucKeyType,
								unsigned char *Key);

/*
函数声明	int  __stdcall PiccMemWriteKeyHex(unsigned char ucSectorNo, 
								unsigned char ucKeyType,
								unsigned char *Key)
功能描述	非接触式存储卡写主密钥
参数含义	ucSectorNo：输入参数，扇区号
			ucKeyType：输入参数，密钥类型 'A' A密钥 'B' B密钥
			Key：输入参数，密钥，12字节拆分后的密钥
返回值		0 成功　-1通讯失败 >0错误号
*/
int  __stdcall PiccMemWriteKeyHex(unsigned char ucSectorNo, 
								unsigned char ucKeyType,
								unsigned char *Key);
/*
函数声明	int  PiccMemHalt(void)
功能描述	非接触式存储卡停活
参数含义	输入参数 ：无
返回值		0 成功　-1失败 >0错误号
*/
int  __stdcall PiccMemHalt(void);

/*--------------------------------------以下为长城医疗S50卡专用读写卡号API------------------------------------------*/
/*
注：在使用这些函数前，均和正常S50卡操作一样，必须先激S50卡并认证要读写的扇区
*/
/*
函数声明	int  PiccMemReadRecord(unsigned char ucBlockNo, 
								unsigned char *RecordData,
								int ReadLen) 
功能描述	非接触式存储卡读记录
参数含义	ucBlockNo：输入参数，块号
			RecordData：输出参数，数据，最长32字节
			ReadLen: 输入参数，要读取的长度
返回值		0 成功　-1失败 >0错误号
*/
int  __stdcall PiccMemReadRecord(unsigned char ucBlockNo, 
								unsigned char *RecordData,
								int ReadLen);

/*
函数声明	int  PiccMemWriteRecord(unsigned char ucBlockNo, 
								unsigned char *RecordData,
								int ReadLen) 
功能描述	非接触式存储卡写记录
参数含义	ucBlockNo：输入参数，块号
			RecordData：输入参数，数据，最长32字节
			WriteLen: 输入参数，要写入的长度
返回值		0 成功　-1失败 >0错误号
*/
int  __stdcall PiccMemWriteRecord(unsigned char ucBlockNo, 
								unsigned char *RecordData,
								int WriteLen);
/*--------------------------------------以下为浙江维尔指纹API------------------------------------------*/
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
/*--------------------------------------以下为二代证API------------------------------------------*/
/**********************************************************
 ********************** 端口类API *************************
 **********************************************************/
/*
int SDT_GetCOMBaud (int iComID,unsigned int *puiBaud);
功能描述	查看SAM_V当前波特率(该函数只用于SAM_V采用RS232串口的情形，如果采用USB接口则不支持该API)。
参数含义    iComID：输入参数,整数，表示端口号。此处端口号必须为1-16，表示串口。
			puiBaudRate：输出参数，无符号整数指针，指向普通串口当前波特率, 默认情况下为115200。
返回值		0x90 成功
			0x01 端口打开失败/端口号不合法
			0x05 无法获得该SAM_V的波特率，该SAM_V串口不可用。
*/
int __stdcall SDT_GetCOMBaud(int iComID,unsigned int *puiBaud);

/*
int SDT_SetCOMBaud(int iComID,unsigned int  uiCurrBaud,unsigned int  uiSetBaud);
功能描述	设置SAM_V的串口的波特率(该函数只用于SAM_V采用RS232串口的情形，如果采用USB接口则不支持该API)，
			设置成功后，在该SAM_V和主机注册表中都记录设置后的波特率，保证在SAM_V重新启动和该套API被重新调
			用时采用设置后的波特率。该函数调用成功后，需要延时5毫秒，然后才能继续与SAM_V通信。
参数含义    iComID：输入参数,整数，表示端口号。此处端口号必须为1-16，表示串口。
			uiCurrBaud：输入参数，无符号整数，调用该API前已设置的业务终端与SAM_V通信的波特率(SAM_V出厂时默认,
						业务终端与SAM_V通信的波特率为115200).业务终端以该波特率与SAM_V通信,发出设置SAM_V新波
						特率的命令.。uiCurrBaud只能为下列数值之一：115200，57600，38400，19200，9600.
						如果uiCurrBaud数值不是这些值之一，函数返回0x21;如果已设置的波特率与uiCurrBaud不一致, 
						则函数返回0x02,表示不能设置,调用API不成功。
			uiSetBaud：输入参数，无符号整数，将要设置的SAM_V与业务终端通信波特率。uiSetBaud只能取下列值之一：
						115200，57600，38400，19200，9600，如果输入uiSetBaud参数不是这些数值之一,，函数返回0x21,
						设置不成功,保持原来的波特率不变。
返回值		0x90 成功
			0x01 端口打开失败/端口号不合法。
			0x02 超时，设置不成功。
			0x21 uiCurrBaud 、uiSetBaud输入参数数值错误.。
*/
int __stdcall SDT_SetCOMBaud(int iComID,unsigned int  uiCurrBaud,unsigned int  uiSetBaud);
int __stdcall SDT_OpenPort(int iPortID);
int __stdcall SDT_ClosePort(int iPortID);

/**********************************************************
 ********************** SAM类API **************************
 **********************************************************/
int __stdcall SDT_GetSAMStatus(int iPortID,int iIfOpen);
int __stdcall SDT_ResetSAM(int iPortID,int iIfOpen);
int __stdcall SDT_SetMaxRFByte(int iPortID,unsigned char ucByte,int bIfOpen);
int __stdcall SDT_GetSAMID(int iPortID,unsigned char *pucSAMID,int iIfOpen);
int __stdcall SDT_GetSAMIDToStr(int iPortID,char *pcSAMID,int iIfOpen);

/**********************************************************
 ******************* 身份证卡类API ************************
 **********************************************************/
int __stdcall SDT_StartFindIDCard(int iPortID,unsigned char *pucManaInfo,int iIfOpen);
int __stdcall SDT_SelectIDCard(int iPortID,unsigned char *pucManaMsg,int iIfOpen);
int __stdcall SDT_ReadBaseMsg(int iPortID,unsigned char * pucCHMsg,unsigned int *	puiCHMsgLen,unsigned char * pucPHMsg,unsigned int  *puiPHMsgLen,int iIfOpen);
int __stdcall SDT_ReadMngInfo(int iPortID,unsigned char * pucManageMsg,int iIfOpen);
int __stdcall SDT_ReadNewAppMsg(int iPortID,unsigned char * pucAppMsg,unsigned int *	puiAppMsgLen,int iIfOpen);

/*--------------------------------------以下为磁条卡API------------------------------------------*/
/*
函数声明	int  ReadCardTrack(int ReadMode,unsigned char *Track2Data,unsigned char *Track3Data, double OutTime = 48) 
功能描述	读磁条卡数据
参数含义	ReadMode：输入参数，读模式 =1, 读二磁道  =2, 读三磁道  =3, 读二三磁道
			Track2Data：输出参数，二磁道数据，最长37字节（低密）107字节（高密）
			Track3Data: 输出参数，三磁道数据，最长107字节
			OutTime: 输入参数，可选， 操作超时时间，单位为秒 缺省为48秒
返回值		0 成功　-1失败 >0错误号
*/
int __stdcall ReadCardTrack(int ReadMode,unsigned char *Track2Data,unsigned char *Track3Data, double OutTime);

/*
函数声明	int  WriteCardTrack(int WriteMode, unsigned char *Track2Data, unsigned char *Track3Data, double OutTime = 48) 
功能描述	写磁条卡数据
参数含义	WriteMode：输入参数，写模式 =1, 写二磁道  =2, 写三磁道  =3, 写二三磁道
			Track2Data：输入参数，二磁道数据，最长37字节（低密）107字节（高密）
			Track3Data: 输入参数，三磁道数据，最长107字节
			OutTime: 输入参数，可选， 操作超时时间，单位为秒 缺省为48秒
返回值		0 成功　-1失败 >0错误号
*/
int __stdcall WriteCardTrack(int WriteMode, unsigned char *Track2Data, unsigned char *Track3Data, double OutTime);

//以下为银医应用接口
/*
函数声明	int BeginReadCardNo(int ReadMode)
功能描述	进入读医疗卡卡号模式
参数含义	ReadMode：输入参数，读模式 =1, 读磁条  =2, 读M1卡 =3读磁条加M1卡
返回值		0 成功　-1失败 >0错误号
*/
int __stdcall BeginReadCardNo(int ReadMode);

/*
函数声明	int EndReadCardNo(int ReadMode)
功能描述	退出读医疗卡卡号模式
参数含义	ReadMode：输入参数，读模式 =1, 读磁条  =2, 读M1卡 =3读磁条加M1卡
返回值		0 成功　-1失败 >0错误号
*/
int __stdcall EndReadCardNo(int ReadMode);

/*
函数声明	int GetHospitalCardNo(int ReadMode, int MsrTrk, int MemBlock, unsigned char *MemPass, unsigned char MemPassType, unsigned char *CardNo, unsigned char *CardUID, int &CurMode, double OutTime = 48)
功能描述	读医疗卡卡号
参数含义	ReadMode：输入参数，读模式 =1, 读磁条  =2, 读M1卡 =3读磁条加M1卡
			MsrTrk：输入参数，读磁条模式 =1, 读二磁道  =2, 读三磁道
			MemBlock：输入参数，M1卡块号，从0开始
			MemPass：输入参数，M1卡扇区密码
			MemPassType：输入参数，M1卡扇区密码类型 ='A' A密钥, ='B' A密钥
			CardNo: 输出参数，返回卡号
			CurMode: 输出参数，返回卡号模式 =1, 磁条  =2, M1卡
			OutTime: 输入参数，可选， 操作超时时间，单位为秒 缺省为48秒
返回值		0 成功　-1失败 >0错误号
*/
int __stdcall GetHospitalCardNo(int ReadMode, int MsrTrk, int MemBlock, unsigned char *MemPass, unsigned char MemPassType, unsigned char *CardNo, unsigned char *CardUID, int &CurMode, double OutTime = 48);

/*
功能:    获取客户信息
输入参数：
         port:	使用串口的端口号,int型,输入int型的值即可,如:1
	   baud:	波特率,int型,一般置9600
	   extport:	扩展口,随自己设备而定,没有就置"0"或NULL,如;"A"
         outtime:	超时,int型,字符读取的最大间隔时间
	   icflag:	卡座号,int型,分顶卡座和侧卡座,顶卡座置1,其它为侧卡座
  	   aidlist:	应用选择列表,借记卡为"A000000333010101",贷记卡为"A000000333010102"
         taglist:	标签列表，如"ABCDEFGHIJKL",
			数据元		来自IC卡的数据	标签
			帐号		19位 ans格式	0x41
			姓名		STRING		0x42
			证件类型	HEX     	0x43
					值:
					00：身份证
					01：军官证
					02：护照
					03：入境证      
					04：临时身份证
					05：其它	
			证件号码	Ans		0x44
			二磁道信息（可选）Ans		0x45
			一磁道信息（可选）Ans		0x46
			余额		Ans,不带小数点	0x47
			电子现金余额上限		0x48
			应用失效日期			0x49
			应用主帐号(PAN)系列号		0x4A
				     
输出参数：
	  userinfo:	卡中的客户信息,包含多个标签变量(tag+3字节长度+内容，比如A001xB000C003123)
	  ictype:       返回卡片类型,1是顶卡座,其它为侧卡座
返回值：
	  0:   	成功
	  -4:  	无对应目录
   	  -7:	超时（_ERR_NT_OUTTIME）
	  -16:	_ERR_NT_DATA
	  -100:	打开串口失败
	   其它：执行IC卡脚本出错时IC卡返回的错误码

例：userinfo
A0196217995510000011675B00511675C00201D018520112198212152540E0386217995510000011675D49122201230600000FG012000000015469H012000000100000I006301231J00201
*/
int WINAPI IC_GetInfo(int port, int baud, char *extport, int outtime, int icflag,char* aidlist,
					  char* taglist, char* userinfo, int *ictype);

/*
函数声明	int ReadCardInfo(int iPort, int iBaud, int iReadMode, char *pucIDMsg, int &iCurMode, double dbOutTime)
功能描述	读卡片信息
参数含义	iReadMode：输入参数，读模式：
						0x01: 磁条
						0x02: M1卡（暂不支持，保留）
						0x04: 接触式金融卡
						0x08: 非接触式金融卡
						0x10: 二代身份证
			pucIDMsg: 输出参数，返回卡片信息，由多种卡片信息组成，根据信息来源不同，组成形式如下：
						1、磁条数据格式：
							二磁道数据 + ‘A’ + 三磁道数据
						2、M1卡数据：
							16字节块数据
						3、接触式金融卡和非接触式金融卡：
							标签(0x41) + 三字节十进制长度（如：“100”表示长度为100）+ 帐号 + 标签(0x42) + 
										…… + 标签(0x4A) + 三字节十进制长度（如：“100”表示长度为100）+ 应用主帐号(PAN)系列号
							标签的定义如下：
								数据元				来自IC卡的数据	标签
								帐号				19位 ans格式	0x41A
								姓名				STRING		0x42B
								证件类型			HEX     	0x43C
												值:
												00：身份证
												01：军官证
												02：护照
												03：入境证      
												04：临时身份证
												05：其它	
								证件号码			Ans		0x44D
								二磁道信息（可选）	Ans			0x45E
								一磁道信息（可选）	Ans			0x46F
								余额				Ans,不带小数点	0x47G
								电子现金余额上限				0x48H
								应用失效日期					0x49I
								应用主帐号(PAN)系列号				0x4AJ
						4、二代身份证：
							标签(0x41) + 三字节十进制长度（如：“100”表示长度为100）+ 姓名 + 标签(0x42) + 
										…… + 标签(0x4A) + 三字节十进制长度（如：“100”表示长度为100）+ 新家庭住址
							标签的定义如下：
								数据元					标签
								姓名					0x41
								性别					0x42
								民族					0x43													
								出生日期				0x44
								家庭住址				0x45
								身份证号码				0x46
								发证机关				0x47
								有效期起始日期				0x48
								有效期结束日期				0x49
								新家庭住址				0x4A
			iCurMode: 输出参数，返回当前卡片信息来源:
						0x01: 磁条
						0x02: M1卡（暂不支持，保留）
						0x04: 接触式金融卡
						0x08: 非接触式金融卡
						0x10: 二代身份证
			dbOutTime: 输入参数，可选， 操作超时时间，单位为秒 缺省为48秒
返回值		0 成功　-1失败 >0错误号
*/
int __stdcall ReadCardInfo(int iPort, int iBaud, int iReadMode, char *pucIDMsg, int &iCurMode, double dbOutTime);
}
#endif