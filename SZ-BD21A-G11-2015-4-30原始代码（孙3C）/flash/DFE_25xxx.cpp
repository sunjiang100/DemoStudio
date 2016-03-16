/**************************************Copyright (c)*****************************************
**								���� ���Ӽ������޹�˾
**								
**                     	
**							  
***************************************Copyright (c)*****************************************/
/*  									�ļ�����                                    
** �ļ���: FM25LC04.CPP	 
** ��������������洢��FM25LC04�ײ������ļ�	
** ���뻷����Keil uVision3 + ARM
** Ŀ��CPU:  LPC2109 
** ���ߣ�    
** ����ʱ�䣺2009.04.29
** ����༭������UltraEdit 11.00a+  �Ʊ�����Ϊ�����ַ�������ɶ��Ը��á�
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
** �������ƣ�DF1_ReadStatusRegister()
** ������������״̬�Ĵ���
** ��ڲ�������
** ���ڲ�����״̬
** ȫ�ֱ���: ��
** ȫ�ֺ�����DisableIRQ();NOP();
** ����ʱ�䣺2009.04.29
** �޸�ʱ�䣺
***********************************************************************************************************/ 	
uchar	DFECls::ReadStatusRegister(void)
{
	uchar	s;
	
	while(SPI0_Busy==1)					// ��ȡSPI��ʹ��Ȩ
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
** �������ƣ�WriteStatusRegister()
** ����������д״̬�Ĵ���
** ��ڲ�����״̬s
** ���ڲ�������
** ȫ�ֱ���: ��
** ����ʱ�䣺2009.04.29
** �޸�ʱ�䣺
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
** �������ƣ�Read()
** ������������ָ����ַ����
** ��ڲ�������ʼ��ַ�����ȣ��������ݵĴ洢��ַ��ַ
** ���ڲ������ɹ�����־���ɹ�Ϊ0�����ɹ�Ϊ��0
** ȫ�ֱ���: ��
** ����ʱ�䣺2009.04.29
** �޸�ʱ�䣺
*********************************************************************************************************/ 	
uchar	DFECls::Read(uint16 Addr,uint16 Length,uchar *Data)
{
	uint16	i;
	union		
	{
		uchar	B8[2];
		uint16	B16;
	} Tp;

	while(SPI0_Busy==1)					// ��ȡSPI��ʹ��Ȩ
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
** �������ƣ�Write()
** ������������ָ����ַд������
** ��ڲ�������ʼ��ַ�����ȣ�����
** ���ڲ���������ɹ�����־���ɹ�Ϊ0�����ɹ�Ϊ��0
** ȫ�ֱ���: ��
** ����ʱ�䣺2009.04.29
** �޸�ʱ�䣺
*********************************************************************************************************/ 

uchar	DFECls::Write(uint16 Addr,uint16 Length,uchar *Data)
{
	uint16 i,k,Page,wLen;
	union		
	{
		uchar	B8[2];
		uint16	B16;
	}Tp;

	while(SPI0_Busy==1)					// ��ȡSPI��ʹ��Ȩ
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
	�������ƣ�FM25L256_ReadWord(uint16 Addr)
   ����������дһ���ֵ�ָ���ĵ�ַ
   ��ڲ�������ַ����
   ���ڲ�������
   ȫ�ֱ���: 
   ����ʱ�䣺
   �޸�ʱ�䣺
	ע���洢ԭ�򰴸߸ߵ͵ͣ����Ͷ˶���
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
	�������ƣ�FM25L256_WriteWord(uint16 Addr,uint16 Data)
   ����������дһ���ֵ�ָ���ĵ�ַ
   ��ڲ�������ַ����
   ���ڲ�������
   ȫ�ֱ���: 
   ����ʱ�䣺
   �޸�ʱ�䣺
	ע���洢ԭ�򰴸߸ߵ͵ͣ����Ͷ˶���
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
**�������ƣ�DF1_Init()
**�������ܣ�FM25LC04�ĳ�ʼ��
**�����������
**�����������
**ȫ�ֱ�������
**����ʱ�䣺2009.04.29
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
