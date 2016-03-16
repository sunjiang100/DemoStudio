/*********************************** Copyright (c)  *******************************************************
**							    
**										 
**                       	 
**								  		All rights reserved.	  
***********************************  Copyright (c)  *******************************************************
**                                      �ļ�����
**	�ļ���:   SPI1.cpp
**	����������SPI1��������
**	���뻷����Keil uVision3 + ARM
**	Ŀ��CPU:  LPC2000
**	���ߣ�
**	����ʱ�䣺
******************************************************************************************************************/					  

#define		SSP_GLOBALS			1

#include	"SSP.h"

#define     SIMULATE_SPI1        	1

#include	"..\Config\UserConfig.h"

//ʹ��ģ��SPI��,��ֲʱҪ�޸�//////////////
#if(SIMULATE_SPI1)
	#define		SPI1_CLK			(1UL<<20)
	#define		SPI1_MISO			(1UL<<23)
	#define		SPI1_MOSI			(1UL<<24)
	#define		SPI1_Delay()        {NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();}
#else
//	#define		SPI1_FREQUNCY       1382400
#endif



/***************************************************************************************
** �������ƣ�SSP0.Init()
** ����������SPI�ڳ�ʼ��
** ��ڲ�������
** ���ڲ�������
** ȫ�ֱ���: ��
** ����ʱ�䣺2009��04��07��
** �޸�ʱ�䣺
******************************************************************************************/ 	
void	SSPCls::Init(void)
{	
	#if(SIMULATE_SPI1)
	//LPC_PINCON->PINSEL0 &= ~0xc0000000;
	LPC_PINCON->PINSEL3 &= ~0x0003FF00;
	LPC_GPIO1->FIODIR  &= ~(SPI1_MISO);
	LPC_GPIO1->FIODIR  |=  SPI1_CLK|SPI1_MOSI;			// out put 
	LPC_GPIO1->FIOCLR  |=  SPI1_CLK|SPI1_MISO|SPI1_MOSI;		
	#else
	
	PINSEL1|=  0x2a8;   					// config P0.17~P0.19 as SPI port
	SSPCR0  =  (0x01<<8)|  		  			// SCR  ����ʱ������
			   (0x01<<7)|			  		// CPHA Ϊ0����һ��ʱ�ӣ�Ϊ1���ڶ���ʱ��
			   (0x01<<6)|			  		// CPOL Ϊ0���͵�ƽ��Ϊ1���ߵ�ƽ
			   (0x00<<4)|			  		// FRF  Ϊ0��SPI֡��Ϊ1��SSI֡
			   (0x07<<0);			  		// DSS   Ϊ0111��8λ���ݣ�Ϊ1111��16λ����    
	SSPCR1  =  (0x00<<3)|  		  			// SOD   Ϊ0���ӻ������ֹ��Ϊ1���ӻ����ʹ��
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
	S1SPCCR = 0xfe&(Fpclk / SPI1_FREQUNCY);	*/	 			 
	
	#endif
}															   

/***************************************************************************************
** �������ƣ�  SPI1.WriteByte() 
** ���������� 
** ��ڲ����������������
** ���ڲ�������ɹ�����־���ɹ�Ϊ0�����ɹ�Ϊ��0��
** ȫ�ֱ���: 
** ����ʱ�䣺
** �޸�ʱ�䣺
****************************************************************************************/
uchar	SSPCls::WriteByte(uchar Data)
{
	#if(SIMULATE_SPI1)
		uint16		i;
		NOP();	NOP();
		while(Busing)	{os_dly_wait(1);}
		Busing = 1;
//		LPC_GPIO1->FIODIR  &= ~(SPI1_MISO);				 // �����۰�G11�ڶ��������У�	P1.23��λ����ʾ�����ݺ�����(C/D),�������ﲻҪ����
		LPC_GPIO1->FIODIR  |= SPI1_CLK|SPI1_MOSI;			// out put 	
		SPI1_Delay();
		for(i=0;i<8;i++)
		{
			LPC_GPIO1->FIOCLR |= SPI1_CLK;
			if(Data&0x80)
				LPC_GPIO1->FIOSET |= SPI1_MOSI;
			else
				LPC_GPIO1->FIOCLR |= SPI1_MOSI;
			SPI1_Delay();
			LPC_GPIO1->FIOSET |= SPI1_CLK;
			SPI1_Delay();
			Data <<= 1;							// left shift 1 bit
		}
		//LPC_GPIO1->FIOCLR |= SPI1_CLK;
		//LPC_GPIO1->FIOCLR |= SPI1_MOSI;
		Busing = 0;
		return 0;
	#else
		SSPDR = Data;
		while((SSPSR&0x01)==0);     			// wait until SPI transmit complete		
		return (SSPDR);	
	#endif
}


