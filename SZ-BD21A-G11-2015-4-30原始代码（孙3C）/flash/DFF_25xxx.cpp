						   /*****************************************Copyright (c)*****************************************
**										���� ���Ӽ������޹�˾
**									 
**                     	 
**						  		All rights reserved.	  
******************************************Copyright (c)*****************************************/
/*  										�ļ�����                                    
** �ļ���:   W25P16.C	 
** ����������W25P16.C�ײ������ļ�	
** ���뻷����Keil uVision3 + ARM
** Ŀ��CPU:  LPC2109 
** ���ߣ�    
** ����ʱ�䣺
** ע�����滻��SPI FLash������ÿҳ256�ֽڣ�ÿ��������256ҳ���������
******************************************************************************************************/					  
#define			DF_GLOBALS	    1

#include			"DFF_25xxx.h"

#include			"..\UserSys.h"	
#include		   "..\SPI\SPI.H"
#include 		"..\Uart\Uart0.h"

#define			FLASH_NUM		5

//������ID������ID
#pragma		pack(1)
const	uint32	MID_Tab[FLASH_NUM] = 
{
	0x00ef2015,0x00ef3015,0x00ef3016,0x00ef3017,
	0x00202015
};

//�����������ֽ�������������������һһ��Ӧ
const	uint32	DfTotalByteTab[FLASH_NUM] = 
{ 
	2097152L,2097152L,4194304L,8388608L,
	2097152L
};			


