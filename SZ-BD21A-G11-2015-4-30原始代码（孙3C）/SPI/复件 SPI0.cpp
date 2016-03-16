/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$  Copyright (c)  $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
$												 
$										 
$                       	 
$								  		All rights reserved.	  
$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$  Copyright (c)  $$$$$$$$$$$$$$$$$$$$$$$$$$$ */
/*  
                                      文件描述
	文件名: 
	功能描述： LPC2xxx  SPI0驱动程序。默认为主模式，如果从模式的函数需加Slaver标识
	编译环境：Keil for ARM + ARTX1.10
	目标CPU:  LPC2000
	作者：
	创建时间：
	建议编辑环境：UltraEdit 11.00a+  制表符宽度为三个字符，程序可读性更好。
$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
*/					  


#define			SPI_GLOBALS			1
#include			"SPI.h"



#include			"..\Common\UserGlobals.h"
#include			"..\Common\CppGlobals.h"





#define			SIMULATE_SPI0		1			// 模拟SPI口
#define			SPI0_CLK				(1UL<<15)
#define			SPI0_MISO			(1UL<<17)
#define			SPI0_MOSI			(1UL<<18)
#define			SPI0_FREQUNCY     1382400

#if(SIMULATE_SPI0)
#define		SPI0_Delay() {NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP()}



//SPI0_DelayForPS2801() {NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP()}

#endif


/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	函数名称：
   功能描述：
   入口参数：
   出口参数：
   全局变量: 
   创建时间：
   修改时间：
*/ 	
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ 以上原形声明的函数供本函数调用
void			SPICls::Init(void)
{
	//PINSEL0 &= ~((1UL<<30)|(1UL<<31));		// config P0.15 as GPIO
	//PINSEL1 &= ~(0x0000003f);					// config P0.16~18 as GPIO
	Busing = 0;
	#if(SIMULATE_SPI0)	
	IODIR0  &= ~(SPI0_MISO);
	IODIR0  |=  SPI0_CLK|SPI0_MOSI;	// out put 			
	IOSET0  |= 	SPI0_CLK|SPI0_MOSI;
	#else
	PINSEL0 |= 0x80000000;						// config P0.15 as SPI port
	PINSEL1 |= ((1UL<<1)|(1UL<<3)|(1UL<<5))   // config P0.16~P0.18 as SPI port
	S0SPCR = 0;						// MSL在先
	S0SPCR = (((uint)(1<<8))|(1<<MSTR));
	S0SPCCR = 0xfe&(Fpclk / SPI0_FREQUNCY);
	#endif
}



/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
    函数名： SPI0_WriteByte
    功能： 从SPI0输出数据
    入口： 待输出的数据
    出口： 输出成功与否标志，成功为0，不成功为非0
*/
/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	函数名称：
   功能描述：
   入口参数：
   出口参数：
   全局变量: 
   创建时间：
   修改时间：
*/ 	
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ 以上原形声明的函数供本函数调用
uchar			SPICls::WriteByte(uchar Data)
{
	#if(SIMULATE_SPI0)
	uint16		i;
	NOP();	NOP();	
	while(Busing);
	Busing = 1;
	IODIR0  |=  SPI0_CLK|SPI0_MOSI;	// out put 	
	SPI0_Delay();
	for(i=0;i<8;i++)
	{
		IOCLR0 |= SPI0_CLK;
		if(Data&0x80)
			IOSET0 |= SPI0_MOSI;
		else
			IOCLR0 |= SPI0_MOSI;
		SPI0_Delay();
		IOSET0 |= SPI0_CLK;
		SPI0_Delay();
		Data <<= 1;					// left shift 1 bit
	}	
	Busing = 0;
	return 0;
	#else
	NOP();	NOP();
	while((S0SPSR&(1<<SPIF)==0));     // wait until SPI transmit complete
	S0SPDR = Data;	
	return 0;	
	#endif
} // End of SPI0_WriteByte Function


/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
    函数名： SPI0_ReadByte
    功能： 从SPI0读取数据
    入口： 无
    出口： 要读取的数据
*/


/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	函数名称：
   功能描述：
   入口参数：
   出口参数：
   全局变量: 
   创建时间：
   修改时间：
