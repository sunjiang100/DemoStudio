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

#include			"W25X64.h"

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
void	DFCls::ReadJEDEC_ID(uchar *pID)
{
	DisableIRQ();
	DF_CSEnable;
	SPI0.WriteByte(DF_CMD_JEDEC_ID);		
	*pID = SPI0.ReadByte();	pID++;
	*pID = SPI0.ReadByte();	pID++;
	*pID = SPI0.ReadByte();	pID++;
	DF_CSDisable;			
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
uint32	 DFCls::JugeModel(uchar *pMID)
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
uchar	DFCls::ReadStatusRegister(void)
{
	uchar	s;

	while(SPI0_Busy==1)					// ��ȡSPI��ʹ��Ȩ
	{ os_dly_wait(1);	}
	SPI0_Busy = 1;
	
	DisableIRQ();
	DF_CSEnable;
	SPI0.WriteByte(DF_CMD_READ_STATUS_REG);
	s = SPI0.ReadByte();	
	DF_CSDisable;
	EnableIRQ();

	SPI0_Busy = 0;
		
	return s;
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
void	DFCls::WriteStatusRegister(uchar s)
{
	DisableIRQ();
	DF_CSEnable;
	SPI0.WriteByte(DF_CMD_WRITE_STATUS_REG);
	SPI0.WriteByte(s);
	DF_CSDisable;
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
void	DFCls::PowerDown(void)
{
	DisableIRQ();
	DF_CSEnable;
	SPI0.WriteByte(DF_CMD_POWER_DOWN);
	DF_CSDisable;
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
void	DFCls::ReleasePowerDown(void)
{
	DisableIRQ();
	DF_CSEnable;
	SPI0.WriteByte(DF_CMD_RELEASE_POWER_DOWN);
	DF_CSDisable;
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
uchar	DFCls::PageRead(unsigned long PageAddr,uchar *pData)
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
	
	Tp.B32 = PageAddr;	
	DisableIRQ();
	DF_CSEnable;
	SPI0.WriteByte(DF_CMD_READ_DATA);
	SPI0.WriteByte(Tp.B8[1]);
	SPI0.WriteByte(Tp.B8[0]);			 
	SPI0.WriteByte(0);	
	for(i=0;i<DF_BYTE_PER_PAGE;i++)
	{
		*pData++ = SPI0.ReadByte();
	}
	DF_CSDisable;
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
/*uchar	DFCls::PageReadFast2PIN(unsigned long PageAddr,uchar *pData)
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
	DF_CSEnable;
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
	DF_CSDisable;
	EnableIRQ();
		
	return 0;
}		  */

extern	"C"		void       Delay_mS(unsigned char  c);
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
uchar	DFCls::PageProgram(unsigned long PageAddr,uchar *pData)
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
	
	DisableIRQ();			
	DF_CSEnable;
	SPI0.WriteByte(DF_CMD_WRITE_ENABLE);			// write enable
	DF_CSDisable;	
	Delay_mS(1);
	Tp.B32 = PageAddr;	
	DF_CSEnable;
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
	DF_CSDisable;		
	EnableIRQ();

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
**********************************************************************************************************/ 	
uchar	DFCls::SectorErase(uint32 SectorAddr)
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
	DF_CSEnable;
	SPI0.WriteByte(DF_CMD_WRITE_ENABLE);			// write enable
	DF_CSDisable;	
	Delay_mS(1);				
	DF_CSEnable;
	Tp.B32 = SectorAddr<<12;							
	SPI0.WriteByte(DF_CMD_SECTOR_ERASE);			// write command
	SPI0.WriteByte(Tp.B8[2]);						
	SPI0.WriteByte(Tp.B8[1]);					
	SPI0.WriteByte(Tp.B8[0]);											
	DF_CSDisable;		
	EnableIRQ();

	SPI0_Busy = 0;
		
	return 0;
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
uchar	DFCls::BlockErase(uint32 BlockAddr)
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
	DF_CSEnable;
	SPI0.WriteByte(DF_CMD_WRITE_ENABLE);		// write enable
	DF_CSDisable;	
	Delay_mS(1);				
	DF_CSEnable;
	Tp.B32 = BlockAddr<<16;
	SPI0.WriteByte(DF_CMD_BLOCK_ERASE);		// write command
	SPI0.WriteByte(Tp.B8[2]);					//����������Ե�ַ
	SPI0.WriteByte(Tp.B8[1]);
	SPI0.WriteByte(Tp.B8[0]);						
	DF_CSDisable;		
	EnableIRQ();

	SPI0_Busy = 0;
		
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
uchar	DFCls::ChipErase(void)
{
	while(SPI0_Busy==1)					// ��ȡSPI��ʹ��Ȩ
	{ os_dly_wait(1);	}
	SPI0_Busy = 1;
	
	DisableIRQ();
	DF_CSEnable;
	SPI0.WriteByte(DF_CMD_WRITE_ENABLE);		// write enable
	DF_CSDisable;	
	Delay_mS(1);								//д��ʱ,�ú����ڹ���ģ����
	DF_CSEnable;
	SPI0.WriteByte(DF_CMD_CHIP_ERASE);			// write command		
	DF_CSDisable;	
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
void   DFCls::Init()
{
	IODIR0  |= (1UL<<21);     
	IOSET0  |= (1UL<<21);	 
//	SPI0.Init();		 //SPI Init...
}


DFCls::DFCls()
{
}

/*********************************************************************************************
**											End of File
**********************************************************************************************/ 
