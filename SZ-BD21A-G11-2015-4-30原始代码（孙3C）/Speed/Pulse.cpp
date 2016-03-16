/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$  Copyright (c)  $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
$											���ݹ�ҵ���ӿƼ����޹�˾
$							GuangZhou GuoYe Electronics Sciense&Technology Co.,LTD.
$                   	http://www.guoyee.com		 Email: guoyegps@126.com
$						 	QQȺ��9069471/9069433/9524253		All rights reserved.	  
$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$  Copyright (c)  $$$$$$$$$$$$$$$$$$$$$$$$$$$ */
/*  
                                      �ļ�����
	�ļ���: 	 	Pulse.h
	����������	�ٶ����崦���������崦��
	���뻷����	
	Ŀ��CPU: 	LPC176x
	���ߣ�
	����ʱ�䣺
	����༭������UltraEdit 11.00a+  �Ʊ�����Ϊ�����ַ�������ɶ��Ը��á�
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

// ���ٶ�����ʱʹ�����ڼ���
#define		USE_USE_PERIOD_CALC_SPEED			1			// 1��ʹ�����ڼ����ٶ�		0: ʹ��������������ٶ�

#if (USE_USE_PERIOD_CALC_SPEED == 1)
#define		LOW_SPEED_USE_PERIOD			1			  
#define		LOW_SPPED_PULSE_DOT2SEC		1000									// 0.2���������
#define		LOW_SPEED_PULSE_FREQUENCY	(LOW_SPEED_PULSE_FRQ*5)		// ������5��������

#else
#define		LOW_SPEED_USE_PERIOD			0
#define		LOW_SPPED_PULSE_DOT2SEC		1000									// 0.2���������
#define		LOW_SPEED_PULSE_FREQUENCY	(LOW_SPEED_PULSE_FRQ*5)		// ������5��������	  

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
	LPC_TIM0->IR = 0x3f;			// ��ȫ���жϱ�־
	NOP();	NOP(); 	
}



/********************************************************************************
** �������ƣ�Timer0_Init()
** ����������Timer2��ʼ��
** ��ڲ�������
** ���ڲ�������
** ȫ�ֱ���: ��
** ����ʱ�䣺2009.05.07
** �޸�ʱ�䣺
***********************************************************************************/ 
void 			PulseCls::Timer0Init(void)
{
	CAP01_PIN_SELECT;
	LPC_TIM0->TC  = 0;				  		  //����������
	//LPC_TIM0->IR = 0;
	LPC_TIM0->CTCR = 0x00;					  //CAP0.1���ڶ�ʱ��ģʽ
	LPC_TIM0->PR  = 0;						  //������25��Ƶ   1uS����һ��
	//LPC_TIM0->MCR = 0x00;
	LPC_TIM0->CCR = (0x05<<3);					  //CAP0.1������׽ģʽ������Ե��׽���������ж�
	NVIC_SetPriority(TIMER0_IRQn, TIMER0_INT_PRIORITY);     
   NVIC_EnableIRQ(TIMER0_IRQn); 	
	LPC_TIM0->TCR = 0x01;					  //��������λ������
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
	LPC_TIM1->IR = 0x3f;			// ��ȫ���жϱ�־
	NOP();	NOP(); 	
}




/********************************************************************************
** �������ƣ�Timer0_Init()
** ����������Timer2��ʼ��
** ��ڲ�������
** ���ڲ�������
** ȫ�ֱ���: ��
** ����ʱ�䣺2009.05.07
** �޸�ʱ�䣺
***********************************************************************************/ 
void 		PulseCls::Timer1Init(void)
{
	
   CAP11_PIN_SELECT;
	LPC_TIM1->TC  = 0;				  		  //����������	
	//LPC_TIM1->IR = 0;
	LPC_TIM1->CTCR = 0x00;					  //CAP1.1���ڶ�ʱ��ģʽ
	LPC_TIM1->PR  = 0;						  //������25��Ƶ   1uS����һ��
	//LPC_TIM1->MCR = 0x00;
	LPC_TIM1->CCR = (0x05<<3);					  //CAP1.1������׽ģʽ������Ե��׽���������ж�
	NVIC_SetPriority(TIMER1_IRQn, TIMER1_INT_PRIORITY);     
   NVIC_EnableIRQ(TIMER1_IRQn); 	
	LPC_TIM1->TCR = 0x01;					  //��������λ������
}







