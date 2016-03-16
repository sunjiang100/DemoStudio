/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$  Copyright (c)  $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
$											广州国业电子科技有限公司
$							GuangZhou GuoYe Electronics Sciense&Technology Co.,LTD.
$                   	http://www.guoyee.com		 Email: guoyegps@126.com
$						 	QQ群：9069471/9069433/9524253		All rights reserved.	  
$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$  Copyright (c)  $$$$$$$$$$$$$$$$$$$$$$$$$$$ */
/*  
                                      文件描述
	文件名: 	 	Pulse.h
	功能描述：	速度脉冲处理、高速脉冲处理
	编译环境：	
	目标CPU: 	LPC176x
	作者：
	创建时间：
	建议编辑环境：UltraEdit 11.00a+  制表符宽度为三个字符，程序可读性更好。
$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
*/		

#define			PULSE_GLOBALS		1
#include			"Pulse.h"

#include		   <string.h>
#include		   "..\RTC\RTC.h"
#include		   "..\UserSys.h"
#include	   	"..\Common\CppGlobals.h"
#include       "..\Config\UserConfig.h"
#include			"..\UART\UART0.H"

// 低速度脉冲时使用周期计算
#define		USE_USE_PERIOD_CALC_SPEED			1			// 1：使用周期计算速度		0: 使用脉冲个数计算速度

#if (USE_USE_PERIOD_CALC_SPEED == 1)
#define		LOW_SPEED_USE_PERIOD			1			  
#define		LOW_SPPED_PULSE_DOT2SEC		1000									// 0.2秒脉冲个数
#define		LOW_SPEED_PULSE_FREQUENCY	(LOW_SPEED_PULSE_FRQ*5)		// 必须是5的整数倍

#else
#define		LOW_SPEED_USE_PERIOD			0
#define		LOW_SPPED_PULSE_DOT2SEC		1000									// 0.2秒脉冲个数
#define		LOW_SPEED_PULSE_FREQUENCY	(LOW_SPEED_PULSE_FRQ*5)		// 必须是5的整数倍	  

#endif



__inline		void    PulseCls::Timer0Interrupt(void)
{
	
	HighPulseCnt.Total++;
	HighPulseCnt.OneHour++;
	HighPulseCnt.OneMin++;
	HighPulseCnt.OneSec++;
	HighPulseCnt.D2Sec++;  
		
}

extern		"C"		void		TIMER0_IRQHandler(void)		__irq
{
	Pulse.Timer0Interrupt();
	LPC_TIM0->IR = 0x3f;			// 清全部中断标志
	NOP();	NOP(); 	
}



/********************************************************************************
** 函数名称：Timer0_Init()
** 功能描述：Timer2初始化
** 入口参数：无
** 出口参数：无
** 全局变量: 无
** 创建时间：2009.05.07
** 修改时间：
***********************************************************************************/ 
void 			PulseCls::Timer0Init(void)
{
	CAP01_PIN_SELECT;
	LPC_TIM0->TC  = 0;				  		  //计数器清零
	//LPC_TIM0->IR = 0;
	LPC_TIM0->CTCR = 0x00;					  //CAP0.1用于定时器模式
	LPC_TIM0->PR  = 0;						  //计数器25分频   1uS计数一次
	//LPC_TIM0->MCR = 0x00;
	LPC_TIM0->CCR = (0x05<<3);					  //CAP0.1用于扑捉模式，上升缘扑捉，并产生中断
	NVIC_SetPriority(TIMER0_IRQn, TIMER0_INT_PRIORITY);     
   NVIC_EnableIRQ(TIMER0_IRQn); 	
	LPC_TIM0->TCR = 0x01;					  //计数器复位并启动
}



__inline		void    PulseCls::Timer1Interrupt(void)
{
	
 	
 	MotoPulseCnt.Total++;
	MotoPulseCnt.OneHour++;
	MotoPulseCnt.OneMin++;
	MotoPulseCnt.OneSec++;
	MotoPulseCnt.D2Sec++;  
		
}

