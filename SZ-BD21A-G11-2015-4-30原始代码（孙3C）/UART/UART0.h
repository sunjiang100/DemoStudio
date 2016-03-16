/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$  Copyright (c)  $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
$									
$									
$                       
$								  
$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$  Copyright (c)  $$$$$$$$$$$$$$$$$$$$$$$$$$$ */
/*  
                                      文件描述
	文件名: 
	功能描述：
	编译环境：
	目标CPU: 
	作者：
	创建时间：
	建议编辑环境：UltraEdit 11.00a+  制表符宽度为三个字符，程序可读性更好。
$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
*/					  
#ifndef		__UART0_H__
#define		__UART0_H__


#ifdef      UART0_GLOBALS
#define     UART0_EXT
#else
#define     UART0_EXT    extern
#endif


#include		"..\Config\RTOS.h"
#include			"..\common\Algol.h"


#define   	OWN_ADDRESS      1
#define   	LEAD_OK          0x55
#define   	LENGTH_OK        0xaa



#define		LEN_U0_GB_FRAME_HEAD			6

//#define		LEN_U0_GY_FRAME_HEAD			8

// $$$$$$$$$$$$$$$$$$ <<< Use Configuration Wizard in Context Menu >>> $$$$$$$$$$$$$$$$$
// 



// <h>UART0初始化相关配置(CPU主频在CPU.h文件中设置)


/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//   	<o>UART0_BPS: 串口波特率设置 <1-250000>
//   	<i> Default: 9600
*/
#define      UART0_BPS    		115200UL         
/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//   	<o>UART0_BPS_IAP: IAP时串口波特率设置 <1-250000>
//   	<i> Default: 115200
*/
#define      UART0_BPS_IAP		115200UL
/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//   	<o>U0_DATA_BIT: 数据位配置 <0=> 5位 <1=> 6位 <2=> 7位 <3=> 8位
//   	<i> Default: 3
*/
#define      U0_DATA_BIT    	3

//	<e>禁止或使能校验位

/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//   	<o>U0_CHECK_BIT_EN: 校验位使能 <0=> 禁止校验 <1=> 允许校验
//   	<i> Default: 1
*/
#define      U0_CHECK_BIT_EN  1		 // 注意：GB19056-2012 需要奇校验，所以一定使能，上位机需要设置奇校验才能通讯 2015.1.15


/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//   	<o>U0_CHECK_BIT: 校验位配置 <0=> 奇校验 <1=> 偶校验 <2=> 强制1 <3=> 强制0
//   	<i> Default: 0
*/
#define      U0_CHECK_BIT    	0

//	</e>

/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//   	<o>U0_STOP_BIT: 停止位配置 <0=> 1位 <1=> 2位 
//   	<i> Default: 0
*/
#define      U0_STOP_BIT    	0

/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//   	<o>U0_PROIRITY: 串口中断优先级 <0=> 不优先 <1=> 优先
//   	<i> Default: 1
*/
#define      U0_PROIRITY    	1


/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//   	<o>UART0_REC_LENGTH: 接收缓冲区长度字节设置 <1-1024>
//   	<i> Default: 100
*/
#define		UART0_REC_LENGTH	 	(1024+256)

/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//   	<o>UART0_TXD_LENGTH: 发送缓冲区长度字节设置 <1-1024>
//   	<i> Default: 100
*/
#define		UART0_TXD_LENGTH		512

/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//   	<o>LEN_TRANSPARENT: 设置透传缓冲长度 <1-1024>
//   	<i> Default: 128
*/
#define		LEN_TRANSPARENT		512


/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//   	<o>LEN_UART0_SEND_ALIG: 发送队列长度 <128-1024>
//   	<i> Default: 256
*/
#define		UART0_SEND_ALIG_LENGTH		(1024+256)

/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//   	<o>USE_QUERY_SEND: 使用查询发送函数 <0=> 不使用 <1=> 使用
//   	<i> Default: 0
*/
#define		USE_QUERY_SEND		0

// </h>



// $$$$$$$$$$$$$$$$$$$ <<< end of configuration section >>> $$$$$$$$$$$$$$$$$$$$$$$$$$$$

#define     IAP_File_MaxSize           1048576			 //1M字节大小

#define     IAP_HEAD_LEN		         16

#define     IAP_HEAD_FLAG              0x50414947     			 //GIAP

#define     IAP_Firmware_Bag_LEN		   1024  //镜像一包数据大小

#define     IAP_HAND_SHANE_STEP_1      0x01

#define     IAP_HAND_SHANE_STEP_2      0x02

#define     IAP_DOWN_LOAD              0x03

#define     IAP_Up_LOAD					   0x04


#define     IAP_Firmware1_Base_Block_Add       0				  //单位  64k	  块地址

#define     IAP_Firmware2_Base_Block_Add       8				  //单位  64K	   块地址

#define     IAP_Firmware1                 1				   //		镜像1

#define     IAP_Firmware2                 2				  // 		镜像2




UART0_EXT   uint32   U0_Iap_cnt,U0_IapOverTime;
UART0_EXT   uchar    U0_Iap_Flag;

#pragma		pack(1)
typedef		struct 
{
	uint32   Head;  			  //包头 GIAP
	uint32	SumCRC; 			  //包校验和
	uchar		Cmd;				  //命令
   uchar 	image; 			  //固件号1或2
   uint16 	BagNumber;		  //包序
   uint16	BagSum;			  //总包数
   uint16	DataLen;			  //数据长度
	//uchar  Model[8];        //产品型号
	//uint32	FirmWare;        //硬件版本
	//uint32	SofeVersion;     //软件版本
	//uint32	Time;            //编译时间 秒
}  IAPHAED;


