/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$  Copyright (c)  $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
$												肖遥电子（广州）研发中心
$										Shawyoo Electronics (GuangZhou) R&D Center
$                       	http://www.shawyoo.com		 Email: autoele@qq.com
$								 QQ群：9069471/9069433/9524253		All rights reserved.	  
$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$  Copyright (c)  $$$$$$$$$$$$$$$$$$$$$$$$$$$ */
/*  
                                      文件描述
	文件名: 
	功能描述：
	编译环境：
	目标CPU: 
	作者：
	创建时间：
	建议编辑环境：UltraEdit 11.00a+  制表符宽度为三个字符，程序可读性更好。
$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
*/					  


#define  TEST_LCM  1

#define     E308_GLOBALS   1
#define     CN_GLOBALS		1

#include		"LEDe308.h"
#include		"KeyDisp.h"
#include		"..\UART\UART0.H"
#include		"CN_Code.h"
#include		"..\SPI\SSP.h"
/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
   函数名称：
   功能描述：
   入口参数：
   出口参数：
   全局变量: 
   创建时间：
   修改时间：
	注：OLED电源由两片HC595控制，需要适合的地方调用
*/ 	
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ 以上原形声明的函数供本函数调用

void			OLEDCls::PowerOnTiming(void)
{
# if TEST_LCM
	NOP();
#else
	LCM_POWER_OFF;		
	LCM_BL_POWER_OFF;
	os_dly_wait(1);
	//HC595_OUTPUT;
	os_dly_wait(1);
	//HC595_OUTPUT2;
	os_dly_wait(200);
	////////////////////////////////// 
   LCM_POWER_ON;
	//HC595_OUTPUT;
	os_dly_wait(200);
	LCM_BL_POWER_ON;
	os_dly_wait(100);
#endif
}




void OLEDCls::Lcd_Data(uchar data)
{	  
	  CS_DISP_ENABLE;
	  RS_LCM_HIGH;
	  SSP0.WriteByte(data);
	  CS_DISP_DISABLE;
}

void OLEDCls::Lcd_Command(uchar cmd)
{	  
	  CS_DISP_ENABLE;
	  RS_LCM_LOW;
	  SSP0.WriteByte(cmd);
	  CS_DISP_DISABLE;
}







/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
   函数名称：
   功能描述：初始化LED
   入口参数：
   出口参数：
   全局变量: 
   创建时间：
   修改时间：
