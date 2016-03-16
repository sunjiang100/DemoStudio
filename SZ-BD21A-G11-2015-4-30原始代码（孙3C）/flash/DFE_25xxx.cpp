/**************************************Copyright (c)*****************************************
**								广州 电子技术有限公司
**								
**                     	
**							  
***************************************Copyright (c)*****************************************/
/*  									文件描述                                    
** 文件名: FM25LC04.CPP	 
** 功能描述：铁电存储器FM25LC04底层驱动文件	
** 编译环境：Keil uVision3 + ARM
** 目标CPU:  LPC2109 
** 作者：    
** 创建时间：2009.04.29
** 建议编辑环境：UltraEdit 11.00a+  制表符宽度为三个字符，程序可读性更好。
******************************************************************************************************/					  
#define			DFE_GLOBALS	    1

#include			"DFE_25xxx.h"

#include			"..\UserSys.h"	

#define			USE_STATUS_REG	1
#define			FLASH_NUM		5

#include			"..\SPI\SPI.H"
#include			"..\uart\uart0.h"
#include			"..\Config\RTOS.h" 
#include			"..\Common\UserGlobals.h"

#if(USE_STATUS_REG)

/***********************************************************************************************************
** 函数名称：DF1_ReadStatusRegister()
** 功能描述：读状态寄存器
** 入口参数：无
** 出口参数：状态
** 全局变量: 无
** 全局函数：DisableIRQ();NOP();
** 创建时间：2009.04.29
** 修改时间：
***********************************************************************************************************/ 	
uchar	DFECls::ReadStatusRegister(void)
{
	uchar	s;
	
	while(SPI0_Busy==1)					// 获取SPI口使用权
	{ os_dly_wait(1);	}
	SPI0_Busy = 1;

	DisableIRQ();
	DFE_CSEnable;
	SPI0.WriteByte(DFE_CMD_READ_STATUS_REG);
	s = SPI0.ReadByte();	
	DFE_CSDisable;
	EnableIRQ();

	SPI0_Busy = 0;
		
	return s;
}


/*********************************************************************************************************
** 函数名称：WriteStatusRegister()
** 功能描述：写状态寄存器
** 入口参数：状态s
** 出口参数：无
** 全局变量: 无
** 创建时间：2009.04.29
** 修改时间：
**********************************************************************************************************/ 	
void	DFECls::WriteStatusRegister(uchar s)
{

	DisableIRQ();
	DFE_CSEnable;
	SPI0.WriteByte(DFE_CMD_WRITE_STATUS_REG);
	SPI0.WriteByte(s);
	DFE_CSDisable;
	EnableIRQ();	
}
#endif

/*********************************************************************************************************
** 函数名称：Read()
** 功能描述：读指定地址数据
** 入口参数：开始地址，长度，读出数据的存储地址首址
** 出口参数：成功与否标志，成功为0，不成功为非0
** 全局变量: 无
** 创建时间：2009.04.29
** 修改时间：
*********************************************************************************************************/ 	
uchar	DFECls::Read(uint16 Addr,uint16 Length,uchar *Data)
{
	uint16	i;
	union		
	{
		uchar	B8[2];
		uint16	B16;
	} Tp;

	while(SPI0_Busy==1)					// 获取SPI口使用权
	{ os_dly_wait(1);	}
	SPI0_Busy = 1;
	
	Tp.B16 = Addr;		
	DisableIRQ();
	DFE_CSEnable;
	SPI0.WriteByte(DFE_CMD_READ_DATA);
	SPI0.WriteByte(Tp.B8[1]);
	SPI0.WriteByte(Tp.B8[0]);
	for(i=0;i<Length;i++)
	{
  		*Data = SPI0.ReadByte();
  		Data++;
	}
	DFE_CSDisable;
	EnableIRQ();
	
	SPI0_Busy = 0;	

	return 0;
}	// End of SPI0_WriteByte Function*/




