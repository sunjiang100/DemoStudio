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
#define			DF1_GLOBALS	    1

#include			"FM25LC64.h"
#define			USE_STATUS_REG	1
#define			FLASH_NUM		5



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
uchar	DF1Cls::ReadStatusRegister(void)
{
	uchar	s;
	
	while(SPI0_Busy==1)					// 获取SPI口使用权
	{ os_dly_wait(1);	}
	SPI0_Busy = 1;

	DisableIRQ();
	DF1_CSEnable;
	SPI0.WriteByte(DF1_CMD_READ_STATUS_REG);
	s = SPI0.ReadByte();	
	DF1_CSDisable;
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
void	DF1Cls::WriteStatusRegister(uchar s)
{

	DisableIRQ();
	DF1_CSEnable;
	SPI0.WriteByte(DF1_CMD_WRITE_STATUS_REG);
	SPI0.WriteByte(s);
	DF1_CSDisable;
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
uchar	DF1Cls::Read(uint16 Addr,uint16 Length,uchar *Data)
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
	DF1_CSEnable;
	SPI0.WriteByte(DF1_CMD_READ_DATA);
	SPI0.WriteByte(Tp.B8[1]);
	SPI0.WriteByte(Tp.B8[0]);
	for(i=0;i<Length;i++)
	{
  		*Data = SPI0.ReadByte();
  		Data++;
	}
	DF1_CSDisable;
	EnableIRQ();
	
	SPI0_Busy = 0;	

	return 0;
}	// End of SPI0_WriteByte Function*/



extern	"C"		void       Delay_uS(unsigned char  c);
/*********************************************************************************************************
** 函数名称：Write()
** 功能描述：向指定地址写入数据
** 入口参数：开始地址，长度，数据
** 出口参数：输出成功与否标志，成功为0，不成功为非0
** 全局变量: 无
** 创建时间：2009.04.29
** 修改时间：
*********************************************************************************************************/ 
uchar	DF1Cls::Write(uint16 Addr,uint16 Length,uchar *Data)
{
	uint16 i;
	union		
	{
		uchar	B8[2];
		uint16	B16;
	}Tp;

	while(SPI0_Busy==1)					// 获取SPI口使用权
	{ os_dly_wait(1);	}
	SPI0_Busy = 1;
	
	Tp.B16 = Addr;	
	DisableIRQ();			
	DF1_CSEnable;
	SPI0.WriteByte(DF1_CMD_WRITE_ENABLE);			// write enable
	DF1_CSDisable;	
	Delay_uS(1);
	DF1_CSEnable;
	SPI0.WriteByte(DF1_CMD_WRITE_DATA);
	SPI0.WriteByte(Tp.B8[1]);
	SPI0.WriteByte(Tp.B8[0]);
	for(i=0;i<Length;i++)
	{
  		SPI0.WriteByte(*Data);
  		Data++;
	}
	DF1_CSDisable;
	Delay_uS(1);
	DF1_CSEnable;
	SPI0.WriteByte(DF1_CMD_WRITE_DISABLE);        // write disable command
	DF1_CSDisable;
	EnableIRQ();	

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
uint16	DF1Cls::ReadWord(uint16 Addr)
{
	union
	{
		uint16	Data;
		uchar		HL[2];
	}	Tp;
	NOP();	NOP();
	DF1.Read(Addr,2,Tp.HL);
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
void	 DF1Cls::WriteWord(uint16 Addr,uint16	Data)
{
	union
	{
		uint16	Da;
		uchar		HL[2];
	}	Tp;	
	Tp.Da = Data;
	DF1.Write(Addr,2,Tp.HL);
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
void   DF1Cls::Init()
{
	//PINSEL1 &= 0xfffffcff;				  //CE
	IODIR0 |= (1UL<<20);
	IOSET0 |= (1UL<<20);
	//SPI0.SPI_Init();
}


DF1Cls::DF1Cls()
{
}
/************************************End of File****************************************/ 
