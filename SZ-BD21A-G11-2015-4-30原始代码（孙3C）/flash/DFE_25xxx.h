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
				 	
#ifdef      	DFE_GLOBALS
#define     	DFE_EXT
#else
#define    	 	DFE_EXT    extern
#endif

#define			__FM25LC64_DEBUG__		0


//#include			"..\SPI\SPI.H"
//#include			"..\uart\uart0.h"
#include			"..\Config\RTOS.h" 
//#include			"..\Common\UserGlobals.h"
										




/******************FM25LC64命令枚举*******************************/
DFE_EXT	enum		
{
	DFE_CMD_WRITE_ENABLE		 	= 0x06,				 
	DFE_CMD_WRITE_DISABLE        = 0x04,
	DFE_CMD_READ_STATUS_REG      = 0x05,
	DFE_CMD_WRITE_STATUS_REG     = 0x01,
	DFE_CMD_READ_DATA        	 = 0x03,
	DFE_CMD_WRITE_DATA 			 = 0X02,
} DFE_CMD;



class DFECls
{
public:
	DFECls();
	void    	Init();
	uchar		ReadStatusRegister(void);
	void		WriteStatusRegister(uchar s);
	uchar       Read(uint16 Addr,uint16 Length,uchar *Data);
	uchar       Write(uint16 Addr,uint16 Length,uchar *Data);
	////////////////////////////////////////////////////
	uint16	    ReadWord(uint16 Addr);
	void	    WriteWord(uint16 Addr,uint16	Data);
};

DFE_EXT	__noinit__		DFECls		DFE;

#endif

/**************************************End of File*********************************************/ 


