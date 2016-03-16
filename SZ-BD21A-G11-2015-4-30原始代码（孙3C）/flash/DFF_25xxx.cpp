						   /*****************************************Copyright (c)*****************************************
**										广州 电子技术有限公司
**									 
**                     	 
**						  		All rights reserved.	  
******************************************Copyright (c)*****************************************/
/*  										文件描述                                    
** 文件名:   W25P16.C	 
** 功能描述：W25P16.C底层驱动文件	
** 编译环境：Keil uVision3 + ARM
** 目标CPU:  LPC2109 
** 作者：    
** 创建时间：
** 注：可替换的SPI FLash条件：每页256字节，每个擦除块256页，否则出错。
******************************************************************************************************/					  
#define			DF_GLOBALS	    1

#include			"DFF_25xxx.h"

#include			"..\UserSys.h"	
#include		   "..\SPI\SPI.H"
#include 		"..\Uart\Uart0.h"

#define			FLASH_NUM		5

//生产商ID和器件ID
#pragma		pack(1)
const	uint32	MID_Tab[FLASH_NUM] = 
{
	0x00ef2015,0x00ef3015,0x00ef3016,0x00ef3017,
	0x00202015
};

//器件容量总字节数，必须与上面数组一一对应
const	uint32	DfTotalByteTab[FLASH_NUM] = 
{ 
	2097152L,2097152L,4194304L,8388608L,
	2097152L
};			


/*******************************************************************************
** 函数名称：DF_ReadJEDEC_ID()
** 功能描述：读W26X16的信息，包括厂商与型号
** 入口参数：存储返回信息的指针pID
** 出口参数：无
** 全局变量: 无
** 创建时间：
** 修改时间：2009.04.17
*********************************************************************************/ 	
void	DFFCls::ReadJEDEC_ID(uchar *pID)
{
	DisableIRQ();
	DFF_CSEnable;
	SPI0.WriteByte(DF_CMD_JEDEC_ID);		
	*pID = SPI0.ReadByte();	pID++;
	*pID = SPI0.ReadByte();	pID++;
	*pID = SPI0.ReadByte();	pID++;
	DFF_CSDisable;			
	EnableIRQ();	
}


/*******************************************************************************
** 函数名称：DF_JugeModel()
** 功能描述：
** 入口参数：存储返回信息的指针pID
** 出口参数：无
** 全局变量: 无
** 创建时间：
** 修改时间：2009.04.17
*********************************************************************************/ 
uint32	 DFFCls::JugeModel(uchar *pMID)
{
	uint32	i;
	#pragma		pack(1)
	union
	{  
		uchar	B8[16];
		uint32	B32[4];
	} MID;

	ReadJEDEC_ID(MID.B8);		
	MID.B8[4] = MID.B8[2];
	MID.B8[5] = MID.B8[1];
	MID.B8[6] = MID.B8[0];
	MID.B8[7] = 0;
	Df.Model  = MID.B32[1];			// 型号
	for(i=0;i<FLASH_NUM;i++)
	{
		if(MID.B32[1]==MID_Tab[i] )
			break;
	}
	Df.TotalByte = DfTotalByteTab[i];		
	Df.TotalPage = Df.TotalByte/DF_BYTE_PER_PAGE;
	Df.TotalBlock = Df.TotalPage/DF_PAGE_PER_BLOCK;
	MID.B32[0] = Df.Model;
	MID.B32[1] = Df.TotalByte;
	MID.B32[2] = Df.TotalPage;
	MID.B32[3] = Df.TotalBlock;
	if(pMID != NULL)
	{
		for(i=0;i<16;i++)
		{	*(pMID+i) = MID.B8[i];	}
		return 16;
	}
	return 0;
}


/***********************************************************************************************************
** 函数名称：ReadStatusRegister()
** 功能描述：读状态寄存器
** 入口参数：无
** 出口参数：状态
** 全局变量: 无
** 全局函数：DisableIRQ();NOP();
** 创建时间：2007-8-14 12:00
** 修改时间：
***********************************************************************************************************/ 	
uchar	DFFCls::ReadStatusRegister(void)
{
	uchar	s;

	while(SPI0_Busy==1)					// 获取SPI口使用权
	{ os_dly_wait(1);	}
	SPI0_Busy = 1;
	
	DisableIRQ();
	DFF_CSEnable;
	SPI0.WriteByte(DF_CMD_READ_STATUS_REG);
	s = SPI0.ReadByte();	
	DFF_CSDisable;
	EnableIRQ();

	SPI0_Busy = 0;
		
	return s;
}




