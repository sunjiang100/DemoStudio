/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$  Copyright (c)  $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
$												广州国业电子科技有限公司
$										Shawyoo Electronics (GuangZhou) R&D Center
$                       	http://www.guoyee.com		 Email: guoyegps@126.com
$								 QQ群：9069471/9069433/9524253		All rights reserved.	  
$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$  Copyright (c)  $$$$$$$$$$$$$$$$$$$$$$$$$$$ */
/*  
                                      文件描述
	文件名: 	  IC_Card4442.c
	功能描述： 
	编译环境： 
	目标CPU:   LPC2000
	作者：	  肖建辉
	创建时间： 
	建议编辑环境：UltraEdit 11.00a+  制表符宽度为三个字符，程序可读性更好。
					  Keil中可用配置工具
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
	函数名称：
   功能描述：
   入口参数：
   出口参数：无卡返回0,AT24C卡返回1,4442卡返回2
   全局变量: 
   创建时间：
   修改时间：
	
*/ 	
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ 以上原形声明的函数供本函数调用
void        IICCls::CheckIC_Card(void)
{
	uint32	tl;
	union
	{
		uint32	B32[2];
		uchar 	Buf[8];
	} Tp;

	if((GET_IC_CARD_STATUS)==0)		// IC卡已插入 （用于常开卡座）  
	//if((GET_IC_CARD_STATUS)!=0)		// IC卡已插入	(用于常闭卡座)
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
	函数名称：
   功能描述：
   入口参数：
   出口参数：
   全局变量: 
   创建时间：
   修改时间：
*/ 	
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ 以上原形声明的函数供本函数调用
void        IICCls::PinFor4442(uchar Pin)
{
	if(Pin)		// 普通IO
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
	函数名称：Reset4442Card
   功能描述：复位及复位应答
   入口参数：无
   出口参数： 32位数据
   全局变量: 
   创建时间：
   修改时间：
*/ 	
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ 以上原形声明的函数供本函数调用
uint32        IICCls::Reset4442Card(void)
{
	uint32	i, Data;

	 //*** 复位	***
	IC_CARD_RESET_PIN_LOW;
	I2C0_SCL_PIN_OUT;
	I2C0_SDA_PIN_IN;
	I2C0_SCL_PIN_LOW;
	Sys.Delay_mS(1);

	IC_CARD_RESET_PIN_HIGH;	 
	Sys.Delay_uS(5);

	// *** 复位应答 ***
	I2C0_SCL_PIN_HIGH;
	Sys.Delay_uS(10);
	I2C0_SCL_PIN_LOW;

	Sys.Delay_uS(5);
	IC_CARD_RESET_PIN_LOW;

	Sys.Delay_uS(5);

	// *** 读32位应答数据, SCL 为47.8kHz ***
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

	//*** 使I/O 为高阻状态, 等待进入命令模式 ***

	I2C0_SCL_PIN_HIGH;
   Sys.Delay_uS(10);
	I2C0_SCL_PIN_LOW;
	Sys.Delay_uS(5);
	I2C0_SDA_PIN_OUT;
	I2C0_SDA_PIN_HIGH;
	
	return Data;
}	// End of <Reset4442Card> function






/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	函数名称：WriteByte4442Card
   功能描述：写8位数据到EEPROM
   入口参数：Data - 8位数据
   出口参数： 无
   全局变量: 
   创建时间：
   修改时间：
	
*/ 	
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ 以上原形声明的函数供本函数调用
void        IICCls::WriteByte4442Card(uchar Data)
{
	uint32	i;

	// *** 写8位数据，时钟频率为47.8kHz ***
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
	函数名称：ReadByte4442Card
   功能描述：读EEPROM中一个字节数据
   入口参数：无
   出口参数：一个字节数据
   全局变量: 
   创建时间：
   修改时间：
	
*/ 	
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ 以上原形声明的函数供本函数调用
uchar        IICCls::ReadByte4442Card(void)		  
{
	uchar		Data;
	uint32	i;
	Data = 0;
	
	// *** 读取8位数据，时钟频率为47.8kHz ***
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
	函数名称：WriteCmd4442Card
   功能描述：命令模式
   入口参数： Cmd - 命令，Addr - 地址，Data - 数据
   出口参数： 无
   全局变量: 
   创建时间：
   修改时间：
*/ 	
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ 以上原形声明的函数供本函数调用
void        IICCls::WriteCmd4442Card(uchar Cmd,uchar Addr,uchar Data)   // command mode
{
	

	
	Start4442Card();   // 启动信号

	WriteByte4442Card(Cmd);
	WriteByte4442Card(Addr);
	WriteByte4442Card(Data);

	// *** 设置I/0为 L 状态  ***
	I2C0_SDA_PIN_LOW;
	Sys.Delay_uS(10);

	// *** 停止信号 ***
	Stop4442Card();

}	// End of <WriteCmd4442Card> function



/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	函数名称： Read4442Card
   功能描述： 读取4442卡的信息
   入口参数： Addr - 地址， Len - 数据长度，pBuffer - 数据指针
   出口参数：
   全局变量: 
   创建时间：
   修改时间：
*/ 	
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ 以上原形声明的函数供本函数调用
bool        IICCls::Read4442Card(uint16 Addr,uint16 Len,uchar *pBuffer)
{
	uint32	i;
	uchar		Data;
	Data = 0;
	WriteCmd4442Card(eIC_CARD_READ_MAIN_MEMORY,Addr,Data);


	//*** 设置成数据输出模式 ***
	I2C0_SDA_PIN_OUT;
	I2C0_SCL_PIN_LOW;
	Sys.Delay_uS(1);
	I2C0_SDA_PIN_LOW;
	Sys.Delay_uS(9);
	I2C0_SDA_PIN_IN;

	//*** 接收数据 ***
	for(i=0;i<Len;i++)
	{
		*(pBuffer+i) = ReadByte4442Card();		
	}

   //*** 使I/O 为高阻状态 ***
	I2C0_SCL_PIN_HIGH;
   Sys.Delay_uS(10);
	I2C0_SCL_PIN_LOW;
	Sys.Delay_uS(5);
	I2C0_SDA_PIN_OUT;
	I2C0_SDA_PIN_HIGH;

	return OK;
}	// End of <Read4442Card> function




/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	函数名称：
   功能描述：
   入口参数：
   出口参数：
   全局变量: 
   创建时间：
   修改时间：
	
*/ 	
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ 以上原形声明的函数供本函数调用
bool        IICCls::Write4442Card(uint16 Addr,uint16 Len,uchar *pBuffer)	 // 未调试
{	/*
	uint32	i, j;
	//uchar		Data;
	//Data = 0;
  	for(i=0;i<Len;i++)
	{

		WriteCmd4442Card(eIC_CARD_WRITE_MAIN_MEMORY,Addr, *(pBuffer+i));

		// *** 设置处理模式 ***
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
		
	   //*** 使I/O 为高阻状态 ***
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


void			IICCls::Break4442Card(void)	 // 未调试
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
