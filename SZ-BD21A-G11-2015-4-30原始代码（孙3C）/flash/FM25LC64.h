/***********************************Copyright (c)******************************************
**								 
**								
**                     	
**							  
************************************** Copyright (c) ***************************************
** 
**                                    文件描述
**	文件名:   FM25LC04.C
**	功能描述：FM25LC04的底层驱动程序
**	编译环境：Keil uVision3
**	目标CPU:  LPC2109
**	作者：     
**	创建时间：2009.04.25
**	建议编辑环境：UltraEdit 11.00a+  制表符宽度为三个字符，程序可读性更好。
*********************************************************************************************/
#ifndef      	__FM25LC64_H__
#define      	__FM25LC64_H__
				 	
#ifdef      	DF1_GLOBALS
#define     	DF1_EXT
#else
#define    	 	DF1_EXT    extern
#endif

#define			__FM25LC64_DEBUG__		0


#include			"..\SPI\SPI.H"
#include			"..\uart\uart0.h"
#include			"..\Config\RTOS.h" 
#include			"..\Common\UserGlobals.h"
										
//#define			DF1_CSEnable	{ IODIR0 |= (1UL<<21)|(1UL<<19); 	IOCLR0 |= (1UL<<21);IOSET0 |= (1UL<<19); NOP();	NOP();}
//#define			DF1_CSDisable	{ IODIR0 |= (1UL<<21)|(1UL<<19); 	IOSET0 |= (1UL<<21);IOSET0 |= (1UL<<19); NOP();	NOP();}

#define			DF1_CSEnable		{ IODIR0 |= (1UL<<21); IOCLR0 |= (1UL<<21); IODIR1 |= (1UL<<16); IOSET1 |= (1UL<<16);NOP();	NOP();}
#define			DF1_CSDisable		{ IODIR0 |= (1UL<<21); IOSET0 |= (1UL<<21); IODIR1 |= (1UL<<16); IOSET1 |= (1UL<<16);NOP();	NOP();}


/******************FM25LC64命令枚举*******************************/
DF1_EXT	enum		
{
	DF1_CMD_WRITE_ENABLE		 	= 0x06,				 
	DF1_CMD_WRITE_DISABLE        = 0x04,
	DF1_CMD_READ_STATUS_REG      = 0x05,
	DF1_CMD_WRITE_STATUS_REG     = 0x01,
	DF1_CMD_READ_DATA        	 = 0x03,
	DF1_CMD_WRITE_DATA 			 = 0X02,
} DF1_CMD;



class DF1Cls
{
public:
	DF1Cls();
	void    	Init();
	uchar		ReadStatusRegister(void);
	void		WriteStatusRegister(uchar s);
	uchar    Read(uint16 Addr,uint16 Length,uchar *Data);
	uchar    Write(uint16 Addr,uint16 Length,uchar *Data);
	////////////////////////////////////////////////////
	uint16	ReadWord(uint16 Addr);
	void	   WriteWord(uint16 Addr,uint16	Data);
};

DF1_EXT	__noinit__		DF1Cls		DF1;

#endif

/**************************************End of File*********************************************/ 


