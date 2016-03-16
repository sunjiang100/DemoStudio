/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$  Copyright (c)  $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
$												 
$										 
$                       	 
$								  		All rights reserved.	  
$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$  Copyright (c)  $$$$$$$$$$$$$$$$$$$$$$$$$$$ */
/*  
                                      文件描述
	文件名: 	 GSM_GPRS.h
	功能描述：GSM_GPRS公共信息相关头文件
	编译环境：
	目标CPU: 
	作者：
	创建时间：
	建议编辑环境：UltraEdit 11.00a+  制表符宽度为三个字符，程序可读性更好。
$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
*/					  

#ifndef		__GSM_GPRS_H__
#define		__GSM_GPRS_H__

#ifdef      GSM_GPRS_GLOBALS
#define     GSM_GPRS_EXT
#else
#define     GSM_GPRS_EXT    extern
#endif

#include		<stdio.h>
#include		<string.h>

#include		"..\UART\UART1.h"
#include		"..\UART\UART0.h"
#include		"..\KeyDisp\KeyDisp.h"

#include		"..\camera\camera.h"

#include			"..\SMS\SMS.h"


#define			__GPRS_DEBUG__						1


#define		TEST_CODE_JUST_FOR_TEST_LUOYANG			0
#define		TEST_CODE_JUST_FOR_TEST_LUOYANG_NEW		0


// $$$$$$$$$$$$$$$$$$ <<< Use Configuration Wizard in Context Menu >>> $$$$$$$$$$$$$$$$$
// <h>GSM GPRS公共头文件相关配置


/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//   	<o>LOGIN_SERVER_INTERVAL: 登录服务器时间间隔  <10-60>
//   	<i> Default: 使用
*/
#define			LOGIN_SERVER_INTERVAL		30

/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//   	<o>GSM_GPRS_EBB_SIGNAL: GSM弱信号定义  <1-10>
//   	<i> Default: 使用
*/
#define			GSM_GPRS_EBB_SIGNAL			8


/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//   	<o>GPRS_SEND_OVER_TIME: GPRS发送超时时间  <5-300>
//   	<i> Default: 30
*/
#define			GPRS_SEND_OVER_TIME			180



/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//   	<o>SMS_ALARM_OVER_TIME: 短信报警超时时间  <20-60>
//   	<i> Default: 30
*/
#define			SMS_ALARM_OVER_TIME			30


/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//   	<o>LEN_ONE_FRAME: 单帖命令数据最大长度  <20-1024>
//   	<i> Default: 256
*/
#define			LEN_ONE_FRAME				1024			// 因为有可能出现256个汉字




/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//   	<o>MOBILE_PROCCESS_TEST: 模块相关函数过程测试 <0=> 不需要 <1=> 需要
//   	<i> Default: 需要
*/
#define			MOBILE_PROCCESS_TEST		0


/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//   	<o>USE_SMS_CHANNEL: 是否使用短消息通道模式 <0=> 不使用 <1=> 使用
//   	<i> Default: 使用
*/
#define			USE_SMS_CHANNEL			1


/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//   	<o>USE_SMS_CHANNEL: 是否使用GPRS通道模式 <0=> 不使用 <1=> 使用
//   	<i> Default: 使用
*/
#define			USE_GPRS_CHANNEL		1


/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//   	<o>USE_DTMF_CHANNEL: 是否使用语音通道息模式 <0=> 不使用 <1=> 使用
//   	<i> Default: 使用
*/
#define			USE_DTMF_CHANNEL			1




/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//   	<o>GPRS_DATA_PACK_LEN: GPRS发送队列中每条数据的最大长度 <16-1024>
//   	<i> Default: 64
*/
#define		LEN_GPRS_DATA_PACK			256

/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//   	<o>GPRS_ALIG_PACK_NUM: GPRS发送队列中允许最多包数量 <1-64>
//   	<i> Default: 16
*/
#define		GPRS_ALIG_PACK_NUM			8



/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//   	<o>SAVE_HISTORY_DATA_EE_NF: 选择存储历史数据的存储器 <0=> 用NandFlash <1=> 用EEPROM
//   	<i> Default: 1
*/
#define		SAVE_HISTORY_DATA_EE_NF			1