*/ 	
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ 以上原形声明的函数供本函数调用
void			OLEDCls::Init(void)
{
	uchar		t,k,Page;
	uchar		s,PageBuffer[130];
	NOP();	NOP();
	NOP();	NOP();


# if TEST_LCM
	
	  RET_LCM_LOW;
	  WD_DELAY;
	  RET_LCM_HIGH;
	  os_dly_wait(5);
	  Lcd_Command(0xe2); //reset
	  Lcd_Command(0xa2); //LCD BIAS=1/7行写入数据地址加1，但读入不加
	  Lcd_Command(0xa0); //列地址移动方向:ADC=0(SEG1->SEG132)
	  Lcd_Command(0xc8); //行扫描方向:SHL=1(COM64->COM1)
	  Lcd_Command(0x2c); //VC ON
	  WD_DELAY;
	  Lcd_Command(0x2e); //VR ON
	  WD_DELAY;
	  Lcd_Command(0x2f); //电源控制模式：VC,VR,VF=111
	  WD_DELAY;
	  Lcd_Command(0x81); //对比度寄存器模式
	  WD_DELAY;
	  Lcd_Command(0x24); //对比度调整	 0x34	  // 0x1F
	  WD_DELAY;
	  Lcd_Command(0x24); //设置电阻分压
	  WD_DELAY;
	  Lcd_Command(0xaf); //开显示
	  WD_DELAY;
	  Lcd_Command(0x60); //起始行com33
	  WD_DELAY;
	  ClearLCD();
	  
#else


	t = 0;
	while(t==0)
	{
		Disp_Command_WI_Enable;
		WD_DELAY;
		WD_DELAY;
		WD_DELAY;
		WD_DELAY;
		Disp_CSEnable;
		WD_DELAY;
		WD_DELAY;
		WD_DELAY;
		WD_DELAY;
		WD_DELAY;
		WD_DELAY;
		WD_DELAY;
		WD_DELAY;
	   //if(Cnt.SystemSecond<5)
		  WriteCommand(0xae);//(display off)
		WD_DELAY;	 	 //WD_DELAY;	 WD_DELAY;	 WD_DELAY;	 WD_DELAY;	 WD_DELAY;	 WD_DELAY;	 WD_DELAY;	 WD_DELAY;	 WD_DELAY;	 WD_DELAY;	 WD_DELAY;	 WD_DELAY;	 WD_DELAY;	 
		//WriteCommand(0xa5);//Output follows RAM content
		WD_DELAY;	
		WriteCommand(0xd5);//(set display clock divide ratio/oscillator/frequency)
      WD_DELAY;
		WriteCommand(0x10);// divide ratio = 1  Oscillator Frequency, 1111b
		WD_DELAY;
   	WriteCommand(0xa8);//(set multiplex ratio)
      WD_DELAY;
		WriteCommand(0x1f);	//from 16MUX to 64MUX, RESET= 111111b
		WD_DELAY;
		WriteCommand(0xd3);//(set display offset)
      WD_DELAY;
		WriteCommand(0x00);//(set display 00)
		WD_DELAY;
//		WriteCommand(0x40);//(display start set)
		WriteCommand(0xad);//(set dc-dc on/off)
      WD_DELAY;
		WriteCommand(0x8f);// Select external VCC supply
      WD_DELAY;
//		WriteCommand(0xd8);//(set area color mode on/off & low power display mode )
//      WriteCommand(0x05);// color monochrome mode     Set low power display mode
		WriteCommand(0xa1);//(set segment re-map)					//a0
		WD_DELAY;
		WriteCommand(0xc8);
      WD_DELAY;
//		WriteCommand(0xc8);//(set com output scan direction) remapped mode. Scan from COM[N~1] to	COM0
		 
		WriteCommand(0xda);//(set com pins hardware configuration)
		WD_DELAY;
		WriteCommand(0x12); //reset mode
      WD_DELAY;
//		WriteCommand(0x81);//(set contrast control register)
//    WriteCommand(0xbf);

		//WriteCommand(0xd9);//(set pre-charge period)				 bb
		//WriteCommand(0xc2);												 //bb
 
		WriteCommand(0xdb);//(set vcom deselect level)
		WD_DELAY;
		WriteCommand(0x28);//	~ 0.77 x VCC (RESET)		// 原来08，稍便宜款的需28
      WD_DELAY;
		//WriteCommand(0xa5);//Output follows RAM content
		WD_DELAY;
		WriteCommand(0xa4);//Output follows RAM content
		//WD_DELAY;
		WriteCommand(0xa6);//(set normal/inverse display)
 
		//WriteCommand(0x20); //addressing mode select
 
		//WriteCommand(0x02); //page address mode	   复位默认
 
      //WriteCommand(0x00);//set low column address	复位默认			 00		 F
 
      //WriteCommand(0x10);//set high column address	复位默认			 10		  1f
		WD_DELAY;
		WriteCommand(0x21); //addressing mode select
      WD_DELAY;
		WriteCommand(0x00);
		WD_DELAY;
		WriteCommand(0x83);
      WD_DELAY;
//        WriteCommand(0x2e);//(stop horzontal scroll)
//        WriteCommand(0xb3);//(page address)
//		WriteCommand(0x12);    
//	   WriteCommand(0x91);
//	   WriteCommand(0x3F);
//	   WriteCommand(0x3F);
//	   WriteCommand(0x3F);
//	   WriteCommand(0x3F);
		//WriteCommand(0xa4);//Output follows RAM content
		WD_DELAY;   
		WriteCommand(0xaf);//(display on)				 
//
//		WriteCommand(0xae); /* set  display off */   
//	   WriteCommand(0x00); /* set  lower column start address */
//	   WriteCommand(0x10); /* set  higher column start address */  
//	   WriteCommand(0x40); /* set  display start line */   
//	   WriteCommand(0x2E);   
//	   WriteCommand(0x81); /* set  contrast control */
//	   WriteCommand(0x32);    
//	   WriteCommand(0x82);    
//	   WriteCommand(0x80);   
//	   WriteCommand(0xa1); /* set  segment remap  */  
//	   WriteCommand(0xa6); /* set  normal display */   
//	   WriteCommand(0xa8); /* set  multiplex ratio */
//	   WriteCommand(0x3f); /* 1/64 */   
//	   WriteCommand(0xad); /* master configuration */
//	   WriteCommand(0x8e); /* external vcc supply */  
//	   WriteCommand(0xc8); /* set  com scan direction */  
//	   WriteCommand(0xd3); /* set  display offset  */
//	   WriteCommand(0x40);   
//	   WriteCommand(0xd5); /* set  display clock divide/oscillator frequency */
//	   WriteCommand(0xf0);    
//	   WriteCommand(0xD8);    /*set area color mode off */
//	   WriteCommand(0x05);       
//	   WriteCommand(0xD9);
//	   WriteCommand(0xF1);   
//	   WriteCommand(0xda); /* set  com pin configuartion */
//	   WriteCommand(0x12);    
//	   WriteCommand(0x91);
//	   WriteCommand(0x3F);
//	   WriteCommand(0x3F);
//	   WriteCommand(0x3F);
//	   WriteCommand(0x3F);   
//	   WriteCommand(0xaf); /* set  display on */		 


		t = 1;
		if((t==1)&&(gCnt.SystemSecond<20))											  //清屏
		//if(1)
		{  
			Disp_Data_WI_Enable;
			WD_DELAY;
		 for(Page=0;Page<4;Page++)
		  {
			for(s=0;s<16;s++)
			{ 
				
				t = 8;
				for(k=0;k<t;k++)								  
				{	PageBuffer[s*t+k] = 0x00;	}
			}

			DataDisplaying(0,Page,4,128,PageBuffer); 
		  }
		}
		NOP(); NOP();
		Disp_Command_WI_Enable;
		WD_DELAY;
		WriteCommand(0xaf);        //(display on)
		WD_DELAY;
		Disp_CSDisable;
		NOP(); NOP();
		NOP(); NOP();	   
	}
#endif
}