uchar	DFFCls::WaitDFFok(uint16 waittime)
{
   uint16  i;
	for(i=0;i<waittime;i++)				// 等待完成 (不能用while),10s
   {																				
      if(((DFF.ReadStatusRegister())&0x01)!=0)					 	
      {os_dly_wait(1); }	
      else
      {return OK;  }
   }
	return ERROR;
}
/*********************************************************************************************************
** 函数名称：DF_WriteStatusRegister()
** 功能描述：写状态寄存器
** 入口参数：状态s
** 出口参数：无
** 全局变量: 无
** 创建时间：2007-8-14 12:03
** 修改时间：
**********************************************************************************************************/ 	
void	DFFCls::WriteStatusRegister(uchar s)
{
	DisableIRQ();
	DFF_CSEnable;
	SPI0.WriteByte(DF_CMD_WRITE_STATUS_REG);
	SPI0.WriteByte(s);
	DFF_CSDisable;
	EnableIRQ();	
}


/**********************************************************************************************************
** 函数名称：PowerDown()
** 功能描述：
** 入口参数：无
** 出口参数：无
** 全局变量: 无
** 创建时间：2007-12-14 16:47
** 修改时间：
**********************************************************************************************************/ 	
void	DFFCls::PowerDown(void)
{
	DisableIRQ();
	DFF_CSEnable;
	SPI0.WriteByte(DF_CMD_POWER_DOWN);
	DFF_CSDisable;
	EnableIRQ();
}


/***********************************************************************************************************
** 函数名称：ReleasePowerDown()
** 功能描述：
** 入口参数：无
** 出口参数：无
** 全局变量: 无
** 创建时间：2007-12-14 16:47
** 修改时间：
***********************************************************************************************************/ 	
void	DFFCls::ReleasePowerDown(void)
{
	DisableIRQ();
	DFF_CSEnable;
	SPI0.WriteByte(DF_CMD_RELEASE_POWER_DOWN);
	DFF_CSDisable;
	EnableIRQ();
}

/*********************************************************************************************************
** 函数名称：PageRead
** 功能描述：指定页地址读数据
** 入口参数：页地址(取值为0-8191)，数据存储指针
** 出口参数：
** 全局变量: 无
** 创建时间：2007-8-14 10:46
** 修改时间：
** 注：每页固定长度字节为256,共8192页。页地址为4字节长整形，不清零，循环使用
**     命令为参数页命令或普通存储页命令
*********************************************************************************************************/ 	
uchar	DFFCls::PageRead(unsigned long PageAddr,uchar *pData)
{
	uint16	i;
	union		
	{
		uchar		B8[4];
		uint32	B32;
	} Tp;

	while(SPI0_Busy==1)					// 获取SPI口使用权
	{ os_dly_wait(1);	}
	SPI0_Busy = 1;
	
	Tp.B32 = PageAddr;	
	DisableIRQ();
	DFF_CSEnable;
	SPI0.WriteByte(DF_CMD_READ_DATA);
	SPI0.WriteByte(Tp.B8[1]);
	SPI0.WriteByte(Tp.B8[0]);			 
	SPI0.WriteByte(0);	
	for(i=0;i<DF_BYTE_PER_PAGE;i++)
	{
		*pData++ = SPI0.ReadByte();
	}
	DFF_CSDisable;
	EnableIRQ();

	SPI0_Busy = 0;
		
	return 0;
}




/*********************************************************************************************************
** 函数名称：AnyNumberRead
** 功能描述：指定任意地址读数据
** 入口参数：任意地址，数据长度（uchar），数据存储指针
** 出口参数：
** 全局变量: 无
** 创建时间：2007-8-14 10:46
** 修改时间：
** 
*********************************************************************************************************/ 	
uchar	DFFCls::AnyNumberRead(uint32 Addr,uchar DataLen, uchar *pData)									//王
{
	uint16	i;
	union		
	{
		uchar	B8[4];
		uint32	B32;
	} Tp;

	while(SPI0_Busy==1)					// 获取SPI口使用权
	{ os_dly_wait(1);	}
	SPI0_Busy = 1;
	
	Tp.B32 = Addr;	
	DisableIRQ();
	DFF_CSEnable;
	SPI0.WriteByte(DF_CMD_READ_DATA);
	SPI0.WriteByte(Tp.B8[2]);
	SPI0.WriteByte(Tp.B8[1]);			 
	SPI0.WriteByte(Tp.B8[0]);	
	for(i=0;i<DataLen;i++)
	{
		*pData++ = SPI0.ReadByte();
	}
	DFF_CSDisable;
	EnableIRQ();

	SPI0_Busy = 0;
		
	return 0;
}

