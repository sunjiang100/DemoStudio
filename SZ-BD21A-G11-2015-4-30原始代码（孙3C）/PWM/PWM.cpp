/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$  Copyright (c)  $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
$												 
$										 
$                       	 		  
$								  		All rights reserved.	  
$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$  Copyright (c)  $$$$$$$$$$$$$$$$$$$$$$$$$$$ */
/*  
                                      文件描述
	文件名: 	 RTC.pcc
	功能描述：
	编译环境：Keil for ARM + RTX
	目标CPU:  LPC2000
	作者：
	创建时间：
	建议编辑环境：UltraEdit 11.00a+  制表符宽度为三个字符，程序可读性更好。
	注：LPC2000系列的RTC寄存器是16进制格式，PCF8563的RTC寄存器是BCD格式
$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
*/					  

#define			PWM_GLOBALS		1
#include     	"PWM.h"
#include			"..\Config\RTOS.h"
#include			"..\UserSys.h"



/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	函数名称：
   功能描述：
   入口参数：频率（单位：0.01Hz）
   出口参数：
   全局变量: 
   修改时间：
*/ 
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ 以上原形声明的函数供本函数调用
void		PwmCls::Init(uint32 S)
{
	uint32	Per,Fclk;	
	///////////////  								UART0缓冲区及相关标志清零
	NOP();	NOP();	
	LPC_PWM1->TCR	= 0x00;

	if( (S==0)||(S>0xf0000000) )	return;

	Fclk = LPC_SC->PCLKSEL0;
	Fclk &= (3UL<<12);				// B.12.13
	Fclk >>= 12;
	Fclk = PeriClock[Fclk];
	
	LPC_PINCON->PINSEL7 |=((3UL<<18));		// P3.25 for PWM1.2
	
	LPC_PWM1->CTCR = 0;
	
	LPC_PWM1->PR  = 0x00;   				// 定时器计数频率和Fclk相同
	LPC_PWM1->MCR = 0x02;
	
	LPC_PWM1->PCR = ( (1UL<<2) | (1UL<<10) );		// 单边沿，只使能PWM2


	Per = 100000000L;
	Per /= Fclk;				// 每个计数时钟0.04us

	Fclk = 100000000L;
	Fclk /= S;		

	Fclk *= 100;			  	// 周期

	Fclk /= Per;				// 需要计数时钟个数，即初值

	LPC_PWM1->MR0 = Fclk;
	
	LPC_PWM1->MR1 = 0;
	LPC_PWM1->MR2 = Fclk>>1;

	LPC_PWM1->LER 	= 0x7f;
	LPC_PWM1->TCR	= 0x02;
	LPC_PWM1->TCR	= 0x09;	
}

void		PwmCls::UnInit(void)
{
	NOP();	NOP();	
	LPC_PINCON->PINSEL7 &=(~(3UL<<18));		// P3.25 for PWM1.2
	//LPC_PWM1->LER 	= 0x00;
	//LPC_PWM1->TCR	= 0x00;

}

/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	函数名称：SetTargetSpeed
   功能描述：在Time秒钟加速或减速到S
   入口参数：
   出口参数：
   全局变量: 
   修改时间：
*/ 
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ 以上原形声明的函数供本函数调用
void		PwmCls::SetTargetSpeed(uint32 S,uint32 Time)
{
	uint32	tl;
	NOP();	NOP();
	S *= VehiclePulseKV;	// 把km/h，换算成频率
	S /= 36;				// 频率，单位：0.01Hz	
	TargetSpeed = S;	
	if(TargetSpeed>NowSpeed)
		tl = TargetSpeed - NowSpeed;
	else
		tl = NowSpeed - TargetSpeed;
	Time *= 5;	
	if(Time==0)	Time = 1;	
	Acceleration = tl/Time;	
	if(Acceleration==0)	
		Acceleration = 1;
}



/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	函数名称：
   功能描述：模拟汽车行驶停止，包括车速脉冲、ACC、发动机、刹车
   入口参数：
   出口参数：
   全局变量: 
   修改时间：
	注：此函数应该每200ms调用1次，对应SetTargetSpeed的Time
*/ 
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ 以上原形声明的函数供本函数调用
void		PwmCls::SimuSpeedProcess(void)
{
	uchar		t;
	///////////////  								
	NOP();	NOP();
	if(SimuSpeed.Enable != ENABLE_FLAG)		return;
	
	if(NowSpeed>TargetSpeed)		// 减速
	{
		if(NowSpeed>Acceleration)	// 防止目标速度为0时，不够减导致溢出
			NowSpeed -=	Acceleration;
		else
			NowSpeed =	TargetSpeed;
		Init(NowSpeed);
		SimuSpeed.Brake = TRUE;	// 减速，认为刹车		
		t = TRUE;
	}
	else if(NowSpeed<TargetSpeed)		// 加速
	{
		NowSpeed +=	Acceleration;
		if(NowSpeed>TargetSpeed)
			NowSpeed =	TargetSpeed;
		Init(NowSpeed);
		SimuSpeed.Brake = 0;	
		t = TRUE;			
	}
	else
	{
		SimuSpeed.Brake = 0;		// 
		t = 0;
	}
}








PwmCls::PwmCls(void)
{
	
}



/*
 $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$  End of File $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
 */