/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
   函数名称：WriteCommand
   功能描述：LED写指令
   入口参数：指令代码
   出口参数：无
   全局变量: 
   创建时间：
   修改时间：
*/ 	
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ 以上原形声明的函数供本函数调用
void OLEDCls::WriteCommand(uchar aa)
{

	//Disp_Data_WI_Enable;
	//WD_DELAY;
	//Disp_Command_WI_Enable;//DC=0
	//WD_DELAY;
	//Disp_CSEnable;//CS=0
	//WD_DELAY;
   SSP0.WriteByte(aa);//              IOSET = aa;//--------input command
   WD_DELAY;
   //Disp_CSDisable;//CS=1


}

#define uint unsigned int
void OLEDCls::disp_all(uint x1,uint y1,uint x2,uint y2,uint x3,uint y3,uint x4,uint y4)  //带
{   
    unsigned int i=0,j=0;


     Lcd_Command(0xb0);    //写页地址
     Lcd_Command(0x10); //写初始列地址
     Lcd_Command(0x0);	 // SET SEG =0
     for(j=0;j<8;j++) //j<64;是一样的
		 Lcd_Data(0xaa); //写显示数据

     Lcd_Command(0xb1);    //写页地址
     Lcd_Command(0x11); //写初始列地址
     Lcd_Command(0x0);	 // SET SEG =0
     for(j=0;j<8;j++) //j<64;是一样的
		 Lcd_Data(0xFF); //写显示数据
	  
	  Lcd_Command(0xb2);    //写页地址
     Lcd_Command(0x10); //写初始列地址
     Lcd_Command(0x0);	 // SET SEG =0
     for(j=0;j<8;j++) //j<64;是一样的
		 Lcd_Data(0xFF); //写显示数据

	  Lcd_Command(0xb3);    //写页地址
     Lcd_Command(0x11); //写初始列地址
     Lcd_Command(0x0);	 // SET SEG =0
     for(j=0;j<16;j++) //j<64;是一样的
		 Lcd_Data(0xFF); //写显示数据




//    for(i=0xb0;i<0xb8;i++)
//    { 
//        Lcd_Command(i);    //写页地址
//        Lcd_Command(0x10); //写初始列地址
//        Lcd_Command(0x0);	 // SET SEG =0
//		  Lcd_Command(0xAF);   // DIAPLAY ON    
////        WBYTE_Date(0x0);
//
//        for(j=0;j<17;j++) //j<64;是一样的
////			for(j=0;j<9;j++) //j<64;是一样的        
//		  {
//         Lcd_Data(x1); //写显示数据
//         Lcd_Data(y1);
//         Lcd_Data(x2); //写显示数据
//         Lcd_Data(y2);
//         Lcd_Data(x3); //写显示数据
//         Lcd_Data(y3);
//         Lcd_Data(x4); //写显示数据
//         Lcd_Data(y4);
//        }
//    }

}




