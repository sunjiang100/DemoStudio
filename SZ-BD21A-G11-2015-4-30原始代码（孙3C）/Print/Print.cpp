/***************************************  Copyright(c)  **************************************
**									 		
**								
**                      
**	  
****************************************  Copyright (c)  **************************************
**                                         文件描述
** 文件名:   Print.cpp
** 功能描述：热敏打印机驱动
** 编译环境：Keil uVision4 + RTX
** 目标CPU:  LPC1766
** 接口模块: 
** 作者：	 	
** 创建时间：2011-8-3 7:09:15
** 修改时间: 
   701技术支持 13911521370  王工  
	GYE-Test \x00 \x00 \x00 \x00 \x00 \x00  P   \x00  M 9  \x00 \x00      5       \x00   \x00 \x00 \x00 \x00 
***************                   **********************************************************************************/	
#define      	PRINT_GLOBALS		 1
#define     CN_GLOBALS		1


#include 		"Print.h"
#include 		"..\Common\flash_addr.h"
#include			"..\FLASH\DFE_25xxx.H"
#include			"..\FLASH\DFF_25xxx.H"

#include			"..\SPI\SSP.h"
#include		   <string.h>
#include			"..\UserSys.h"
#include		  "..\RTC\RTC.h"
#include		  "..\KeyDisp\KeyDisp.h"
#include		  "..\Common\GB_T19056.h"
#include			"..\common\Algol.H"
#include			"..\PWM\Pwm.H"
#include			"..\Switch\Switch.H"
#include			"..\RTC\RTC.H"

#include			"..\UART\UART2.h"
#include			"..\UART\UART0.h"
#include 		"..\Speed\Speed.h"

#include			"..\KeyDisp\CN_Code.h"



extern   uchar  *pShareMem;

const	unsigned char PrintTableGB[][32] = 
{
{"兰州交通大学2016届毕业论文\0\0"},		  //1
{"学号: 02130618\0\0"},	   //2
{"姓名: 孙江正\0\0"},				//3
{"题目: 基于部标的北斗双模\0"},	 //4
{"      行驶记录仪的研究与设计\0\0"},					 //5
{"打印时间: 2011-08-03 09:51:48\0\0"},		   //6

{"停车前15分钟内每分钟平均速度: \0"},

{"疲劳驾驶记录: \0\0"},
{"记录01: \0\0"},
{"驾驶证号码: 123456789012345678\0"},
{"开始时间: 2011-08-03 09:52:00\0"},
{"结束时间: 2011-08-03 09:53:00\0"},

{"超速驾驶记录: \0\0"},
{"记录01: \0\0"},
{"驾驶证号码: 123456789012345678\0"},
{"超速范围: 130km/h ~ 135km/h\0\0"},
{"开始时间: 2011-08-03 09:54:40\0\0"},
{"结束时间: 2011-08-03 09:55:07\0\0"},

{"                              "},
{"签名：\0\0"},						  //8
//{"      __________________\0\0"},		 // 打这个时，接24V电源，会重启，未知，所以用下面的代替
{"       -------------\0\0"},
{"专业: 电路与系统\0\0"},		//	 7
{"   无停车记录!\0\0"},
};
 
	

/*************************************************************************************************
** 函数名称: TaskPrint()
** 函数功能: 打印任务
** 输入参数: 无
** 输出参数: 无
** 全局变量: 无
** 修改时间: 2009.08.21
************************************************************************************************/
__task		void			TaskPrint(void)	
{	

	NOP();	NOP(); 	NOP();  	NOP();
	while(1)
	{	
		NOP();	NOP();
		TC.Print = 0;
		os_dly_wait(10);				//100ms
		Print.TickCnt++;
	   #if(IFPrint)
		if(TD.Display==tDISP_PRINT_INIT)
		{
			TD.Display = 0x01;																					  
			PrintBusy = TRUE;	
			Uart.puts("Print init......\r\n\0");	
			Print.Init(TRUE);			
		}
		else if((TD.Display==tDISP_PRINT_TEST)||(PrintEvent2==1))
		{	
			NOP();	NOP();	
			if((TD.Display==tDISP_PRINT_TEST)&&(TD.Value=='A'))
			{
				PrintEvent2 = 1;
				PrintBusy = TRUE;	
				Uart.puts("Print init......\r\n\0");	
//				Print.Init(TRUE);	
				os_dly_wait(100);
			}
			
			//if(Print.TickCnt%20==0)
			{
				PRINT_POWER_ON;
				os_dly_wait(100);
				Uart.puts("Printing......\r\n\0");
				Print.eTask();	
				Uart.puts("Print finish\r\n\0");			
				NOP();	NOP();
			}
			TD.Display = 0;  	PrintEvent2 = 0;
						
		}	
		else if(TD.Display==tDISP_PRINT_MOTOR_FORWARD)
		{
			TD.Display = 0x01;
			Print.SetPrintStatus(epWALKING_PAPER_N_DOT_LINE);
			Print.WalkingPaperOneDotLine(MOTOR_FORWARD,(16));
		}
		else if(TD.Display==tDISP_PRINT_MOTOR_BACK)
		{
			TD.Display = 0x01;
			Print.SetPrintStatus(epWALKING_PAPER_N_DOT_LINE);
			Print.WalkingPaperOneDotLine(MOTOR_BACK,(16));
		}
		else if(TD.Display==tDISP_PRINT_POWER_OFF)
		{
			PRINT_POWER_OFF;
			PRINT_HOT_OFF;//HC595.Out2.Byte &= 0xc0;			// 禁止加热输出
			STEP_MOTOR_POWER_OFF;				// 关电			
			PrintBusy = FALSE;
		}
		else if(TD.Display==tDISP_PRINT_LATCH)
		{
			if(Print.TickCnt&0x01)
			{	PRINT_DATA_LATCH_HIGH;	}
			else
			{	PRINT_DATA_LATCH_LOW;	}
		}
		#endif
		if( (PrintEvent2==0)&&(Print.StatusCnt==epPRINT_FREE))
		//if(0)
		{
		  
		  Print.Init(FALSE);
		  //#if(SYSTEM_BOOT_MESSAGE)
		  Uart.puts("Print task  delect\r\n\0");
		  //#endif	
		  os_dly_wait(2);
		  PrintTaskCreat=0;
		  os_tsk_delete_self();
		}
	}
	
}