#define		GPRS_ALIG_MAX_LEN				(LEN_GPRS_DATA_PACK*GPRS_ALIG_PACK_NUM)

/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//   	<o>GPRS_MAX_SEND_LEN: GPRS发送队列最大长度字节  <512-1024>
//   	<i> Default: 600
*/
#define		GPRS_MAX_SEND_LEN				(1024)

/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//   	<o>EBB_SIGNAL_MAX_SEND_LEN: 弱信号时最大长度字节  <512-1024>
//   	<i> Default: 600
*/
#define		EBB_SIGNAL_MAX_SEND_LEN			(800)


#define			GPRS_WAIT_RET_STATUS		1


// Modem引脚
#define			M_RTS			10				// 输出
#define			M_CTS			11				// 输入
#define			M_DSR			12				// 输入
#define			M_DTR			13				// 输出
#define			M_DCD			14				// 输入
#define			M_RI			15				// 输入

								
									
									
/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//   	<o>NUM_OF_MAX_FRAME_SEG: 一帧数据里包含的数据段最大数目  <4-64>
//   	<i> Default: 10
*/
#define		NUM_OF_MAX_FRAME_SEG			64
									
									
#define		LEN_PSC							(NUM_OF_MAX_FRAME_SEG<<1)



#define		LEN_ACK_BUFFER					(256 + 16)
									
#define     ATResendTimes				   30

// </h>

// $$$$$$$$$$$$$$$$$$$ <<< end of configuration section >>> $$$$$$$$$$$$$$$$$$$$$$$$$$$$

uchar		const		CallOverTimeT[][17] = 
{
{"本次通话超时！"},
{"月度通话超时！"},
{"\0"},

};




/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	GPRS链接状态枚举
*/
GSM_GPRS_EXT	enum
{
	IP_INITIAL = 0,
	IP_START,
	IP_CONFIG,
	IP_IND,
	IP_GPRSACT,
	IP_STATUS,
	IP_CONNECTING,
	IP_CLOSE,
	CONNECT_OK,
	NOT_KNOW_STATUS = 0xf0,
	SW_TCP_UDP_CNT = 0xfa,
} GPRS_LINK_STATUS;


/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	GPRS发送数据的命令枚举
*/
GSM_GPRS_EXT	enum
{
	NO_DATA = 0,
	GRPS_PERIOD_SEND,
	
} GPRS_SEND_DATA_CMD;

/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	GPRS发送数据包类型枚举
*/
GSM_GPRS_EXT	enum
{
	GPRS_NOT_NKOW_DATA = 0,
	GPRS_NEW_DATA,
	GPRS_OLD_DATA,
	GPRS_ACK_DATA,	
} GPRS_DATA_PACK_TYPE;
						

/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	GPRS链接类型枚举
*/
GSM_GPRS_EXT	enum
{
	UDP_LINK = 0,
	TCP_LINK,
} GPRS_LINK_TYPE;


/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	PC控制GPRS上线和断线命令字
*/
GSM_GPRS_EXT	enum
{
	NO_CMD = 0,
	HANG,
	DIAL,
} GPRS_HD_CMD;


/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	PC控制GPRS上线和断线命令字
*/
GSM_GPRS_EXT	enum
{
	GPRS_SEND_IDLE = 0,
	GPRS_SEND_CMD = 1,
	GPRS_SEND_DATA = 2,
} GPRS_SEND_STATUS;

/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	GSM_GPRS主任务状态机
*/
GSM_GPRS_EXT	enum
{
	stGSM_GPRS_IDLE = 0,
	stMOBILE_COOL_BOOT_INIT,	
	stGPRS_IDLE,
	stGPRS_SEND_DATA,	
	
} GMS_GPRS_STATUS;





/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	GSM/GPRS相关状态定义 
*/
#define		GG_FLAG_INIT_DATA			0x00008000
#pragma		pack(1)

