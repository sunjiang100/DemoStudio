/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$  Copyright (c)  $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
$												 
$										 
$                       	 
$								  		All rights reserved.	  
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

#ifndef   	__CAN_H__
#define   	__CAN_H__

#include    "..\Config\UserConfig.h"


#ifdef      CAN_GLOBALS
#define     CAN_EXT
#else
#define     CAN_EXT    extern
#endif


#define			CAN_BPS			(250000L)


#define STANDARD_FORMAT  0
#define EXTENDED_FORMAT  1

#define DATA_FRAME       0
#define REMOTE_FRAME     1


CAN_EXT	 enum   
{ CAN_OK = 0,                       /* No error                              */
  CAN_NOT_IMPLEMENTED_ERROR,        /* Function has not been implemented     */
  CAN_MEM_POOL_INIT_ERROR,          /* Memory pool initialization error      */
  CAN_BAUDRATE_ERROR,               /* Baudrate was not set                  */
  CAN_TX_BUSY_ERROR,                /* Transmitting hardware busy            */
  CAN_OBJECTS_FULL_ERROR,           /* No more rx or tx objects available    */
  CAN_ALLOC_MEM_ERROR,              /* Unable to allocate memory from pool   */
  CAN_DEALLOC_MEM_ERROR,            /* Unable to deallocate memory           */
  CAN_TIMEOUT_ERROR,                /* Timeout expired                       */
  CAN_UNEXIST_CTRL_ERROR,           /* Controller does not exist             */
  CAN_UNEXIST_CH_ERROR,             /* Channel does not exist                */
} CAN_ERROR;


#define		LEN_CAN_MSG_STRU			15
typedef struct  
{
  unsigned int   id;                    /* 29 bit identifier */
  unsigned char  data[8];               /* Data field */
  unsigned char  len;                   /* Length of data field in bytes */
  unsigned char  format;                /* 0 - STANDARD, 1- EXTENDED IDENTIFIER */
  unsigned char  type;                  /* 0 - DATA FRAME, 1 - REMOTE FRAME */  
} CAN_msg;



#define			CAN_DATA_NUMBER_PER_PAGE			21
#define			LEN_ONE_CAN_DATA 						12



/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	RTC类
*/
class			CanBusCls
{
	//////////////////////////////////////////	

public:
	CanBusCls(void);
	__inline		void			Interrupt(void);
	void CAN_setup         (uint32_t ctrl,uint32_t  Bps);
	void CAN_start         (uint32_t ctrl);
	void CAN_waitReady     (uint32_t ctrl);
	void CAN_wrMsg         (uint32_t ctrl, CAN_msg *msg);
	void CAN_rdMsg         (uint32_t ctrl, CAN_msg *msg);
	void CAN_wrFilter      (uint32_t ctrl, uint32_t id, uint8_t filter_type);
	void 	CAN_cfgBaudrate (uint32_t ctrl, uint32_t baudrate);



	bool		Init(uchar ch,uint32 Baudrate);	
	void		eTaskReceive(void);
	void		eTaskSend(void);
	uint32	GetSpecifyCanData(uchar *pData);
	uint32	SetGetSpecifyCanID(uchar Type,uchar Number,uint32 *pID);
	
	uint32	ReadTemporaryCanData(uchar Number,uint32 ID,uchar *pData);	
	uchar		JTSetSpecifyCanID(uchar IdIndex, uchar *pID_Set);
	void		DataUpLoadToServer(void);
	uint32		SendTickCnt;
private:

	CAN_msg       CAN_TxMsg[2];      /* CAN messge for sending */
	CAN_msg       CAN_RxMsg[2];      /* CAN message for receiving */                                
	unsigned int  CAN_TxRdy[2];      /* CAN HW ready to transmit a message */
	unsigned int  CAN_RxRdy[2];      /* CAN HW received a message */

	uint32		GetCanID,ByPassCanID;
	uint32		GetAllDataCnt;
	uint32		SpecifyCanID[16];
	uchar			SpecifyCanData[8*16];
	uchar			SpecifyNumber,SpecifyCnt;
	
	uint32		TemporaryCanID[16];	
	uchar			TemporaryCanData[512];	
	uchar			GetDataBuffer[512];
	uchar			TemporaryNumber;
	uint32		Can1RecOverTimeCnt;
	
	uint32		TestCanStartTime;

	uint32 		JTCanSampleInterval[16];			// 采样通道的采集间隔 下标与 SpecifyCanID的下标相对应	   added 2013.5
	
	uchar			LastPageCanData;

	uint16		GetDataCnt,GetSpecifyCanIdCnt,ByPassCanCnt,CanBusNumberSID;
	

};


CAN_EXT	__noinit__		CanBusCls		CanBus;		//



#endif    // 
/*
 $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$  End of File $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
 */