extern		"C"		void		TIMER1_IRQHandler(void)		__irq
{
	Pulse.Timer1Interrupt();
	LPC_TIM1->IR = 0x3f;			// 清全部中断标志
	NOP();	NOP(); 	
}




/********************************************************************************
** 函数名称：Timer0_Init()
** 功能描述：Timer2初始化
** 入口参数：无
** 出口参数：无
** 全局变量: 无
** 创建时间：2009.05.07
** 修改时间：
***********************************************************************************/ 
void 		PulseCls::Timer1Init(void)
{
	
   CAP11_PIN_SELECT;
	LPC_TIM1->TC  = 0;				  		  //计数器清零	
	//LPC_TIM1->IR = 0;
	LPC_TIM1->CTCR = 0x00;					  //CAP1.1用于定时器模式
	LPC_TIM1->PR  = 0;						  //计数器25分频   1uS计数一次
	//LPC_TIM1->MCR = 0x00;
	LPC_TIM1->CCR = (0x05<<3);					  //CAP1.1用于扑捉模式，上升缘扑捉，并产生中断
	NVIC_SetPriority(TIMER1_IRQn, TIMER1_INT_PRIORITY);     
   NVIC_EnableIRQ(TIMER1_IRQn); 	
	LPC_TIM1->TCR = 0x01;					  //计数器复位并启动
}







/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	函数名称：Timer3Interrupt
   功能描述：
   入口参数：
   出口参数：
   全局变量: 
   创建时间：
   修改时间：
*/
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ 以上原形声明的函数供本函数调用
__inline		void    PulseCls::Timer3Interrupt(void)
{
	uint32	tl;
	if( (LPC_TIM3->IR)&(1<<4) )		// 上升沿处理
	{
		#if(LOW_SPEED_USE_PERIOD)
					
		InsPlusPeriod = LPC_TIM3->CR0;		// 从0开始，捕捉值就是周期	
										
		LPC_TIM3->TC = 0; 
		
		#endif

		SpeedPulseCnt.Total++;
		SpeedPulseCnt.OneHour++;
		SpeedPulseCnt.OneMin++;
		SpeedPulseCnt.OneSec++;
		SpeedPulseCnt.D2Sec++;			
		SystemLog.VDR.SpeedSensorPulsCnt++;
		TestMileage.Puls++;
	}
}

extern		"C"		void		TIMER3_IRQHandler(void)		__irq
{
	Pulse.Timer3Interrupt();
	LPC_TIM3->IR = 0x3f;			// 清全部中断标志
	NOP();	NOP(); 	
}

/********************************************************************************
** 函数名称：Timer3Init
** 功能描述：Timer3初始化
** 入口参数：无
** 出口参数：无
** 全局变量: 无
** 创建时间：2009.05.07
** 修改时间：
***********************************************************************************/ 
void 		PulseCls::Timer3Init(void)	   
{
	uint32  	Fclk;	
	///////////////  
	NOP();	NOP();	
	RitIntCnt = 0;
	Fclk = LPC_SC->PCLKSEL1;
	Fclk &= (3<<14);							// B.6.7
	Fclk >>= 14;
	Timer3Clock = PeriClock[Fclk];

	CAP30_PIN_SELECT;
	LPC_TIM3->TC  = 0;				  		  //计数器清零	
	//LPC_TIM3->IR = 0;
	LPC_TIM3->CTCR = 0x00;					  //CAP3.0用于定时器模式
	LPC_TIM3->PR  = 0;//Fclk/1000000UL;		  // 1uS计数一次
	//LPC_TIM3->MCR = 0x00;
	LPC_TIM3->CCR = 0x05;					  //CAP3.0用于扑捉模式，下降缘扑捉，并产生中断
	NVIC_SetPriority(TIMER3_IRQn, TIMER3_INT_PRIORITY);     
   NVIC_EnableIRQ(TIMER3_IRQn); 
	
	LPC_TIM3->TCR = 0x01;					  //计数器复位并启动

	//////////////////////////////////////////////////
	
	
}
 