GSM_GPRS_EXT		struct
{
	union
	{			
		uint32		Word;
		struct		
		{
			uchar		SimOK:1;							// SIM卡正常			
			uchar		SignalOK:1;		  				// 信号足够强
			uchar		ChinaMobile:1;					// 
			uchar		ChinaUnion:1;		   		// 
			uchar		ChinaTelcom:1;	   			//
			uchar		RegGSM:1;						// 找注册GSM network 
			uchar		Roaming:1;						// 漫游
			uchar		SeviceNet:1;					// 
			uchar		AttachGPRS:1;					// 
			uchar		DomainName:1;					// 
			uchar		LocalIP:1;						// 
			uchar		FirstDial:1;					// 			
			uchar		Online:1;						// 
			uchar		RegServer:1;					// 向服务器请求注册
			uchar		FirstReg:1;						// 
			uchar		DeAttachGPRS:1;				//
			uchar		DomainNameM:1;				   // 主服务器DNS解析
			uchar		DomainNameB:1;				   //	备份服务器DNS解析
			uchar		DomainNameC:1;				   //	临时连接或升级服务器DNS解析
			uchar		SetCASA:1;						// 设置移动公司服务中心短信号码
			uchar		Updating:1;		   	   	// 
			uchar		UpdateOnLine:1;				// 
			uchar		UpdateRemoteSet:1;							// 
			uchar		ReadCCID:1;							// 	SIM卡串号 
			uchar		Rsv24:1;							// 
			uchar		Rsv25:1;							// 
			uchar		Rsv26:1;							// 
			uchar		Rsv27:1;							// 
			uchar		Rsv28:1;							// 
			uchar		Rsv29:1;							// 
			uchar		Rsv30:1;							// 
			uchar		Rsv31:1;							// 
		} Bits;
	} Flag;

	uint32	InitStatus;
	uint32	SendTotalBytes;
	uint32	AckTotalBytes;
	uchar		NoSimCardCnt;
	uchar		DialErCnt;
	uchar		SendDataErCnt;				// 
	uchar		Signal;						// 信号强弱 0x00~0x62 (0~99)
	uchar		FrameDataBusing;


	uint16	SIM_Status;					// SIM卡接触良好标志
	uint16	FindCM;						// 找到营运商标志

	uchar		WaitRet;
	uchar		PowerOn;

	uchar		NoSimCard;	
	uchar		Registration;				// 网络注册标识 0: 无网络, 1: 中国移动 2: 中国联通
	uchar		ServerReg;					// 服务器注册标识
	uchar		OnLine;						// GPRS是否在线标志，
	uchar		InitOK;		
	uchar		LinkStatus;					// 链接状态
	uchar		LinkType;					// 链接类型(TCP/UDP)
	uchar		Calling;						// 正在通话中或拨号中
	uchar		CheckMobileRetStatus;	// 检测模块返回状态
	uchar		GPRS_UrgencySend;			// 紧急发送标志		
	uchar		ServerRegistration;		// 
	uchar		GPRS_SendEnable;			// 发送允许
	uchar		GPRS_SendStatus;			// 发送状态，处于	
	uchar		MobileWorking;				// 模块工作，有反应
	uchar		DialOK_ShakePack;			// 拨号成功，发一个握手数据包
	uint16	DialInterval;
	uint16	ConnectFailCnt;	
	uchar		DialFailCnt[12];
	uchar    ATCMDtimes;
	uchar    RSV2;
} GG_Status;






/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	GSM/GPRS相关标志位
*/
GSM_GPRS_EXT	struct
{
	struct
	{			
			uchar		CoolBoot;						// 冷启动
			uchar		Rsv1;							//
			uchar		Rsv2;		  					// 
			uchar		Rsv3;							// 
			uchar		Rsv4;		   				// 
			uchar		Rsv5;	   					// 
			uchar		Rsv6;							// 
			uchar		Rsv7;							// 
			uchar		Rsv8;							// 
			uchar		Rsv9;							// 
			uchar		Rsv10;							// 
			uchar		Rsv11;							// 
			uchar		Rsv12;							// 
			uchar		Rsv13;							// 
			uchar		Rsv14;							// 
			uchar		Rsv15;							// 
	} GSM;
	struct
	{			
			uchar		Dial;							// 拨号标志
			uchar		Dialing;						//
			uchar		ReDial;		  				// 重拨号标志
			uchar		SendData;						// 发送数据标志						 
			uchar		AckGSMS;		   				//提问应答 
			uchar		Rsv5;	   					// 
			uchar		Rsv6;							// 
			uchar		Rsv7;							// 
			uchar		Rsv8;							// 
			uchar		Rsv9;							// 
			uchar		Rsv10;							// 
			uchar		Rsv11;							// 
			uchar		Rsv12;							// 
			uchar		Rsv13;							// 
			uchar		Rsv14;							// 
			uchar		Rsv15;							// 
	} GPRS;	
} GG_Evt;  	// GSM GPRS 事件标志






		