/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
   函数名称：E308_write_data
   功能描述：LED写数据
   入口参数：数据指针
   出口参数：无
   全局变量: 
   创建时间：
   修改时间：
*/ 	
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ 以上原形声明的函数供本函数调用
void OLEDCls::DataDisplaying(uchar Line,uchar Page,uchar Column,uint16 Length,uchar *pData)
{

   char   t;


# if TEST_LCM

	  Lcd_Command(Page+0xb0);    //写页地址
     Lcd_Command(0x10); //写初始列地址
     Lcd_Command(0x0);	 // SET SEG =0
     for(;Length;Length--)
	  {
		 Lcd_Data(*pData); //写显示数据
		 WD_DELAY;
		 pData++;
	  }								  

#else


	Disp_Command_WI_Enable;
  	Disp_CSEnable; 			
	if(Line<E308_TOTAL_LINE)
	{
		WriteCommand(0x40+Line);		  //页地址的起始行
	}
	if(Page<E308_TOTAL_PAGE)
	{
		Page &= 0x07;
		WriteCommand(0xb0|Page);		 //页地址
	}
	if(Column<E308_TOTAL_COLUMN)
	{
		t = Column >> 4;
		WriteCommand(0x10|t);			    //页地址的列范围
		WriteCommand(0x0f&Column);
	}	 	
    Disp_Data_WI_Enable;						    // 写数据
// 	Disp_CSEnable;
	WD_DELAY;
 	for(;Length;Length--)
	{
		SSP0.WriteByte(*pData);
		WD_DELAY;
		pData++;
	}
	          
//	WriteCommand(E308_CMD_DISPLAY_ON);	// 
	WD_DELAY;
   //Disp_CSDisable;//CS=1	 



    uchar ColLow;
    uchar ColHigh;
    uchar i;
    uchar asc;
    
    i = Column;
    ColHigh = i & 0xf0;
    ColHigh = ColHigh/16;
    ColHigh = ColHigh | 0x10;
    ColLow = i & 0x0f;
    Page = 0xB0;
    WriteCommand(Page);
    WriteCommand(ColHigh);
    WriteCommand(ColLow);
    
 	for(;Length;Length--)
	{
		SSP0.WriteByte(*pData);
		WD_DELAY;
		pData++;
	}




#endif



	 
}





 /* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
   函数名称：E308_Give_Pwr
   功能描述：打开LED电源
   入口参数：无
   出口参数：无
   全局变量: 
   创建时间：
   修改时间：
*/ 	
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ 以上原形声明的函数供本函数调用

//uchar   static    POWER_Outside;						//GLOBAL

void   OLEDCls::Give_Pwr(uchar PWR)

{	// 应付检测，一直开电
	//HC595.Out1.Bits.PowerLCM  = 0;					  //开OLED电源
	//if(PWR)
	//HC595.Out1.Bits.PowerLCM  = 1;

	WD_DELAY;

}

void   OLEDCls::OLEDTest(uchar TDa) 