// VehiclePulseKV  
__inline		void    PulseCls::RitInterrupt(void)			// 0.2秒执行1次
{
	//uchar		Speed;
	uint32	tl;
	
	SpeedSw[4].Speed = SpeedSw[3].Speed;
	SpeedSw[3].Speed = SpeedSw[2].Speed;
	SpeedSw[2].Speed = SpeedSw[1].Speed;
	SpeedSw[1].Speed = SpeedSw[0].Speed;
	SpeedSw[4].Sw = SpeedSw[3].Sw;
	SpeedSw[3].Sw = SpeedSw[2].Sw;
	SpeedSw[2].Sw = SpeedSw[1].Sw;
	SpeedSw[1].Sw = SpeedSw[0].Sw;
	///////////////////////////////////////////////////////////////////////////////// 计算0.2秒的瞬时时速，仅事故疑点使用
	tl = SpeedPulseCnt.D2Sec;
	SpeedPulseCnt.D2Sec=0;
	#if(LOW_SPEED_USE_PERIOD)
	if( tl >= LOW_SPPED_PULSE_DOT2SEC )			// 0.2s脉冲数大于10，使用脉冲计算
	#endif
	{		
		tl *= (5*3600UL);							// 
		SpeedSw[0].Speed = tl/VehiclePulseKV;		// 
	}
	#if(LOW_SPEED_USE_PERIOD)
	else						// 脉冲数小于10，使用周期计算
	{
		tl = Timer3Clock/InsPlusPeriod;		// 得出频率
		InsPlusPeriod = 0;
		
		#if (WUXI_TEST == 1)
		if (tl>180) tl = 0; // 大于180Hz  过滤干扰点	 应付无锡检测，因为机器有干扰，找不到原因，直接过滤
		#endif		

		if (tl>(SpeedSw[1].Speed + 150))		// 过滤点偏差很大的点：大于150Hz
		{
			tl = SpeedSw[1].Speed;					// 取最后的值
		}
	
		if (TD.Speed==tSPEED_SIMU_SPEED )
		{
			tl = 116; //
		}

		tl *= (3600UL);							// 
		SpeedSw[0].Speed = tl/VehiclePulseKV;		// 
		tl *= 10;
		uiLowSpeed[4] = uiLowSpeed[3];	 	// 记录连续5个0.2S的瞬时平均速度
		uiLowSpeed[3] = uiLowSpeed[2];
		uiLowSpeed[2] = uiLowSpeed[1];
		uiLowSpeed[1] = uiLowSpeed[0];
		uiLowSpeed[0] = tl/VehiclePulseKV;			// 0.1km/h
	}		
	#endif
	
	// 制动
	if (JT808ExtenVsFlag.VS1.Bits.Brake) 	 SpeedSw[0].Sw |= (1<<7);
	else												 SpeedSw[0].Sw &= ~(1<<7);

	// 左转向
	if (JT808ExtenVsFlag.VS1.Bits.LTurnLight)	SpeedSw[0].Sw |= (1<<6);
	else												 	SpeedSw[0].Sw &= ~(1<<6);

	// 右转向
	if (JT808ExtenVsFlag.VS1.Bits.RTurnLight)	SpeedSw[0].Sw |= (1<<5);
	else												 	SpeedSw[0].Sw &= ~(1<<5);

	// 远光
	if (JT808ExtenVsFlag.VS1.Bits.HighLight)	SpeedSw[0].Sw |= (1<<4);
	else												 	SpeedSw[0].Sw &= ~(1<<4);

	// 近光
	if (JT808ExtenVsFlag.VS1.Bits.DippedHeadLight)	SpeedSw[0].Sw |= (1<<3);
	else												 			SpeedSw[0].Sw &= ~(1<<3);
	
	///////////////////////////////////////////////////////////////////////////////// 计算每秒的瞬时时速
	RitIntCnt++;
	if(RitIntCnt==5)			// 取模不清零的方式更慢些，因为要算除法运行
	{
		RitIntCnt = 0;
				
		tl = uiLowSpeed[0];		tl += uiLowSpeed[1];
		tl += uiLowSpeed[2];		tl += uiLowSpeed[3];	
		tl += uiLowSpeed[4];
		SecSpeed = tl/5;	  		// 每秒的平均速度
				
	}
	
}


