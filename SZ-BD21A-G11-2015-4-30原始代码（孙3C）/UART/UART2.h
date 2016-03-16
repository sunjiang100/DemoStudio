/***************************************  Copyright (c)  **************************************
**									 
**									
**                      
**	  
****************************************  Copyright (c)  **************************************
**                                         文件描述
**	文件名:   Uart2.H
**	功能描述：UART2底层驱动头文件.
**	编译环境：Keil for ARM uVision3
**	目标CPU:  LPC2378
**	作者：	 
**	创建时间：
**  修改时间: 2009年05月18日
**	建议编辑环境：UltraEdit 11.00a+  制表符宽度为三个字符，程序可读性更好。	
*************************************************************************************************/
			  
#ifndef			__UART2_H__
#define			__UART2_H__

#ifdef      	UART2_GLOBALS
#define     	UART2_EXT
#else
#define     	UART2_EXT    extern
#endif

#include			<string.h>
#include    	<stdio.h>
#include			"..\Config\RTOS.h"
#include 		"..\COMMON\CppGlobals.h"


#define			FIFO_LENGTH				14
#define			TRIGGER_WITH_FIFO		1

// $$$$$$$$$$$$$$$$$$ <<< Use Configuration Wizard in Context Menu >>> $$$$$$$$$$$$$$$$$
// <h>请选择是否使用UART，不使用则不占资源
// <h>UART0初始化相关配置(CPU主频在CPU.h文件中设置)
/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//   	<o>UART0_BPS: 串口波特率设置 <1-250000>
//   	<i> Default: 9600
*/
#define      	UART2_BPS    			57600//19200

/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//   	<o>U0_DATA_BIT: 数据位配置 <0=> 5位 <1=> 6位 <2=> 7位 <3=> 8位
//   	<i> Default: 3
*/
#define     	 U2_DATA_BIT    		3

//	<e>禁止或使能校验位
/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//   	<o>U0_CHECK_BIT_EN: 校验位使能 <0=> 禁止校验 <1=> 允许校验
//   	<i> Default: 1
*/
#define     	 U2_CHECK_BIT_EN  	0

/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//   	<o>U0_CHECK_BIT: 校验位配置 <0=> 奇校验 <1=> 偶校验 <2=> 强制1 <3=> 强制0
//   	<i> Default: 0
*/
#define      	 U2_CHECK_BIT    		0

//	</e>
/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//   	<o>U0_STOP_BIT: 停止位配置 <0=> 1位 <1=> 2位 
//   	<i> Default: 0
*/
#define      	U2_STOP_BIT    			0

/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//   	<o>U0_PROIRITY: 串口中断优先级 <0=> 不优先 <1=> 优先
//   	<i> Default: 1
*/
#define			U2_PROIRITY    			1

/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//   	<o>UART0_REC_LENGTH: 接收缓冲区长度字节设置 <1-1024>
//   	<i> Default: 100
*/
#define			UART2_REC_LENGTH		640

/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//   	<o>UART0_TXD_LENGTH: 发送缓冲区长度字节设置 <1-1024>
//   	<i> Default: 100
*/
#define			UART2_TXD_LENGTH		512

/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//   	<o>USE_QUERY_SEND: 使用查询发送函数 <0=> 不使用 <1=> 使用
//   	<i> Default: 0
*/
#define			USE_QUERY_SEND			0
		   	
// </h>

// </h>

// $$$$$$$$$$$$$$$$$$$ <<< end of configuration section >>> $$$$$$$$$$$$$$$$$$$$$$$$$$$$



/******************* UART类 ****************************/
class			Uart2Cls
{
public:
	Uart2Cls(void);
   bool		Init(uint32,uchar);
	uchar		puts(char *p);
	uchar		putb(uchar,uint16,uchar *p);
	void		SendTask();	
	inline	void	Interrupt(void);
	void		ClearRecFlag(void);
	uint16	WaitGetRecBuffer(uchar *p);
	void		RecEnable(void);
	uint16	GetRecBuffer(uchar *pData);
	uint16	RFID_ReceiveData(uchar *pData);
	void		ReceiveProcessData(void);
	uint16	ReceiveAudioData(uint16 Len,uchar *pRec, uchar *pSend);
	uint16	SendDataToAudioDevice(uint16 Cmd,uint16 Length,uchar *pData);
private:	
	uchar		RecBuffer[UART2_REC_LENGTH];		//uchar		RecBuffer[UART2_REC_LENGTH];
	uchar		TxdBuffer[UART2_TXD_LENGTH];
	uchar		TxdBufUsing;							//写发送缓冲区时的保护
	uint32	RecOverTimeCnt;						//接收超时标志
	uint16	Counter;								//
	uint16   RecLength,TxdLength,CameraDataLen;					//接收缓冲区长度，发送缓冲区长度
	//uchar		FrameHead[8];
	uchar		Camera;
};

UART2_EXT	__noinit__		Uart2Cls		Uart2;		// 






#endif

/*******************************  End of File *************************************/


