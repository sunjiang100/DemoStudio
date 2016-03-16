/***************************************  Copyright (c)  **************************************
**										广州国业电子科技有限公司
**									
**                      
**	  
****************************************  Copyright (c)  **************************************
**                                         文件描述
**	文件名:   Timer2.cpp
**	功能描述：Timer2中断服务程序.用于扫描显示屏.
**	编译环境：Keil for ARM uVision3
**	目标CPU:  LPC2109

**	创建时间：2009年05月07日
**  修改时间: 
**	建议编辑环境：UltraEdit 11.00a+  制表符宽度为三个字符，程序可读性更好。
*************************************************************************************************/				

#define			TIMER2_GLOBALS		1

#include		"Timer2.h"
#include		"..\KeyDisp\KeyDisp.h"
#include		"..\UART\UART0.h"
#include		"..\RTC\RTC.h"
#include		"..\Common\CppGlobals.h"
#include		"..\UserSys.h"
#include		<string.h>
#include			"..\common\Algol.H"

#define			EANBLE_TASK_MONITOR			1



extern	"C"	void	TIMER2_IRQHandler(void) 	__irq
{
	
	Timer2.Interrupt();

	LPC_TIM2->IR = 0x01;
	//VICVectAddr = 0x00;	
}

 
/********************************************************************************
** 函数名称：Timer2_Init()
** 功能描述：Timer2初始化
** 入口参数：无
** 出口参数：无
** 全局变量: 无
** 创建时间：2009.05.07
** 修改时间：
***********************************************************************************/ 
//#define		CLK_FOR_TIMER2			(Fcclk/8)

void Timer2Cls::Init(uint32 Clock)
{	
	uint32	Fclk;

	Fclk = LPC_SC->PCLKSEL1;
	Fclk &= (3UL<<12);					// B.12.13
	Fclk >>= 6;
	Fclk = PeriClock[Fclk];

	LPC_TIM2->TC  = 0;
	LPC_TIM2->PR  = 0;
	LPC_TIM2->MCR = 0x03;
	LPC_TIM2->MR0 = (Fclk);  //1S
	LPC_TIM2->TCR = 0x01;

	NVIC_SetPriority(TIMER2_IRQn, TIMER2_INT_PRIORITY);     
   NVIC_EnableIRQ(TIMER2_IRQn); 

}
 
 
/**************************************************************************************
** 函数名称：Timer2.Interrupt()
** 功能描述：Timer2中断服务函数,1S中断一次
** 入口参数：无
** 出口参数：无
** 全局变量: 
** 创建时间：
** 修改时间：2012.07.30
**************************************************************************************/ 	
//static	char		tBuf[64];
//extern	"C"		void       Delay_mS(unsigned char  c);

__inline	void	Timer2Cls::Interrupt(void)    // Timer2 interrupt function
{
	//Sys.TaskMonitor();
	if (USBTaskCreat == 1)
		RESET_WATCH_DOG;


	if(GPSValidFlag==GPS_VALID_FLAG)				 // GPS定位
	{
		SystemLog.GPS.ValidTime++;					 // 定位总时间
		TempGPSValidTime++;
		InvalidGPSValidTime = 0;
	}
	else
	{
		TempGPSValidTime = 0;
		InvalidGPSValidTime++;
	}

	SystemLog.VDR.SystemWorkTime++;         // 系统工作累积


	
}


Timer2Cls::Timer2Cls(void)
{


	
}

/***********************  End of File *****************************************/
