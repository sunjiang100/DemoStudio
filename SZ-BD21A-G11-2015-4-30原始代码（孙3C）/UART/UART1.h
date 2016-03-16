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

#ifndef		__UART1_H__
#define		__UART1_H__

#include		"..\Config\RTOS.h"


#ifdef      UART1_GLOBALS
#define     UART1_EXT
#else
#define     UART1_EXT    extern
#endif


// $$$$$$$$$$$$$$$$$$ <<< Use Configuration Wizard in Context Menu >>> $$$$$$$$$$$$$$$$$
// <h>请选择是否使用UART1，不使用则不占资源


// <h>UART1初始化相关配置(CPU主频在CPU.h文件中设置)
  

/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//   	<o>UART1_BPS: 串口波特率设置 <1-250000>
//   	<i> Default: 19200
*/
#define      UART1_BPS    		115200 //38400 //115200

/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//   	<o>U1_DATA_BIT: 数据位配置 <0=> 5位 <1=> 6位 <2=> 7位 <3=> 8位
//   	<i> Default: 3
*/
#define      U1_DATA_BIT    	3

//	<e>禁止或使能校验位

/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//   	<o>U1_CHECK_BIT_EN: 校验位使能 <0=> 禁止校验 <1=> 允许校验
//   	<i> Default: 1
*/
#define      U1_CHECK_BIT_EN  0


/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//   	<o>U1_CHECK_BIT: 校验位配置 <0=> 奇校验 <1=> 偶校验 <2=> 强制1 <3=> 强制0
//   	<i> Default: 0
*/
#define      U1_CHECK_BIT    	0

//	</e>

/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//   	<o>U1_STOP_BIT: 停止位配置 <0=> 1位 <1=> 2位 
//   	<i> Default: 0
*/
#define      U1_STOP_BIT    	0

/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//   	<o>U1_PROIRITY: 串口中断优先级 <0=> 不优先 <1=> 优先
//   	<i> Default: 1
*/
#define      U1_PROIRITY    	1




/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//   	<o>UART1_TXD_LENGTH: 发送缓冲区长度字节设置 <1-2048>
//   	<i> Default: 100
*/
#define		UART1_TXD_LENGTH			1024

/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//   	<o>UART1_RET_LENGTH: 设置手机模块状态返回缓冲 <1-2048>
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
typedef		struct 				// 接收相关定义
{	
   uint16  	volatile	Counter;	
   uint32	volatile	OverTimeCnt;
}  REC1,*pREC1;

#pragma		pack(1)
typedef		 struct 			// 发送相关定义
{   
   uchar    volatile	CheckSum,Sending;   
   uint16   volatile	Length;
}  TXD1,*pTXD1;


UART1_EXT	uchar    U1_TxdBuffer[UART1_TXD_LENGTH];//  		__attribute__((at(0x7fe00800)));	// 1K
UART1_EXT	uchar		U1_RecBuffer[UART1_REC_BUF_LENGTH];//	__attribute__((at(0x7fe00c00)));	// 1K

/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	UART类
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
