/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$  Copyright (c)  $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
$												 
$										 
$                       	 		  
$								  		All rights reserved.	  
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

#ifndef		__UART1_H__
#define		__UART1_H__

#include		"..\Config\RTOS.h"


#ifdef      UART1_GLOBALS
#define     UART1_EXT
#else
#define     UART1_EXT    extern
#endif


// $$$$$$$$$$$$$$$$$$ <<< Use Configuration Wizard in Context Menu >>> $$$$$$$$$$$$$$$$$
// <h>��ѡ���Ƿ�ʹ��UART1����ʹ����ռ��Դ


// <h>UART1��ʼ���������(CPU��Ƶ��CPU.h�ļ�������)
  

/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//   	<o>UART1_BPS: ���ڲ��������� <1-250000>
//   	<i> Default: 19200
*/
#define      UART1_BPS    		115200 //38400 //115200

/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//   	<o>U1_DATA_BIT: ����λ���� <0=> 5λ <1=> 6λ <2=> 7λ <3=> 8λ
//   	<i> Default: 3
*/
#define      U1_DATA_BIT    	3

//	<e>��ֹ��ʹ��У��λ

/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//   	<o>U1_CHECK_BIT_EN: У��λʹ�� <0=> ��ֹУ�� <1=> ����У��
//   	<i> Default: 1
*/
#define      U1_CHECK_BIT_EN  0


/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//   	<o>U1_CHECK_BIT: У��λ���� <0=> ��У�� <1=> żУ�� <2=> ǿ��1 <3=> ǿ��0
//   	<i> Default: 0
*/
#define      U1_CHECK_BIT    	0

//	</e>

/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//   	<o>U1_STOP_BIT: ֹͣλ���� <0=> 1λ <1=> 2λ 
//   	<i> Default: 0
*/
#define      U1_STOP_BIT    	0

/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//   	<o>U1_PROIRITY: �����ж����ȼ� <0=> ������ <1=> ����
//   	<i> Default: 1
*/
#define      U1_PROIRITY    	1




/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//   	<o>UART1_TXD_LENGTH: ���ͻ����������ֽ����� <1-2048>
//   	<i> Default: 100
*/
#define		UART1_TXD_LENGTH			1024

/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//   	<o>UART1_RET_LENGTH: �����ֻ�ģ��״̬���ػ��� <1-2048>
//   	<i> Default: 100
*/
#define		UART1_REC_BUF_LENGTH		(1024+256)


// </h>

// </h>

// $$$$$$$$$$$$$$$$$$$ <<< end of configuration section >>> $$$$$$$$$$$$$$$$$$$$$$$$$$$$


#define		OWN_ADDRESS      1
#define		LEAD_OK          0x55
#define   	LENGTH_OK        0xaa

#define		UART1_FIFO_LENGTH	 14


#pragma		pack(1)
typedef		struct 				// ������ض���
{	
   uint16  	volatile	Counter;	
   uint32	volatile	OverTimeCnt;
}  REC1,*pREC1;

#pragma		pack(1)
typedef		 struct 			// ������ض���
{   
   uchar    volatile	CheckSum,Sending;   
   uint16   volatile	Length;
}  TXD1,*pTXD1;


UART1_EXT	uchar    U1_TxdBuffer[UART1_TXD_LENGTH];//  		__attribute__((at(0x7fe00800)));	// 1K
UART1_EXT	uchar		U1_RecBuffer[UART1_REC_BUF_LENGTH];//	__attribute__((at(0x7fe00c00)));	// 1K

/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	UART��
*/
class			Uart1Cls
{
public:
	Uart1Cls(void);
	bool		Init(uint32);
	uint16	GetFrameBuffer(uchar *pR);
	uint16	GetRecBuffer(uchar *pR);
	uint16	GetRecLength(void);
	void		WaitSendFree(void);
	uint16	WaitRecBuffer(uint16 WaitTick,uint16 Length,uchar *pR);
	uchar		puts(char *p);
	uchar		putb(uchar,uint16,uchar *p);
	void		ClearReceiveFlag(uchar);
	void		ClearTransmitFlag(void);	
	__inline		void		Interrupt(void);
	void		TaskReceiveOK(void);
	void		TaskTransmitOK(void);
	uchar		WaitChar(void);
public:
	REC1		Rec;
	TXD1		Txd;		
private:
	uchar		volatile		*pTxd;
	uchar		PackFlag[6];
};

UART1_EXT	__noinit__		Uart1Cls		Uart1;	// 



#endif

/*
 $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$  End of File $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
 */