/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	函数名称：
   功能描述：
   入口参数：
   出口参数：
   全局变量: 
   创建时间：
   此函数局部变量占用空间较大
*/ 
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ 以上原形声明的函数供本函数调用
void		PrintCls::TestLattice(uchar Index)
{	
	//////////////////////////////////////////////////////
	NOP();	NOP();
	strcpy((char*)OneCharLineBuffer,"浙津证号码：123456789012345678\0");	
	GetOneLineCharLattice();
	
}

/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	函数名称：
   功能描述：
   入口参数：
   出口参数：
   全局变量: OneCharLineBuffer,LatticeBuffer
   创建时间：
   此函数局部变量占用空间较大
*/ 
void			GetEN_Code12_24(unsigned char  Data,unsigned char  *pData);
void			GetCN_Code24_24(unsigned short Data,unsigned char *pData);
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ 以上原形声明的函数供本函数调用
uchar		PrintCls::GetOneLineCharLattice(void)
{	
	uint16	i,j,k,Cnt,BaseAddr,Addr,hAddr,hAddr1;
	uint32	    sunCode;	
	union
	{
		uchar 	Buf[4];
		uint16	B16[2];
		uint32	B32;
//		struct		
//		{
//			uint32		bit1:1;		 					 
//			uint32		bit2:1;	 			 
//			uint32		bit3:1;		  		 
//			uint32		bit4:1;	 
//			uint32		bit5:1;		    
//			uint32		bit6:1;	    
//			uint32		bit7:1;		 
//			uint32		bit8:1;    
//			
//			uint32		bit9:1;		 					 
//			uint32		bit10:1;	 			 
//			uint32		bit11:1;		  		 
//			uint32		bit12:1;	 
//			uint32		bit13:1;		    
//			uint32		bit14:1;	    
//			uint32		bit15:1;		 
//			uint32		bit16:1; 
//			
//			uint32		bit17:1;		 					 
//			uint32		bit18:1;	 			 
//			uint32		bit19:1;		  		 
//			uint32		bit20:1;	 
//			uint32		bit21:1;		    
//			uint32		bit22:1;	    
//			uint32		bit23:1;		 
//			uint32		bit24:1; 
//			
//			uint32		bit25:1;		 					 
//			uint32		bit26:1;	 			 
//			uint32		bit27:1;		  		 
//			uint32		bit28:1;	 
//			uint32		bit29:1;		    
//			uint32		bit30:1;	    
//			uint32		bit31:1;		 
//			uint32		bit32:1;  
//		} Bits;
	} Tp;
	uchar		t,n;		// 每个汉字点阵数72字节（24*24点阵）	
	uchar		cChar,cCode[80],gCode[80];
	hAddr = 0;		// 半字节地址，每行32字节，64半字字
	for(i=0;i<BYTE_LATTICE;i++)	{	LatticeBuffer[i] = 0;	}
	//////////////////////////////////////////////////////// 取1行字的全部点阵			
	for(i=0;i<CHAR_NUMBER_PER_LINE;i++)		// 字符个数	32	
	{
		BaseAddr = 0;
		Tp.B32 = 0;
		cChar = OneCharLineBuffer[i];		//需要打印的字符
		if( cChar >=0x80 )				
		{			
			Tp.Buf[1] = cChar;		  //	 cChar
			i++;
			Tp.Buf[0] = OneCharLineBuffer[i];		 //	   OneCharLineBuffer[i]
		//	GetCN_Code24_24(Tp.B16[0],cCode);		// 取汉字点阵，72字节	  PrintCls::shift_Value(uchar org, int index)
			////
			
			  sunCode = KeyDisp.CalZiMo_Addr(Tp.B16[0]);		               // 该汉字和或特殊符号在字库存储的首址	 GB2312_CODE_ADDR  从0x3C0000开始，占256Kbyte 
			  sunCode += GB2312_CODE_ADDR;										            
			  DFF.AnyNumberRead(sunCode, 16, cCode); 
			  n=0;
			  for(j=0;j<16;j++)
			  {
			   	if(j%2 != 0) n=1;
				   else n=0;

				gCode[j] = (shift_Value(cCode[n*8+0],j/2))<<7 |
						   (shift_Value(cCode[n*8+1],j/2))<<6 |
						   (shift_Value(cCode[n*8+2],j/2))<<5 |
						   (shift_Value(cCode[n*8+3],j/2))<<4 |
						   (shift_Value(cCode[n*8+4],j/2))<<3 |
						   (shift_Value(cCode[n*8+5],j/2))<<2 |
						   (shift_Value(cCode[n*8+6],j/2))<<1 |
						   (shift_Value(cCode[n*8+7],j/2));

			  }
			   for(j=0;j<24;j++)
			  {
				if((j+1)%3 == 0) cCode[j]=0;
				else
				{
				 cCode[j]=gCode[j-(j+1)/3];
				}

			  }

			  n=0;
			  sunCode += 16;										            
			  DFF.AnyNumberRead(sunCode, 16, cCode+16); 
			  n=0;
			  for(j=0;j<16;j++)
			  {
			   	if(j%2 != 0) n=1;
				   else n=0;

				gCode[j] = (shift_Value(cCode[n*8+16],j/2))<<7 |
						   (shift_Value(cCode[n*8+17],j/2))<<6 |
						   (shift_Value(cCode[n*8+18],j/2))<<5 |
						   (shift_Value(cCode[n*8+19],j/2))<<4 |
						   (shift_Value(cCode[n*8+20],j/2))<<3 |
						   (shift_Value(cCode[n*8+21],j/2))<<2 |
						   (shift_Value(cCode[n*8+22],j/2))<<1 |
						   (shift_Value(cCode[n*8+23],j/2));

			  }
			   for(j=0;j<24;j++)
			  {
				if((j+1)%3 == 0) cCode[j+15]=0;
				else
				{
				 cCode[j+15]=gCode[j-(j+1)/3];
				}

			  }
			  ////
			
			  ////
			  for(j=0;j<24;j++)
			  {
			  		Tp.B32 = 0;
				   	if((j+1)%3 == 0)
					{
						gCode[j*3]=gCode[(j-1)*3];
						gCode[j*3+1]=gCode[(j-1)*3+1];
						gCode[j*3+2]=gCode[(j-1)*3+2];
					}
					else
					{


					gCode[j*3] =	cCode[(j-(j+1)/3)*3];
					gCode[j*3+1] =	 cCode[(j-(j+1)/3)*3+1];
					gCode[j*3+2] =	 0x00;
//						Tp.Buf[0] = cCode[(j-(j+1)/3)*3];
//						Tp.Buf[1] =	cCode[(j-(j+1)/3)*3+1]; 
//						//
//						Tp.Bits.bit24 = Tp.Bits.bit16;
//						Tp.Bits.bit23 = Tp.Bits.bit15;
//						Tp.Bits.bit22 = Tp.Bits.bit14;
//						Tp.Bits.bit21 = Tp.Bits.bit13;
//						Tp.Bits.bit20 = Tp.Bits.bit12;
//						Tp.Bits.bit19 = Tp.Bits.bit11;
//						Tp.Bits.bit18 = Tp.Bits.bit10;
//						Tp.Bits.bit17 = Tp.Bits.bit10;
//						Tp.Bits.bit16 = Tp.Bits.bit9;
//						Tp.Bits.bit15 = Tp.Bits.bit8;
//						Tp.Bits.bit14 = Tp.Bits.bit7;
//						Tp.Bits.bit13 = Tp.Bits.bit6;
//						Tp.Bits.bit12 = Tp.Bits.bit5;
//						Tp.Bits.bit11 = Tp.Bits.bit4;
//						Tp.Bits.bit10 = Tp.Bits.bit3;
//						Tp.Bits.bit9 = Tp.Bits.bit2;
//						Tp.Bits.bit8 = Tp.Bits.bit1;
////						Tp.Bits.bit7 = Tp.Bits.bit6;
////						Tp.Bits.bit6 = Tp.Bits.bit5;
////						Tp.Bits.bit5 = Tp.Bits.bit5;
////						Tp.Bits.bit4 = Tp.Bits.bit4;
////						Tp.Bits.bit3 = Tp.Bits.bit3;
////						Tp.Bits.bit2 = Tp.Bits.bit2;
////						Tp.Bits.bit1 = Tp.Bits.bit1;
//						Tp.Bits.bit8 = 0;
//						Tp.Bits.bit7 = 0;
//						Tp.Bits.bit6 = 0;
//						Tp.Bits.bit5 = 0;
//						Tp.Bits.bit4 = 0;
//						Tp.Bits.bit3 = 0;
//						Tp.Bits.bit2 = 0;
//						Tp.Bits.bit1 = 0;
//
//					
//						gCode[j*3]=	 Tp.Buf[0];
//						gCode[j*3+1]=  Tp.Buf[1];
//						gCode[j*3+2]=  Tp.Buf[2];
		
						//gCode[j*3+(j+1)/3]=Tp.Buf[0];
						//gCode[j*3+(j+1)/3+1]=Tp.Buf[1];
						//gCode[j*3+(j+1)/3+2]=Tp.Buf[2];
				  	}
					
			  }
			   
			/////////////////// 取字符点阵后，直接按16行存放点阵
			k = 0;
			for(j=0;j<DOT_LINE_NUMBER_PER_CHAR;j++)						// 每个字符24行点阵
			{				
				hAddr1 = hAddr;				
				////////////////////////////////////////////
				for(Cnt=0;Cnt<3;Cnt++)
				{
					t = gCode[k++];				//ccode								
					Addr = hAddr1>>1;
					if(hAddr1%2)
						LatticeBuffer[BaseAddr+Addr] |= t>>4;
					else
						LatticeBuffer[BaseAddr+Addr] |= t&0xf0;
					hAddr1++;
					
					Addr = hAddr1>>1;
					if(hAddr1%2)
						LatticeBuffer[BaseAddr+Addr] |= t&0x0f;
					else
						LatticeBuffer[BaseAddr+Addr] |= t<<4;					
					hAddr1++;    
				}		
				BaseAddr += BYTE_NUMBER_PER_DOT_LINE;
			}	
			hAddr += 6;					
		}
		else
		{
			GetEN_Code12_24(cChar,cCode);				// 取E文点阵，48字节
			k = 0;
			/////////////////// 取字符点阵后，直接按24行存放点阵
			for(j=0;j<DOT_LINE_NUMBER_PER_CHAR;j++)						// 每个字符24行点阵
			{	
				hAddr1 = hAddr;
				t = cCode[k++];				
								
				Addr = hAddr1>>1;
				if(hAddr1%2)
					LatticeBuffer[BaseAddr+Addr] |= t>>4;	   //前四位加在字节的后四位
				else
					LatticeBuffer[BaseAddr+Addr] |= t&0xf0;
				hAddr1++;
				
				Addr = hAddr1>>1;
				if(hAddr1%2)
					LatticeBuffer[BaseAddr+Addr] |= t&0x0f;	   
				else
					LatticeBuffer[BaseAddr+Addr] |= t<<4;		//后四位加在字节的前四位			
				hAddr1++;
				
				t = cCode[k++];
				
				Addr = hAddr1>>1;
				if(hAddr1%2)
					LatticeBuffer[BaseAddr+Addr] |= t>>4;
				else
					LatticeBuffer[BaseAddr+Addr] |= t&0xf0;
				
								
				BaseAddr += BYTE_NUMBER_PER_DOT_LINE;
			}
			hAddr += 3;	
		}		
	}
	return OK;
}

