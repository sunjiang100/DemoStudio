/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$  Copyright (c)  $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
$												 
$									 
$                      	 
$						  		All rights reserved.	  
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
#ifndef		__ADC_H__
#define		__ADC_H__


#ifdef      ADC_GLOBALS
#define     ADC_EXT
#else
#define     ADC_EXT    extern
#endif	

#define		__ADC_DEBUG__		1


#include		"..\Config\RTOS.h"
#include    "..\UART\UART0.h"
//#include    "..\MC33993\MC33993.h"
#include		<string.h>


// Non-aligned Access: ARM Instruction at 00002E74H, Memory Access at 40002095H
#pragma		pack(1)

#define     CHANNEL_0         0    // temperature
#define	   CHANNEL_1		   1    // voltage
#define	   CHANNEL_2		   2    // current
#define	   CHANNEL_3		   3    // 
#define	   CHANNEL_4		   4




//#define		KEY_AD_RANGE		30//300
//#define		KEY1_AD				4032//4032//3130//0				  // 3.25v
//#define		KEY2_AD				3950//739					  // 3.16v
//#define		KEY3_AD				3675//1504					  // 2.94v
//#define		KEY4_AD				2900//2136					  // 2.32v
//#define		KEY5_AD				0//2955

#define		KEY_AD_RANGE		80//300
#define		KEY1_AD				3300				  // 2.62v	 2.67		2.64
#define		KEY2_AD				3100//739					  // 2.46v	  2.50			 2.48
#define		KEY3_AD				2750//1504					  // 2.18v		2.22		  2.20
#define		KEY4_AD				2062//2136					  // 1.64v		 1.67			1.65
#define		KEY5_AD				0//2955


//#define		MV_PER_DOT			80				// AD值每点的电压（毫伏）

#define		MV_PER_DOT			  80				// AD值每点的电压（0.01微伏）	   12位的ADC		 3.3V/4096

#define	   MIN_CHANNEL		     0
#define	   MAX_CHANNEL         8//5
#define     TOTAL_CHANNEL       9//5
#define		NO_USE_CHANNEL		  0	// 为了节省RAM，故要设定保留通道



//#define    DATUM_MARK        1023/3
#define    	ENABLE_ERROR        100

#define	   SAMPLE_TIMES		  10

//     // 内部基准2.56V，对应1024点，每个点2.5mV

#define		LEN_ADC_BUFFER		(TOTAL_CHANNEL*SAMPLE_TIMES*2)


#pragma		pack(2)			// 多字节变量数组，前面加个变量，对齐方式才正确。
typedef	struct   
{
	uint16  	 volatile	Counter,MainPower,Temperature,Rsv;
  	uint16    volatile	Buffer[TOTAL_CHANNEL/2+1][SAMPLE_TIMES];
  	uint16    volatile	Sum[TOTAL_CHANNEL/2+1];	
	uint16    volatile	Avg[TOTAL_CHANNEL/2+1];	
 	uint16	 volatile	Vol[TOTAL_CHANNEL/2+1];
} ADC_STR,*pADC_STR;


typedef		struct
{
	uchar		KeyDown,KeyPress,KeyUp,KeyFree;	
	uchar		KeyCode;		
	uchar 	Fifo[3];
}	KEY_ATTR,pKEY_ATTR;



/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	UART类 
*/
class			ADCCls
{
public:
	ADCCls(void);
	void		Init(void);
	void		eTaskADC(void);
	void		GetAllResult(uint16	*pAdc);
	uint32	GetKeyCode(void);
	uint32	Get_ADC(void);	
//	ADC_STR	AdcR;
private:
	ADC_STR	AdcR;
	uint32	TickCnt;
	uchar    ucChannel,ChannelValid,sunchannel;		
	KEY_ATTR		Key;		
};


ADC_EXT    __noinit__		ADCCls			Adc;



#endif

/*
 $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$  End of File $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
 */
