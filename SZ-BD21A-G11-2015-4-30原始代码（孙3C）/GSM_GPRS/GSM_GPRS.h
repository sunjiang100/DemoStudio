/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$  Copyright (c)  $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
$												 
$										 
$                       	 
$								  		All rights reserved.	  
$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$  Copyright (c)  $$$$$$$$$$$$$$$$$$$$$$$$$$$ */
/*  
                                      �ļ�����
	�ļ���: 	 GSM_GPRS.h
	����������GSM_GPRS������Ϣ���ͷ�ļ�
	���뻷����
	Ŀ��CPU: 
	���ߣ�
	����ʱ�䣺
	����༭������UltraEdit 11.00a+  �Ʊ�����Ϊ�����ַ�������ɶ��Ը��á�
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
// <h>GSM GPRS����ͷ�ļ��������


/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//   	<o>LOGIN_SERVER_INTERVAL: ��¼������ʱ����  <10-60>
//   	<i> Default: ʹ��
*/
#define			LOGIN_SERVER_INTERVAL		30

/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//   	<o>GSM_GPRS_EBB_SIGNAL: GSM���źŶ���  <1-10>
//   	<i> Default: ʹ��
*/
#define			GSM_GPRS_EBB_SIGNAL			8


/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//   	<o>GPRS_SEND_OVER_TIME: GPRS���ͳ�ʱʱ��  <5-300>
//   	<i> Default: 30
*/
#define			GPRS_SEND_OVER_TIME			180



/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//   	<o>SMS_ALARM_OVER_TIME: ���ű�����ʱʱ��  <20-60>
//   	<i> Default: 30
*/
#define			SMS_ALARM_OVER_TIME			30


/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//   	<o>LEN_ONE_FRAME: ��������������󳤶�  <20-1024>
//   	<i> Default: 256
*/
#define			LEN_ONE_FRAME				1024			// ��Ϊ�п��ܳ���256������




/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//   	<o>MOBILE_PROCCESS_TEST: ģ����غ������̲��� <0=> ����Ҫ <1=> ��Ҫ
//   	<i> Default: ��Ҫ
*/
#define			MOBILE_PROCCESS_TEST		0


/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//   	<o>USE_SMS_CHANNEL: �Ƿ�ʹ�ö���Ϣͨ��ģʽ <0=> ��ʹ�� <1=> ʹ��
//   	<i> Default: ʹ��
*/
#define			USE_SMS_CHANNEL			1


/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//   	<o>USE_SMS_CHANNEL: �Ƿ�ʹ��GPRSͨ��ģʽ <0=> ��ʹ�� <1=> ʹ��
//   	<i> Default: ʹ��
*/
#define			USE_GPRS_CHANNEL		1


/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//   	<o>USE_DTMF_CHANNEL: �Ƿ�ʹ������ͨ��Ϣģʽ <0=> ��ʹ�� <1=> ʹ��
//   	<i> Default: ʹ��
*/
#define			USE_DTMF_CHANNEL			1




/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//   	<o>GPRS_DATA_PACK_LEN: GPRS���Ͷ�����ÿ�����ݵ���󳤶� <16-1024>
//   	<i> Default: 64
*/
#define		LEN_GPRS_DATA_PACK			256

/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//   	<o>GPRS_ALIG_PACK_NUM: GPRS���Ͷ����������������� <1-64>
//   	<i> Default: 16
*/
#define		GPRS_ALIG_PACK_NUM			8



/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//   	<o>SAVE_HISTORY_DATA_EE_NF: ѡ��洢��ʷ���ݵĴ洢�� <0=> ��NandFlash <1=> ��EEPROM
//   	<i> Default: 1
*/
#define		SAVE_HISTORY_DATA_EE_NF			1



#define		GPRS_ALIG_MAX_LEN				(LEN_GPRS_DATA_PACK*GPRS_ALIG_PACK_NUM)