/*********************************************************************************************************
** 函数名称：PageRead
** 功能描述：指定页地址读数据
** 入口参数：页地址(取值为0-8191),数据存储指针
** 出口参数：
** 全局变量: 无
** 创建时间：2007-8-14 10:46
** 修改时间：
** 注：每页固定长度字节为256,,共8192页。页地址为4字节长整形，不清零，循环使用
**     命令为参数页命令或普通存储页命令
*********************************************************************************************************/ 	
/*uchar	DFFCls::PageReadFast2PIN(unsigned long PageAddr,uchar *pData)
{
	uint16	i;
	union		
	{
		uchar	B8[4];
		uint32	B32;
	} Tp;

	if( (Df.Model == 0x00202015)||(Df.Model==0x00ef2015) )			// 有些器件不支持双引脚输出快速读
	{
		DF_PageRead(PageAddr,pData);
		return 0;
	}	
	Tp.B32 = PageAddr;	
	DisableIRQ();
	DFF_CSEnable;
	SPI1.SPI1_WriteByte(DF_CMD_FAST_READ_2PIN);//Cmd);
	SPI1.SPI1_WriteByte(Tp.B8[1]);
	SPI1.SPI1_WriteByte(Tp.B8[0]);
	SPI1.SPI1_WriteByte(0);										  
	i = SPI1.SPI1_ReadByte();			// dummy byte	
	for(i=0;i<DF_BYTE_PER_PAGE;i++)
	{
		*pData = SPI1.SPI1_ReadByte2PIN();						  ///////////////////////
		pData++;
	}
	DFF_CSDisable;
	EnableIRQ();
		
	return 0;
}		  */


/********************************************************************************************
** 函数名称：DF_PageProgram()
** 功能描述：页编程，把数据写到指定的页
** 入口参数：页地址PageAddr(取值为0-8191)，数据
** 出口参数：成功返回0
** 全局变量: 无
** 全局函数: Sys.Delay_mS();DisableIRQ();EnableIRQ();NOP();
** 创建时间：2007-8-14 11:02
** 修改时间：
** 注：每页固定长度字节为256,,共8192页。页地址为4字节长整形，不清零，循环使用
**     命令为参数页命令或普通存储页命令(编程前要先擦除)
***********************************************************************************************/
uchar		DFFCls::PageProgram(unsigned long PageAddr,uchar *pData)
{
	uint16	i;
	union		
	{
		uchar	B8[4];
		uint32	B32;
	} Tp;

	while(SPI0_Busy==1)					// 获取SPI口使用权
	{ os_dly_wait(1);	}
	SPI0_Busy = 1;
	
	if (FTPdowningflag != TRUE)		//
		DisableIRQ();			
	DFF_CSEnable;
	SPI0.WriteByte(DF_CMD_WRITE_ENABLE);			// write enable
	DFF_CSDisable;	
	Sys.Delay_mS(1);
	Tp.B32 = PageAddr;	
	DFF_CSEnable;
	SPI0.WriteByte(DF_CMD_PAGE_PROGRAM);			// write command
	SPI0.WriteByte(Tp.B8[1]);
	SPI0.WriteByte(Tp.B8[0]);
	SPI0.WriteByte(0);									
	for(i=0;i<(DF_BYTE_PER_PAGE/2);i++)					// Write Data
	{
		SPI0.WriteByte(*pData);
		pData++;
		SPI0.WriteByte(*pData);
		pData++;
	}
	DFF_CSDisable;
	if (FTPdowningflag != TRUE)	// 	
		EnableIRQ();

	SPI0_Busy = 0;
		
	return 0;	
} 







uchar		DFFCls::PageProgramWaitOK(unsigned long PageAddr,uchar *pSrc,uchar Times)
{
	
	uint32	i,t,OKCnt;
	
	while(WaitOK_Busing)	{	os_dly_wait(1);	}
	WaitOK_Busing = TRUE;

	if( (Times==0)||(Times>10) ) 	Times = 3;
	
	
	for(t=0;t<Times;t++)
	{
		PageProgram(PageAddr,pSrc);		// 典型写时间为1.4ms
		WaitDFFok(10);
		PageRead(PageAddr,tPage);
		for(OKCnt=0,i=0;i<256;i++)
		{
			if( tPage[i] == (*(pSrc+i)) )		OKCnt++;			
		}
		if( OKCnt>250) 		// 正确，返回
		{	
			WaitOK_Busing = 0;
			return OK;
		}		
	}
	WaitOK_Busing = 0;
	return ERROR;
}













