
/**************************************Copyright (c)*****************************************
**								广州国业电子技术有限公司
**								
**                     	
**							  
***************************************Copyright (c)*****************************************/
/*  									文件描述                                    
** 文件名: FT_GT20L16.CPP	 
** 功能描述：字库芯片驱动	
** 编译环境：MDK4.20
** 目标CPU:  LPC1765
** 作者：    
** 创建时间：2013.10.26
** 建议编辑环境：UltraEdit 11.00a+  制表符宽度为三个字符，程序可读性更好。
******************************************************************************************************/					  
#define			FT_GLOBALS	    1

#include		"FT_GT20L16.h"

#include			"..\UserSys.h"	

#define			USE_STATUS_REG	1
#define			FLASH_NUM		5

#include			"..\SPI\SPI.H"
#include			"..\uart\uart0.h"
#include			"..\Config\RTOS.h" 
#include			"..\Common\UserGlobals.h"




/*********************************************************************************************************
** 函数名称：FastRead()
** 功能描述：快速读指定地址数据
** 入口参数：开始地址，长度，读出数据的存储地址首址, 读取类型
** 出口参数：成功与否标志，成功为0，不成功为非0
** 全局变量: 无
** 创建时间：2013.10.26
** 修改时间：
*********************************************************************************************************/ 	
uchar	FTCls::Read(uint32 Addr,uint16 Length,uchar *Data, uchar ReadType)
{
	uint16	i;
	uchar		FtAddr[3];
	uchar		Dummy = 0xaa;

	while(SPI0_Busy==1)					// 获取SPI口使用权
	{ os_dly_wait(1);	}
	SPI0_Busy = 1;
	
	FtAddr[0] = Addr >> 16;
	FtAddr[1] = (Addr & 0xff00)>>8;
	FtAddr[2] = Addr & 0xff;	
	DisableIRQ();
	FT_CSEnable;
	if (ReadType == FT_FAST_READ)
	{
		SPI0.WriteByte(FT_CMD_FAST_READ_DATA);
	}
	else
	{
			SPI0.WriteByte(FT_CMD_COMMON_READ_DATA);
	}

	SPI0.WriteByte(FtAddr[0]);
	SPI0.WriteByte(FtAddr[1]);
	SPI0.WriteByte(FtAddr[2]);
	
	if (ReadType == FT_FAST_READ)
	{
		SPI0.WriteByte(Dummy);
	}

	for(i=0;i<Length;i++)
	{
  		*Data = SPI0.ReadByte();
  		Data++;
	}
	FT_CSDisable;
	EnableIRQ();
	
	SPI0_Busy = 0;	

	return 0;
}	// End of SPI0_WriteByte Function*/



/*********************************************************************************************************
** 函数名称：CalcFontAddress()
** 功能描述：计算GB2312汉字在字库芯片中的地址
** 入口参数：汉字GB2312编码
** 出口参数：汉字在字库芯片中的地址
** 全局变量: 无
** 创建时间：2013.10.28
** 修改时间：
*********************************************************************************************************/ 	
uint32	FTCls::CalcFontAddress(uint16 FontCode)
{
	uint32	ZFAdd, HZAdd;  // 表示字符和汉字的起始地址
	uint32	MSB,LSB;    // 表示GB2312编码的高字节和低字节
	uint32	GB2312tab;		// 表示GB2312转码表起始地址
	uint32	Address = 0;     // 表示汉字点阵在芯片中的字节地址
	uchar		buffer[2];

	ZFAdd = 	36224;
	HZAdd = 	93452;
	GB2312tab = 87436;

	MSB = (FontCode >> 8) & 0xff;
	LSB = FontCode & 0xff; 

	
	if (MSB >= 0xA1 && MSB <= 0xA5) // 字符区
	{
	
		if (MSB == 0x0A1 && LSB >= 0xA1 && LSB <= 0xBF) 
			Address = 32 * (LSB - 0xA1) + ZFAdd;
		else if (MSB == 0xA3 && LSB >= 0xA1 && LSB <= 0xFE)
			Address = 32 * (31 + LSB - 0xA1) + ZFAdd;
		else if (MSB == 0xA4 && LSB >= 0xA1 && LSB <= 0xF3)
			Address = 32 * (31 + 94 + LSB - 0xA1) + ZFAdd; 
		else if (MSB == 0xA5 && LSB >= 0xA1 && LSB <= 0xF6)
			Address = 32 * (31 + 94 + 83 + LSB - 0xA1) + ZFAdd; 
		else
			Address = ZFAdd;
	}
	else if ((MSB >= 0xB0 && MSB <= 0xD7) && (LSB >=0xA1 && LSB <= 0xFE)) // 汉字 5270
		Address = 32 * ((MSB - 0xB0) * 94 + (LSB - 0xA1) + 1) + HZAdd;
	else if ((MSB >= 0xD8 && MSB <= 0xF7) && (LSB >=0xA1 && LSB <= 0xFE)) // 汉字 5270 ~ 6763
	{
		Address = ((MSB - 0xD8) * 94 + (LSB - 0xA1)) * 2 + GB2312tab;
		Read( Address, 2, buffer,FT_COMMON_READ); //  从字库中读取二个字节
		Address = 32 * (256 * buffer[0] + buffer[1]) + HZAdd;
	}

	return Address;

}
	
	  



/*************************************************************************************************
**函数名称：DF1_Init()
**函数功能：FM25LC04的初始化
**输入参数：无
**输出参数：无
**全局变量：无
**创建时间：2009.04.29
**************************************************************************************************/
void   FTCls::Init()
{
	//PINSEL1 &= 0xfffffcff;				  //CE
	LPC_GPIO2->FIODIR |= (1UL<<13);
	LPC_GPIO2->FIOSET |= (1UL<<13);
	//SPI0.SPI_Init();
}


FTCls::FTCls()
{
}
/************************************End of File****************************************/ 