/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	函数名称：
   功能描述：
   入口参数：
   出口参数：
   全局变量: 
   创建时间：
*/ 	

// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ 以上原形声明的函数供本函数调用
void		PrintCls::eTask(void)
{
	uchar		c,Cnt,Line,t,st;
	uchar		tBuffer[64];
	//uchar    time[20]; 
	uint32   dm;
	
	if((TD.Display==tDISP_PRINT_TEST)&&(TD.Value))
	{	
		if((TD.Value&0x00ff)=='M')
		{	////////// 测试多行打印
			Line = TD.Value>>8;
			if((Line>='0')&&(Line<='9'))
			{
				Line -= '0';
				c = '0';
			}
			else if(Line>96)
			{
				Line = 96;
				c = 0x21;
			}
			else
			{					
				c = 0x21;
			}
			GetNewCharLine = 0;
			StatusCnt = epPRINT_START;
			CharLineCnt = Line;
			for(Cnt=0;Cnt<Line;Cnt++)
			{
				SetBufferValue(OneCharLineBuffer,CHAR_NUMBER_PER_LINE,(Cnt+c));
				while(GetNewCharLine==0)	{os_dly_wait(1);	}
				GetNewCharLine = 0;									
			}
		}
		else if((TD.Value&0x00ff)=='C')
		{
			GetNewCharLine = 0;
			StatusCnt = epPRINT_START;
			CharLineCnt = 4;
			for(Cnt=0;Cnt<4;Cnt++)
			{
				SetBufferValue(OneCharLineBuffer,CHAR_NUMBER_PER_LINE,0);
				strcpy((char*)OneCharLineBuffer,(char*)&PrintTableGB[Cnt][0]);
				if(Cnt==0)	OneCharLineBuffer[13] = ASCII_CENTER_DOT;
				while(GetNewCharLine==0)	{os_dly_wait(1);	}
				GetNewCharLine = 0;					
			}
			
		}
		else
		{	
			GetNewCharLine = 0;	
			WalkingPaperOneDotLine('F',4);		
			StatusCnt = epWALKING_PAPER_4DOT_LINE;
			CharLineCnt = 0;
			TestLattice(TD.Value);
		}
	}
	else if(PrintEvent2==1)
	{	
			
			if(PrintPaperStu==0)
			{
				PrintEvent1=0;														 //清打印进程标识
			   PrintEvent2=0;
			   KeyDisp.KEY_DL.LongTimeNoKey = 20*5;
			   uLED.MainMenuCnt = PRINT_NO_PAPER_MENU;
			   return;
			} 
			SetBufferValue((uchar *)OVT_AddrTab,80,0);
			c = KeyDisp.GetOVT_Addr(TWO_DAY_OVT,OVT_AddrTab); 			//取疲劳驾驶记录
			CharLineCnt = c*4+12;
			Cnt = CharLineCnt;
			GetNewCharLine = 0;
			SetBufferValue(tBuffer, 17, 0);
			DFE.Read(FM_STOP_TIME, 6, tBuffer);				      //
			st = Rtc.DateTimeValid((RTC_STR *)tBuffer);
			SetBufferValue(tBuffer, 64, 0);
			//DFE.Read(FM_VEHICLE_CODE, 24, tBuffer);				      //车牌，车辆分类
			DFE.Read(FM_VEHICLE_CODE,12,tBuffer);	
			DFE.Read(FM_VEHICLE_VIN,17,&tBuffer[12]);
			DFE.Read(FM_VEHICLE_CLASS,12,&tBuffer[29]);
			DFE.Read(FM_DRIVER_CODE, 18, &tBuffer[41]);					//驾驶证号
			//DFE.Read(FM_DRIVER_NUMBER, 4, (uchar *)&dm);
			StatusCnt = epPRINT_START;
			/*if((st==0)||(st==7)) 	 		// 停车时间非法，说明无停车记录
			{  
			   t = 7;
			   CharLineCnt += 15;
				Cnt = CharLineCnt;
			}
			 else
			   t = 6; */

			if((st==0)||(st==7)) 	 		// 停车时间非法，说明无停车记录
			{  
			   t = 6;
			   CharLineCnt += 15;
				Cnt = CharLineCnt;
			}
			 else
			   t = 5;

			for(Line=0;Line<t;Line++)
			{
				if(PrintPaperStu==0)
				{
				   PrintEvent1=0;														 //清打印进程标识
				   PrintEvent2=0;
				   KeyDisp.KEY_DL.LongTimeNoKey = 20*5;
				   uLED.MainMenuCnt = PRINT_NO_PAPER_MENU;
				   return;
				} 
				SetBufferValue(OneCharLineBuffer,CHAR_NUMBER_PER_LINE+2,0);
				strcpy((char*)OneCharLineBuffer,(char*)&PrintTableGB[Line][0]);
				if(Line==0)
				{
				  // memmove(&OneCharLineBuffer[10], &tBuffer[0], 12);					
				}
				else if(Line==1)
				{
				//	memmove(&OneCharLineBuffer[10], &tBuffer[12], 17);
				}
				else if(Line==2)
				{
				//	memmove(&OneCharLineBuffer[10], &tBuffer[29], 12);
				}
				else if(Line==3)
				{
				//	memmove(&OneCharLineBuffer[12], &tBuffer[41], 18);	 //驾驶员证号			
				}
	         else if(Line==4)
				{															 
				//	strcpy((char*)OneCharLineBuffer,(char*)&PrintTableGB[4][0]);
				}
				else if(Line==5)
				{
					Rtc.GetNowDateTimeString(1,&Rtc.DT, tBuffer);		 //打印时间
					memmove(&OneCharLineBuffer[10], &tBuffer[0], 19);
				} 
				/*else if(Line==6)															 
					strcpy((char*)OneCharLineBuffer,(char*)&PrintTableGB[Line][0]);	 //速度记录字头	*/
				 
				
				while(GetNewCharLine==0) 
				{
					os_dly_wait(1);	
					
				}	//等待打印结束
				GetNewCharLine = 0;					
			}
			
			if(c!=0)									///////////////////////////有两个日历天疲劳驾驶记录
			{	
				if(PrintPaperStu==0)
				{
					StatusCnt = epPRINT_FREE;
					PrintEvent1=0;														 //清打印进程标识
				   PrintEvent2=0;
				   KeyDisp.KEY_DL.LongTimeNoKey = 20*5;
				   uLED.MainMenuCnt = PRINT_NO_PAPER_MENU;
				   return;
				} 
				SetBufferValue(OneCharLineBuffer,CHAR_NUMBER_PER_LINE,0);
				strcpy((char*)OneCharLineBuffer,(char*)&PrintTableGB[7][0]);
				while(GetNewCharLine==0) 
				{
				   os_dly_wait(1);
				}  
				GetNewCharLine = 0;
			   for(t=0;c>0;c--,t++)
				{
					if(PrintPaperStu==0)
					{
						StatusCnt = epPRINT_FREE;
						PrintEvent1=0;														 //清打印进程标识
					   PrintEvent2=0;
					   KeyDisp.KEY_DL.LongTimeNoKey = 20*5;
					   uLED.MainMenuCnt = PRINT_NO_PAPER_MENU;
					   return;
					} 
					SetBufferValue(OneCharLineBuffer,CHAR_NUMBER_PER_LINE,0);
					if(OVT_AddrTab[t]<8192)			// 取铁电
					  DFE.Read(OVT_AddrTab[t],LEN_SAVE_FATIGUE_DRIVING_INFO,tBuffer);
					else							  // 取flash
					  DFF.AnyNumberRead(OVT_AddrTab[t],LEN_SAVE_FATIGUE_DRIVING_INFO,tBuffer);

	
					for(Line=0;Line<4;Line++)
					{
						SetBufferValue(OneCharLineBuffer,CHAR_NUMBER_PER_LINE,0);
						strcpy((char*)OneCharLineBuffer,(char*)&PrintTableGB[Line+8][0]);
						if(Line==0)
						{
						  OneCharLineBuffer[4] = (t+1)/10 + 0x30;											//疲劳次数
						  OneCharLineBuffer[5] = (t+1)%10 + 0x30;
						}
						else if(Line==1)
						  memmove(&OneCharLineBuffer[12], tBuffer, 18);									//驾驶证号
						else if(Line>=2)
						{  
						  OneCharLineBuffer[12] = (tBuffer[(Line+1)*6]>>4) + 0x30;				    //日期
						  OneCharLineBuffer[13] = (tBuffer[(Line+1)*6]&0x0f) + 0x30;
						  OneCharLineBuffer[15] = (tBuffer[(Line+1)*6+1]>>4) + 0x30;
						  OneCharLineBuffer[16] = (tBuffer[(Line+1)*6+1]&0x0f) + 0x30;
						  OneCharLineBuffer[18] = (tBuffer[(Line+1)*6+2]>>4) + 0x30;
						  OneCharLineBuffer[19] = (tBuffer[(Line+1)*6+2]&0x0f) + 0x30;		 
	
						  OneCharLineBuffer[21] = (tBuffer[(Line+1)*6+3]>>4) + 0x30;				//时间
						  OneCharLineBuffer[22] = (tBuffer[(Line+1)*6+3]&0x0f) + 0x30;
						  OneCharLineBuffer[24] = (tBuffer[(Line+1)*6+4]>>4) + 0x30;
						  OneCharLineBuffer[25] = (tBuffer[(Line+1)*6+4]&0x0f) + 0x30;
						  OneCharLineBuffer[27] = (tBuffer[(Line+1)*6+5]>>4) + 0x30;
						  OneCharLineBuffer[28] = (tBuffer[(Line+1)*6+5]&0x0f) + 0x30;	

						}
						while(GetNewCharLine==0) 
						{
						    os_dly_wait(1);
			       	}  
						GetNewCharLine = 0;					
					}
				 }
			 }
			 else																									  //无疲劳驾驶记录
			 {
			 	  SetBufferValue(OneCharLineBuffer,CHAR_NUMBER_PER_LINE,0);
				  strcpy((char*)OneCharLineBuffer,(char*)&PrintTableGB[21][0]);
				  while(GetNewCharLine==0) 
				  {
					  os_dly_wait(1);
				  }  
				  GetNewCharLine = 0;
			 }
			 for(Line=0;Line<4;Line++)
			 {										 //打印签名
				  
				if(PrintPaperStu==0)
				{
					StatusCnt = epPRINT_FREE;
					PrintEvent1=0;														 //清打印进程标识
				   PrintEvent2=0;
				   KeyDisp.KEY_DL.LongTimeNoKey = 20*5;
				   uLED.MainMenuCnt = PRINT_NO_PAPER_MENU;
				   return;
				 } 
				 SetBufferValue(OneCharLineBuffer,CHAR_NUMBER_PER_LINE,0);
				 strcpy((char*)OneCharLineBuffer,(char*)&PrintTableGB[Line+18][0]);
				 while(GetNewCharLine==0) 
				 {
				   os_dly_wait(1);
				 }  
				 GetNewCharLine = 0;
			 }
			 Print.SetPrintStatus(epWALKING_PAPER_N_DOT_LINE);			  
			 Print.WalkingPaperOneDotLine(MOTOR_FORWARD,(16*8));		 //打印完走纸8行
		 	 PrintEvent1=0;														 //清打印进程标识
		 	 PrintEvent2=0;
		 
	}
	else
	{
		GetNewCharLine = 0;
		StatusCnt = epPRINT_START;
		CharLineCnt = 1;
		strcpy((char*)OneCharLineBuffer,"1234567890ABCDEF-GHIJKLMNOPQRSTU\0");
	}

	if(TD.Display==tDISP_PRINT_TEST)
		TD.Display = 0x01;	
}