{	
	 uchar    const *pASCII;
	uchar		t,k;
	uchar		s,PageBuffer[130];

	Disp_CSEnable;
	Disp_Data_WI_Enable;
			os_dly_wait(1);
			for(s=0;s<16;s++)
			{ 
				pASCII = ASCII_Code + 0x100 + TDa +s*16;
				t = 8;
				for(k=0;k<t;k++)								               // 每个汉字或字符的下半部分字模，即16或8字节
		   	{	PageBuffer[s*t+k] = *(pASCII++);	}
			}

			DataDisplaying(0,0,4,128,PageBuffer); 

			for(s=0;s<16;s++)
			{ 
				
				pASCII = ASCII_Code + 0x100 + TDa +s*16;
				t = 8;
				for(k=0;k<t;k++)								               // 每个汉字或字符的下半部分字模，即16或8字节
		   	{	PageBuffer[s*t+k] = *(pASCII++);	}
			}

			DataDisplaying(0,2,4,128,PageBuffer); 
			
    	 for(s=0;s<16;s++)
			{ 
				
				pASCII = ASCII_Code + 0x100 - TDa  +s*16+8;
				t = 8;
				for(k=0;k<t;k++)								               // 每个汉字或字符的下半部分字模，即16或8字节
		   	{	PageBuffer[s*t+k] = *(pASCII++);	}
			}

			DataDisplaying(0,1,4,128,PageBuffer); 
			os_dly_wait(1);
		 for(s=0;s<16;s++)
			{ 
				
				pASCII = ASCII_Code + 0x100 - TDa +s*16+8;
				t = 8;
				for(k=0;k<t;k++)								               // 每个汉字或字符的下半部分字模，即16或8字节
		   	{	PageBuffer[s*t+k] = *(pASCII++);	}
			}
			//Uart.putb(1,128,(uchar*)PageBuffer);
			DataDisplaying(0,3,4,128,PageBuffer); 				

			NOP(); NOP();
//	        WriteCommand(E308_CMD_DISPLAY_ON); 
			Disp_CSDisable;
}

void  OLEDCls::ClearLCD(void)
{
    uchar page;
    uchar seg;
    
    for(page=0xb0;page<0xb9;page++)
    {
	    Lcd_Command(page);
	    Lcd_Command(0x10);			/*列地址高位，D4位为1表示高位地址*/
	    Lcd_Command(0x00);			/*列地址低位，D4位为0表示低位地址*/
	    for(seg=0;seg<132;seg++)
	    {
	    	Lcd_Data(0x00);
	    }
    }
}
/*******************************************************************************
显示8*16数字
Page必须在0xb0~0xb7之间
*******************************************************************************/
//void OLEDCls::display_168(uchar Page,uchar Column,uchar temp)
//{
//    uchar ColLow;
//    uchar ColHigh;
//    uchar i;
//    uchar asc;
//    
//    i = Column;
//    ColHigh = i & 0xf0;
//    ColHigh = ColHigh/16;
//    ColHigh = ColHigh | 0x10;
//    ColLow = i & 0x0f;
//    
//    Lcd_Command(Page);
//    Lcd_Command(ColHigh);	
//    Lcd_Command(ColLow);
//    
//    asc = temp*16;
//    for(i=0;i<8;i++)
//    {
//    	Lcd_Data(zk_ShuZi[asc]);
//    	asc++;
//    }
//    Lcd_Command(Page+1);
//    Lcd_Command(ColHigh);	
//    Lcd_Command(ColLow);
//    for(i=0;i<8;i++)
//    {
//    	Lcd_Data(zk_ShuZi[asc]);
//    	asc++;
//    }
//}


/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
   函数名称：
   功能描述：在坐标 x,y 处显示GB2312内码为CodeAddr 的 16*16 的字体
   入口参数：128 * 64 的屏幕 ，y代表第几行取值 0 到 3， x 代表第几列取值 0 到 8  
   出口参数：
   全局变量: 
   创建时间：
   修改时间：