*/ 	
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ 以上原形声明的函数供本函数调用
uchar			SPICls::ReadByte(void)
{
	#if(SIMULATE_SPI0)
	uint16		i;
	uint32		pin;
	uchar		Data = 0;	
	while(Busing);
	Busing = 1;
	IODIR0  &= ~(SPI0_MISO);
	IODIR0  |=  SPI0_CLK|SPI0_MOSI;	// out put 	
	SPI0_Delay();
	for(i=0;i<8;i++)
	{
		IOCLR0 |= SPI0_CLK;	// Output are driven from the falling edge of SCK		
		SPI0_Delay();
		Data <<= 1;			
		pin = IOPIN0;
		pin &= (uint32)SPI0_MISO;
		if(pin)
			Data |= 0x01;
		else
			Data &= ~0x01;
		IOSET0 |= SPI0_CLK;		
		NOP();	NOP();
		SPI0_Delay();		
	}
	Busing = 0;
	return Data;
	#else	
	while((S0SPSR&(1<<SPIF)==0));     // wait until SPI transmit complete
	S0SPDR = 0;	
	while((S0SPSR&(1<<SPIF)==0));     // wait until SPI transmit complete	
	return S0SPDR;
	#endif
} // End of SPI0_WriteByte Function













/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	函数名称：
   功能描述：
   入口参数：
   出口参数：
   全局变量: 
   创建时间：
   修改时间：
*/ 	
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ 以上原形声明的函数供本函数调用
uchar			SPICls::ReadByte2PIN(void)
{
	#if(SIMULATE_SPI0)
	uint16		i;
	uint32		pin;
	uchar		Data = 0;	
	while(Busing);
	Busing = 1;
	IODIR0  &= ~(SPI0_MISO|SPI0_MOSI);	
	IODIR0  |=  SPI0_CLK;	// out put 	
	SPI0_Delay();
	for(i=0;i<4;i++)
	{
		IOCLR0 |= SPI0_CLK;	// Output are driven from the falling edge of SCK		
		SPI0_Delay();
				
		pin = IOPIN0;
		
		Data <<= 1;	
		if(pin&((uint32)SPI0_MISO))	// B.7.5.3.1
			Data |= 0x01;
		else
			Data &= ~0x01;
			
		Data <<= 1;	
		if(pin&((uint32)SPI0_MOSI))	// B.6.4.2.0
			Data |= 0x01;
		else
			Data &= ~0x01;
			
		IOSET0 |= SPI0_CLK;		
		NOP();	NOP();
		SPI0_Delay();		
	}
	IODIR0  &= ~(SPI0_MISO);
	IODIR0  |=  SPI0_CLK|SPI0_MOSI;	// out put 	
	Busing = 0;
	return Data;
	
	#else	
	
	while((S0SPSR&(1<<SPIF)==0));     // wait until SPI transmit complete
	S0SPDR = 0;	
	while((S0SPSR&(1<<SPIF)==0));     // wait until SPI transmit complete	
	return S0SPDR;
	#endif
} // End of SPI0_WriteByte Function




/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	函数名称：
   功能描述：
   入口参数：
   出口参数：
   全局变量: 
   创建时间：
   修改时间：
*/ 	
#define				PS2801_CLK		50
extern	"C"		void           Delay_uS(uint16 t);
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ 以上原形声明的函数供本函数调用
uchar			SPICls::WriteReadByte(uchar Data)
{
	#if(SIMULATE_SPI0)
	uint16		i;
	uint32		pin;
	NOP();	NOP();
	while(Busing);
	Busing = 1;	
	IODIR0  &= ~(SPI0_MISO);
	IODIR0  |=  SPI0_CLK|SPI0_MOSI;	// out put 	
	Delay_uS(PS2801_CLK);
	for(i=0;i<8;i++)
	{
		IOCLR0 |= SPI0_CLK;	// Output are driven from the falling edge of SCK				
		
		if(Data&0x80)
			IOSET0 |= SPI0_MOSI;
		else
			IOCLR0 |= SPI0_MOSI;		
		Delay_uS(PS2801_CLK);
		Data <<= 1;			
		
		pin = IOPIN0;
		pin &= (uint32)SPI0_MISO;
		if(pin)
			Data |= 0x01;
		else
			Data &= ~0x01;
					
		IOSET0 |= SPI0_CLK;		
		NOP();	NOP();
		Delay_uS(PS2801_CLK);		
	}
	Busing = 0;
	return Data;
	#else	
	while((S0SPSR&(1<<SPIF)==0));     // wait until SPI transmit complete
	S0SPDR = 0;	
	while((S0SPSR&(1<<SPIF)==0));     // wait until SPI transmit complete	
	return S0SPDR;
	#endif
} // End of SPI0_WriteByte Function





SPICls::SPICls(void)
	{
		
	}





/*
 $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$  End of File $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
 */