void		PrintCls::SetPrintStatus(uint32 s)
{
	StatusCnt = s;
}



/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	函数名称：
   功能描述：
   入口参数：
   出口参数：
   全局变量: 
   创建时间：
*/ 	
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ 以上原形声明的函数供本函数调用
void		PrintCls::SendOneLineDotData(void)
{
	uint32	i;
	NOP();	NOP();
	if(DotLineCnt) //24
	{
		for(i=0;i<BYTE_NUMBER_PER_DOT_LINE;i++)		// 每行48字符,384点阵
		{
			Print.WriteByte(LatticeBuffer[DotLineAddr+i]);
			//Sys.Delay_uS(50);
		}
		PRINT_DATA_LATCH_LOW;
		Sys.Delay_uS(5);
		PRINT_DATA_LATCH_HIGH;	
		Sys.Delay_uS(500);
		//Uart.putb(1, BYTE_NUMBER_PER_DOT_LINE, &LatticeBuffer[DotLineAddr] );		 
		DotLineAddr += BYTE_NUMBER_PER_DOT_LINE;					
		DotLineCnt--;
				
	}
}





extern		"C"		void		MCPWM_IRQHandler(void)		__irq
{
	Print.PrintStatus();
	LPC_MCPWM->MCINTFLAG_CLR = 0xffffffff;	
	
}





