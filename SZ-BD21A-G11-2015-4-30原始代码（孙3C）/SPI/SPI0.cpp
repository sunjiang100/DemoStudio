/*********************************** Copyright (c)  *******************************************************
**							    
**										 
**                       	 
**								  		All rights reserved.	  
***********************************  Copyright (c)  *******************************************************
**                                      �ļ�����
**	�ļ���:   SPI0.cpp
**	����������SPI0��������
**	���뻷����Keil uVision3 + ARM
**	Ŀ��CPU:  LPC2000
**	���ߣ�
**	����ʱ�䣺
******************************************************************************************************************/					  

#define		SPI0_GLOBALS			1

#include		"SPI.h"

#define     SIMULATE_SPI0        1

#include		"..\Config\UserConfig.h"


//ʹ��ģ��SPI��,��ֲʱҪ�޸�//////////////
#if(SIMULATE_SPI0)
	#define		SPI0_CLK			(1UL<<15)
	#define		SPI0_MISO		(1UL<<17)
	#define		SPI0_MOSI		(1UL<<18)
	#define		SPI0_Delay() {NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();}
#else
	//#define		SPI0_FREQUNCY       1382400
#endif



/***************************************************************************************
** �������ƣ�  SPI0.WriteByte() 
** ���������� 
** ��ڲ����������������
** ���ڲ�������ɹ�����־���ɹ�Ϊ0�����ɹ�Ϊ��0��
** ȫ�ֱ���: 
** ����ʱ�䣺
** �޸�ʱ�䣺
****************************************************************************************/
uchar	SPI0Cls::WriteByte(uchar Data)
{	
	#if(SIMULATE_SPI0)
		uint16		i;
		NOP();	NOP();
		while(Busing)	{os_dly_wait(1);}
		Busing = 1;
		LPC_GPIO0->FIODIR  &= ~(SPI0_MISO);
		LPC_GPIO0->FIODIR  |= SPI0_CLK|SPI0_MOSI;			// out put 
		SPI0_Delay();
		for(i=0;i<8;i++)
		{
			LPC_GPIO0->FIOCLR |= SPI0_CLK;
			if(Data&0x80)
				LPC_GPIO0->FIOSET |= SPI0_MOSI;
			else
				LPC_GPIO0->FIOCLR |= SPI0_MOSI;
			SPI0_Delay();
			LPC_GPIO0->FIOSET |= SPI0_CLK;
			SPI0_Delay();
			Data <<= 1;							// left shift 1 bit
		}	
		Busing = 0;
		return 0;
	#else
		NOP();	NOP();
		while(Busing)	{os_dly_wait(1);}
		Busing = 1;
		SSPDR = Data;
		while((SSPSR&0x01)==0);     			// wait until SPI transmit complete		
		Busing = 0;
		return (SSPDR);	
	#endif
	
}


/***************************************************************************************************
** �������ƣ�SPI0.ReadByte()
** �������ܣ���SPI0��ȡ����
** ��ڲ�������
** ���ڲ�����
** ȫ�ֱ���: 
** ����ʱ�䣺
** �޸�ʱ�䣺
*******************************************************************************************/ 	
uchar	SPI0Cls::ReadByte(void)
{
	#if(SIMULATE_SPI0)
		uint16		i;
		uint32		pin;
		uchar		Data = 0;
		NOP();	NOP();
		while(Busing)	{os_dly_wait(1);}
		Busing = 1;
		LPC_GPIO0->FIODIR  &= ~(SPI0_MISO);
		LPC_GPIO0->FIODIR  |= SPI0_CLK|SPI0_MOSI;			// out put 
		SPI0_Delay();	
		for(i=0;i<8;i++)
		{
			LPC_GPIO0->FIOCLR |= SPI0_CLK;					// Output are driven from the falling edge of SCK		
			SPI0_Delay();
			Data <<= 1;			
			pin = LPC_GPIO0->FIOPIN;
			pin &= (uint32)SPI0_MISO;
			if(pin)
				Data |= 0x01;
			else
				Data &= ~0x01;
			LPC_GPIO0->FIOSET |= SPI0_CLK;		
			NOP();	NOP();
			SPI0_Delay();		
		}
		Busing = 0;
		return Data;
	#else	
		
		NOP();	NOP();
		while(Busing)	{os_dly_wait(1);}
		Busing = 1;

		SSPDR = 0xff;							//��дһ��		
		while((SSPSR&0x01)==0);					//�ȴ�TFE��λ,������FIFO��
		//while((SSPSR&0x04)==0);					
		//tmp = SSPDR;							//������ȡһ�ֽ��ⲿ����
		Busing = 0;
		return(SSPDR);
	#endif
}


