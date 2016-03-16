/***************************************  Copyright (c)  **************************************
**										���ݹ�ҵ���ӿƼ����޹�˾
**									
**                      
**	  
****************************************  Copyright (c)  **************************************
**                                         �ļ�����
**	�ļ���:   Timer2.cpp
**	����������Timer2�жϷ������.����ɨ����ʾ��.
**	���뻷����Keil for ARM uVision3
**	Ŀ��CPU:  LPC2109

**	����ʱ�䣺2009��05��07��
**  �޸�ʱ��: 
**	����༭������UltraEdit 11.00a+  �Ʊ�����Ϊ�����ַ�������ɶ��Ը��á�
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
** �������ƣ�Timer2_Init()
** ����������Timer2��ʼ��
** ��ڲ�������
** ���ڲ�������
** ȫ�ֱ���: ��
** ����ʱ�䣺2009.05.07
** �޸�ʱ�䣺
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
** �������ƣ�Timer2.Interrupt()
** ����������Timer2�жϷ�����,1S�ж�һ��
** ��ڲ�������
** ���ڲ�������
** ȫ�ֱ���: 
** ����ʱ�䣺
** �޸�ʱ�䣺2012.07.30
**************************************************************************************/ 	
//static	char		tBuf[64];
//extern	"C"		void       Delay_mS(unsigned char  c);

__inline	void	Timer2Cls::Interrupt(void)    // Timer2 interrupt function
{
	//Sys.TaskMonitor();
	if (USBTaskCreat == 1)
		RESET_WATCH_DOG;


	if(GPSValidFlag==GPS_VALID_FLAG)				 // GPS��λ
	{
		SystemLog.GPS.ValidTime++;					 // ��λ��ʱ��
		TempGPSValidTime++;
		InvalidGPSValidTime = 0;
	}
	else
	{
		TempGPSValidTime = 0;
		InvalidGPSValidTime++;
	}

	SystemLog.VDR.SystemWorkTime++;         // ϵͳ�����ۻ�


	
}


Timer2Cls::Timer2Cls(void)
{


	
}

/***********************  End of File *****************************************/