/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	函数名称：
   功能描述：
   入口参数：
   出口参数：
   全局变量: 
   创建时间：
   注：本中断程序只支持连续字符行，即不管是否空白行，都启动正常打印程序（行距、取数加热16点行）
   如果需要留空白行，请在后台程序直接驱动走纸步进电机。（每点行4拍，每行字64拍）
*/ 	
void			GetEN_Code(unsigned char  Data,unsigned char  *pData);
void			GetCN_Code(unsigned short Data,unsigned char *pData);
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ 以上原形声明的函数供本函数调用
void		PrintCls::PrintStatus(void)
{
		
	//uint32	i;
	/////////////////////////////////////////////////////////////
	NOP();	NOP();
	IntCnt++;
	switch(StatusCnt)
	{
		case epPRINT_FREE:
			
			STEP_MOTOR_STOP;
			//HC595.Out2.Byte &= 0xc0;
			//HC595_OUTPUT2;
			PRINT_DATA_LATCH_HIGH;
			Frinting = FALSE;	
			PrintBusy = FALSE;		
		break;

		/////////////////////////////////////////////////////////// 指定步进电机节拍
		case epWALKING_PAPER_N_DOT_LINE:
			
			Frinting = TRUE;
			PrintBusy = TRUE;	
			if(StepTickCnt)
			{
				WalkingPaper();
				StepTickCnt--;
			}
			else
			{
				StatusCnt = epPRINT_FREE;
			}			
		break;

		/////////////////////////////////////////////////////////// 启动打印（）
		case epPRINT_START:		// 注：启动打印前必须先做电源开启、写字符行数据
			
			StatusCnt = epGET_NEW_LINE_CHAR;
			Frinting = TRUE;
			PrintBusy = TRUE;	
			
		break;
		
		/////////////////////////////////////////////////////////// 取新的字符行
		case epGET_NEW_LINE_CHAR:
			
			PrintBusy = TRUE;	
			if(CharLineCnt)
			{
				// 注：取点阵之前在后台程序里先更新OneCharLineBuffer
				GetOneLineCharLattice();
				CharLineCnt--;				
				StatusCnt = epWALKING_PAPER_4DOT_LINE;	// 走纸，行距
				WalkingPaperOneDotLine('F',1);	
				/// 立即步进1节拍
				WalkingPaper();
				StepTickCnt--;		
				GetNewCharLine = TRUE;			// 后台检测到为真，可以更新OneCharLineBuffer			
			}
			else
			{
				StatusCnt = epPRINT_FREE;
			}
			
		break;

		/////////////////////////////////////// 走纸4点行（步进16拍），留行距，准备打印新的一行字
		case epWALKING_PAPER_4DOT_LINE:
			
			PrintBusy = TRUE;	
			if(StepTickCnt)
			{
				WalkingPaper();
				StepTickCnt--;
			}
			else
			{
				//STEP_MOTOR_STOP;				
				////////////////////////////// 走纸的同时，取1行字符的点阵				
				DotLineAddr = 0;
				DotLineCnt = DOT_LINE_NUMBER_PER_CHAR;
				/// 留空行距步进最后1拍，无需等待，立即送数1点行
				SendOneLineDotData();
				/// 送完1点行数据，立即启动加热
				//HC595.Out2.Byte &= 0xc0;
				//HC595.Out2.Byte |= 0x07;			//一次性加热所有的点，可以避免误加热点，但是瞬间电流过大。有待确认  打印越快越不会出现误加热点，但是也越不清晰，因为加热时间不够
				//HC595.Out2.Byte |= 0x3F;	
				//HC595_OUTPUT2;	
				PRINT_HOT_ON;			
				StatusCnt = epHEATING;
				HeatCnt = 6;
				IntCnt = 0;
			}			
		break;		
		
		/////////////////////////////////////////////////////////// 加热
		case epHEATING:			
		
			//STEP_MOTOR_STOP;
			if(IntCnt%3==0)									  //加热时间越长，打印的字体越清晰
			{
				if(HeatCnt<6)
				{	
					PRINT_HOT_ON;
					HeatCnt++;
				}
				else
				{
					StatusCnt = epWALKING_PAPER_1DOT_LINE;
					WalkingPaperOneDotLine('F',1);					
					PRINT_HOT_OFF;
					/// 加热完成，无需等待，立即步进一节拍
					WalkingPaper();
					StepTickCnt--;
				}
			}
		break;
		
		/////////////////////////////////////////////////////////// 走纸1点行（步进4拍）
		case epWALKING_PAPER_1DOT_LINE:
						
			if(StepTickCnt)
			{
				WalkingPaper();
				StepTickCnt--;
			}
			else
			{								
				/// 走纸步进最后1拍，无需等待，立即送数1点行
				if(DotLineCnt)
				{
					//STEP_MOTOR_STOP;
					SendOneLineDotData();
					/// 送完1点行数据，立即启动加热
					//HC595.Out2.Byte &= 0xc0;
					//HC595.Out2.Byte |= 0x07;
					//HC595.Out2.Byte |= 0x3F;	
					//HC595_OUTPUT2;	
					PRINT_HOT_ON;			
					StatusCnt = epHEATING;
					HeatCnt = 6;
					IntCnt = 0;
				}
				else 	// 16点行完毕，启动新的一行字符
				{
					StatusCnt = epGET_NEW_LINE_CHAR;	
				}
			}			
					
		break;
		
		
		default:	break;
		
			
	}
}