/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
   GPRS发送队列，LIFO形式
*/


#pragma		pack(1)

//GSM_GPRS_EXT		uchar			SmsAlarm;
GSM_GPRS_EXT		uint32		SwitchDialTCP,SwitchDialUDP;	//SmsAlarmOverTimeCnt
GSM_GPRS_EXT		uchar			JT808_PackSaveFlag;



//GSM_GPRS_EXT	uchar		GPRS_SendAligBuffer[GPRS_ALIG_MAX_LEN];		// 4K
GSM_GPRS_EXT   uchar		TransparentBuffer[UART1_REC_BUF_LENGTH];		// 1K
GSM_GPRS_EXT   uchar		OneFrame[LEN_ONE_FRAME];	 						// 1024






#pragma		pack(1)
typedef	struct
{	
	uint16	Len;
	uchar		Buffer[128];
} GPRS_PACK_BAK,*pGPRS_PACK_BAK;

/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
   GPRS发送队列，LIFO形式，链形
*/
#define		LEN_PRIORITY_POP_BUFFER			80
typedef		struct	 _GPRS_SEND_ALIG
{
	uint16		Length;			// 包长，与地址相对应	
	uint16		Busy;				// 忙标志	
	uchar			Buffer[GPRS_ALIG_MAX_LEN];
	uint16		gPriorityLen;
	uint16		gPriorityID;
	uchar			gPriorityBuffer[LEN_PRIORITY_POP_BUFFER];
} GPRS_SEND_ALIG,*pGPRS_SEND_ALIG;



GSM_GPRS_EXT	struct
{
	uint16	Length;
	uint16	MsgId;
	uint16	Index;
	uchar		Ack;
	uchar		GPS;
	uchar		Error;
	char		Data[LEN_ACK_BUFFER];
} AckBuffer;

// 出队数据类型
GSM_GPRS_EXT	enum
{
	POP_DATA_TYPE_NULL		= 0,
	POP_DATA_TYPE_RAM			,
	POP_DATA_TYPE_HISTORY	,
	POP_DATA_TYPE_BLIND		,
	POP_DATA_TYPE_ALARM		,
	POP_DATA_TYPE_NORMAL		,
	POP_DATA_TYPE_GNSS		,
	POP_DATA_TYPE_CAN_BUS	,
	POP_DATA_TYPE_48H			,
	POP_DATA_TYPE_360H		,
	POP_DATA_TYPE_PARK_DATA	,
	POP_DATA_TYPE_OVTD		,
	POP_DATA_TYPE_LOG_IN_OUT,
	POP_DATA_TYPE_PWR			,
	POP_DATA_TYPE_PARA_MDF	,
	POP_DATA_TYPE_SPEED_S	,	
	POP_DATA_TYPE_GET_08H_15H_DATA,
	
} EMUN_POP_DATA_TYPE;


GSM_GPRS_EXT struct  
{
	uint16	OverTime;		// 当前超时计数器，等待为0时才可发送
	uint16	PreOverTime;	// 
	uint16	Times;
	uchar		DataType;
} ReSend;
// end


#define  UpdatePara_Len   168 
#pragma  pack(1)
typedef struct
{
	uchar  RemoteAddr[32];
	uchar  RemoteIP[16];
	uchar  APN[16];
	uchar  RemotePort[6];
	char   FTPname[16];
	char   FTPpassword[16];
	char   FileName[32];
	char   FileList[32];
	uchar  Enable;	  //使能
	uchar  LinkTimes; //连接次数
	uchar  Frame;	  //镜像1或2
}Update;

GSM_GPRS_EXT  	Update  	 UpdatePara;
//GSM_GPRS_EXT	GPRS_PACK_BAK	LastGprsPack,GpsGprsPack,AlarmPack;

GSM_GPRS_EXT	uint16	LastPackLen;
GSM_GPRS_EXT	uchar		LastPackBuffer[156], CallInNumber[17];
GSM_GPRS_EXT 	uint32	NetConnectCnt;