*/ 
void OLEDCls::OLEDDisplay_ZT1616(uchar y,uchar x,uint16 CodeAddr,uchar InvertFlag)
{
	uchar Addh,Addm,Addl;
#if 0 
	ulong	ZFAdd, HZAdd;  // 表示字符和汉字的起始地址
	ulong	MSB,LSB;    // 表示GB2312编码的高字节和低字节
	ulong	GB2312tab;		// 表示GB2312转码表起始地址
	ulong	Address = 0;     // 表示汉字点阵在芯片中的字节地址
	uchar    hzbuf[32];
	
	ZFAdd = 	36224;		// 表示字符起始地址
	HZAdd = 	93452;		// 表示汉字的起始地址
	GB2312tab = 87436;	// 表示GB2312转码表起始地址
   
		MSB = (CodeAddr >> 8) & 0xff;
		LSB = CodeAddr & 0xff; 
	
		
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
		{
			Address = (unsigned long)((unsigned long)(32 * ((MSB - 0xB0) * 94 + (LSB - 0xA1) + 1)) + HZAdd);				
		}

	   Addh=(unsigned char)(Address>>16L);  //将Addr高八位取出，Addh
      Addm=(unsigned char)((Address& 0xff00)>>8L);   //将Addr中八位取出，Addm
      Addl=(unsigned char)(Address& 0xff);         //将Addr低八位取出，Addl
	  SetBufferValue(hzbuf,32,0);
     GT_Read_n_bytes(Addh,Addm,Addl,hzbuf,32);		//将Addh,Addm,Addl地址数据送到字库芯片后，并从字库芯片读出16个字节的数据
//     if(InvertFlag == 1)
//	  	  InvertOLED(hzbuf,32);     
	  
	  DataDisplaying(0,2*y,x*8+2,16,hzbuf);
     DataDisplaying(0,2*y+1,x*8+2,16,(uchar *)&hzbuf[16]);
#endif
}

/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
   函数名称：
   功能描述：在坐标 x,y 处显示GB2312内码为CodeAddr 的 8*16 的字体
   入口参数：128 * 64 的屏幕 ，y代表第几行取值 0 到 3， x 代表第几列取值 0 到 15  
   出口参数：
   全局变量: 
   创建时间：
   修改时间：
*/ 
void OLEDCls::OLEDDisplay_ASC816(uchar y,uchar x,uchar CodeAddr,uchar InvertFlag)
{
#if 0
	ulong	ZFAdd, HZAdd;  // 表示字符和汉字的起始地址
	ulong	Address = 0;     // 表示汉字点阵在芯片中的字节地址
	uchar Ascbuf[17];
	uchar Addh,Addm,Addl;

	 if ((CodeAddr >= 0x20L) && (CodeAddr <= 0x7FL)) //为防止溢出，常数后面有加小写的L，而不是1请注意。
			Address = (CodeAddr-0x20L ) * 16L;//+AscAdd; //为防止溢出，常数后面有加小写的L，而不是1请注意。

	   Addh=(unsigned char)(Address>>16L);  //将Addr高八位取出，Addh
      Addm=(unsigned char)((Address& 0xff00)>>8L);   //将Addr中八位取出，Addm
      Addl=(unsigned char)(Address& 0xff);         //将Addr低八位取出，Addl

	  SetBufferValue(Ascbuf,17,0);
     GT_Read_n_bytes(Addh,Addm,Addl,Ascbuf,16);//将Addh,Addm,Addl地址数据送到字库芯片后，并从字库芯片读出16个字节的数据
//     if(InvertFlag == 1)
//	  	  InvertOLED(Ascbuf,16);
	  DataDisplaying(0,2*y,x*8+2,8,Ascbuf);
     DataDisplaying(0,2*y+1,x*8+2,8,(uchar *)&Ascbuf[8]);
#endif
}


void OLEDCls::OLEDDispScreen(uchar xs,uchar ys,uchar len)
{
	  uchar InvertFlag;
	  uint16 xc,CN_Code,i,j;


	  for(i=0;i<4;i++)			  // 4行
	  {
		  for(j=0;j<16;j++)		  // 16列
		  {
		  	  xc = OLED_Buffer[i][j];
			  
			  if(xc==0)				// 如果显示数据没有，默认显示空格（空白）
			  	xc =' ';

			  if(len ==0)
			  {
			  	  InvertFlag = 0;
			  }
			  else
			  {
				  if((i==xs)&&(j>=ys)&&(j<=(ys+len)))
					 InvertFlag = 1;		  // 字体反显标志
				  else
				    InvertFlag = 0;
			  }

			  if((xc<=0x7F)&&(xc>=0x20))						 // 判断是否ASCII码
		        OLEDDisplay_ASC816(i,j,xc,InvertFlag);
			  else													 // GB2312 内码
			  {
			  	  CN_Code = xc<<8;
				  j += 1;			  
			  	  xc = OLED_Buffer[i][j];
				  CN_Code |= xc;
				  OLEDDisplay_ZT1616(i,j-1,CN_Code,InvertFlag);
			  }
		  }
	  }

}
OLEDCls::OLEDCls(void)
{ 

}
 
/*
 $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$  End of File $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
 */