/**********************************************************************************************
** �������ƣ�SPI0.ReadByte2PIN()
** ����������
** ��ڲ�����
** ���ڲ�����
** ȫ�ֱ���: 
** ����ʱ�䣺
** �޸�ʱ�䣺
************************************************************************************************/
/*uchar	SPI0Cls::SPI0_ReadByte2PIN(void)	  //???
{
	#if(SIMULATE_SPI0)
	uint16		i;
	uint32		pin;
	uchar		Data = 0;	
	IODIR0 &= ~(SPI0_MISO|SPI0_MOSI);	
	IODIR0 |=  SPI0_CLK;					// out put 	
	SPI0_Delay();
	for(i=0;i<4;i++)
	{
		IOCLR0 |= SPI0_CLK;					// Output are driven from the falling edge of SCK		
		SPI0_Delay();				
		pin = IO0PIN;		
		Data <<= 1;	
		if(pin&((uint32)SPI0_MISO))			// B.7.5.3.1
			Data |= 0x01;
		else
			Data &= ~0x01;		
		Data <<= 1;	
		if(pin&((uint32)SPI0_MOSI))			// B.6.4.2.0
			Data |= 0x01;
		else
			Data &= ~0x01;			
		IOSET0 |= SPI0_CLK;		
		NOP();	NOP();
		SPI0_Delay();		
	}
	IODIR0  &= ~(SPI0_MISO);
	IODIR0  |=  SPI0_CLK|SPI0_MOSI;			// out put 	
	return Data;
	
	#else	
	uchar dat;
	while((SSPSR&(0x01<<2)==0));     		// wait until SPI transmit complete
	SSPDR = 0;	
	while((SSPSR&(0x01<<2)==0));     		// wait until SPI transmit complete	
	dat=(uchar)SSPDR;
	return dat;
	#endif
}		 */


 /***************************************************************************************
** �������ƣ�SPI0.Init()
** ����������SPI�ڳ�ʼ��
** ��ڲ�������
** ���ڲ�������
** ȫ�ֱ���: ��
** ����ʱ�䣺2009��04��07��
** �޸�ʱ�䣺
******************************************************************************************/ 	
void	SPI0Cls::Init(void)
{	
	#if(SIMULATE_SPI0)
	LPC_PINCON->PINSEL0 &= ~0xc0000000;
	LPC_PINCON->PINSEL1 &= ~0x0000003f;
	LPC_GPIO0->FIODIR  &= ~(SPI0_MISO);
	LPC_GPIO0->FIODIR  |=  SPI0_CLK|SPI0_MOSI;			// out put 			  
	#else	
	PINSEL1|=  0x2a8;   					// config P0.17~P0.19 as SPI port
	SSPCR0  =  (0x01<<8)|  		  			// SCR  ����ʱ������
			   (0x01<<7)|			  		// CPHA Ϊ0����һ��ʱ�ӣ�Ϊ1���ڶ���ʱ��
			   (0x01<<6)|			  		// CPOL Ϊ0���͵�ƽ��Ϊ1���ߵ�ƽ
			   (0x00<<4)|			  		// FRF  Ϊ0��SPI֡��Ϊ1��SSI֡
			   (0x07<<0);			  		// DSS   Ϊ0111��8λ���ݣ�Ϊ1111��16λ����    
	SSPCR1  =(0x00<<3)|  		  			// SOD   Ϊ0���ӻ������ֹ��Ϊ1���ӻ����ʹ��
			   (0x00<<2)|			  		// MS    Ϊ0��������Ϊ1���ӻ�
			   (0x01<<1)|			  		// SSE   SSPʹ�ܣ�
			   (0x00<<0);			  		// LBM   ��дģʽ					
	SSPCPSR = 0x1a;					  		// pclk��Ƶֵ����ֲʱҪ�޸�(1M)
	SSPICR  = 0x00;					  	// �ж�����Ĵ���
	SSPIMSC = 0X00;								   
		
	/*S1SPCR   =(0<<3)|	   					// CPHA=0,������SCK�ĵ�һ��ʱ���ز���
				(0<<4)|	   					// CPOL=0,SCKΪ����Ч
				(1<<5)|	   					// MST1=1,SPI������ģʽ
				(0<<6)|	  					// LSBF=0��SPI���ݴ���MSB��bit7������
				(0<<7);	   					// SPIE=0,SPI�жϱ�����
	S1SPCCR = 0xfe&(Fpclk / SPI0_FREQUNCY);	*/	 			 
	
	#endif
}															   


SPI0Cls::SPI0Cls(void)
{

}

/**************************************** End of File *************************************************/
