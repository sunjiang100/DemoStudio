/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$  Copyright (c)  $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
$												���ݹ�ҵ���ӿƼ����޹�˾
$										Shawyoo Electronics (GuangZhou) R&D Center
$                       	http://www.guoyee.com		 Email: guoyegps@126.com
$								 QQȺ��9069471/9069433/9524253		All rights reserved.	  
$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$  Copyright (c)  $$$$$$$$$$$$$$$$$$$$$$$$$$$ */
/*  
                                      �ļ�����
	�ļ���: 	  IC_Card4442.c
	���������� 
	���뻷���� 
	Ŀ��CPU:   LPC2000
	���ߣ�	  Ф����
	����ʱ�䣺 
	����༭������UltraEdit 11.00a+  �Ʊ�����Ϊ�����ַ�������ɶ��Ը��á�
					  Keil�п������ù���
$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
*/					  



#include		"IIC0.h"
#include		"IC_Card.h"

#include		"..\UART\UART0.h"

#include		"..\UserSys.h"		
#include		<string.h>


void			IICCls::Test4442Card(void)
{
	uchar		tBuffer[34];
	strcpy((char*)tBuffer,"0123456789ABCDEF0123456789ABCDEF\0");
	Reset4442Card();	
	Write4442Card(0,32,tBuffer);
	
}




/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	�������ƣ�
   ����������
   ��ڲ�����
   ���ڲ������޿�����0,AT24C������1,4442������2
   ȫ�ֱ���: 
   ����ʱ�䣺
   �޸�ʱ�䣺
	
*/ 	
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ ����ԭ�������ĺ���������������
void        IICCls::CheckIC_Card(void)
{
	uint32	tl;
	union
	{
		uint32	B32[2];
		uchar 	Buf[8];
	} Tp;

	if((GET_IC_CARD_STATUS)==0)		// IC���Ѳ��� �����ڳ���������  
	//if((GET_IC_CARD_STATUS)!=0)		// IC���Ѳ���	(���ڳ��տ���)
	{		
		if(gCnt.InsertIC_Card==0)
		{					
			IC_CardType = 0;
			os_dly_wait(10);
			RESET_WATCH_DOG;

			if (I2C_Busy_Now_Flag == 1)	{	os_dly_wait(1);}
			I2C_Busy_Now_Flag = 1;

			Current_Use_4442_Flag = 0;
			I2C0_PIN_SELECT;
			os_dly_wait(1);

	//{"GPSBD00000000000\xD5\xC5\xC8\xFD\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00123456789012345678\x15\x12\x31\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00");

			//I2C0.ReadEEPROM(EE_DA_AT24C01_16_IC_CARD,0,4,Tp.Buf);
			I2C0.ReadEEPROM(CARD_DEVICE_ADDR,0,5,Tp.Buf);
			Uart.puts("Insert IC card \0");
			//if( (Tp.Buf[0]=='G')&&(Tp.Buf[1]=='P')&&(Tp.Buf[2]=='S')&&(Tp.Buf[3]=='B') &&(Tp.Buf[4]=='D'))
			if(( (Tp.Buf[0]=='G')&&(Tp.Buf[1]=='P')&&(Tp.Buf[2]=='S')&&(Tp.Buf[3]=='B') &&(Tp.Buf[4]=='D'))||
			 ((Tp.Buf[0]=='G')&&(Tp.Buf[1]=='Y')&&(Tp.Buf[2]=='I')&&(Tp.Buf[3]=='C')))
			{
				IC_CardType = IC_AT24C_CARD;
				Uart.puts("AT24Cxx\r\n\0");
			}
			else
			{
				PinFor4442(1);
				os_dly_wait(1);
				//Tp.Buf[0] = 'G';	Tp.Buf[1] = 'U';	Tp.Buf[2] = 'O';	Tp.Buf[3] = 'Y';	
				tl = I2C0.Reset4442Card();
				//if(tl == Tp.B32)
				if(1)
				{
					IC_CardType = IC_4442_CARD;
					Uart.puts("4442...\r\n\0");
				}
			}

			I2C_Busy_Now_Flag = 0;

			
		}
		gCnt.InsertIC_Card++;				
	}
	else
	{
		if(gCnt.InsertIC_Card)
			Uart.puts("Pull out IC card\r\n\0");
		gCnt.InsertIC_Card = 0;
		IC_CardType = 0;
		
	}	
		
}	// End of <CheckIC_Card> function












	 


/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	�������ƣ�
   ����������
   ��ڲ�����
   ���ڲ�����
   ȫ�ֱ���: 
   ����ʱ�䣺
   �޸�ʱ�䣺