/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//   	<o>GPRS_MAX_SEND_LEN: GPRS���Ͷ�����󳤶��ֽ�  <512-1024>
//   	<i> Default: 600
*/
#define		GPRS_MAX_SEND_LEN				(1024)

/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//   	<o>EBB_SIGNAL_MAX_SEND_LEN: ���ź�ʱ��󳤶��ֽ�  <512-1024>
//   	<i> Default: 600
*/
#define		EBB_SIGNAL_MAX_SEND_LEN			(800)


#define			GPRS_WAIT_RET_STATUS		1


// Modem����
#define			M_RTS			10				// ���
#define			M_CTS			11				// ����
#define			M_DSR			12				// ����
#define			M_DTR			13				// ���
#define			M_DCD			14				// ����
#define			M_RI			15				// ����

								
									
									
/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//   	<o>NUM_OF_MAX_FRAME_SEG: һ֡��������������ݶ������Ŀ  <4-64>
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
{"����ͨ����ʱ��"},
{"�¶�ͨ����ʱ��"},
{"\0"},

};




/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	GPRS����״̬ö��
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
	GPRS�������ݵ�����ö��
*/
GSM_GPRS_EXT	enum
{
	NO_DATA = 0,
	GRPS_PERIOD_SEND,
	
} GPRS_SEND_DATA_CMD;

/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	GPRS�������ݰ�����ö��
*/
GSM_GPRS_EXT	enum
{
	GPRS_NOT_NKOW_DATA = 0,
	GPRS_NEW_DATA,
	GPRS_OLD_DATA,
	GPRS_ACK_DATA,	
} GPRS_DATA_PACK_TYPE;
						

/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	GPRS��������ö��
*/
GSM_GPRS_EXT	enum
{
	UDP_LINK = 0,
	TCP_LINK,
} GPRS_LINK_TYPE;


/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	PC����GPRS���ߺͶ���������
*/
GSM_GPRS_EXT	enum
{
	NO_CMD = 0,
	HANG,
	DIAL,
} GPRS_HD_CMD;


/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	PC����GPRS���ߺͶ���������
*/
GSM_GPRS_EXT	enum
{
	GPRS_SEND_IDLE = 0,
	GPRS_SEND_CMD = 1,
	GPRS_SEND_DATA = 2,
} GPRS_SEND_STATUS;

/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	GSM_GPRS������״̬��
*/
GSM_GPRS_EXT	enum
{
	stGSM_GPRS_IDLE = 0,
	stMOBILE_COOL_BOOT_INIT,	
	stGPRS_IDLE,
	stGPRS_SEND_DATA,	
	
} GMS_GPRS_STATUS;