/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	�������ƣ�Timer3Interrupt
   ����������
   ��ڲ�����
   ���ڲ�����
   ȫ�ֱ���: 
   ����ʱ�䣺
   �޸�ʱ�䣺
*/
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ ����ԭ�������ĺ���������������
__inline		void    PulseCls::Timer3Interrupt(void)
{
	uint32	tl;
	if( (LPC_TIM3->IR)&(1<<4) )		// �����ش���
	{
		#if(LOW_SPEED_USE_PERIOD)
					
		InsPlusPeriod = LPC_TIM3->CR0;		// ��0��ʼ����׽ֵ��������	
										
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
	LPC_TIM3->IR = 0x3f;			// ��ȫ���жϱ�־
	NOP();	NOP(); 	
}

/********************************************************************************
** �������ƣ�Timer3Init
** ����������Timer3��ʼ��
** ��ڲ�������
** ���ڲ�������
** ȫ�ֱ���: ��
** ����ʱ�䣺2009.05.07
** �޸�ʱ�䣺
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
	LPC_TIM3->TC  = 0;				  		  //����������	
	//LPC_TIM3->IR = 0;
	LPC_TIM3->CTCR = 0x00;					  //CAP3.0���ڶ�ʱ��ģʽ
	LPC_TIM3->PR  = 0;//Fclk/1000000UL;		  // 1uS����һ��
	//LPC_TIM3->MCR = 0x00;
	LPC_TIM3->CCR = 0x05;					  //CAP3.0������׽ģʽ���½�Ե��׽���������ж�
	NVIC_SetPriority(TIMER3_IRQn, TIMER3_INT_PRIORITY);     
   NVIC_EnableIRQ(TIMER3_IRQn); 
	
	LPC_TIM3->TCR = 0x01;					  //��������λ������

	//////////////////////////////////////////////////
	
	
}
 

// VehiclePulseKV  
__inline		void    PulseCls::RitInterrupt(void)			// 0.2��ִ��1��
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
	///////////////////////////////////////////////////////////////////////////////// ����0.2���˲ʱʱ�٣����¹��ɵ�ʹ��
	tl = SpeedPulseCnt.D2Sec;
	SpeedPulseCnt.D2Sec=0;
	#if(LOW_SPEED_USE_PERIOD)
	if( tl >= LOW_SPPED_PULSE_DOT2SEC )			// 0.2s����������10��ʹ���������
	#endif
	{		
		tl *= (5*3600UL);							// 
		SpeedSw[0].Speed = tl/VehiclePulseKV;		// 
	}
	#if(LOW_SPEED_USE_PERIOD)
	else						// ������С��10��ʹ�����ڼ���
	{
		tl = Timer3Clock/InsPlusPeriod;		// �ó�Ƶ��
		InsPlusPeriod = 0;
		
		#if (WUXI_TEST == 1)
		if (tl>180) tl = 0; // ����180Hz  ���˸��ŵ�	 Ӧ��������⣬��Ϊ�����и��ţ��Ҳ���ԭ��ֱ�ӹ���
		#endif		

		if (tl>(SpeedSw[1].Speed + 150))		// ���˵�ƫ��ܴ�ĵ㣺����150Hz
		{
			tl = SpeedSw[1].Speed;					// ȡ����ֵ
		}
	
		if (TD.Speed==tSPEED_SIMU_SPEED )
		{
			tl = 116; //
		}

		tl *= (3600UL);							// 
		SpeedSw[0].Speed = tl/VehiclePulseKV;		// 
		tl *= 10;
		uiLowSpeed[4] = uiLowSpeed[3];	 	// ��¼����5��0.2S��˲ʱƽ���ٶ�
		uiLowSpeed[3] = uiLowSpeed[2];
		uiLowSpeed[2] = uiLowSpeed[1];
		uiLowSpeed[1] = uiLowSpeed[0];
		uiLowSpeed[0] = tl/VehiclePulseKV;			// 0.1km/h
	}		
	#endif
	
	// �ƶ�
	if (JT808ExtenVsFlag.VS1.Bits.Brake) 	 SpeedSw[0].Sw |= (1<<7);
	else												 SpeedSw[0].Sw &= ~(1<<7);

	// ��ת��
	if (JT808ExtenVsFlag.VS1.Bits.LTurnLight)	SpeedSw[0].Sw |= (1<<6);
	else												 	SpeedSw[0].Sw &= ~(1<<6);

	// ��ת��
	if (JT808ExtenVsFlag.VS1.Bits.RTurnLight)	SpeedSw[0].Sw |= (1<<5);
	else												 	SpeedSw[0].Sw &= ~(1<<5);

	// Զ��
	if (JT808ExtenVsFlag.VS1.Bits.HighLight)	SpeedSw[0].Sw |= (1<<4);
	else												 	SpeedSw[0].Sw &= ~(1<<4);

	// ����
	if (JT808ExtenVsFlag.VS1.Bits.DippedHeadLight)	SpeedSw[0].Sw |= (1<<3);
	else												 			SpeedSw[0].Sw &= ~(1<<3);
	
	///////////////////////////////////////////////////////////////////////////////// ����ÿ���˲ʱʱ��
	RitIntCnt++;
	if(RitIntCnt==5)			// ȡģ������ķ�ʽ����Щ����ΪҪ���������
	{
		RitIntCnt = 0;
				
		tl = uiLowSpeed[0];		tl += uiLowSpeed[1];
		tl += uiLowSpeed[2];		tl += uiLowSpeed[3];	
		tl += uiLowSpeed[4];
		SecSpeed = tl/5;	  		// ÿ���ƽ���ٶ�
				
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
	LPC_RIT->RICTRL |= (1<<0);         //���ж�
  	LPC_RIT->RICOUNTER = 0;            //RIT����ֵ����
   LPC_RIT->RICTRL |= (1<<0)|(1<<3);  //�жϸ�λ������RIT		  
	
	NOP();	NOP(); 	
}


/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	�������ƣ�RitInit
   ����������
   ��ڲ�������
   ���ڲ�������
   ȫ�ֱ���: 
   ����ʱ�䣺
   �޸�ʱ�䣺
*/ 	

// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ ����ԭ�������ĺ���������������


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
	Ricom = Fclk/(1000/mSecend);			 //���ݲ��������ж�ʱ��
	LPC_SC->PCONP |= (1UL<<PCONP_RIT);
	LPC_RIT->RICTRL |= (1<<0);         	//���ж�
	LPC_RIT->RICOMPVAL = Ricom;      	//5000000;   //Ricom;         //���ñȽ�ֵ		200mS
	LPC_RIT->RIMASK = 0x00000000;					  //
	LPC_RIT->RICOUNTER = 0;            	//����ֵ����		  
	NVIC_SetPriority(RIT_IRQn, RIT_INT_PRIORITY);     
   NVIC_EnableIRQ(RIT_IRQn); 	
	return 	TRUE; 
}  // End of  initialize function