/*********************************************************************************************************
** 函数名称：Write()
** 功能描述：向指定地址写入数据
** 入口参数：开始地址，长度，数据
** 出口参数：输出成功与否标志，成功为0，不成功为非0
** 全局变量: 无
** 创建时间：2009.04.29
** 修改时间：
*********************************************************************************************************/ 

uchar	DFECls::Write(uint16 Addr,uint16 Length,uchar *Data)
{
	uint16 i,k,Page,wLen;
	union		
	{
		uchar	B8[2];
		uint16	B16;
	}Tp;

	while(SPI0_Busy==1)					// 获取SPI口使用权
	{ os_dly_wait(1);	}
	SPI0_Busy = 1;

	if(Length>EE_PAGE_LEN)
	{
		Page = Length/EE_PAGE_LEN;
		if(Length%EE_PAGE_LEN!=0)
			Page += 1;
	}
	else
	{
		Page = 1;
	}

	for(k=0;k<Page;k++)		
	{
		Tp.B16 = Addr;
		
		if(Length>EE_PAGE_LEN)
		{
			wLen = EE_PAGE_LEN;
			Length -= EE_PAGE_LEN;
		}
		else
			wLen = Length;
		
			
		DisableIRQ();			
		DFE_CSEnable;
		SPI0.WriteByte(DFE_CMD_WRITE_ENABLE);			// write enable
		DFE_CSDisable;	
		Sys.Delay_uS(1);
		DFE_CSEnable;
		SPI0.WriteByte(DFE_CMD_WRITE_DATA);
		SPI0.WriteByte(Tp.B8[1]);
		SPI0.WriteByte(Tp.B8[0]);
		for(i=0;i<wLen;i++)
		{
  			SPI0.WriteByte(*Data);
  			Data++;
		}
		DFE_CSDisable;
		Sys.Delay_uS(1);
		DFE_CSEnable;
		SPI0.WriteByte(DFE_CMD_WRITE_DISABLE);        // write disable command
		DFE_CSDisable;
		EnableIRQ();
		if(osInitFlag)
			os_dly_wait(1);
		else
			Sys.Delay_mS(10);
		Addr += 	EE_PAGE_LEN;
	}
	SPI0_Busy = 0;

	
	return 0;
}




/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	函数名称：FM25L256_ReadWord(uint16 Addr)
   功能描述：写一个字到指定的地址
   入口参数：地址，待
   出口参数：无
   全局变量: 
   创建时间：
   修改时间：
	注：存储原则按高高低低，即低端对齐
*/		  
uint16	DFECls::ReadWord(uint16 Addr)
{
	union
	{
		uint16	Data;
		uchar		HL[2];
	}	Tp;
	NOP();	NOP();
	DFE.Read(Addr,2,Tp.HL);
	NOP();	NOP();
	return Tp.Data;
	
}	// End of <FM25L256_WriteWord> function


/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	函数名称：FM25L256_WriteWord(uint16 Addr,uint16 Data)
   功能描述：写一个字到指定的地址
   入口参数：地址，待
   出口参数：无
   全局变量: 
   创建时间：
   修改时间：
	注：存储原则按高高低低，即低端对齐
*/		  
void	 DFECls::WriteWord(uint16 Addr,uint16	Data)
{
	union
	{
		uint16	Da;
		uchar		HL[2];
	}	Tp;	
	Tp.Da = Data;
	DFE.Write(Addr,2,Tp.HL);
	NOP();	NOP();
}// End of <FM25L256_WriteWord> function


/*************************************************************************************************
**函数名称：DF1_Init()
**函数功能：FM25LC04的初始化
**输入参数：无
**输出参数：无
**全局变量：无
**创建时间：2009.04.29
**************************************************************************************************/
void   DFECls::Init()
{
	//PINSEL1 &= 0xfffffcff;				  //CE
	LPC_GPIO0->FIODIR |= (1UL<<20);
	LPC_GPIO0->FIOSET |= (1UL<<20);
	//SPI0.SPI_Init();
}


DFECls::DFECls()
{
}
/************************************End of File****************************************/ 