/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	函数名称：
   功能描述：
   入口参数：
   出口参数：
   全局变量: 
   创建时间：
*/ 	
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ 以上原形声明的函数供本函数调用
void		PrintCls::WalkingPaperOneDotLine(uchar Dir,uint32 Line)
{
	StepDir = Dir;
	StepTickCnt = Line;
	StepTickCnt <<= 2;		// *4  1行点阵4个节拍
}




/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	函数名称：
   功能描述：
   入口参数：
   出口参数：
   全局变量: 
   创建时间：
*/ 	
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ 以上原形声明的函数供本函数调用
void		PrintCls::WalkingPaper(void)
{

	NOP();	NOP();
	if(StepDir==MOTOR_FORWARD)		// 正向
	{
		switch(StepTick)
		{
			case 0:	STEP_MOTOR_TICK_1;	StepTick=1;		break;
			case 1:	STEP_MOTOR_TICK_2;	StepTick=2;		break;
			case 2:	STEP_MOTOR_TICK_3;	StepTick=3;		break;
			case 3:	STEP_MOTOR_TICK_4;	StepTick=4;		break;
			case 4:	STEP_MOTOR_TICK_5;	StepTick=5;		break;
			case 5:	STEP_MOTOR_TICK_6;	StepTick=6;		break;
			case 6:	STEP_MOTOR_TICK_7;	StepTick=7;		break;
			case 7:	STEP_MOTOR_TICK_8;	StepTick=0;		break;
			default: STEP_MOTOR_STOP;	break;				
		}
	}
	else if(StepDir==MOTOR_BACK)
	{
		switch(StepTick)
		{
			case 0:	STEP_MOTOR_TICK_8;	StepTick=1;		break;
			case 1:	STEP_MOTOR_TICK_7;	StepTick=2;		break;
			case 2:	STEP_MOTOR_TICK_6;	StepTick=3;		break;
			case 3:	STEP_MOTOR_TICK_5;	StepTick=4;		break;
			case 4:	STEP_MOTOR_TICK_4;	StepTick=5;		break;
			case 5:	STEP_MOTOR_TICK_3;	StepTick=6;		break;
			case 6:	STEP_MOTOR_TICK_2;	StepTick=7;		break;
			case 7:	STEP_MOTOR_TICK_1;	StepTick=0;		break;
			default: STEP_MOTOR_STOP;	break;				
		}
	}		
}