uchar	SSPCls::WriteByte_no_OS(uchar Data)
{
	#if(SIMULATE_SPI1)
		uint16		i;
		NOP();	NOP();
		//while(Busing)	{os_dly_wait(1);}
		//Busing = 1;
		//LPC_GPIO1->FIODIR  &= ~(SPI1_MISO);
		LPC_GPIO1->FIODIR  |= SPI1_CLK;
		LPC_GPIO1->FIODIR  |= SPI1_MOSI;			// out put 	
		SPI1_Delay();
		for(i=0;i<8;i++)
		{
			LPC_GPIO1->FIOCLR |= SPI1_CLK;
			if(Data&0x80)
				LPC_GPIO1->FIOSET |= SPI1_MOSI;
			else
				LPC_GPIO1->FIOCLR |= SPI1_MOSI;
			SPI1_Delay();
			LPC_GPIO1->FIOSET |= SPI1_CLK;
			SPI1_Delay();
			Data <<= 1;							// left shift 1 bit
		}
		//LPC_GPIO1->FIOCLR |= SPI1_CLK;
		//LPC_GPIO1->FIOCLR |= SPI1_MOSI;
		//Busing = 0;		
		return 0;
	#else
		SSPDR = Data;
		while((SSPSR&0x01)==0);     			// wait until SPI transmit complete		
		return (SSPDR);	
	#endif
}


/***************************************************************************************************
** �������ƣ�SPI1.ReadByte()
** �������ܣ���SPI1��ȡ����
** ��ڲ�������
** ���ڲ�����
** ȫ�ֱ���: 
** ����ʱ�䣺
** �޸�ʱ�䣺
*******************************************************************************************/ 	
uchar	SSPCls::ReadByte(void)
{
	#if(SIMULATE_SPI1)
		uint16		i;
		uint32		pin;
		uchar		Data = 0;
		NOP();	NOP();	
		while(Busing)	{os_dly_wait(1);}
		Busing = 1;	
		LPC_GPIO1->FIODIR  &= ~(SPI1_MISO);
		LPC_GPIO1->FIODIR  |= SPI1_CLK|SPI1_MOSI;			// out put 	
		SPI1_Delay();	
		for(i=0;i<8;i++)
		{
			LPC_GPIO1->FIOCLR |= SPI1_CLK;					// Output are driven from the falling edge of SCK
			SPI1_Delay();
			Data <<= 1;			
			pin = LPC_GPIO1->FIOPIN;
			pin &= (uint32)SPI1_MISO;
			if(pin)
				Data |= 0x01;
			else
				Data &= ~0x01;
			LPC_GPIO1->FIOSET |= SPI1_CLK;		
			SPI1_Delay();		
		}
		Busing = 0;	
		return Data;
	#else	
		SSPDR = 0xff;							//��дһ��		
		while((SSPSR&0x01)==0);					//�ȴ�TFE��λ,������FIFO��
		//while((SSPSR&0x04)==0);					
		//tmp = SSPDR;							//������ȡһ�ֽ��ⲿ����
		return(SSPDR);
	#endif
}

uchar	SSPCls::ReadByte_no_OS(void)
{
	#if(SIMULATE_SPI1)
		uint16		i;
		uint32		pin;
		uchar		Data = 0;
		NOP();	NOP();	
		//while(Busing)	{os_dly_wait(1);}
		//Busing = 1;	
		LPC_GPIO1->FIODIR  &= ~(SPI1_MISO);
		LPC_GPIO1->FIODIR  |= SPI1_CLK|SPI1_MOSI;			// out put 	
		SPI1_Delay();	
		for(i=0;i<8;i++)
		{
			LPC_GPIO1->FIOCLR |= SPI1_CLK;					// Output are driven from the falling edge of SCK
			//SPI1_Delay();
			Data <<= 1;			
			pin = LPC_GPIO1->FIOPIN;
			pin &= (uint32)SPI1_MISO;
			if(pin)
				Data |= 0x01;
			else
				Data &= ~0x01;
			LPC_GPIO1->FIOSET |= SPI1_CLK;		
			//SPI1_Delay();		
		}
		//Busing = 0;	
		return Data;
	#else	
		SSPDR = 0xff;							//��дһ��		
		while((SSPSR&0x01)==0);					//�ȴ�TFE��λ,������FIFO��
		//while((SSPSR&0x04)==0);					
		//tmp = SSPDR;							//������ȡһ�ֽ��ⲿ����
		return(SSPDR);
	#endif
}


/**********************************************************************************************
** �������ƣ�SPI1.ReadByte2PIN()
** ����������
** ��ڲ�����
** ���ڲ�����
** ȫ�ֱ���: 
** ����ʱ�䣺
** �޸�ʱ�䣺
************************************************************************************************/
/*uchar	SSPCls::SPI1_ReadByte2PIN(void)	  //???
{
	#if(SIMULATE_SPI0)
	uint16		i;
	uint32		pin;
	uchar		Data = 0;	
	IO0DIR &= ~(SPI1_MISO|SPI1_MOSI);	
	IO0DIR |=  SPI1_CLK;					// out put 	
	SPI1_Delay();
	for(i=0;i<4;i++)
	{
		IO0CLR |= SPI1_CLK;					// Output are driven from the falling edge of SCK		
		SPI1_Delay();				
		pin = IO0PIN;		
		Data <<= 1;	
		if(pin&((uint32)SPI1_MISO))			// B.7.5.3.1
			Data |= 0x01;
		else
			Data &= ~0x01;		
		Data <<= 1;	
		if(pin&((uint32)SPI1_MOSI))			// B.6.4.2.0
			Data |= 0x01;
		else
			Data &= ~0x01;			
		IO0SET |= SPI1_CLK;		
		NOP();	NOP();
		SPI1_Delay();		
	}
	IO0DIR  &= ~(SPI1_MISO);
	IO0DIR  |=  SPI1_CLK|SPI1_MOSI;			// out put 	
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


SSPCls::SSPCls(void)
{

}

/**************************************** End of File *************************************************/