*/ 	
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ ����ԭ�������ĺ���������������
void        IICCls::PinFor4442(uchar Pin)
{
	if(Pin)		// ��ͨIO
	{
		I2C0_PIN_FOR_GPIO;
		I2C0_SCL_PIN_OUT;			// SCL
		I2C0_SDA_PIN_OUT;
		IC_CARD_RESET_PIN_LOW;

		Current_Use_4442_Flag = 1;
	}
	else			// I2C
	{
		I2C0_PIN_SELECT;
		Current_Use_4442_Flag = 0;
	}
	
}	// End of <Reset4442Card> function



/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	�������ƣ�Reset4442Card
   ������������λ����λӦ��
   ��ڲ�������
   ���ڲ����� 32λ����
   ȫ�ֱ���: 
   ����ʱ�䣺
   �޸�ʱ�䣺
*/ 	
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ ����ԭ�������ĺ���������������
uint32        IICCls::Reset4442Card(void)
{
	uint32	i, Data;

	 //*** ��λ	***
	IC_CARD_RESET_PIN_LOW;
	I2C0_SCL_PIN_OUT;
	I2C0_SDA_PIN_IN;
	I2C0_SCL_PIN_LOW;
	Sys.Delay_mS(1);

	IC_CARD_RESET_PIN_HIGH;	 
	Sys.Delay_uS(5);

	// *** ��λӦ�� ***
	I2C0_SCL_PIN_HIGH;
	Sys.Delay_uS(10);
	I2C0_SCL_PIN_LOW;

	Sys.Delay_uS(5);
	IC_CARD_RESET_PIN_LOW;

	Sys.Delay_uS(5);

	// *** ��32λӦ������, SCL Ϊ47.8kHz ***
	Data = 0;
	for(i=0;i<32;i++)
	{
		Data >>= 1;
		I2C0_SCL_PIN_HIGH;
		Sys.Delay_uS(10);			 
		if(GET_I2C0_SDA_STATUS)
			Data |= 0x80000000;
		
		I2C0_SCL_PIN_LOW;
		Sys.Delay_uS(10);
	}

	//*** ʹI/O Ϊ����״̬, �ȴ���������ģʽ ***

	I2C0_SCL_PIN_HIGH;
   Sys.Delay_uS(10);
	I2C0_SCL_PIN_LOW;
	Sys.Delay_uS(5);
	I2C0_SDA_PIN_OUT;
	I2C0_SDA_PIN_HIGH;
	
	return Data;
}	// End of <Reset4442Card> function






/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	�������ƣ�WriteByte4442Card
   ����������д8λ���ݵ�EEPROM
   ��ڲ�����Data - 8λ����
   ���ڲ����� ��
   ȫ�ֱ���: 
   ����ʱ�䣺
   �޸�ʱ�䣺
	
*/ 	
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ ����ԭ�������ĺ���������������
void        IICCls::WriteByte4442Card(uchar Data)
{
	uint32	i;

	// *** д8λ���ݣ�ʱ��Ƶ��Ϊ47.8kHz ***
	for(i=0;i<8;i++)
	{	
		if(Data&0x01)
		{	I2C0_SDA_PIN_HIGH;	}
		else
		{	I2C0_SDA_PIN_LOW;		}
		I2C0_SCL_PIN_HIGH;
		Sys.Delay_uS(10);
		I2C0_SCL_PIN_LOW;
		Sys.Delay_uS(10);
		Data >>= 1;
	}

				
}	// End of <Write4442ByteCard> function


/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	�������ƣ�ReadByte4442Card
   ������������EEPROM��һ���ֽ�����
   ��ڲ�������
   ���ڲ�����һ���ֽ�����
   ȫ�ֱ���: 
   ����ʱ�䣺
   �޸�ʱ�䣺
	
*/ 	
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ ����ԭ�������ĺ���������������
uchar        IICCls::ReadByte4442Card(void)		  
{
	uchar		Data;
	uint32	i;
	Data = 0;
	
	// *** ��ȡ8λ���ݣ�ʱ��Ƶ��Ϊ47.8kHz ***
	for(i=0;i<8;i++)
	{
		Data >>= 1;
		I2C0_SCL_PIN_HIGH;
		Sys.Delay_uS(10);
		if(GET_I2C0_SDA_STATUS)
			Data |= 0x80;		
		I2C0_SCL_PIN_LOW;
		Sys.Delay_uS(10);
	}	

	return  Data;	
}	// End of <Write4442ByteCard> function





							

/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	�������ƣ�WriteCmd4442Card
   ��������������ģʽ
   ��ڲ����� Cmd - ���Addr - ��ַ��Data - ����
   ���ڲ����� ��
   ȫ�ֱ���: 
   ����ʱ�䣺
   �޸�ʱ�䣺
