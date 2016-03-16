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
#ifndef			__INTVECT_H__
#define			__INTVECT_H__


#define			IRQ_ENABLE				(1<<5)

#define			WDT_INT_VECT			0	
#define			SOFT_INT_VECT			1	
#define			TIME0_INT_VECT			4	
#define			TIME1_INT_VECT			5	
#define			UART0_INT_VECT			6	
#define			UART1_INT_VECT			7	
#define			PWM0_INT_VECT			8	
#define			IIC0_INT_VECT			9	
#define			SSP0_INT_VECT			10	
#define			SSP1_INT_VECT			11	
#define			PLL_INT_VECT			12	
#define			RTC_INT_VECT			13	
#define			EINT0_INT_VECT			14	
#define			EINT1_INT_VECT			15	
#define			EINT2_INT_VECT			16	
#define			EINT3_INT_VECT			17	
#define			ADC0_INT_VECT			18	
#define			IIC1_INT_VECT			19	
#define			BOD_INT_VECT			20	
#define			ENET_INT_VECT			21
#define			USB_INT_VECT			22
#define			CAN_INT_VECT			23
#define			SD_INT_VECT				24
#define			GPDMA_INT_VECT			25
#define			TIMER2_INT_VECT		26
#define			TIMER3_INT_VECT		27
#define			UART2_INT_VECT			28
#define			UART3_INT_VECT			29
#define			I2C2_INT_VECT			30
#define			I2S_INT_VECT			31

#define			ADC1_INT_VECT			22		// 分配IRQ通道

/////////////	EXTMODE	
#define			EXTMODE0					0		// 
#define			EXTMODE1					1		// 
#define			EXTMODE2					2		// 
#define			EXTMODE3					3		// 

/////////////	EXTPOLAR	
#define			EXTPOLAR0				0		// 
#define			EXTPOLAR1				1		// 
#define			EXTPOLAR2				2		// 
#define			EXTPOLAR3				3		// 


/////////////  EXTINT
#define			EINT0						0
#define			EINT1						1
#define			EINT2						2
#define			EINT3						3





#endif

/*
 $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$  End of File $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
 */
