/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$  Copyright (c)  $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
$												 
$										 
$                       	 		  
$								  		All rights reserved.	  
$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$  Copyright (c)  $$$$$$$$$$$$$$$$$$$$$$$$$$$ */
/*  
                                      �ļ�����
	�ļ���: 	 RTC.pcc
	����������
	���뻷����Keil for ARM + RTX
	Ŀ��CPU:  LPC2000
	���ߣ�
	����ʱ�䣺
	����༭������UltraEdit 11.00a+  �Ʊ�����Ϊ�����ַ�������ɶ��Ը��á�
	ע��LPC2000ϵ�е�RTC�Ĵ�����16���Ƹ�ʽ��PCF8563��RTC�Ĵ�����BCD��ʽ
$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
*/					  

#define			PWM_GLOBALS		1
#include     	"PWM.h"
#include			"..\Config\RTOS.h"
#include			"..\UserSys.h"



/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	�������ƣ�
   ����������
   ��ڲ�����Ƶ�ʣ���λ��0.01Hz��
   ���ڲ�����
   ȫ�ֱ���: 
   �޸�ʱ�䣺
*/ 
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ ����ԭ�������ĺ���������������
void		PwmCls::Init(uint32 S)
{
	uint32	Per,Fclk;	
	///////////////  								UART0����������ر�־����
	NOP();	NOP();	
	LPC_PWM1->TCR	= 0x00;

	if( (S==0)||(S>0xf0000000) )	return;

	Fclk = LPC_SC->PCLKSEL0;
	Fclk &= (3UL<<12);				// B.12.13
	Fclk >>= 12;
	Fclk = PeriClock[Fclk];
	
	LPC_PINCON->PINSEL7 |=((3UL<<18));		// P3.25 for PWM1.2
	
	LPC_PWM1->CTCR = 0;
	
	LPC_PWM1->PR  = 0x00;   				// ��ʱ������Ƶ�ʺ�Fclk��ͬ
	LPC_PWM1->MCR = 0x02;
	
	LPC_PWM1->PCR = ( (1UL<<2) | (1UL<<10) );		// �����أ�ֻʹ��PWM2


	Per = 100000000L;
	Per /= Fclk;				// ÿ������ʱ��0.04us

	Fclk = 100000000L;
	Fclk /= S;		

	Fclk *= 100;			  	// ����

	Fclk /= Per;				// ��Ҫ����ʱ�Ӹ���������ֵ

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
	�������ƣ�SetTargetSpeed
   ������������Time���Ӽ��ٻ���ٵ�S
   ��ڲ�����
   ���ڲ�����
   ȫ�ֱ���: 
   �޸�ʱ�䣺
*/ 
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ ����ԭ�������ĺ���������������
void		PwmCls::SetTargetSpeed(uint32 S,uint32 Time)
{
	uint32	tl;
	NOP();	NOP();
	S *= VehiclePulseKV;	// ��km/h�������Ƶ��
	S /= 36;				// Ƶ�ʣ���λ��0.01Hz	
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
	�������ƣ�
   ����������ģ��������ʻֹͣ�������������塢ACC����������ɲ��
   ��ڲ�����
   ���ڲ�����
   ȫ�ֱ���: 
   �޸�ʱ�䣺
	ע���˺���Ӧ��ÿ200ms����1�Σ���ӦSetTargetSpeed��Time
*/ 
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ ����ԭ�������ĺ���������������
void		PwmCls::SimuSpeedProcess(void)
{
	uchar		t;
	///////////////  								
	NOP();	NOP();
	if(SimuSpeed.Enable != ENABLE_FLAG)		return;
	
	if(NowSpeed>TargetSpeed)		// ����
	{
		if(NowSpeed>Acceleration)	// ��ֹĿ���ٶ�Ϊ0ʱ���������������
			NowSpeed -=	Acceleration;
		else
			NowSpeed =	TargetSpeed;
		Init(NowSpeed);
		SimuSpeed.Brake = TRUE;	// ���٣���Ϊɲ��		
		t = TRUE;
	}
	else if(NowSpeed<TargetSpeed)		// ����
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