/********************************************************************************************
** 函数名称：DF_AnyNumberWrite()
** 功能描述：页编程，把数据写到指定的页
** 入口参数：页地址PageAddr(取值为0-8191)，数据
** 出口参数：成功返回0
** 全局变量: 无
** 全局函数: Sys.Delay_mS();DisableIRQ();EnableIRQ();NOP();
** 创建时间：2007-8-14 11:02
** 修改时间：
** 注：每页固定长度字节为256,,共8192页。页地址为4字节长整形，不清零，循环使用
**     命令为参数页命令或普通存储页命令(编程前要先擦除)
***********************************************************************************************/ 
uchar	DFFCls::AnyNumberWrite(uint32 Addr, uint16 DataLen,uchar *pData)
{
	uint16	i;
	union		
	{
		uchar	B8[4];
		uint32	B32;
	} Tp;

	while(SPI0_Busy==1)					// 获取SPI口使用权
	{ os_dly_wait(1);	}
	SPI0_Busy = 1;
	
	//DisableIRQ();			
	DFF_CSEnable;
	SPI0.WriteByte(DF_CMD_WRITE_ENABLE);			// write enable
	DFF_CSDisable;	
	Sys.Delay_mS(1);
	Tp.B32 = Addr;	
	DFF_CSEnable;
	SPI0.WriteByte(DF_CMD_PAGE_PROGRAM);			// write command
	SPI0.WriteByte(Tp.B8[2]);
	SPI0.WriteByte(Tp.B8[1]);			 
	SPI0.WriteByte(Tp.B8[0]);							
	for(i=0;i<DataLen;i++)					// Write Data
	{
		SPI0.WriteByte(*pData);
		pData++;
		//SPI0.WriteByte(*pData);
		//pData++;
	}
	DFF_CSDisable;		
	//EnableIRQ();

	SPI0_Busy = 0;
		
	return 0;	
}

/**********************************************************************************************************
** 函数名称：SectorErase()
** 功能描述：擦除指定的扇区
** 入口参数：扇区地址Sector(取值为0-511)
** 出口参数：
** 全局变量: 无
** 全局函数:Sys.Delay_mS();DisableIRQ();EnableIRQ();
** 创建时间：2007-8-14 11:45
** 修改时间：2009.04.17
** 其它：	 W25X16每个扇区的大小为4K，共512个扇区。注：有的芯片不支持此命令
********************************************************************************************************* */	
uchar	DFFCls::SectorErase(uint32 SectorAddr)
{
	union		
	{
		uchar	B8[4];
		uint32	B32;
	} Tp;

	while(SPI0_Busy==1)					// 获取SPI口使用权
	{ os_dly_wait(1);	}
	SPI0_Busy = 1;
	
	DisableIRQ();
	DFF_CSEnable;
	SPI0.WriteByte(DF_CMD_WRITE_ENABLE);			// write enable
	DFF_CSDisable;	
	Sys.Delay_mS(1);				
	DFF_CSEnable;
	Tp.B32 = SectorAddr<<12;							
	SPI0.WriteByte(DF_CMD_SECTOR_ERASE);			// write command
	SPI0.WriteByte(Tp.B8[2]);						
	SPI0.WriteByte(Tp.B8[1]);					
	SPI0.WriteByte(Tp.B8[0]);											
	DFF_CSDisable;		
	EnableIRQ();

	SPI0_Busy = 0;
		
	return 0;
}





uchar		DFFCls::SectorEraseWaitOK(uint32 SectorAddr,uchar Times)
{	
	uchar		WriteOK;
	uint32	i,j,t,Addr;
	
	while(WaitOK_Busing)	{	os_dly_wait(1);	}
	WaitOK_Busing = TRUE;
	
	if( (Times==0)||(Times>10) ) 	Times = 3;

	for(t=0;t<Times;t++)
	{
		WaitDFFok(100);
		SectorErase( SectorAddr );			
		WaitDFFok(100);
		WriteOK = OK;
		Addr = SectorAddr*DF_PAGE_PER_SECTOR;
		for(i=0;i<DF_PAGE_PER_SECTOR;i++)
		{
			PageRead(Addr+i,tPage);
			for(j=0;j<256;j++)
			{
				if(tPage[j]!=0xff)	
				{
					WriteOK = ERROR;	break;
				}
			}
			if(WriteOK==ERROR)	break;	
		}
		if(WriteOK==OK)
		{
			WaitOK_Busing = 0;
			return OK;
		}
	}	
	WaitOK_Busing = 0;
	return ERROR;
}