PulseCls::PulseCls(void)
{
	
}



/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	�������ƣ�GetSecSpeed
   ������������ȡ���ٶ��������
   ��ڲ�������
   ���ڲ�������
   ȫ�ֱ���: 
   ����ʱ�䣺
   �޸�ʱ�䣺
*/ 	

// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ ����ԭ�������ĺ���������������
uint16		PulseCls::GetSecSpeed(uchar	 *pSpeedSw)
{
	memmove(pSpeedSw,(uchar*)SpeedSw,10);
	return SecSpeed;	
}

// ��ȡ��������
uint32		PulseCls::GetMinPlus(void)
{
	uint32	tl;
	tl = SpeedPulseCnt.OneMin;		
	SpeedPulseCnt.OneMin = 0;	  					// ÿ��������������
	return tl;	
}

// ��ȡСʱ����
uint32		PulseCls::GetHourPlus(void)
{
	uint32	tl;
	tl = SpeedPulseCnt.OneHour;		
	SpeedPulseCnt.OneHour = 0;	  					// ÿ��������������
	return tl;	
}

// ��ȡ������
uint32		PulseCls::GetTotalPlus(void)
{	
	return SpeedPulseCnt.Total;	
}



void			PulseCls::SetPulseInitValue(uint32	 Value)
{
	SpeedPulseCnt.Total = Value;
}