GSM_GPRS_EXT	uchar		GetVDR_Cmd,GetVDR_DataFlag,VDR_CmdTab[64];
GSM_GPRS_EXT	uint16	GetVDR_Index[64];

//#define		LEN_SMS_PDU_STRING			512
#define		LEN_HR_PACK		(64+2)

/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
   GSM类  
*/
class			GSMCls
{
public:
	GSMCls(void);
	void		eTaskSend(void);
	void		eTaskReceive(void);
	uchar		Init(void);
	void		CoolBoot(void);
	//uchar		SendCommandWaitRetStatus(char *pComm,char *pStatus);
	
	//void		SMS_SetAckData(uchar *pNum, uchar *pData);
	//void		ChangeSMS_SendFlag(void);
	//void		eDialSendAT();
	void		PhoneCallStatus(void);
	uchar		DialPhoneFun(uchar len,char *pNum);
	//void		ClearPhoneFlag();
	uchar		AT_CommandWaitRetStatus(uchar Cmd,uchar *pData,uchar *pRet);
	
	//void		ReceiveErrorProccess();	
	//void		ReceiveRingProccesss();
	
	uchar		GPRS_Connect(uchar Type,uchar *pIP, uchar Channel);
	
	//void		CheckSystemStatus();
	//uchar		GetNetSignal();
	//void		GetGsmSave(uchar *p);
	uchar   	SetSMS_Buffer(char *number,char *msg);
	uchar    SendShortMessage(char *number,char *msg);
	
	//void		AirUpDateProcess(void);
	void		SecondCounter(void);

	void		AutoPeriodSend(void);
	uchar		WaitGSM_Free(void);
	uint32	GetGG_FlagBits(void);
	void     DisposeCalling(uchar *tBuffer);
	void     CalculateSendInterval(uint32);
	void		SetDefaultSendInterval(uint32);
	void		GetModelIMEIForPrint(uchar Type);
	void		JT808_SendPeriodFotJT808Check(void);
	void		ClearPhoneCallFlag(void);
	uchar		TTS_Opreation(uint16 Len, uchar *pData);
private:		
	//uchar		CheckSignalValue(uchar);
	///////////////////////////////////////// 
	uchar		IAP_Get_Firmware_File(void);
	void	   IAP_UpDate(void);
	

public:
	uchar		PowerOn;						 //
	//uchar		ReadyHelpDialFlag,HelpDialFlag;
	uint32	CallingTimeCnt;					  //每次通话时长
   uint32	CallTimeMonth;					  //月累计通话时长
	uint32	OneMonthSec;					  //月累计通话时长
	
	//uchar		HeartbeatPack[LEN_HR_PACK];
	//uchar		RegisterPack[LEN_HR_PACK];
	//uint32	HeartbeatTime;	// 秒

	

private:	
	uint16	CoolStartCnt;
	uchar    CntGSMInit;
	uchar		DomainErCnt;	
	//uint32	LastLoginServerTime;	
 //char		SMS_PDU_String[LEN_SMS_PDU_STRING];	
	//char		NowSendSmsNumber[20];
	//uchar		DialNumber[20];	
	//uchar		NoResponseCnt;
	uchar    UpDateError;
	
};
GSM_GPRS_EXT		GSMCls		GSM;


/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
   GPRS类  
*/
class			GPRSCls
{
public:
	GPRSCls(void);
	void			InitAlig(uchar);
	void			Close(void);
	uchar			Dial(uchar);
	void			ReDialFlag(void);
	uchar	 		GetRegInfo(uchar *pData);
	
	uint16		PopAlignmentSend(uchar);
	uchar			PushAlignment(uint16 AckID,uint16 MsgID,uint16 Len,uchar *pData);
	uchar			PushAlignmentToFlash(uchar Len);
	void			JT808_DataPackPushAlignment(uint16 AckID,uint16 Cmd,uint16 Len,uint32 Pack,uchar *pData);
	void			ProcessFrameData(uchar Ch,uint16 CmdLen,uchar *pData);	
	uint16		GetAligLen(void);
	void			ClearAligBuffer();	
	
	//void			NewPictureReport();

	void			UnRegTerminal(void);

	uchar			SendData(uchar Ch,uint16 DataLen,uchar *pData);
	