/*******************************************************************************
** �������ƣ�DF_ReadJEDEC_ID()
** ������������W26X16����Ϣ�������������ͺ�
** ��ڲ������洢������Ϣ��ָ��pID
** ���ڲ�������
** ȫ�ֱ���: ��
** ����ʱ�䣺
** �޸�ʱ�䣺2009.04.17
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
** �������ƣ�DF_JugeModel()
** ����������
** ��ڲ������洢������Ϣ��ָ��pID
** ���ڲ�������
** ȫ�ֱ���: ��
** ����ʱ�䣺
** �޸�ʱ�䣺2009.04.17
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
	Df.Model  = MID.B32[1];			// �ͺ�
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
** �������ƣ�ReadStatusRegister()
** ������������״̬�Ĵ���
** ��ڲ�������
** ���ڲ�����״̬
** ȫ�ֱ���: ��
** ȫ�ֺ�����DisableIRQ();NOP();
** ����ʱ�䣺2007-8-14 12:00
** �޸�ʱ�䣺
***********************************************************************************************************/ 	
uchar	DFFCls::ReadStatusRegister(void)
{
	uchar	s;

	while(SPI0_Busy==1)					// ��ȡSPI��ʹ��Ȩ
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
	for(i=0;i<waittime;i++)				// �ȴ���� (������while),10s
   {																				
      if(((DFF.ReadStatusRegister())&0x01)!=0)					 	
      {os_dly_wait(1); }	
      else
      {return OK;  }
   }
	return ERROR;
}
/*********************************************************************************************************
** �������ƣ�DF_WriteStatusRegister()
** ����������д״̬�Ĵ���
** ��ڲ�����״̬s
** ���ڲ�������
** ȫ�ֱ���: ��
** ����ʱ�䣺2007-8-14 12:03
** �޸�ʱ�䣺
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
** �������ƣ�PowerDown()
** ����������
** ��ڲ�������
** ���ڲ�������
** ȫ�ֱ���: ��
** ����ʱ�䣺2007-12-14 16:47
** �޸�ʱ�䣺
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
** �������ƣ�ReleasePowerDown()
** ����������
** ��ڲ�������
** ���ڲ�������
** ȫ�ֱ���: ��
** ����ʱ�䣺2007-12-14 16:47
** �޸�ʱ�䣺
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
** �������ƣ�PageRead
** ����������ָ��ҳ��ַ������
** ��ڲ�����ҳ��ַ(ȡֵΪ0-8191)�����ݴ洢ָ��
** ���ڲ�����
** ȫ�ֱ���: ��
** ����ʱ�䣺2007-8-14 10:46
** �޸�ʱ�䣺
** ע��ÿҳ�̶������ֽ�Ϊ256,��8192ҳ��ҳ��ַΪ4�ֽڳ����Σ������㣬ѭ��ʹ��
**     ����Ϊ����ҳ�������ͨ�洢ҳ����
*********************************************************************************************************/ 	
uchar	DFFCls::PageRead(unsigned long PageAddr,uchar *pData)
{
	uint16	i;
	union		
	{
		uchar		B8[4];
		uint32	B32;
	} Tp;

	while(SPI0_Busy==1)					// ��ȡSPI��ʹ��Ȩ
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
** �������ƣ�AnyNumberRead
** ����������ָ�������ַ������
** ��ڲ����������ַ�����ݳ��ȣ�uchar�������ݴ洢ָ��
** ���ڲ�����
** ȫ�ֱ���: ��
** ����ʱ�䣺2007-8-14 10:46
** �޸�ʱ�䣺
** 
*********************************************************************************************************/ 	
uchar	DFFCls::AnyNumberRead(uint32 Addr,uchar DataLen, uchar *pData)									//��
{
	uint16	i;
	union		
	{
		uchar	B8[4];
		uint32	B32;
	} Tp;

	while(SPI0_Busy==1)					// ��ȡSPI��ʹ��Ȩ
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
** �������ƣ�PageRead
** ����������ָ��ҳ��ַ������
** ��ڲ�����ҳ��ַ(ȡֵΪ0-8191),���ݴ洢ָ��
** ���ڲ�����
** ȫ�ֱ���: ��
** ����ʱ�䣺2007-8-14 10:46
** �޸�ʱ�䣺
** ע��ÿҳ�̶������ֽ�Ϊ256,,��8192ҳ��ҳ��ַΪ4�ֽڳ����Σ������㣬ѭ��ʹ��
**     ����Ϊ����ҳ�������ͨ�洢ҳ����
*********************************************************************************************************/ 	
/*uchar	DFFCls::PageReadFast2PIN(unsigned long PageAddr,uchar *pData)
{
	uint16	i;
	union		
	{
		uchar	B8[4];
		uint32	B32;
	} Tp;

	if( (Df.Model == 0x00202015)||(Df.Model==0x00ef2015) )			// ��Щ������֧��˫����������ٶ�
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
** �������ƣ�DF_PageProgram()
** ����������ҳ��̣�������д��ָ����ҳ
** ��ڲ�����ҳ��ַPageAddr(ȡֵΪ0-8191)������
** ���ڲ������ɹ�����0
** ȫ�ֱ���: ��
** ȫ�ֺ���: Sys.Delay_mS();DisableIRQ();EnableIRQ();NOP();
** ����ʱ�䣺2007-8-14 11:02
** �޸�ʱ�䣺
** ע��ÿҳ�̶������ֽ�Ϊ256,,��8192ҳ��ҳ��ַΪ4�ֽڳ����Σ������㣬ѭ��ʹ��
**     ����Ϊ����ҳ�������ͨ�洢ҳ����(���ǰҪ�Ȳ���)
***********************************************************************************************/
uchar		DFFCls::PageProgram(unsigned long PageAddr,uchar *pData)
{
	uint16	i;
	union		
	{
		uchar	B8[4];
		uint32	B32;
	} Tp;

	while(SPI0_Busy==1)					// ��ȡSPI��ʹ��Ȩ
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
		PageProgram(PageAddr,pSrc);		// ����дʱ��Ϊ1.4ms
		WaitDFFok(10);
		PageRead(PageAddr,tPage);
		for(OKCnt=0,i=0;i<256;i++)
		{
			if( tPage[i] == (*(pSrc+i)) )		OKCnt++;			
		}
		if( OKCnt>250) 		// ��ȷ������
		{	
			WaitOK_Busing = 0;
			return OK;
		}		
	}
	WaitOK_Busing = 0;
	return ERROR;
}













/********************************************************************************************
** �������ƣ�DF_AnyNumberWrite()
** ����������ҳ��̣�������д��ָ����ҳ
** ��ڲ�����ҳ��ַPageAddr(ȡֵΪ0-8191)������
** ���ڲ������ɹ�����0
** ȫ�ֱ���: ��
** ȫ�ֺ���: Sys.Delay_mS();DisableIRQ();EnableIRQ();NOP();
** ����ʱ�䣺2007-8-14 11:02
** �޸�ʱ�䣺
** ע��ÿҳ�̶������ֽ�Ϊ256,,��8192ҳ��ҳ��ַΪ4�ֽڳ����Σ������㣬ѭ��ʹ��
**     ����Ϊ����ҳ�������ͨ�洢ҳ����(���ǰҪ�Ȳ���)
***********************************************************************************************/ 
uchar	DFFCls::AnyNumberWrite(uint32 Addr, uint16 DataLen,uchar *pData)
{
	uint16	i;
	union		
	{
		uchar	B8[4];
		uint32	B32;
	} Tp;

	while(SPI0_Busy==1)					// ��ȡSPI��ʹ��Ȩ
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
** �������ƣ�SectorErase()
** ��������������ָ��������
** ��ڲ�����������ַSector(ȡֵΪ0-511)
** ���ڲ�����
** ȫ�ֱ���: ��
** ȫ�ֺ���:Sys.Delay_mS();DisableIRQ();EnableIRQ();
** ����ʱ�䣺2007-8-14 11:45
** �޸�ʱ�䣺2009.04.17
** ������	 W25X16ÿ�������Ĵ�СΪ4K����512��������ע���е�оƬ��֧�ִ�����
********************************************************************************************************* */	
uchar	DFFCls::SectorErase(uint32 SectorAddr)
{
	union		
	{
		uchar	B8[4];
		uint32	B32;
	} Tp;

	while(SPI0_Busy==1)					// ��ȡSPI��ʹ��Ȩ
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
** �������ƣ�BlockErase()
** ��������������ָ���Ŀ�
** ��ڲ��������ַBlockAddr(ȡֵΪ0-31)
** ���ڲ�����
** ȫ�ֱ���: ��
** ȫ�ֺ���: Sys.Delay_mS();DisableIRQ();EnableIRQ();
** ����ʱ�䣺2007-8-14 11:45
** �޸�ʱ�䣺2009.04.17	 14:02
** ������    W25X16ÿ��Ĵ�СΪ64K����32���顣
**********************************************************************************************************/ 	
uchar	DFFCls::BlockErase(uint32 BlockAddr)
{
	union		
	{
		uchar	B8[4];
		uint32	B32;
	} Tp;

	while(SPI0_Busy==1)					// ��ȡSPI��ʹ��Ȩ
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
	SPI0.WriteByte(Tp.B8[2]);					//����������Ե�ַ
	SPI0.WriteByte(Tp.B8[1]);
	SPI0.WriteByte(Tp.B8[0]);						
	DFF_CSDisable;		
	EnableIRQ();

	SPI0_Busy = 0;
		
	return 0;
}

/**********************************************************************************************************
** �������ƣ�SomeBlockErase()
** ��������������ָ��������
** ��ڲ��������ַBlockAddr(ȡֵΪ0-31)������
** ���ڲ�����
** ȫ�ֱ���: ��
** ȫ�ֺ���: Sys.Delay_mS();DisableIRQ();EnableIRQ();
** ����ʱ�䣺2007-8-14 11:45
** �޸�ʱ�䣺2009.04.17	 14:02
** ������    W25X16ÿ��Ĵ�СΪ64K����32���顣
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
** �������ƣ�ChipErase()
** ������������Ƭ����
** ��ڲ�������
** ���ڲ����������ɹ�������
** ȫ�ֱ���: ��
** ȫ�ֺ���:Sys.Delay_mS();DisableIRQ();EnableIRQ();
** ����ʱ�䣺
** �޸�ʱ�䣺2009.04.17
**************************************************************************************************/ 	
uchar	DFFCls::ChipErase(void)
{
	while(SPI0_Busy==1)					// ��ȡSPI��ʹ��Ȩ
	{ os_dly_wait(1);	}
	SPI0_Busy = 1;
	
	DisableIRQ();
	DFF_CSEnable;
	SPI0.WriteByte(DF_CMD_WRITE_ENABLE);		// write enable
	DFF_CSDisable;	
	Sys.Delay_mS(1);								//д��ʱ,�ú����ڹ���ģ����
	DFF_CSEnable;
	SPI0.WriteByte(DF_CMD_CHIP_ERASE);			// write command		
	DFF_CSDisable;	
	EnableIRQ();

	SPI0_Busy = 0;
		
	return 0;
}



/*************************************************************************************************
**�������ƣ�Init()
**�������ܣ�W25X16�ĳ�ʼ��
**�����������
**�����������
**ȫ�ֱ�������
**����ʱ�䣺2009.04.17
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