/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	函数名称：
   功能描述：
   入口参数：
   出口参数：
   全局变量: 
   创建时间：
*/ 	
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ 以上原形声明的函数供本函数调用
void			PrintCls::Init(uchar Set)
{
	uint32	Fclk;
	StepTick = 0;
	//PrintEvent1 = 0;
	//PrintEvent2 = 0;	
	STEP_MOTOR_PIN_OUT;
	STEP_MOTOR_STOP;						// 
	//LPC_PINCON->PINSEL3 |= (1UL<<18)|(1UL<<20)|(1UL<<24)|(1UL<<26);
	if(Set)
	{
		Fclk = LPC_SC->PCLKSEL1;	 //
		Fclk &= (3UL<<30);				 	// B.31.30
		Fclk >>= 30;
		Fclk = PeriClock[Fclk];
		LPC_MCPWM->MCCON_CLR = 0xffffffff;	
		LPC_MCPWM->MCCNTCON_CLR = 0x01;				// 定时器模式	
		LPC_MCPWM->MCPER0 = Fclk/500;					// 周期
		LPC_MCPWM->MCPW0 = Fclk/250;				// 半周期
		NVIC_SetPriority(MCPWM_IRQn, MCPWM_INT_PRIORITY);     
   		NVIC_EnableIRQ(MCPWM_IRQn);	
		LPC_MCPWM->MCINTEN_SET = (1UL<<0);			// 上限中断
		LPC_MCPWM->MCCON_SET = 0x01;	
		StatusCnt = epPRINT_FREE;
		///////////////////////////////////////////////////////////////
		PRINT_POWER_ON;
		PRINT_HOT_OFF;		// 禁止加热输出
		STEP_MOTOR_POWER_ON;				// 电机上电	
		PRINT_DATA_LATCH_HIGH;
	}
	else
	{
		NVIC_DisableIRQ(MCPWM_IRQn); 
		
		StatusCnt = epPRINT_FREE;
		Frinting = FALSE;	
		PrintBusy = FALSE;	
		PRINT_HOT_OFF;		// 禁止加热输出
		LPC_MCPWM->MCCON_CLR = 0xffffffff;	
		PRINT_DATA_LATCH_HIGH;
		STEP_MOTOR_STOP

		PRINT_POWER_OFF;				  //打印头电源不能关闭，因为会导致SSP0口电平被拉低
		
		
		STEP_MOTOR_POWER_OFF;				// 关闭电机	
		
		
			
		LPC_GPIO1->FIOSET |= ((3UL<<25)|(3UL<<28)); 	// 正常工作时，步时电机驱动停止为低电平。这里高电平为了省电（7407输出）

	}

	

}	

