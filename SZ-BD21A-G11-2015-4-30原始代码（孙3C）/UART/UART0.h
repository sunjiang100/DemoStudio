/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$  Copyright (c)  $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
$									
$									
$                       
$								  
$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$  Copyright (c)  $$$$$$$$$$$$$$$$$$$$$$$$$$$ */
/*  
                                      �ļ�����
	�ļ���: 
	����������
	���뻷����
	Ŀ��CPU: 
	���ߣ�
	����ʱ�䣺
	����༭������UltraEdit 11.00a+  �Ʊ�����Ϊ�����ַ�������ɶ��Ը��á�
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



// <h>UART0��ʼ���������(CPU��Ƶ��CPU.h�ļ�������)


/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//   	<o>UART0_BPS: ���ڲ��������� <1-250000>
//   	<i> Default: 9600
*/
#define      UART0_BPS    		115200UL         
/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//   	<o>UART0_BPS_IAP: IAPʱ���ڲ��������� <1-250000>
//   	<i> Default: 115200
*/
#define      UART0_BPS_IAP		115200UL
/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//   	<o>U0_DATA_BIT: ����λ���� <0=> 5λ <1=> 6λ <2=> 7λ <3=> 8λ
//   	<i> Default: 3
*/
#define      U0_DATA_BIT    	3

//	<e>��ֹ��ʹ��У��λ

/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//   	<o>U0_CHECK_BIT_EN: У��λʹ�� <0=> ��ֹУ�� <1=> ����У��
//   	<i> Default: 1
*/
#define      U0_CHECK_BIT_EN  1		 // ע�⣺GB19056-2012 ��Ҫ��У�飬����һ��ʹ�ܣ���λ����Ҫ������У�����ͨѶ 2015.1.15


/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//   	<o>U0_CHECK_BIT: У��λ���� <0=> ��У�� <1=> żУ�� <2=> ǿ��1 <3=> ǿ��0
//   	<i> Default: 0
*/
#define      U0_CHECK_BIT    	0

//	</e>

/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//   	<o>U0_STOP_BIT: ֹͣλ���� <0=> 1λ <1=> 2λ 
//   	<i> Default: 0
*/
#define      U0_STOP_BIT    	0

/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//   	<o>U0_PROIRITY: �����ж����ȼ� <0=> ������ <1=> ����
//   	<i> Default: 1
*/
#define      U0_PROIRITY    	1


/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//   	<o>UART0_REC_LENGTH: ���ջ����������ֽ����� <1-1024>
//   	<i> Default: 100
*/
#define		UART0_REC_LENGTH	 	(1024+256)

/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//   	<o>UART0_TXD_LENGTH: ���ͻ����������ֽ����� <1-1024>
//   	<i> Default: 100
*/
#define		UART0_TXD_LENGTH		512

/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//   	<o>LEN_TRANSPARENT: ����͸�����峤�� <1-1024>
//   	<i> Default: 128
*/
#define		LEN_TRANSPARENT		512


/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//   	<o>LEN_UART0_SEND_ALIG: ���Ͷ��г��� <128-1024>
//   	<i> Default: 256
*/
#define		UART0_SEND_ALIG_LENGTH		(1024+256)

/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//   	<o>USE_QUERY_SEND: ʹ�ò�ѯ���ͺ��� <0=> ��ʹ�� <1=> ʹ��
//   	<i> Default: 0
*/
#define		USE_QUERY_SEND		0

// </h>



// $$$$$$$$$$$$$$$$$$$ <<< end of configuration section >>> $$$$$$$$$$$$$$$$$$$$$$$$$$$$

#define     IAP_File_MaxSize           1048576			 //1M�ֽڴ�С

#define     IAP_HEAD_LEN		         16

#define     IAP_HEAD_FLAG              0x50414947     			 //GIAP

#define     IAP_Firmware_Bag_LEN		   1024  //����һ�����ݴ�С

#define     IAP_HAND_SHANE_STEP_1      0x01

#define     IAP_HAND_SHANE_STEP_2      0x02

#define     IAP_DOWN_LOAD              0x03

#define     IAP_Up_LOAD					   0x04


#define     IAP_Firmware1_Base_Block_Add       0				  //��λ  64k	  ���ַ

#define     IAP_Firmware2_Base_Block_Add       8				  //��λ  64K	   ���ַ

#define     IAP_Firmware1                 1				   //		����1

#define     IAP_Firmware2                 2				  // 		����2




UART0_EXT   uint32   U0_Iap_cnt,U0_IapOverTime;
UART0_EXT   uchar    U0_Iap_Flag;

#pragma		pack(1)
typedef		struct 
{
	uint32   Head;  			  //��ͷ GIAP
	uint32	SumCRC; 			  //��У���
	uchar		Cmd;				  //����
   uchar 	image; 			  //�̼���1��2
   uint16 	BagNumber;		  //����
   uint16	BagSum;			  //�ܰ���
   uint16	DataLen;			  //���ݳ���
	//uchar  Model[8];        //��Ʒ�ͺ�
	//uint32	FirmWare;        //Ӳ���汾
	//uint32	SofeVersion;     //����汾
	//uint32	Time;            //����ʱ�� ��
}  IAPHAED;


UART0_EXT   enum
{
   eIAP_CMD_HEAD_SHAKE1      = 0x31,    //��һ������
	eIAP_CMD_HEAD_SHAKE2      = 0x32,		//�ڶ�������
	eIAP_CMD_SET_FIRMWARE     = 0x33,		//�·���������
	eIAP_CMD_GET_FIRMWARE     = 0x34,		//�ϴ���������
	eIAP_CMD_CRE_FIRMWARE     = 0x35,		//У���û�����
	eIAP_CMD_GET_FLASH_DATA_N_PAGE = 0x36,	// ��ȡFlash����
	eIAP_CMD_FINISH				 = 'E',			// END
}IAPCMD;



/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
    ������ر�������
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
	uint16	Length;			// ���������ַ���Ӧ		
	uchar		Busy;				// æ��־
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
	UART��
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