UART0_EXT   enum
{
   eIAP_CMD_HEAD_SHAKE1      = 0x31,    //第一次握手
	eIAP_CMD_HEAD_SHAKE2      = 0x32,		//第二次握手
	eIAP_CMD_SET_FIRMWARE     = 0x33,		//下发镜像数据
	eIAP_CMD_GET_FIRMWARE     = 0x34,		//上传镜像数据
	eIAP_CMD_CRE_FIRMWARE     = 0x35,		//校验用户代码
	eIAP_CMD_GET_FLASH_DATA_N_PAGE = 0x36,	// 读取Flash数据
	eIAP_CMD_FINISH				 = 'E',			// END
}IAPCMD;



/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
    接收相关变量定义
*/
#pragma		pack(1)
typedef		struct 
{
	uint32	OverTimeCnt;
	uint16	Counter;
}  REC;


/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
   
*/
#pragma		pack(1)
typedef		struct 
{
   uchar   	CheckSum,Sending;   
   uint16  	Length;   
}  TXD;

/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
   
*/
#pragma		pack(1)
typedef		struct 
{  	
	uint16	Length;			// 包长，与地址相对应		
	uchar		Busy;				// 忙标志
}  SEND_ALIG,*pSEND_ALIG;

typedef  struct
{
		uint32   Btime;
		uint32   Etime;
		uint16   ULen;

}DownComPARA;

UART0_EXT   DownComPARA    DownComPara;


UART0_EXT	uchar		GB_CheckMode, GB_CheckMode_Type;						 
UART0_EXT	uchar		U0_SendAligBuffer[UART0_SEND_ALIG_LENGTH];//	__attribute__((at(0x7fe00000)));	// 1K
//UART0_EXT	uchar    U0_TxdBuffer[UART0_TXD_LENGTH];//  				__attribute__((at(0x7fe00400)));	// 512B
//UART0_EXT	uchar		U0_RecBuffer[UART0_REC_LENGTH];//				__attribute__((at(0x7fe00600)));	// 512B
		  

/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	UART类
*/
class			UartCls
{
public:
	REC			Rec;
	TXD			Txd;	
	SEND_ALIG	SendAlig;
public:
	UartCls(void);
	bool		Init(uint32);
	uchar		puts(char *p);
	uchar		putb(uchar,uint16,uchar *p);
	uint16	GetRecBuffer(uchar *pR);
	void		ReturnErrorFlag(uchar s);
	void		ClearReceiveFlag(uint16);
	void		ClearTransmitFlag(void);	
	//void		TransparentToGPRS(void);
	__inline		void		Interrupt(void);	
	uint16	GB19056Process(uint16 LenRec,uchar *pRecBuffer,uchar *pSendBuffer);
	void		GY19056Process(void);
	//void		GetFlashDataFun(void);
	void		ReceiveProcessData(void);
	uchar    PopAlignmentSend(void);
	uchar		GetSendBusyFlag(void);
	//void     GetDownComPara(void);			  /////////// add by wrc  ///////////////
	//uchar    SendFramDataToU0(uchar *DataBf, uint16 DataLen, uint32 DataLenFlash);		  /////////// add by wrc  ///////////////
	void     SetU0FramHead(uchar *DataBf, uint32 DataLen, uchar Cmd);												  /////////// add by wrc  ///////////////
	void     SetGB_Down_CMDHead(uchar *DataBf, uint32 DataLen, uchar Cmd);
	//void     EreadFirstBlock(void);																											/////////// add by wrc  ///////////////
	void     EreadFRAM_RecodeMem(uint16 B_Addr, uint16 Len);
	void     Para_Modi_Recode(uchar ParaCmd); 
	//uint32   SumFlashData(uint32 FlashAddr, uint32 FlashDataLen, uint16 FF_Paddr, uchar DataBlockLen, uchar TimePY, uchar *DBuffer);			  /////////// add by wrc  ///////////////
	//uint32   GetDataFromFRAM(uint16 FM_Addr, uchar DataBlokLen, uchar TimePY, uchar *DBuffer);
	//void     SendFlashData(uint32 FlashAddr, uint32 FlashDataLen, uint16 FF_Paddr, uchar DataBlockLen, uchar FM_RD_Times,uint32 DataLen, uchar TimePY, uchar *DBuffer);
	//void     USB_GetGBdata(void);
	uint32   SetHeadUSBBlockData(uchar DataType, uchar DataSierieNb, uint32 DataLen, uchar *DataBuffer);
	void		WaitUSBGetData(uchar DlyTime, uint16 DataLen);
	//void     SendDownCMDtoU0(uchar  *DataBuffer, uint32 DataLen);
	//void     CmdForGetGBdata(uchar  CmdType, uint16 DataBlocks);
	void     IAPprocess(void);
	//uchar    Iap_Save_Firmware(IAPHAED  *Iapdata);
	//uchar    Iap_UpLoad_Firmware(IAPHAED  *Iapdata);
	
	//uint16   GuoyeeProtocolProcess(uint16 LenRec,uchar *pRecBuffer,uchar *pSendBuffer);
	uint16	SendVDR_Data_08H_To_15H(uint16 ReturnBufferLen, uchar *pReturnBuffer);
private:
	uchar		*pTxd;
	
};

UART0_EXT	__noinit__		UartCls		Uart;		// 




/*
 $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$  End of File $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
 */
#endif

