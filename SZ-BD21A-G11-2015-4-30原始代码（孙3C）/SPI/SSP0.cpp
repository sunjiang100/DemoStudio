/*********************************** Copyright (c)  *******************************************************
**							    
**										 
**                       	 
**								  		All rights reserved.	  
***********************************  Copyright (c)  *******************************************************
**                                      文件描述
**	文件名:   SPI1.cpp
**	功能描述：SPI1驱动程序。
**	编译环境：Keil uVision3 + ARM
**	目标CPU:  LPC2000
**	作者：
**	创建时间：
******************************************************************************************************************/					  

#define		SSP_GLOBALS			1

#include	"SSP.h"

#define     SIMULATE_SPI1        	1

#include	"..\Config\UserConfig.h"

//使用模拟SPI口,移植时要修改//////////////
#if(SIMULATE_SPI1)
	#define		SPI1_CLK			(1UL<<20)
	#define		SPI1_MISO			(1UL<<23)
	#define		SPI1_MOSI			(1UL<<24)
	#define		SPI1_Delay()        {NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();}
#else
//	#define		SPI1_FREQUNCY       1382400
#endif



/***************************************************************************************
** 函数名称：SSP0.Init()
** 功能描述：SPI口初始化
** 入口参数：无
** 出口参数：无
** 全局变量: 无
** 创建时间：2009年04月07日
** 修改时间：
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
	SSPCR0  =  (0x01<<8)|  		  			// SCR  串行时钟速率
			   (0x01<<7)|			  		// CPHA 为0，第一个时钟；为1，第二个时钟
			   (0x01<<6)|			  		// CPOL 为0，低电平；为1，高电平
			   (0x00<<4)|			  		// FRF  为0，SPI帧；为1，SSI帧
			   (0x07<<0);			  		// DSS   为0111，8位数据；为1111，16位数据    
	SSPCR1  =  (0x00<<3)|  		  			// SOD   为0，从机输出禁止；为1，从机输出使能
			   (0x00<<2)|			  		// MS    为0，主机；为1，从机
			   (0x01<<1)|			  		// SSE   SSP使能，
			   (0x00<<0);			  		// LBM   回写模式					
	SSPCPSR = 0x1a;					  		// pclk分频值，移植时要修改(1M)
	SSPICR  = 0x00;					  	// 中断清除寄存器
	SSPIMSC = 0X00;								   
		
	/*S1SPCR   =(0<<3)|	   					// CPHA=0,数据在SCK的第一个时钟沿采样
				(0<<4)|	   					// CPOL=0,SCK为高有效
				(1<<5)|	   					// MST1=1,SPI处于主模式
				(0<<6)|	  					// LSBF=0，SPI数据传输MSB（bit7）在先
				(0<<7);	   					// SPIE=0,SPI中断被允许
	S1SPCCR = 0xfe&(Fpclk / SPI1_FREQUNCY);	*/	 			 
	
	#endif
}															   

/***************************************************************************************
** 函数名称：  SPI1.WriteByte() 
** 功能描述： 
** 入口参数：待输出的数据
** 出口参数输出成功与否标志，成功为0，不成功为非0：
** 全局变量: 
** 创建时间：
** 修改时间：
****************************************************************************************/
uchar	SSPCls::WriteByte(uchar Data)
{
	#if(SIMULATE_SPI1)
		uint16		i;
		NOP();	NOP();
		while(Busing)	{os_dly_wait(1);}
		Busing = 1;
//		LPC_GPIO1->FIODIR  &= ~(SPI1_MISO);				 // 在廉价版G11第二，三版中，	P1.23复位于显示的数据和命令(C/D),所以这里不要设置
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
** 函数名称：SPI1.ReadByte()
** 函数功能：从SPI1读取数据
** 入口参数：无
** 出口参数：
** 全局变量: 
** 创建时间：
** 修改时间：
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
		SSPDR = 0xff;							//空写一次		
		while((SSPSR&0x01)==0);					//等待TFE置位,即发送FIFO空
		//while((SSPSR&0x04)==0);					
		//tmp = SSPDR;							//主动读取一字节外部数据
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
		SSPDR = 0xff;							//空写一次		
		while((SSPSR&0x01)==0);					//等待TFE置位,即发送FIFO空
		//while((SSPSR&0x04)==0);					
		//tmp = SSPDR;							//主动读取一字节外部数据
		return(SSPDR);
	#endif
}


/**********************************************************************************************
** 函数名称：SPI1.ReadByte2PIN()
** 功能描述：
** 入口参数：
** 出口参数：
** 全局变量: 
** 创建时间：
** 修改时间：
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
