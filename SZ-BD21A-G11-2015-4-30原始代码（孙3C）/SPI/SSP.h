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

#ifndef        __SSP_H__
#define			__SSP_H__


#ifdef      SSP_GLOBALS
#define     SSP_EXT
#else
#define     SSP_EXT    extern
#endif

#include 		"..\Config\RTOS.h"              /* ARTX kernel functions & defines     */

///      SPI Control Register (S0SPCR - 0xE0020000)
/*
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
*/

/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	SPI类
*/
class			SSPCls
{
public:
	SSPCls(void);
	void		Init(void);	 
	uchar		WriteByte(uchar Data);		
	uchar		ReadByte(void);

	uchar    WriteByte_no_OS(uchar Data);

	uchar		ReadByte_no_OS(void);
	//uchar		SPI1_ReadByte2PIN(void);

private:
	uchar		Busing;

};

SSP_EXT	__noinit__		SSPCls		SSP0;


#endif

/*
 $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$  End of File $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
 */