#define		PrintSPI_LATCH				(1UL<<17)
#define		PrintSPI_CLK				(1UL<<16)
#define		PrintSPI_MOSI				(1UL<<15)
#define		PrintSPI_MISO				(1UL<<14)
#define		PrintSPI_Delay()        {NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();}

/***************************************************************************************
** 函数名称：  WriteByte() 
** 功能描述： 
** 入口参数：待输出的数据
** 出口参数输出成功与否标志，成功为0，不成功为非0：
** 全局变量: 
** 创建时间：
** 修改时间：
****************************************************************************************/
uchar	PrintCls::WriteByte(uchar Data)
{

		uint16		i;
		NOP();	NOP();
		//while(Busing)	{os_dly_wait(1);}
		//Busing = 1;
		LPC_GPIO1->FIODIR  &= ~PrintSPI_MISO;
		LPC_GPIO1->FIODIR  |= (PrintSPI_LATCH|PrintSPI_CLK|PrintSPI_MOSI);			// out put 	
		PrintSPI_Delay();
		for(i=0;i<8;i++)
		{
			LPC_GPIO1->FIOCLR |= PrintSPI_CLK;
			if(Data&0x80)
				LPC_GPIO1->FIOSET |= PrintSPI_MOSI;
			else
				LPC_GPIO1->FIOCLR |= PrintSPI_MOSI;
			PrintSPI_Delay();
			LPC_GPIO1->FIOSET |= PrintSPI_CLK;
			PrintSPI_Delay();
			Data <<= 1;							// left shift 1 bit
		}
		//LPC_GPIO1->FIOCLR |= SPI1_CLK;
		//LPC_GPIO1->FIOCLR |= SPI1_MOSI;
		Busing = 0;
		return 0;
	
}

////addBysun
uchar	PrintCls::shift_Value(uchar org, uint16 index)
{
 	org = (org & (0x01<<index))>>index;
	return org;
}



PrintCls::PrintCls()
{
	OneCharLineBuffer = pShareMem;
	LatticeBuffer = pShareMem + BYTE_LATTICE;
	 //pPrintBuf = pShareMem;					//BYTE_LATTICE 大小的内存分配指针
	//pOneCLB = pShareMem + BYTE_LATTICE;

}

/****************************** end of file ****************************************/
