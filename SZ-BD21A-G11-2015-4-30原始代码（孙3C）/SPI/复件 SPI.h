/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$  Copyright (c)  $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
$												 
$										 
$                       	 
$								  		All rights reserved.	  
$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$  Copyright (c)  $$$$$$$$$$$$$$$$$$$$$$$$$$$ */
/*  
                                      文件描述
	文件名: 
	功能描述：
	编译环境：
	目标CPU: 
	作者：
	创建时间：
	建议编辑环境：UltraEdit 11.00a+  制表符宽度为三个字符，程序可读性更好。
$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
*/					  

#ifndef        __SPI_H__
#define			__SPI_H__


#ifdef      SPI_GLOBALS
#define     SPI_EXT
#else
#define     SPI_EXT    extern
#endif

#include		"..\Config\RTOS.h"


///      SPI Control Register (S0SPCR - 0xE0020000)
#define			CPHA			3
#define			CPOL			4
#define			MSTR			5
#define			LSBF			6
#define			SPIE			7


///		SPI Status Register (S0SPSR - 0xE0020004)
#define			ABRT			3
#define			MODF			4
#define			ROVR			5
#define			VCOL			6
#define			SPIF			7			// SPI transfer complete flag.





/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	IIC类
*/
class			SPICls
{
public:
	SPICls(void);
	void			Init(void);
	uchar			WriteByte(uchar Data);
	uchar			ReadByte(void);
	uchar			ReadByte2PIN(void);
	uchar			WriteReadByte(uchar Data);
	uint16		MCP3208_ADC(uchar ch);
private:
	uchar			Busing;
	
};



SPI_EXT		__noinit__		SPICls		SPI;







#endif

/*
 $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$  End of File $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
 */
