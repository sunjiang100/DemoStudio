/*********************************** Copyright (c) ************************************
**								 广州 电子技术有限公司
**			
**	  
**
************************************ Copyright (c)  ***********************************
**                                      文件描述
**	文件名:  W25P16.H 	 
**	功能描述：
**	编译环境：
**	目标CPU:  
**	作者：    
**	创建时间：
**	建议编辑环境：UltraEdit 11.00a+  制表符宽度为三个字符，程序可读性更好。
****************************************************************************************/
#ifndef      __W25P16_H__
#define      __W25P16_H__

#ifdef      DF_GLOBALS
#define     DF_EXT
#else
#define     DF_EXT    extern
#endif

#define		__W25X64_DEBUG__	0


#include		"..\Config\RTOS.h" 
//#include		"..\Common\UserGlobals.h"



//适用SPI Flash条件：命令相同，以下2个参数相同
#define			DF_BYTE_PER_PAGE		256UL		 //一页256字节
#define			DF_PAGE_PER_BLOCK		256UL		 //一个块256页
#define			DF_PAGE_PER_SECTOR	16
#define			DF_START_PAGE			0			 //起始页
#define			DF_START_SECTOR		0			 //起始扇区

#pragma		pack(1)
typedef	struct
{	
	uint32	TotalByte;
	uint32	TotalPage;
	uint32	TotalBlock;	
	uint32	Model;
} DF_INFO,*pDF_INFO;
	
DF_EXT		DF_INFO		Df;

/*
#define			DF_TOTAL_PAGE			(DfTotalByte/DF_BYTE_PER_PAGE)
#define			DF_TOTAL_BLOCK			(DF_TOTAL_PAGE/DF_PAGE_PER_BLOCK)

#define			DF_TOTAL_PAGE_95PES	(DF_TOTAL_PAGE*95/100)
#define			DF_TOTAL_PAGE_97PES	(DF_TOTAL_PAGE*97/100)
#define			DF_TOTAL_PAGE_99PES	(DF_TOTAL_PAGE*99/100)
*/

/******************W25P16命令枚举*******************************/
DF_EXT	enum		
{
	DF_CMD_WRITE_ENABLE			 = 0x06,				 
	DF_CMD_WRITE_DISABLE        = 0x04,
	DF_CMD_READ_STATUS_REG      = 0x05,
	DF_CMD_WRITE_STATUS_REG     = 0x01,
	DF_CMD_READ_DATA            = 0x03,
	DF_CMD_FAST_READ            = 0x0b,
	DF_CMD_FAST_READ_2PIN		 = 0x3b,
	DF_CMD_PAGE_PROGRAM         = 0x02,
	DF_CMD_SECTOR_ERASE         = 0x20,
	DF_CMD_BLOCK_ERASE          = 0xd8,
	DF_CMD_CHIP_ERASE           = 0xc7,
	DF_CMD_POWER_DOWN				 = 0xb9,
	DF_CMD_READ_PARA_PAGE		 = 0x53,	
	DF_CMD_FAST_READ_PARA_PAGE	 = 0x5b,
	DF_CMD_PROGRAM_PARA_PAGE    = 0x52,
	DF_CMD_ERASE_PARA_PAGE      = 0xd5,
	DF_CMD_RELEASE_POWER_DOWN   = 0xab,
	DF_CMD_MANUFACTURE_ID       = 0x90,
	DF_CMD_JEDEC_ID             = 0x9f,
} DF_CMD,*pDF_CMD;

DF_EXT	uchar	DF_PageBuffer[256+6];
DF_EXT	uchar	UART_Use_DF_Buffer;

class 	DFFCls
{

	
public:
	DFFCls();
	void    	Init();
	void		ReadJEDEC_ID(uchar *pID);
	uint32	JugeModel(uchar *pMID);
	uchar		ReadStatusRegister(void);
	void		WriteStatusRegister(uchar s);	
	uchar		SectorEraseWaitOK(uint32 SectorAddr,uchar Times);
	uchar		BlockErase(uint32 BlockAddr);	
	uchar		ChipErase(void);
	uchar		PageRead(unsigned long PageAddr,uchar *pData);

	uchar    AnyNumberRead(uint32 Addr,uchar DataLen, uchar *pData);
	uchar	   AnyNumberWrite(uint32 Addr,uint16 DataLen,uchar *pData);

	uchar		PageReadFast2PIN(unsigned long PageAddr,uchar *pData);
	uchar		PageProgramWaitOK(unsigned long PageAddr,uchar *pSrc,uchar Times);
	void		PowerDown(void);
	void		ReleasePowerDown(void);
	uchar	   SomeBlockErase(uint32 BlockAddr, uint16 BlockLen);
	uchar	   WaitDFFok(uint16 waittime);
	uchar		PageProgram(unsigned long PageAddr,uchar *pSrc);
private:
	
	uchar		WaitOK_Busing;
	
	uchar		SectorErase(uint32 SectorAddr);
	
	uchar		tPage[260];
};

DF_EXT	__noinit__		DFFCls		DFF;

#endif

/*********************************  End of File **************************************/ 


