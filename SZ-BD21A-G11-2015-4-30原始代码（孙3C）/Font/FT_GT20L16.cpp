
/**************************************Copyright (c)*****************************************
**								���ݹ�ҵ���Ӽ������޹�˾
**								
**                     	
**							  
***************************************Copyright (c)*****************************************/
/*  									�ļ�����                                    
** �ļ���: FT_GT20L16.CPP	 
** �����������ֿ�оƬ����	
** ���뻷����MDK4.20
** Ŀ��CPU:  LPC1765
** ���ߣ�    
** ����ʱ�䣺2013.10.26
** ����༭������UltraEdit 11.00a+  �Ʊ�����Ϊ�����ַ�������ɶ��Ը��á�
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
** �������ƣ�FastRead()
** �������������ٶ�ָ����ַ����
** ��ڲ�������ʼ��ַ�����ȣ��������ݵĴ洢��ַ��ַ, ��ȡ����
** ���ڲ������ɹ�����־���ɹ�Ϊ0�����ɹ�Ϊ��0
** ȫ�ֱ���: ��
** ����ʱ�䣺2013.10.26
** �޸�ʱ�䣺
*********************************************************************************************************/ 	
uchar	FTCls::Read(uint32 Addr,uint16 Length,uchar *Data, uchar ReadType)
{
	uint16	i;
	uchar		FtAddr[3];
	uchar		Dummy = 0xaa;

	while(SPI0_Busy==1)					// ��ȡSPI��ʹ��Ȩ
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
** �������ƣ�CalcFontAddress()
** ��������������GB2312�������ֿ�оƬ�еĵ�ַ
** ��ڲ���������GB2312����
** ���ڲ������������ֿ�оƬ�еĵ�ַ
** ȫ�ֱ���: ��
** ����ʱ�䣺2013.10.28
** �޸�ʱ�䣺
*********************************************************************************************************/ 	
uint32	FTCls::CalcFontAddress(uint16 FontCode)
{
	uint32	ZFAdd, HZAdd;  // ��ʾ�ַ��ͺ��ֵ���ʼ��ַ
	uint32	MSB,LSB;    // ��ʾGB2312����ĸ��ֽں͵��ֽ�
	uint32	GB2312tab;		// ��ʾGB2312ת�����ʼ��ַ
	uint32	Address = 0;     // ��ʾ���ֵ�����оƬ�е��ֽڵ�ַ
	uchar		buffer[2];

	ZFAdd = 	36224;
	HZAdd = 	93452;
	GB2312tab = 87436;

	MSB = (FontCode >> 8) & 0xff;
	LSB = FontCode & 0xff; 

	
	if (MSB >= 0xA1 && MSB <= 0xA5) // �ַ���
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
	else if ((MSB >= 0xB0 && MSB <= 0xD7) && (LSB >=0xA1 && LSB <= 0xFE)) // ���� 5270
		Address = 32 * ((MSB - 0xB0) * 94 + (LSB - 0xA1) + 1) + HZAdd;
	else if ((MSB >= 0xD8 && MSB <= 0xF7) && (LSB >=0xA1 && LSB <= 0xFE)) // ���� 5270 ~ 6763
	{
		Address = ((MSB - 0xD8) * 94 + (LSB - 0xA1)) * 2 + GB2312tab;
		Read( Address, 2, buffer,FT_COMMON_READ); //  ���ֿ��ж�ȡ�����ֽ�
		Address = 32 * (256 * buffer[0] + buffer[1]) + HZAdd;
	}

	return Address;

}
	
	  



/*************************************************************************************************
**�������ƣ�DF1_Init()
**�������ܣ�FM25LC04�ĳ�ʼ��
**�����������
**�����������
**ȫ�ֱ�������
**����ʱ�䣺2009.04.29
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