/**********************************************************************************************************
** 函数名称：BlockErase()
** 功能描述：擦除指定的块
** 入口参数：块地址BlockAddr(取值为0-31)
** 出口参数：
** 全局变量: 无
** 全局函数: Sys.Delay_mS();DisableIRQ();EnableIRQ();
** 创建时间：2007-8-14 11:45
** 修改时间：2009.04.17	 14:02
** 其它：    W25X16每块的大小为64K，共32个块。
**********************************************************************************************************/ 	
uchar	DFFCls::BlockErase(uint32 BlockAddr)
{
	union		
	{
		uchar	B8[4];
		uint32	B32;
	} Tp;

	while(SPI0_Busy==1)					// 获取SPI口使用权
	{ os_dly_wait(1);	}
	SPI0_Busy = 1;
	
	DisableIRQ();
	DFF_CSEnable;
	SPI0.WriteByte(DF_CMD_WRITE_ENABLE);		// write enable
	DFF_CSDisable;	
	Sys.Delay_mS(1);				
	DFF_CSEnable;
	Tp.B32 = BlockAddr<<16;
	SPI0.WriteByte(DF_CMD_BLOCK_ERASE);		// write command
	SPI0.WriteByte(Tp.B8[2]);					//输入的是线性地址
	SPI0.WriteByte(Tp.B8[1]);
	SPI0.WriteByte(Tp.B8[0]);						
	DFF_CSDisable;		
	EnableIRQ();

	SPI0_Busy = 0;
		
	return 0;
}

/**********************************************************************************************************
** 函数名称：SomeBlockErase()
** 功能描述：擦除指定几个块
** 入口参数：块地址BlockAddr(取值为0-31)，块数
** 出口参数：
** 全局变量: 无
** 全局函数: Sys.Delay_mS();DisableIRQ();EnableIRQ();
** 创建时间：2007-8-14 11:45
** 修改时间：2009.04.17	 14:02
** 其它：    W25X16每块的大小为64K，共32个块。
**********************************************************************************************************/ 	
uchar	DFFCls::SomeBlockErase(uint32 BlockAddr, uint16 BlockLen)
{
	uint16  t;

	for(t = 0; t<BlockLen; t++)
	{
		WaitDFFok(500);
		BlockErase(BlockAddr+t);
		WaitDFFok(200);
	
	}
		
	return 0;
}
/************************************************************************************************
** 函数名称：ChipErase()
** 功能描述：整片擦除
** 入口参数：无
** 出口参数：操作成功返回零
** 全局变量: 无
** 全局函数:Sys.Delay_mS();DisableIRQ();EnableIRQ();
** 创建时间：
** 修改时间：2009.04.17
**************************************************************************************************/ 	
uchar	DFFCls::ChipErase(void)
{
	while(SPI0_Busy==1)					// 获取SPI口使用权
	{ os_dly_wait(1);	}
	SPI0_Busy = 1;
	
	DisableIRQ();
	DFF_CSEnable;
	SPI0.WriteByte(DF_CMD_WRITE_ENABLE);		// write enable
	DFF_CSDisable;	
	Sys.Delay_mS(1);								//写延时,该函数在公共模块中
	DFF_CSEnable;
	SPI0.WriteByte(DF_CMD_CHIP_ERASE);			// write command		
	DFF_CSDisable;	
	EnableIRQ();

	SPI0_Busy = 0;
		
	return 0;
}



/*************************************************************************************************
**函数名称：Init()
**函数功能：W25X16的初始化
**输入参数：无
**输出参数：无
**全局变量：无
**创建时间：2009.04.17
**************************************************************************************************/
void   DFFCls::Init()
{
	LPC_GPIO0->FIODIR  |= (1UL<<21);     
	LPC_GPIO0->FIOSET  |= (1UL<<21);	 
	WaitOK_Busing = 0;
//	SPI0.Init();		 //SPI Init...
}


DFFCls::DFFCls()
{
}

/*********************************************************************************************
**											End of File
**********************************************************************************************/ 