	uchar       BBJT808_GPRS_CmdExe(uint16 Length);
	void			BBJT808_GPRS_UploadCmd(uint16 Cmd,uint16 SrcLen,uchar *pSrcData);

	uint16		RemoteGetVDR_Data_08H_15H(uchar *pBuffer);

	

public:
	//void		ClearFM_Data(void);
	//void		AckPackProcess(void);
	////////////////////////////////////////////////////////	*GY帖头	
	void			TH_NewDataPack(uchar *pCmd,uchar *pGPS_VS);
	void			GB_NewDataPack(uchar Cmd,uchar Status,uint16 Len,uchar *pData);
	
	void			RemoteGetVDR_Data(void);	
	
	void			LocationQuery			   (uchar *pPS);
	void			GetSystemLog				(uchar *pPS);
	void			Set3IP						(uchar *pPS);
	void			SystemCoolBoot			   (uchar *pPS);
	void			SetCenterNumber			(uchar *pPS);
	void			SetReturnTime			   (uchar *pPS);
	void			RemoteOilControl		   (uchar *pPS);
	void			SetIP_Port				   (uchar *pPS);
	void			RemoteUpgrade			   (uchar *pPS);
	void			ReadIP_PortAndInterval  (uchar *pPS);
	void			ReadSMS_Number			   (uchar *pPS);
	void			Calling_GPS				   (uchar *pPS);
	void			RestoreFactorySetting	(uchar *pPS);
	void			SetOrGetAccelerometerPara(uchar *pPS);
	void			RemoteMonitor				(uchar *pPS);
	void			PhoneSchedulFunction		(uchar *pPS);
	void			SetOrGetTermialPhoneNumber(uchar *pPS);
	void			SetAPN						(uchar *pPS);
	void			SetRegInfo					(uchar *pPS);
	void			SetNormalPara				(uchar *pPS);	
	///////////////////////////////////////////////
	void			DialHelpPhone();
	//void			SendGpsHistoryMsg();
	//void			RemoteUpgradeProcess();
	//void			MakePicHead(uchar no,uchar block,uchar *dat); 						
	//void			TranPicDataPack(uchar no,uchar block,uint16 len,uchar *dat);		// GPRS传输拍摄头数据		
	
	
	uchar 		get_position_info(uchar Last,uchar *pos_data, uchar AckLen);
	void 	      save_or_send_position_info(uchar  Isave, uchar type);	

	uchar		   GetCommaPositionAndDataLen(uchar EndChar,uchar	*pChar,uchar *pComma);

	uchar			IAP_Get_Firmware_File(uint32 FileLen);
	void			GetDataFromServer(void);
	uint16		JT808_Packaging(uint16 Cmd,uint16 SrcLen,uint32 Pack,uchar *pSrcData,uchar *pDstData);
public:
   uchar             JT_Set_Get_Para(uchar *p, uchar s, uint32 j);
	uchar		         OEM[16];
	uint32				UpDataCount;
	//uint32				LongTimeNoSend;
	//uint16				AllPackIndex;	
	//uchar				ClearAlarmFlag;
	//uchar					UdpSendIdleFlag;
	char					CtrlSendCmdTime[16];
	GPRS_SEND_ALIG		SendAlig;
	
	//	end
private:			
   //char					SeqNumber[10];
	uchar					PSC[LEN_PSC];							// 帧数据中，每段数据的开始位置及长度
	//uchar					PLC_Control;
	uchar					UploadCmdBusing;
	//uint16 	         gMsgIndex; 		  // 流水号
	//uint16            gMsgId;			  // 消息号（命令号）
	//uint16				NeedRemoteAck(void);
	uchar 				Set_Or_Get_Accel_Sub_Para(uchar Cmd, uchar sOrg);
	uchar					SetOrGetAccelSub(uchar (*pFunVoid)(void), uchar (*pFunInt)(int), uchar *pos, 
													uchar Type, uchar Cmd, uchar Index, uchar Len, uint16 SaveAddr);
	//uchar 				SetOrGetAccelSubUchar(uchar (*pFun)(int va),uchar *pos, uchar Type, uchar Cmd, uchar Index, uchar Len);
	
};

GSM_GPRS_EXT	__noinit__		GPRSCls		GPRS;		//


#endif

/*
 $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$  End of File $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
 */
