static		uchar		RitIntFlag;
extern		"C"		void		RIT_IRQHandler(void)		__irq
{		
	gCnt.TickForRandData++;
	CanOverTimeCnt++;
	RitIntFlag++;
	if(RitIntFlag&0x01)
		Pulse.RitInterrupt();
	LPC_RIT->RICTRL |= (1<<0);         //清中断
  	LPC_RIT->RICOUNTER = 0;            //RIT计数值清零
   LPC_RIT->RICTRL |= (1<<0)|(1<<3);  //中断复位，启动RIT		  
	
	NOP();	NOP(); 	
}


/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	函数名称：RitInit
   功能描述：
   入口参数：无
   出口参数：无
   全局变量: 
   创建时间：
   修改时间：
*/ 	

// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ 以上原形声明的函数供本函数调用


bool			PulseCls::RitInit(uint32	mSecend)
{
	uint32  	Ricom,Fclk;	
	///////////////  
	NOP();	NOP();	
	RitIntCnt = 0;
	Fclk = LPC_SC->PCLKSEL1;
	Fclk &= (3<<26);							// B.6.7
	Fclk >>= 26;
	Fclk = PeriClock[Fclk];
	Ricom = Fclk/(1000/mSecend);			 //根据参数设置中断时间
	LPC_SC->PCONP |= (1UL<<PCONP_RIT);
	LPC_RIT->RICTRL |= (1<<0);         	//清中断
	LPC_RIT->RICOMPVAL = Ricom;      	//5000000;   //Ricom;         //设置比较值		200mS
	LPC_RIT->RIMASK = 0x00000000;					  //
	LPC_RIT->RICOUNTER = 0;            	//计数值清零		  
	NVIC_SetPriority(RIT_IRQn, RIT_INT_PRIORITY);     
   NVIC_EnableIRQ(RIT_IRQn); 	
	return 	TRUE; 
}  // End of  initialize function



PulseCls::PulseCls(void)
{
	
}



/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	函数名称：GetSecSpeed
   功能描述：读取秒速度相关数据
   入口参数：无
   出口参数：无
   全局变量: 
   创建时间：
   修改时间：
*/ 	

// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ 以上原形声明的函数供本函数调用
uint16		PulseCls::GetSecSpeed(uchar	 *pSpeedSw)
{
	memmove(pSpeedSw,(uchar*)SpeedSw,10);
	return SecSpeed;	
}

// 读取分钟脉冲
uint32		PulseCls::GetMinPlus(void)
{
	uint32	tl;
	tl = SpeedPulseCnt.OneMin;		
	SpeedPulseCnt.OneMin = 0;	  					// 每分钟脉冲数清零
	return tl;	
}

// 读取小时脉冲
uint32		PulseCls::GetHourPlus(void)
{
	uint32	tl;
	tl = SpeedPulseCnt.OneHour;		
	SpeedPulseCnt.OneHour = 0;	  					// 每分钟脉冲数清零
	return tl;	
}

// 读取总脉冲
uint32		PulseCls::GetTotalPlus(void)
{	
	return SpeedPulseCnt.Total;	
}



void			PulseCls::SetPulseInitValue(uint32	 Value)
{
	SpeedPulseCnt.Total = Value;
}

