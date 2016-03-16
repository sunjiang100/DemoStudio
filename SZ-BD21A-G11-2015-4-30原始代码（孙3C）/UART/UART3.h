			  
#ifndef		__UART3_H__
#define		__UART3_H__


#ifdef      UART3_GLOBALS
#define     UART3_EXT
#else
#define     UART3_EXT    extern
#endif


#include		"..\Config\RTOS.h"




#define   	OWN_ADDRESS      1
#define   	LEAD_OK          0x55
#define   	LENGTH_OK        0xaa



#define      UART3_BPS    		9600 //38400   //9600

#define      U3_DATA_BIT    	3
#define      U3_CHECK_BIT_EN  0

#define      U3_CHECK_BIT    	0

#define      U3_STOP_BIT    	0

#define      U3_PROIRITY    	1

#define		LEN_U3_REC_BUFFER		1000

#define		LEN_U3_TXD_BUFFER		(1024+64)//64

#define		USE_QUERY_SEND		0
#pragma		pack(1)
typedef		struct 
{
	uint32	OverTimeCnt;
   uint16  	Counter;
}  REC3;

#pragma		pack(1)
typedef		struct 
{
   uchar   Buffer[LEN_U3_TXD_BUFFER];
   uchar   CheckSum,Sending;   
   uint16  Length;
}  TXD3;


#define			LEN_GPS_REC				96
#define			LEN_GPS_ANT				64


UART3_EXT	char		U3_RecBuffer[LEN_U3_REC_BUFFER];//	 __attribute__((at(0x7fe02c00)));		// 1K

class			Uart3Cls
{
public:
	Uart3Cls(void);
	bool		Init(uint32);
	uchar		putb(uint16,uchar *p);
	uchar		puts(char *p);	
	uchar		GetRecBuffer(char *pGPS,char *pANT,char *pSatel);	
	void		GetAllGPS_Data(char *pData);
	void		ClearRecBuffer(void);
	void		Interrupt(void);
	uint16	WaitRecBuffer(uint16 WaitTick,uint16 Length,uchar *pR);
private:
	TXD3		Txd;
	REC3		Rec;
	uchar		*pTxd;//,TxdBuffer[32];
	uint32	volatile	U3_Baudrate;
	uint16	volatile	NoDataCnt;		// 无数据计数器，需切换波特率
		
	uchar  	ErrCnt,GPS_Valid;
	
	uchar		EndData[3];

};

UART3_EXT	__noinit__		Uart3Cls		Uart3;		// 







#endif

/*
 $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$  End of File $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
 */