/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	GSM/GPRS���״̬���� 
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
			uchar		SimOK:1;							// SIM������			
			uchar		SignalOK:1;		  				// �ź��㹻ǿ
			uchar		ChinaMobile:1;					// 
			uchar		ChinaUnion:1;		   		// 
			uchar		ChinaTelcom:1;	   			//
			uchar		RegGSM:1;						// ��ע��GSM network 
			uchar		Roaming:1;						// ����
			uchar		SeviceNet:1;					// 
			uchar		AttachGPRS:1;					// 
			uchar		DomainName:1;					// 
			uchar		LocalIP:1;						// 
			uchar		FirstDial:1;					// 			
			uchar		Online:1;						// 
			uchar		RegServer:1;					// �����������ע��
			uchar		FirstReg:1;						// 
			uchar		DeAttachGPRS:1;				//
			uchar		DomainNameM:1;				   // ��������DNS����
			uchar		DomainNameB:1;				   //	���ݷ�����DNS����
			uchar		DomainNameC:1;				   //	��ʱ���ӻ�����������DNS����
			uchar		SetCASA:1;						// �����ƶ���˾�������Ķ��ź���
			uchar		Updating:1;		   	   	// 
			uchar		UpdateOnLine:1;				// 
			uchar		UpdateRemoteSet:1;							// 
			uchar		ReadCCID:1;							// 	SIM������ 
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
	uchar		Signal;						// �ź�ǿ�� 0x00~0x62 (0~99)
	uchar		FrameDataBusing;


	uint16	SIM_Status;					// SIM���Ӵ����ñ�־
	uint16	FindCM;						// �ҵ�Ӫ���̱�־

	uchar		WaitRet;
	uchar		PowerOn;

	uchar		NoSimCard;	
	uchar		Registration;				// ����ע���ʶ 0: ������, 1: �й��ƶ� 2: �й���ͨ
	uchar		ServerReg;					// ������ע���ʶ
	uchar		OnLine;						// GPRS�Ƿ����߱�־��
	uchar		InitOK;		
	uchar		LinkStatus;					// ����״̬
	uchar		LinkType;					// ��������(TCP/UDP)
	uchar		Calling;						// ����ͨ���л򲦺���
	uchar		CheckMobileRetStatus;	// ���ģ�鷵��״̬
	uchar		GPRS_UrgencySend;			// �������ͱ�־		
	uchar		ServerRegistration;		// 
	uchar		GPRS_SendEnable;			// ��������
	uchar		GPRS_SendStatus;			// ����״̬������	
	uchar		MobileWorking;				// ģ�鹤�����з�Ӧ
	uchar		DialOK_ShakePack;			// ���ųɹ�����һ���������ݰ�
	uint16	DialInterval;
	uint16	ConnectFailCnt;	
	uchar		DialFailCnt[12];
	uchar    ATCMDtimes;
	uchar    RSV2;
} GG_Status;






/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	GSM/GPRS��ر�־λ
*/
GSM_GPRS_EXT	struct
{
	struct
	{			
			uchar		CoolBoot;						// ������
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
			uchar		Dial;							// ���ű�־
			uchar		Dialing;						//
			uchar		ReDial;		  				// �ز��ű�־
			uchar		SendData;						// �������ݱ�־						 
			uchar		AckGSMS;		   				//����Ӧ�� 
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
} GG_Evt;  	// GSM GPRS �¼���־






		

/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
   GPRS���Ͷ��У�LIFO��ʽ
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
   GPRS���Ͷ��У�LIFO��ʽ������
*/
#define		LEN_PRIORITY_POP_BUFFER			80
typedef		struct	 _GPRS_SEND_ALIG
{
	uint16		Length;			// ���������ַ���Ӧ	
	uint16		Busy;				// æ��־	
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

// ������������
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
	uint16	OverTime;		// ��ǰ��ʱ���������ȴ�Ϊ0ʱ�ſɷ���
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
	uchar  Enable;	  //ʹ��
	uchar  LinkTimes; //���Ӵ���
	uchar  Frame;	  //����1��2
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
   GSM��  
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
	uint32	CallingTimeCnt;					  //ÿ��ͨ��ʱ��
   uint32	CallTimeMonth;					  //���ۼ�ͨ��ʱ��
	uint32	OneMonthSec;					  //���ۼ�ͨ��ʱ��
	
	//uchar		HeartbeatPack[LEN_HR_PACK];
	//uchar		RegisterPack[LEN_HR_PACK];
	//uint32	HeartbeatTime;	// ��

	

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
   GPRS��  
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
	////////////////////////////////////////////////////////	*GY��ͷ	
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
	//void			TranPicDataPack(uchar no,uchar block,uint16 len,uchar *dat);		// GPRS��������ͷ����		
	
	
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
	uchar					PSC[LEN_PSC];							// ֡�����У�ÿ�����ݵĿ�ʼλ�ü�����
	//uchar					PLC_Control;
	uchar					UploadCmdBusing;
	//uint16 	         gMsgIndex; 		  // ��ˮ��
	//uint16            gMsgId;			  // ��Ϣ�ţ�����ţ�
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
