*/ 	
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ ����ԭ�������ĺ���������������
void        IICCls::WriteCmd4442Card(uchar Cmd,uchar Addr,uchar Data)   // command mode
{
	

	
	Start4442Card();   // �����ź�

	WriteByte4442Card(Cmd);
	WriteByte4442Card(Addr);
	WriteByte4442Card(Data);

	// *** ����I/0Ϊ L ״̬  ***
	I2C0_SDA_PIN_LOW;
	Sys.Delay_uS(10);

	// *** ֹͣ�ź� ***
	Stop4442Card();

}	// End of <WriteCmd4442Card> function



/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	�������ƣ� Read4442Card
   ���������� ��ȡ4442������Ϣ
   ��ڲ����� Addr - ��ַ�� Len - ���ݳ��ȣ�pBuffer - ����ָ��
   ���ڲ�����
   ȫ�ֱ���: 
   ����ʱ�䣺
   �޸�ʱ�䣺
*/ 	
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ ����ԭ�������ĺ���������������
bool        IICCls::Read4442Card(uint16 Addr,uint16 Len,uchar *pBuffer)
{
	uint32	i;
	uchar		Data;
	Data = 0;
	WriteCmd4442Card(eIC_CARD_READ_MAIN_MEMORY,Addr,Data);


	//*** ���ó��������ģʽ ***
	I2C0_SDA_PIN_OUT;
	I2C0_SCL_PIN_LOW;
	Sys.Delay_uS(1);
	I2C0_SDA_PIN_LOW;
	Sys.Delay_uS(9);
	I2C0_SDA_PIN_IN;

	//*** �������� ***
	for(i=0;i<Len;i++)
	{
		*(pBuffer+i) = ReadByte4442Card();		
	}

   //*** ʹI/O Ϊ����״̬ ***
	I2C0_SCL_PIN_HIGH;
   Sys.Delay_uS(10);
	I2C0_SCL_PIN_LOW;
	Sys.Delay_uS(5);
	I2C0_SDA_PIN_OUT;
	I2C0_SDA_PIN_HIGH;

	return OK;
}	// End of <Read4442Card> function




/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	�������ƣ�
   ����������
   ��ڲ�����
   ���ڲ�����
   ȫ�ֱ���: 
   ����ʱ�䣺
   �޸�ʱ�䣺
	
*/ 	
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ ����ԭ�������ĺ���������������
bool        IICCls::Write4442Card(uint16 Addr,uint16 Len,uchar *pBuffer)	 // δ����
{	/*
	uint32	i, j;
	//uchar		Data;
	//Data = 0;
  	for(i=0;i<Len;i++)
	{

		WriteCmd4442Card(eIC_CARD_WRITE_MAIN_MEMORY,Addr, *(pBuffer+i));

		// *** ���ô���ģʽ ***
		I2C0_SDA_PIN_OUT;
		I2C0_SCL_PIN_LOW;
		Sys.Delay_uS(10);
		I2C0_SDA_PIN_LOW;
		Sys.Delay_uS(40); 
	  
		for (j = 122; j > 0; j--)
		{
		  	I2C0_SCL_PIN_HIGH;
	   	Sys.Delay_uS(40);
			I2C0_SCL_PIN_LOW;
			Sys.Delay_uS(40);

		}
		
	   //*** ʹI/O Ϊ����״̬ ***
		I2C0_SCL_PIN_HIGH;
	   Sys.Delay_uS(40);
		I2C0_SCL_PIN_LOW;
		Sys.Delay_uS(20);
		I2C0_SDA_PIN_OUT;
		I2C0_SDA_PIN_HIGH;

	}

	return OK;
	*/
}		// End of <Write4442Card> function




void			IICCls::Start4442Card(void)
{
   I2C0_SDA_PIN_OUT;
	I2C0_SDA_PIN_HIGH;
	Sys.Delay_uS(10);
	I2C0_SCL_PIN_HIGH;
	Sys.Delay_uS(10);
	I2C0_SDA_PIN_LOW;
	Sys.Delay_uS(30);

	I2C0_SCL_PIN_LOW;
	Sys.Delay_uS(40);
	
	
}


void			IICCls::Stop4442Card(void)
{
	
	I2C0_SCL_PIN_HIGH;	
	Sys.Delay_uS(10);
	I2C0_SDA_PIN_HIGH;
	Sys.Delay_uS(10);
}


void			IICCls::Break4442Card(void)	 // δ����
{
	I2C0_SCL_PIN_LOW;
	Sys.Delay_uS(15);
	IC_CARD_RESET_PIN_HIGH;
	Sys.Delay_uS(50);
	IC_CARD_RESET_PIN_LOW;
}



/*
 $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$  End of File $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
 */
