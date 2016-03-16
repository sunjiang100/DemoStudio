/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$  Copyright (c)  $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
$												广州国业电子科技有限公司
$									
$                       	http://www.guoyee.com		 Email: guoyegps@126.com	  
$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$  Copyright (c)  $$$$$$$$$$$$$$$$$$$$$$$$$$$ */
/*  
                                      文件描述
文件名:   GyProtocol.cpp
功能描述：
编译环境：
目标CPU: 
作者：
创建时间：
建议编辑环境：UltraEdit 11.00a+  制表符宽度为三个字符，程序可读性更好。
$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$*/

#include    	"UART0.h"
#include    	"UART1.h"

#define			GY19056_GLOBALS		1
#include			"..\Common\GB_T19056.h"
#include			"..\Common\GY_T19056.h"


#include			"..\GSM_GPRS\GSM_GPRS.h"
#include			"..\GPS\GPS.h"
#include			"..\Common\EEPROM_Addr.h"		  
#include			"..\Common\FLASH_ADDR.H"

#include			"..\Common\Algol.h"

#include			"..\RTC\RTC.h"
#include			"..\Uart\UART2.h"
#include			"..\Uart\UART3.h"

#include			<string.h>
#include			<stdlib.h>
#include			<stdio.h>
#include			"..\FLASH\DFE_25xxx.H"
#include			"..\FLASH\DFF_25xxx.H"
#include			"..\Switch\switch.h"
#include			"..\SPI\SSP.h"
#include			"..\IIC\IIC0.h"

#include			"..\KeyDisp\KeyDisp.h"
#include			"..\Speed\Speed.h"
#include			"..\common\Algol.h"
#include		   "..\PWM\PWM.h"
#include			"..\UserSys.h"


uint32	U0_GyIndex;
extern 	uchar 	CreateUserFlag,jt_auth_code[];
extern	uchar		OnlyUseBD_GPS;

#define		USE_GUOYEE_PROTOCOL		0			 // 空间够，暂时不用 这个协议



#if (USE_GUOYEE_PROTOCOL)
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
	
uint16       UartCls::GuoyeeProtocolProcess(uint16 LenRec,uchar *pRecBuffer,uchar *pSendBuffer)
{

		
	uint32	RecIndex,Sum;
	uint16	Cmd,DataOffSet,Check;
	uchar		t,Error,CheckType;		
	uint16	i,Length,DataLength,Len, tmp;
	uchar		*pData;
	union
	{
		uint32	B32[4];
		uint16	B16[8];
		uchar		Buffer[16];
	}Tp;
	
	struct
	{
		uint16   Y;
		uchar		M;
		uchar		D;
	} RTCD;
	pGY_COMM_PROTOCOL		pGy;
	pGPS_SEND	pSd;
	pRW_EEP		pEE;

	/////////////////// RS232通讯，认为每次通讯只有一个数据包，不处理粘包情况。
	//处理数据不能延时(非测试指令），否则pRecBuffer有可能被刷新
	NOP();	NOP();
	Error = 0;
	Length = 0;
	pGy = (pGY_COMM_PROTOCOL)pRecBuffer;
	Cmd 				= pGy->Cmd;
	RecIndex			= pGy->Index;
	DataLength 		= pGy->Attr.Len;
	DataLength	  &= 0x07ff;
	if( pGy->Attr.Bits.Pack )
	{
		DataOffSet = DATA_OFFSET_M_PACK;
		Len = DataLength+8+4;		// 参与校验的长度
	}
	else
	{
		DataOffSet = DATA_OFFSET_1_PACK;
		Len = DataLength+8;		// 参与校验的长度
	}
	CheckType = 0;
	if( pGy->Attr.Bits.Check == 0)				// 累加和校验
	{  
		CheckType = 0;
		for(Check=0,i=GY_PROTOCOL_CHECK_START_ADDR;i<(Len+GY_PROTOCOL_CHECK_START_ADDR);i++)
		{
			Check += *(pRecBuffer+i);
		}
		if( Check != pGy->Check)		Error = TRUE;
	}
	else if( pGy->Attr.Bits.Check == 1)			// 异或校验
	{
		CheckType = 1;
		for(Check=0,i=GY_PROTOCOL_CHECK_START_ADDR;i<(Len+GY_PROTOCOL_CHECK_START_ADDR);i++)
		{
			Check ^= *(pRecBuffer+i);
		}
		if( Check != pGy->Check)		Error = TRUE;
	}
	else if( pGy->Attr.Bits.Check == 2)			// CRC16校验
	{
		CheckType = 2;
		Check = CalCRC16( (pRecBuffer+GY_PROTOCOL_CHECK_START_ADDR),Len);
		if( Check != pGy->Check )		Error = TRUE;
	}
	else
	{
		
	}

	NOP();	NOP();
	Error = 0;
	Length = 0;		
	SetBufferValue(pSendBuffer,(256+16),0);		
	////////////////////////////////////////////////////////////////////////////////////
	switch(Cmd)
	{	
		////////////////////////////////////////////// 设置疲劳驾驶相关参数
		case	eGY_rCMD_DRIVING_PARA:	// 0x80 // 300 120 1800 600 30
			// GYS \x2c\x01\x00\x00 \x78\x00\x00\x00 \x08\x07\x00\x00 \x58\x02\x00\x00 \x1e\x00\x00\x00 
			if( (pRecBuffer[DataOffSet]=='S') )			// 必须全部同时设置，否则可能出错
			{
				SpeedC.SetGetPara( eSPEED_CMD_SET_DRIVING_TIME,LEN_DRIVING_TIME_LIMIT,(uchar*)&pRecBuffer[DataOffSet+1]);
				Length = DataOffSet;
			}
			else
			{
				SpeedC.SetGetPara( eSPEED_CMD_GET_DRIVING_TIME,LEN_DRIVING_TIME_LIMIT,(uchar*)&pSendBuffer[DataOffSet]);
				Length = DataOffSet+LEN_DRIVING_TIME_LIMIT;
			}			
			break;
		////////////////////////////////////////////// 设置超速相关参数 
		case	eGY_rCMD_OVER_SPEED_PARA:	// 0x81 // GY G 1 \030 \005 \003 \015\000 \000\000
			// GY S 1 \030 \005 \003 \015\000 \000\000 
			if( (pRecBuffer[DataOffSet]=='S') )	  		// 必须全部同时设置，否则可能出错
			{
				
				SpeedC.SetGetPara( eSPEED_CMD_SET_OVER_SPEED,LEN_OVER_SPEED_PARA,(uchar*)&pRecBuffer[DataOffSet+1]);
				Length = DataOffSet;
			}
			else
			{
				SpeedC.SetGetPara( eSPEED_CMD_GET_OVER_SPEED,LEN_OVER_SPEED_PARA,(uchar*)&pSendBuffer[DataOffSet]);
				Length = DataOffSet+LEN_OVER_SPEED_PARA;
			}
			
			break;
			
		case eGY_rCMD_LOW_POWER_ALARM_VALUE:	
			
			// GY S \xff\xff\xff\xff \085\x00\x00\x00
			if( (pRecBuffer[DataOffSet]=='S') )
			{
				memmove( Tp.Buffer,&pRecBuffer[DataOffSet+1],8);
				HighLowLevel = Tp.B32[0];
				LowPowerAlarmValue = Tp.B32[1];			
				DFE.Write(EE_HIGH_LOW_LEVEL,8,Tp.Buffer);		
				Length = DataOffSet;
			}
			else
			{
				Tp.B32[0] = HighLowLevel;
				Tp.B32[1] = LowPowerAlarmValue;
				memmove( (uchar*)&pSendBuffer[DataOffSet],Tp.Buffer,8);
				Length = DataOffSet+8;
			}
			break;
		
		//		$$$$$$$$$$$$$$$$$$$$  读取时实时钟
		case eGY_rCMD_GET_REAL_TIME:
			NOP();	NOP();
			if(pRecBuffer[DataOffSet]=='I')	// CPU内置时钟
			{
				Length = DataOffSet+6;								
				Rtc.GetRTC_BCD((RTC_STR *)&pSendBuffer[DataOffSet]);										
			}
			else if(pRecBuffer[DataOffSet]=='A')
			{
				Length = DataOffSet+16;
				Rtc.ReadR8025((RTC_STR *)NULL,(uchar*)&pSendBuffer[DataOffSet]);		
			}
			else
			{
				Length = DataOffSet+6;
				Rtc.ReadR8025((RTC_STR *)&pSendBuffer[DataOffSet],(uchar*)NULL);				
			}
			break;
		
		/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
		GY \x11 \x08 \x17 \x11 \x54 \x53
		*/
		//		$$$$$$$$$$$$$$$$$$$$  设置时实时钟
		case eGY_rCMD_SET_REAL_TIME:
			NOP();	NOP();
			
			Length = DataOffSet;			
			Rtc.SetRTC_BCD((RTC_STR *)&pRecBuffer[DataOffSet]);
						
			RTCD.Y = LPC_RTC->YEAR;
			RTCD.M = LPC_RTC->MONTH;
			RTCD.D = LPC_RTC->DOM;
			if((RTCD.M==1)||(RTCD.M==2))
			{
  				RTCD.M += 12;
  				RTCD.Y--;
			}
			t = ((RTCD.D+2*RTCD.M+3*(RTCD.M+1)/5+RTCD.Y+RTCD.Y/4-RTCD.Y/100+RTCD.Y/400)+1)%7;	 //算星期
			
			LPC_RTC->DOW = t;
						
			pRecBuffer[DataOffSet+10] = t;			
			Rtc.SetRtcR8025((RTC_STR *)&pRecBuffer[DataOffSet]); 
			
			break;
		
		
		case eGY_rCMD_SELECT_SIM_CARD:				// 选择SIM卡
			if( (pRecBuffer[DataOffSet]=='I')||(pRecBuffer[DataOffSet]=='E'))
			{
				DFE.Write(EE_SELECT_SIM_CARD,1,(uchar*)&pRecBuffer[DataOffSet]);
				if(pRecBuffer[DataOffSet]=='I')	
					{	USE_SOP_SIM_CARD;	 UseWthSimCard = USE_IN_SIM;}
				else														
					{	USE_DIP_SIM_CARD;	 UseWthSimCard = USE_OUT_SIM;}
			}
			else
			{
				DFE.Read(EE_SELECT_SIM_CARD,1,(uchar*)&pSendBuffer[DataOffSet]);
			}
			Length = DataOffSet+1;
		break;

		case	eGY_rCMD_SET_SIM_NUMBER:		//设置SIM卡号码
					
			memmove((uchar*)sTelNum.Sim,(uchar*)&pRecBuffer[DataOffSet],16);
			sTelNum.Sim[16] = 0;
			Length = strlen((char *)sTelNum.Sim);
			DFE.Write(EE_SIM_NUMBER,16,(uchar*)sTelNum.Sim);
			DecCharToPressBCD(Length,(char *)sTelNum.Sim,SimNumberBCD);
			for(i=0;i<8;i++)	{ if(SimNumberBCD[i]==0)	SimNumberBCD[i] = 0x0f;	}
			Length = DataOffSet;
		break;


		case	eGY_rCMD_GET_SIM_NUMBER:	  //采集SIM卡号码
			
	      strcpy((char*)&pSendBuffer[DataOffSet],(char*)sTelNum.Sim);
			Length = strlen((char*)sTelNum.Sim);			
			Length += DataOffSet;
		break;

		//  	$$$$$$$$$$$$$$$$$$$$ 读或写指定页的Nand Flash的数据
		//    串口接收格式 xx xx xxxx xx *x	   
		//    GYE-Test I \x00 \x00 \x00 \x00 \x00 \x00 \x00 \x00 \x00 \x00 \x00 \x00 \x00 

		case eGY_rCMD_GET_SET_EE_DATA:
		
			pEE = (pRW_EEP)&pRecBuffer[DataOffSet];

			t = pRecBuffer[DataOffSet];

			if( t=='S')		// 	SPI铁电  GY S \x00 R \xa0 \x00\x00 \x10\x00
			{
				if(pEE->RW == 'E')
				{
					EreadFRAM_RecodeMem( pEE->Addr,  pEE->Length);
				}				
				else if(pEE->RW == 'W')			// 'W' 写
				{
					DFE.Write(pEE->Addr,pEE->Length,&pRecBuffer[DataOffSet+8]);		 /////////// change by wrc  ///////////////
				}
				else	//if(pEE->RW == 'R')			  	// 'R' 读
				{
					Length = pEE->Length;
					DFE.Read(pEE->Addr,Length,&pSendBuffer[DataOffSet]);					
				}		
			}
			else if( t=='I')			// I2C 	
			{
				if(pEE->RW == 'W')			// 'W' 写	  	GY I \x00 W \xa0 \x00\x00 \x10\x00
				{
					I2C0.WriteEEPROM(pEE->DS,pEE->PageLen,pEE->Addr,pEE->Length,&pRecBuffer[DataOffSet+8]);  //pEE->PageLen		/////////// change by wrc  ///////////////
				}
				else if(pEE->RW == 'R')			  	// 'R' 读		GY I \x00 R \xa0 \x00\x00 \x10\x00
				{
					Length = pEE->Length;
					I2C0.ReadEEPROM(pEE->DS,pEE->Addr,Length,&pSendBuffer[DataOffSet]);
				}
				
			}			
			else
			  Error = TRUE;
			Length += DataOffSet;	
			break;		
		//		$$$$$$$$$$$$$$$$$$$$  读写取数据存储器 SPI-Flash
		//    格式 x x xx xx    GY F \x00 R D \xE0\x1E \x10\x00 
		case eGY_rCMD_GET_SET_DF_DATA:
			
			Length = DataOffSet;
			if( (pRecBuffer[Length+0]=='B')&&(pRecBuffer[Length+1]=='D') )
			{
				GetFlashData.TotalPage = FF_GPS_BLIND_MESSAGE_TOTAL_PAGE;
				GetFlashData.StartPage = FF_GPS_BLIND_MESSAGE_START_PAGE;

				memmove( &pSendBuffer[DataOffSet],(uchar*)"Get All Blind Data Ack",22);
				Length = DataOffSet+22;
			}
			else if( (pRecBuffer[Length+0]=='H')&&(pRecBuffer[Length+1]=='D') )
			{
				GetFlashData.TotalPage = FF_GPS_MESSAGE_TOTAL_PAGE;
				GetFlashData.StartPage = FF_GPS_MESSAGE_START_PAGE;
				memmove( &pSendBuffer[DataOffSet],(uchar*)"Get All History Data Ack",24);
				Length = DataOffSet+24;
			}
			else if( (pRecBuffer[Length+0]=='D')&&(pRecBuffer[Length+1]=='D') )
			{
				memmove( (uchar*)&GetFlashData, &pRecBuffer[Length+2], 8 );
				memmove( &pSendBuffer[DataOffSet],(uchar*)"Get Flash Data Ack",18);
				Length = DataOffSet+18;
			}
			
			if( GetFlashData.TotalPage )
			{
				Length -= DataOffSet;
				memmove(U0_TxdBuffer,&pSendBuffer[DataOffSet],Length);
				Txd.Length = Length;
				pTxd = U0_TxdBuffer;
				Txd.Length--;
				LPC_UART0->THR  = *pTxd++;						// 发送数据		
				Txd.Sending = TRUE;
				os_dly_wait(20);
				return 0;
			}
			
			pEE = (pRW_EEP)&pRecBuffer[DataOffSet];
			
			t = pRecBuffer[DataOffSet];			

			if( t == 'F' )							// SPI Flash
			{
				if(pEE->RW == 'E')			// 'W' 擦除
				{
					if(pEE->DS == 'B')		// 擦除块
					{
						DFF.BlockErase(pEE->Addr);						
					}
					else if(pEE->DS == 'S')	// 擦除扇区
					{
						DFF.SectorEraseWaitOK(pEE->Addr,10);
					}
					Length = 0;
				}
				else if(pEE->RW == 'W')			// 'W' 写
				{
					DFF.PageProgramWaitOK(pEE->Addr,pEE->Buffer,3);
					Length = 0;
				}
				else	//	if(pEE->RW == 'R')		// 'R' 读
				{
					if(pEE->DS == 'D')			//  读数据
					{
						DFF.PageRead(pEE->Addr,&pSendBuffer[DataOffSet]);
						Length = 256;
					}
					else if(pEE->DS == 'S')		// 读状态寄存器和ID
					{
						pSendBuffer[DataOffSet] = DFF.ReadStatusRegister();
						DFF.ReadJEDEC_ID(&pSendBuffer[DataOffSet+1]);
						Length = 5;	
					}
				}
			}
			else
				Error = TRUE;
			Length += DataOffSet;	
			break;		
			
		case eGY_rCMD_GET_SET_NF_DATA:			// Nand Flash

		 	break;	
		 	
		 	
		 	
		case 	eGY_rCMD_SET_INIT_SYS:       //恢复出厂设置		 同无线通道功能一样
		  	DFE.Read(EE_JT_MAIN_SERVER, LEN_JT808_IP_DNS, (uchar *)&JT808_IP_DNS ); //Bf.tBuffer);
			os_dly_wait(1);
			EreadFRAM_RecodeMem(16, 0x2000-16);									   //擦除整片Fram	  除主服务器地址
			os_dly_wait(1);
			DFE.Write(EE_JT_MAIN_SERVER, LEN_JT808_IP_DNS, (uchar *)&JT808_IP_DNS);
		   EreadFirstBlock();	
			Uart.puts("串口指令恢复出厂值，重启系统 ");
			os_dly_wait(100);
			DisableIRQ();
	      NOP();	NOP();	NOP();	NOP();
	      NOP();	NOP();	NOP();	NOP();
			for(;;)											     //恢复出厂设置还是要重启一下
	      {
	      	while(1);	
	      }
			
		  	break;	
		case  eGY_rCMD_ERASE_FLASH:	
			
			if ( (pRecBuffer[DataOffSet]=='S')&&(pRecBuffer[DataOffSet]=='E') )	// 擦除指定的扇区
			{
				
			}
			else if ( (pRecBuffer[DataOffSet]=='B')&&(pRecBuffer[DataOffSet]=='L') )	// 指定块
			{
				
			}
			
			break;
			
		case eGY_rCMD_ERASE_FLASH_RECORD_DATA:																			
			
			

			Length = DataOffSet;
		break;	
		
		//////////////////////////////////////// 擦除单个或全部Flash页存储计数器
		case	eGY_rCMD_CLEAR_FLASH_PAGE_CNT:
			Length = DataOffSet;
			if(  pRecBuffer[DataOffSet+0]=='S')
			{ 				
				if( pRecBuffer[DataOffSet+1]=='A') 
			   {
			   	SetBufferValue( (uchar*)&FlashPageCnt,LEN_FLASH_PAGE_CNT,0);
			   	DFE.Write( FM_FLASH_PAGE_CNT,LEN_FLASH_PAGE_CNT,(uchar*)&FlashPageCnt);
			   }
			   else if((pRecBuffer[DataOffSet+1]+pRecBuffer[DataOffSet+2]) < FLASH_PAGE_CNT_NUM )
				{					
					i = pRecBuffer[DataOffSet+1];		// 开始个数
					t = pRecBuffer[DataOffSet+2];		// 总个数
					pData = (uchar*)&FlashPageCnt;
					pData += (i*4);
					memmove( pData, &pRecBuffer[DataOffSet+3], t*4 );
					DFE.Write( FM_FLASH_PAGE_CNT,LEN_FLASH_PAGE_CNT,(uchar*)&FlashPageCnt);	
				}
			   else
				{
					Error = TRUE;
				}
			}			
			else	// 读
			{
				if( pRecBuffer[DataOffSet+1]=='A') 
			   {
			   	memmove(&pSendBuffer[DataOffSet], (uchar*)&FlashPageCnt,LEN_FLASH_PAGE_CNT);
			   	Length += LEN_FLASH_PAGE_CNT;
			   }
			   else if((pRecBuffer[DataOffSet+1]+pRecBuffer[DataOffSet+2]) < FLASH_PAGE_CNT_NUM )
				{					
					i = pRecBuffer[DataOffSet+1];		// 起始个数
					t = pRecBuffer[DataOffSet+2];
					pData = (uchar*)&FlashPageCnt;
					pData += (i*4);
					strcpy( (char*)pSendBuffer,"12345678 ");
					Length = DataOffSet+1;
					for(i=0;i<t;i++)
					{
						memmove( Tp.Buffer, pData, 4 );						
						Int2Str( Tp.B32[0], (char*)&pSendBuffer[Length] );
						strcat( (char*)pSendBuffer,",");
						Length = strlen( (char*)pSendBuffer );
						pData += 4;
						
					}					
				}
			   else
				{
					Error = TRUE;
				}
			}
		break;
		
		////////////////////////////////////// 读取Flash页存储计数器
		case eGY_rCMD_GET_FLASH_PAGE_CNT:
			
			memmove( &pSendBuffer[DataOffSet],(uchar*)&FlashPageCnt,LEN_FLASH_PAGE_CNT);
			DFE.Read( FM_FLASH_PAGE_CNT,LEN_FLASH_PAGE_CNT,(uchar*)&pSendBuffer[DataOffSet+LEN_FLASH_PAGE_CNT]);
			Length = DataOffSet+LEN_FLASH_PAGE_CNT*2;
		break;
		
		//		$$$$$$$$$$$$$$$$$$$$  8个标志位
		case eGY_rCMD_SET_8_FLAGS:
			NOP();	NOP();
			////////////////////////////////  GYE-Test
			if(  (pRecBuffer[LEN_U0_GB_FRAME_HEAD+0]=='G')&&(pRecBuffer[LEN_U0_GB_FRAME_HEAD+1]=='Y')\
			   &&(pRecBuffer[LEN_U0_GB_FRAME_HEAD+2]=='E')&&(pRecBuffer[LEN_U0_GB_FRAME_HEAD+3]=='-')\
				&&(pRecBuffer[LEN_U0_GB_FRAME_HEAD+4]=='T')&&(pRecBuffer[LEN_U0_GB_FRAME_HEAD+5]=='e')\
				&&(pRecBuffer[LEN_U0_GB_FRAME_HEAD+6]=='s')&&(pRecBuffer[LEN_U0_GB_FRAME_HEAD+7]=='t'))
			{ 
				TD.MainTask 	= 0;
				TD.GPS      	= 0;
				TD.GPRS     	= 0;
				TD.CAN		 	= 0;
				TD.SW       	= 0;
				TD.ADC      	= 0;
				TD.Display  	= 0;
				TD.Camera		= 0;
				TD.Speed			= 0;
				TD.Value			= 0;	
				TD.TestMode		= 0;			
				if(pRecBuffer[LEN_U0_GB_FRAME_HEAD+8])		TD.MainTask 	= ((0x55<<8)+pRecBuffer[LEN_U0_GB_FRAME_HEAD+8]); 							
				if(pRecBuffer[LEN_U0_GB_FRAME_HEAD+9])		TD.GPS      	= ((0x55<<8)+pRecBuffer[LEN_U0_GB_FRAME_HEAD+9]);				
				if(pRecBuffer[LEN_U0_GB_FRAME_HEAD+10])		TD.GPRS     	= ((0x55<<8)+pRecBuffer[LEN_U0_GB_FRAME_HEAD+10]);			// 				
				if(pRecBuffer[LEN_U0_GB_FRAME_HEAD+11])		TD.CAN		 	= ((0x55<<8)+pRecBuffer[LEN_U0_GB_FRAME_HEAD+11]); 		// 
				if(pRecBuffer[LEN_U0_GB_FRAME_HEAD+12])		TD.SW       	= ((0x55<<8)+pRecBuffer[LEN_U0_GB_FRAME_HEAD+12]);
				if(pRecBuffer[LEN_U0_GB_FRAME_HEAD+13])		TD.ADC      	= ((0x55<<8)+pRecBuffer[LEN_U0_GB_FRAME_HEAD+13]);			
				if(pRecBuffer[LEN_U0_GB_FRAME_HEAD+14])		TD.Display  	= ((0x55<<8)+pRecBuffer[LEN_U0_GB_FRAME_HEAD+14]);
				if(pRecBuffer[LEN_U0_GB_FRAME_HEAD+15])		TD.Camera		= ((0x55<<8)+pRecBuffer[LEN_U0_GB_FRAME_HEAD+15]);
				if(pRecBuffer[LEN_U0_GB_FRAME_HEAD+16])		TD.Speed			= ((0x55<<8)+pRecBuffer[LEN_U0_GB_FRAME_HEAD+16]);
				TD.Value = pRecBuffer[LEN_U0_GB_FRAME_HEAD+19];
				TD.Value <<= 8;
				TD.Value += pRecBuffer[LEN_U0_GB_FRAME_HEAD+18];
				if(TD.SW==0)
				{
				  SimuSpeed.Enable = 0;					  //结束开关量检测，也结束速度模拟
			     Pwm.UnInit();     // LPC_PINCON->PINSEL7 &= (~(3UL<<18));
				  Sys.PowerControl(PCONP_PWM1,0);
				}	
				
				if(TD.SW == tHC595_OUT1)		HC595.Out1.Byte = pRecBuffer[LEN_U0_GB_FRAME_HEAD+16];
				if(TD.SW == tHC595_OUT2)		HC595.Out2.Byte = pRecBuffer[LEN_U0_GB_FRAME_HEAD+16];

				if( (TD.GPS==tGPS_USE_GPS)||(TD.GPS==tGPS_USE_GPS_BD)||(TD.GPS==tGPS_USE_BD) )
					OnlyUseBD_GPS = 0;
				
				
				memmove(&pSendBuffer[DataOffSet],(uchar*)&TC,20);
				Length = 20;
				
					
			}
			else if(  (pRecBuffer[DataOffSet+0]=='T')&&(pRecBuffer[DataOffSet+1]=='e')\
			   &&(pRecBuffer[DataOffSet+2]=='s')&&(pRecBuffer[DataOffSet+3]=='t')\
				&&(pRecBuffer[DataOffSet+4]=='M')&&(pRecBuffer[DataOffSet+5]=='o')\
				&&(pRecBuffer[DataOffSet+6]=='d')&&(pRecBuffer[DataOffSet+7]=='e'))
			{ 
				TD.MainTask 	= 0;
				TD.GPS      	= 0;
				TD.GPRS     	= 0;
				TD.CAN		 	= 0;
				TD.SW       	= 0;
				TD.ADC      	= 0;
				TD.Display  	= 0;
				TD.Camera		= 0;
				TD.Value			= 0;	
				TD.TestMode		= 0;	
				if( (pRecBuffer[DataOffSet+8]=='A')&&(pRecBuffer[DataOffSet+9]=='u')&&\
					 (pRecBuffer[DataOffSet+10]=='t')&&(pRecBuffer[DataOffSet+11]=='o'))
					TD.TestMode 	= TEST_MODE_AUTO;
				else
				{
					TD.TestMode 	= TEST_MODE;
					SimuSpeed.Enable = ENABLE_FLAG;
					Sys.PowerControl(PCONP_PWM1,1);	
					Pwm.SetTargetSpeed(100,12); 
				}
			}	
			else if((pRecBuffer[DataOffSet+0]=='I')&&(pRecBuffer[DataOffSet+1]=='D')\
					&&(pRecBuffer[DataOffSet+2]=='L')&&(pRecBuffer[DataOffSet+3]=='E')\
			   	&&(pRecBuffer[DataOffSet+4]=='-')&&(pRecBuffer[DataOffSet+5]=='T')\
					&&(pRecBuffer[DataOffSet+6]=='e')&&(pRecBuffer[DataOffSet+7]=='s')\
					&&(pRecBuffer[DataOffSet+8]=='t'))
			{
			  	Idle.EnableInterIdleMode = 1;
				Idle.IdleModeFlag = 0;
				Idle.InterIdleModeTime = pRecBuffer[LEN_U0_GB_FRAME_HEAD+9];
				Idle.WakeUpTime = pRecBuffer[LEN_U0_GB_FRAME_HEAD+10];
				Idle.WakeUpLongTime = pRecBuffer[LEN_U0_GB_FRAME_HEAD+11];
				Idle.SecondCnt=0;						
				DFE.Write(EE_IDLE_STR,LEN_EE_IDLE,(uchar*)&Idle);	
				#if(DFE_DEBUG==TRUE)
				if(TD.MainTask==tMAIN_WRITE_DFE)	Uart.puts("DFE09\r\n\0");
				#endif									
			}
			
			else if(  (pRecBuffer[DataOffSet+0]=='T')&&(pRecBuffer[DataOffSet+1]=='e')\
			   &&(pRecBuffer[DataOffSet+2]=='s')&&(pRecBuffer[DataOffSet+3]=='t')&&(pRecBuffer[DataOffSet+4]=='M')\
			   &&(pRecBuffer[DataOffSet+5]=='o')&&(pRecBuffer[DataOffSet+6]=='d')&&(pRecBuffer[DataOffSet+7]=='e'))
			{ 
				TD.MainTask 	= 0;
				TD.GPS      	= 0;
				TD.GPRS     	= 0;
				TD.CAN		 	= 0;
				TD.SW       	= 0;
				TD.ADC      	= 0;
				TD.Display  	= 0;
				TD.Value			= 0;
				TD.TestMode 	= TEST_MODE;
			}
			else	
				Error = TRUE;
			//////////////////////////////////////////////
			Length += DataOffSet;	
			break;	

		/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
			读取GSM/GPRS相关信息
		*/
		//		$$$$$$$$$$$$$$$$$$$$  读取短消息服务中心号码
		case eGY_rCMD_GET_SMS_CENTER_NUMBER:
			NOP();	NOP();
			memmove(&pSendBuffer[DataOffSet],sTelNum.SmsCenter,16);
			//strcpy((char*)sTelNum.SmsCenter,(char*)&pRecBuffer[LEN_U0_GB_FRAME_HEAD]);
			//DFE.Read(EE_SMS_CENTER_NUMBER,16,&pSendBuffer[DataOffSet+16]);
			Length = DataOffSet+16;
			break;
		//		$$$$$$$$$$$$$$$$$$$$  读监控中心号码
		case eGY_rCMD_GET_CENTER_NUMBER:
			NOP();	NOP();
			DFE.Read(EE_CENTER_NUMBER,48,&pSendBuffer[DataOffSet]);
			
			Length = DataOffSet+64;
			break;
		//		$$$$$$$$$$$$$$$$$$$$  读取GPRS服务器IP和端口号
		case eGY_rCMD_GET_GPRS_SERVER_IP:						
			NOP();	NOP();
			SetBufferValue(pSendBuffer,160,0);
			strcpy((char*)&pSendBuffer[6],(char*)sIP.TCP);			
			DFE.Read(EE_IP_PORT,96,&pSendBuffer[6+32]);			
			Length = DataOffSet+32*4;						
			break;

		//		$$$$$$$$$$$$$$$$$$$$  读取型号和终端ID
		case eGY_rCMD_GET_OEM_ID:
		case eGY_rCMD_GET_SOLE_NUMBER:
			
			SetBufferValue(pSendBuffer,160,0);
			strcpy((char*)&pSendBuffer[6],(char*)sOEM_ID);			
			Length = DataOffSet+16;
			break;
		
		//		$$$$$$$$$$$$$$$$$$$$  读取生产商Web
		case eGY_rCMD_GET_OEM_WEB:
			NOP();	NOP();	NOP();
			if(  (pRecBuffer[LEN_U0_GB_FRAME_HEAD+0]=='G')&&(pRecBuffer[LEN_U0_GB_FRAME_HEAD+1]=='u')\
			   &&(pRecBuffer[LEN_U0_GB_FRAME_HEAD+2]=='o')&&(pRecBuffer[LEN_U0_GB_FRAME_HEAD+3]=='y')\
				&&(pRecBuffer[LEN_U0_GB_FRAME_HEAD+4]=='e')&&(pRecBuffer[LEN_U0_GB_FRAME_HEAD+5]=='G')\
				&&(pRecBuffer[LEN_U0_GB_FRAME_HEAD+6]=='P')&&(pRecBuffer[LEN_U0_GB_FRAME_HEAD+7]=='S'))		
			{
				//DFE.Read(EE_OEM_WEB,32,&pSendBuffer[DataOffSet]);			
				Length = DataOffSet+32;
			}
			else
				Error = TRUE;
			break;

		case eGY_rCMD_GET_GY_FLAG:			
			NOP();	NOP();			
			if(  (pRecBuffer[LEN_U0_GB_FRAME_HEAD+0]=='G')&&(pRecBuffer[LEN_U0_GB_FRAME_HEAD+1]=='Y')\
			   &&(pRecBuffer[LEN_U0_GB_FRAME_HEAD+2]=='E')&&(pRecBuffer[LEN_U0_GB_FRAME_HEAD+3]=='-')\
				&&(pRecBuffer[LEN_U0_GB_FRAME_HEAD+4]=='F')&&(pRecBuffer[LEN_U0_GB_FRAME_HEAD+5]=='l')\
				&&(pRecBuffer[LEN_U0_GB_FRAME_HEAD+6]=='a')&&(pRecBuffer[LEN_U0_GB_FRAME_HEAD+7]=='g'))	
			{
				//Sys.GetGYFlagToUart();
				return 0;
			}
			else
			{
				Error = TRUE;			
				Length += DataOffSet;
			}
			break;
		
		case eGY_rCMD_GET_GPS_PACK_TIME:			
			
			DFE.Read(EE_GPS_SEND_CONFIG,LEN_SEND_CFG,(uchar*)&pSendBuffer[DataOffSet]);		
			Length = DataOffSet+16;
			pSd = (pGPS_SEND)&pSendBuffer[Length];
			GPS.GetSendConfig(pSd);
			Length += 16;
			break;
							
		case eGY_rCMD_GET_PIN_CODE:			
			memmove(&pSendBuffer[DataOffSet],(uchar*)sPIN_Code,5);
			DFE.Read(EE_PIN_CODE,8,&pSendBuffer[DataOffSet+5]);
			Length = DataOffSet+13;
			break;
						
		case eGY_rCMD_GET_GPS_MILEAGE_ELE_RAIL:
			NOP();	NOP();
			
			break;
						
		case eGY_rCMD_GET_OUTPUT_STATUS:
			
			if(  (pRecBuffer[LEN_U0_GB_FRAME_HEAD+0]=='G')&&(pRecBuffer[LEN_U0_GB_FRAME_HEAD+1]=='Y')\
			   &&(pRecBuffer[LEN_U0_GB_FRAME_HEAD+2]=='S')&&(pRecBuffer[LEN_U0_GB_FRAME_HEAD+3]=='e')\
				&&(pRecBuffer[LEN_U0_GB_FRAME_HEAD+4]=='t')&&(pRecBuffer[LEN_U0_GB_FRAME_HEAD+5]=='O')\
				&&(pRecBuffer[LEN_U0_GB_FRAME_HEAD+6]=='u')&&(pRecBuffer[LEN_U0_GB_FRAME_HEAD+7]=='t'))
			{
				DFE.Read(EE_LOCK_FLAG,LEN_LOCK_CONTROL_OUT,&pSendBuffer[DataOffSet]);
				memmove(&pSendBuffer[DataOffSet+LEN_LOCK_CONTROL_OUT],(uchar*)&VehicleControl,LEN_LOCK_CONTROL_OUT);
				Length = DataOffSet+LEN_LOCK_CONTROL_OUT<<1;
			}
			else
			{
				Error = TRUE;
				Length = DataOffSet;	
			}
			break;		
						
					
		//		$$$$$$$$$$$$$$$$$$$$  设置短消息中心号码
		case eGY_rCMD_SET_SMS_CENTER_NUMBER:
			SetBufferValue(sTelNum.SmsCenter, 16, 0);
			strcpy((char*)sTelNum.SmsCenter,(char*)&pRecBuffer[DataOffSet]);
			DFE.Write(EE_SMS_CENTER_NUMBER,16,sTelNum.SmsCenter);			
			Length = DataOffSet;
			#if(DFE_DEBUG==TRUE)
			if(TD.MainTask==tMAIN_WRITE_DFE)	Uart.puts("DFE08\r\n\0");
			#endif
			break;
		//		$$$$$$$$$$$$$$$$$$$$  设置监控中心号码
		case eGY_rCMD_SET_CENTER_NUMBER:
			
			DFE.Write(EE_CENTER_NUMBER,32,&pRecBuffer[DataOffSet]);
			os_dly_wait(2);

			DFE.Write(EE_CENTER_NUMBER,16,&pRecBuffer[DataOffSet+32]);
			os_dly_wait(2);						
			Length = DataOffSet;
			#if(DFE_DEBUG==TRUE)
			if(TD.MainTask==tMAIN_WRITE_DFE)	Uart.puts("DFE07\r\n\0");
			#endif
			break;
		//		$$$$$$$$$$$$$$$$$$$$  设置GPRS服务器IP和端口号
		case eGY_rCMD_SET_GPRS_SERVER_IP:
						
			NOP();	NOP();	NOP();	NOP();
			if(  (pRecBuffer[LEN_U0_GB_FRAME_HEAD+0]=='G')&&(pRecBuffer[LEN_U0_GB_FRAME_HEAD+1]=='Y')\
			   &&(pRecBuffer[LEN_U0_GB_FRAME_HEAD+2]=='S')&&(pRecBuffer[LEN_U0_GB_FRAME_HEAD+3]=='e')\
				&&(pRecBuffer[LEN_U0_GB_FRAME_HEAD+4]=='t')&&(pRecBuffer[LEN_U0_GB_FRAME_HEAD+5]=='I')\
				&&(pRecBuffer[LEN_U0_GB_FRAME_HEAD+6]=='P')&&(pRecBuffer[LEN_U0_GB_FRAME_HEAD+7]=='P'))
			{ 
				Length = strlen((char*)&pRecBuffer[LEN_U0_GB_FRAME_HEAD+8]);
				SetBufferValue((uchar*)sIP.TCP,LEN_IP_PORT,0);
				strcpy((char*)sIP.TCP,(char*)&pRecBuffer[LEN_U0_GB_FRAME_HEAD+8]);
				
				RESET_WATCH_DOG;
				DFE.Write(EE_IP_PORT,32,(uchar*)sIP.TCP);
				os_dly_wait(2);
				//DFE.Write(EE_IP_PORT_BAK,32,(uchar*)sIP.TCP);
				os_dly_wait(2);				
				//memmove(&pSendBuffer[6],pRecBuffer,96);	
				//GPRS.ReDialFlag();
				#if(DFE_DEBUG==TRUE)		
				if(TD.MainTask==tMAIN_WRITE_DFE)	Uart.puts("DFE06\r\n\0");
				#endif			
			}			
			else	
				Error = TRUE;
			Length = DataOffSet;
			NOP();	NOP();
			break;
			

		//		$$$$$$$$$$$$$$$$$$$$  发送到串口1
		case eGY_rCMD_SET_SEND_TO_UART:
			NOP();	NOP();
			putb(1,Rec.Counter,(uchar*)&pRecBuffer[LEN_U0_GB_FRAME_HEAD]);
			Uart1.putb(1,Rec.Counter,(uchar*)&pRecBuffer[LEN_U0_GB_FRAME_HEAD]);
			Uart2.putb(1,Rec.Counter,(uchar*)&pRecBuffer[LEN_U0_GB_FRAME_HEAD]);
			Uart2.SendTask();
			Uart3.putb(Rec.Counter,(uchar*)&pRecBuffer[LEN_U0_GB_FRAME_HEAD]);
			
		 	break;
		 	
		 ////////////////////////////////// 	
		case eGY_rCMD_SET_OEM_WEB:
			NOP();	NOP();
			
			if(  (pRecBuffer[LEN_U0_GB_FRAME_HEAD+0]=='P')&&(pRecBuffer[LEN_U0_GB_FRAME_HEAD+1]=='o')\
			   &&(pRecBuffer[LEN_U0_GB_FRAME_HEAD+2]=='w')&&(pRecBuffer[LEN_U0_GB_FRAME_HEAD+3]=='e')\
				&&(pRecBuffer[LEN_U0_GB_FRAME_HEAD+4]=='r')&&(pRecBuffer[LEN_U0_GB_FRAME_HEAD+5]=='G')\
				&&(pRecBuffer[LEN_U0_GB_FRAME_HEAD+6]=='P')&&(pRecBuffer[LEN_U0_GB_FRAME_HEAD+7]=='S'))		
			{
				NOP();	NOP();				
				//DFE.Write(EE_OEM_WEB,32,&pRecBuffer[LEN_U0_GB_FRAME_HEAD+8]);
			}
			else
				Error = TRUE;
			Length = DataOffSet;
			break;			
		////////////////////////////////// 
		case eGY_rCMD_SET_U0_U1_CLARITY:
			Length = DataOffSet;			
		   break;   
			 
		//////////////////////////////////////////		
		case eGY_rCMD_SET_GPS_PACK_TIME:
			
			pSd = (pGPS_SEND)&pRecBuffer[LEN_U0_GB_FRAME_HEAD];
			GPS.SetSendConfig(pSd,1);
			Length = DataOffSet;
			break;
									
		case eGY_rCMD_SET_CLR_GPS_MILEAGE:
			
			if(  (pRecBuffer[LEN_U0_GB_FRAME_HEAD+0]=='G')&&(pRecBuffer[LEN_U0_GB_FRAME_HEAD+1]=='Y')\
			   &&(pRecBuffer[LEN_U0_GB_FRAME_HEAD+2]=='C')&&(pRecBuffer[LEN_U0_GB_FRAME_HEAD+3]=='l')\
				&&(pRecBuffer[LEN_U0_GB_FRAME_HEAD+4]=='r')&&(pRecBuffer[LEN_U0_GB_FRAME_HEAD+5]=='G')\
				&&(pRecBuffer[LEN_U0_GB_FRAME_HEAD+6]=='P')&&(pRecBuffer[LEN_U0_GB_FRAME_HEAD+7]=='S'))
			{
				SystemLog.GPS.SpeedCaculus = 0;
				SystemLog.GPS.TotalMileage = 0;
				SystemLog.VDR.MotoRunTimeCnt = 0;				
				DFE.Write(EE_SYS_GPS_SPEED_CALCULUS,12,(uchar *)&SystemLog.GPS.SpeedCaculus);		// 写入
				os_dly_wait(1);
				DFE.Write(EE_SYS_VDR_MOTOR_RUN_TIME,4,(uchar *)&SystemLog.VDR.MotoRunTimeCnt);		// 写入
				//DFE.Write(EE_SPEED_CALCULUS,16,(uchar *)&SystemLog.GPS.SpeedCaculus);		// 写入
				#if(DFE_DEBUG==TRUE)
				if(TD.MainTask==tMAIN_WRITE_DFE)	Uart.puts("DFE05\r\n\0");
				#endif
			}
			else
				Error = ERROR;
				
			Length = DataOffSet;			
			break;
			
									
		case eGY_rCMD_SET_PIN_CODE:
			
			if(  (pRecBuffer[LEN_U0_GB_FRAME_HEAD+0]=='G')&&(pRecBuffer[LEN_U0_GB_FRAME_HEAD+1]=='Y')\
			   &&(pRecBuffer[LEN_U0_GB_FRAME_HEAD+2]=='S')&&(pRecBuffer[LEN_U0_GB_FRAME_HEAD+3]=='e')\
				&&(pRecBuffer[LEN_U0_GB_FRAME_HEAD+4]=='t')&&(pRecBuffer[LEN_U0_GB_FRAME_HEAD+5]=='P')\
				&&(pRecBuffer[LEN_U0_GB_FRAME_HEAD+6]=='I')&&(pRecBuffer[LEN_U0_GB_FRAME_HEAD+7]=='N'))
			{
				NOP();	NOP();				
				memmove((uchar*)sPIN_Code,&pRecBuffer[LEN_U0_GB_FRAME_HEAD+8],4);
				memmove(&pRecBuffer[LEN_U0_GB_FRAME_HEAD+12],&pRecBuffer[LEN_U0_GB_FRAME_HEAD+8],4);
				DFE.Write(EE_PIN_CODE,8,&pRecBuffer[LEN_U0_GB_FRAME_HEAD+8]);
				#if(DFE_DEBUG==TRUE)
				if(TD.MainTask==tMAIN_WRITE_DFE)	Uart.puts("DFE04\r\n\0");
				#endif
			}
			else
				Error = TRUE;
			Length = DataOffSet;
			break;

		case eGY_rCMD_SET_MEMORY_FORMAT:
			
			if(  (pRecBuffer[LEN_U0_GB_FRAME_HEAD+0]=='G')&&(pRecBuffer[LEN_U0_GB_FRAME_HEAD+1]=='Y')\
			   &&(pRecBuffer[LEN_U0_GB_FRAME_HEAD+2]=='F')&&(pRecBuffer[LEN_U0_GB_FRAME_HEAD+3]=='o')\
				&&(pRecBuffer[LEN_U0_GB_FRAME_HEAD+4]=='r')&&(pRecBuffer[LEN_U0_GB_FRAME_HEAD+5]=='m')\
				&&(pRecBuffer[LEN_U0_GB_FRAME_HEAD+6]=='a')&&(pRecBuffer[LEN_U0_GB_FRAME_HEAD+7]=='t'))
			{				// GYFormat
				
				SetBufferValue(pSendBuffer,256,0);
				
				/////////////////////////////////////////////////////////////////////
				for(i=0;i<EE_TOTAL_PAGE;i++)			// 这里的Lengyh 代表 Address
				{					
					RESET_WATCH_DOG;  
					DFE.Write(EE_ELE_SET_PARAMETER,EE_PAGE_LEN,pSendBuffer);
					os_dly_wait(10);
					#if(DFE_DEBUG==TRUE)
					if(TD.MainTask==tMAIN_WRITE_DFE)	Uart.puts("DFE03\r\n\0");
					#endif
				}								
				
				Length = DataOffSet;
			}
			else
				Error = TRUE;
			
			
			break;

		case eGY_rCMD_SET_OUTPUT_STATUS:
			
			if(  (pRecBuffer[LEN_U0_GB_FRAME_HEAD+0]=='G')&&(pRecBuffer[LEN_U0_GB_FRAME_HEAD+1]=='Y')\
			   &&(pRecBuffer[LEN_U0_GB_FRAME_HEAD+2]=='S')&&(pRecBuffer[LEN_U0_GB_FRAME_HEAD+3]=='e')\
				&&(pRecBuffer[LEN_U0_GB_FRAME_HEAD+4]=='t')&&(pRecBuffer[LEN_U0_GB_FRAME_HEAD+5]=='O')\
				&&(pRecBuffer[LEN_U0_GB_FRAME_HEAD+6]=='u')&&(pRecBuffer[LEN_U0_GB_FRAME_HEAD+7]=='t'))
			{				
				memmove((uchar*)&VehicleControl,(uchar*)&pRecBuffer[LEN_U0_GB_FRAME_HEAD+8],LEN_LOCK_CONTROL_OUT);
				DFE.Write(EE_LOCK_FLAG,LEN_LOCK_CONTROL_OUT,(uchar *)&VehicleControl);				
				Length = DataOffSet;
				#if(DFE_DEBUG==TRUE)
				if(TD.MainTask==tMAIN_WRITE_DFE)	Uart.puts("DFE02\r\n\0");
				#endif
			}
			else
			{
				Error = TRUE;
				Length = DataOffSet;	
			}
			break;
		 case eGY_rCMD_SET_IDLE_PARA:
		 	if(  (pRecBuffer[LEN_U0_GB_FRAME_HEAD+0]=='G')&&(pRecBuffer[LEN_U0_GB_FRAME_HEAD+1]=='Y')\
			   &&(pRecBuffer[LEN_U0_GB_FRAME_HEAD+2]=='E')&&(pRecBuffer[LEN_U0_GB_FRAME_HEAD+3]=='-')\
				&&(pRecBuffer[LEN_U0_GB_FRAME_HEAD+4]=='I')&&(pRecBuffer[LEN_U0_GB_FRAME_HEAD+5]=='d')\
				&&(pRecBuffer[LEN_U0_GB_FRAME_HEAD+6]=='l')&&(pRecBuffer[LEN_U0_GB_FRAME_HEAD+7]=='e'))
			{
				Idle.EnableInterIdleMode = pRecBuffer[LEN_U0_GB_FRAME_HEAD+8];	// 允许进入省电模式
				Idle.IdleModeFlag = 0;
				Idle.InterIdleModeTime = pRecBuffer[LEN_U0_GB_FRAME_HEAD+9];		// 多少时间后进入省电模式
				Idle.WakeUpTime = pRecBuffer[LEN_U0_GB_FRAME_HEAD+10];				// 多少时间后唤醒
				Idle.WakeUpLongTime = pRecBuffer[LEN_U0_GB_FRAME_HEAD+11];		// 唤醒持续时间
				Idle.SecondCnt=0;			
				DFE.Write(EE_IDLE_STR,LEN_EE_IDLE,(uchar*)&Idle);
				#if(DFE_DEBUG==TRUE)
				if(TD.MainTask==tMAIN_WRITE_DFE)	Uart.puts("DFE01\r\n\0");
				#endif
			}
			else
			{
				Error = TRUE;
				Length = DataOffSet;	
			}
		 	break;
		 
		 case eGY_rCMD_GET_SYSTEM_LOG:
		 	memmove(&pSendBuffer[DataOffSet],(uchar*)&SystemLog.ResetCnt.SystemRestartCnt,LEN_SYSTEM_LOG);
			Length = DataOffSet+LEN_SYSTEM_LOG;
		 	break;
		 

		///////////////////////////////////////////////////////////////
		case  eGY_rCMD_SYSTEM_RESTART:
			Switch.SaveVehicleStatus();
			os_dly_wait(10);
			Uart.puts("串口操作，直接重启系统指令\r\n\0");
			Uart.PopAlignmentSend();
			os_dly_wait(10);	

			GPRS.RestoreFactorySetting(0);
			DisableIRQ();
			while(1);
			//break;
	

		
		////////////////////////////////////////////// 	串口发送短信
		case  eGY_rCMD_SEND_SMS:							  
		   
		   if(pRecBuffer[DataOffSet+0]=='P')		 //发送PDU短信
			{
				t = pRecBuffer[DataOffSet+1];	
				Length = pRecBuffer[DataOffSet+2];	
				memmove(pSendBuffer,&pRecBuffer[DataOffSet+3], Length);
				i = pRecBuffer[DataOffSet+3+Length];
				SMS.SetPduDataMessage(t, (char *)pSendBuffer, &pRecBuffer[DataOffSet+4+Length], i);
			}
			else if(pRecBuffer[DataOffSet+0]=='T')	  //发送Text短信
			{
				 t = pRecBuffer[DataOffSet+1];	
				 Length = pRecBuffer[DataOffSet+2];	
				 memmove(pSendBuffer,&pRecBuffer[DataOffSet+2], Length);
				 i = pRecBuffer[DataOffSet+2+Length];
				 pRecBuffer[DataOffSet+2+Length+1+i] = 0;
				 GSM.SetSMS_Buffer((char*)pSendBuffer, (char*)&pRecBuffer[DataOffSet+4+Length]);
			}
			else
			{
			  Error = TRUE;
			  Length = DataOffSet;	
			}
			break;
			
			// GY 44 01 00000 BAJ-A1 \x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00 C088888 1 粤C88888 U A 1234567890A \x00 \x00
			// GY 44 01 00000 BAJ-A1 \x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00 C088888 1 粤C88888 U A 13886088888 \x00 \x00
			// GY 44 01 00000 GY-G11A \x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00 2280583 1 粤A80583 U A 1234567890A \x00 \x00
			case eGY_rCMD_SET_REG_CODE:
				
				// 省域
				Len = 0;
				tmp = pRecBuffer[DataOffSet] | (pRecBuffer[DataOffSet+1]<< 8);
				DFE.Write(EE_PROVINCE_REGION_ID, 2, (uchar *)&tmp);
				NOP(); NOP(); NOP();
				Len += 2;
			
				// 市县域
				tmp = pRecBuffer[DataOffSet+Len] | (pRecBuffer[DataOffSet+Len+1]);
				DFE.Write(EE_PREFECTURE_REGION_ID, 2, (uchar *)&tmp);
				NOP(); NOP(); NOP();
				Len += 2;
			
				// 制造商ID（5B)，型号（20B)，终端ID（7B）
				DFE.Write(EE_VENDOR_ID, 32, &pRecBuffer[DataOffSet+Len]);
				NOP(); NOP(); NOP();	 
				Len += 32; 
				
				// 车的颜色
				DFE.Write(FM_CAR_COLOUR, 1, &pRecBuffer[DataOffSet+Len]);	
				NOP(); NOP(); NOP();	      
				Len++;
			
				// 车牌号 //这里不需要点号
				DFE.Write(FM_VEHICLE_CODE, 9, &pRecBuffer[DataOffSet+Len]);
				NOP(); NOP(); NOP();	   
				Len += 9;


				if( pRecBuffer[DataOffSet+Len]=='U')
				{
					CreateUserFlag = OK;		  // 开户
					DFE.Write(EE_CREATE_USER_FLAG, 1, &CreateUserFlag);
					NOP(); NOP(); NOP();	
				}
				Len++;

				if( pRecBuffer[DataOffSet+Len]=='A')
				{
					// 获取鉴权码，输入时必须以0结束
					Len++;
					strcpy((char*)&jt_auth_code[0],(char*)&pRecBuffer[DataOffSet+Len]);
					DFE.Write(EE_AUTH_CODE, strlen((char*)jt_auth_code),(uchar *)&jt_auth_code[0]);			//  写鉴权码到铁电
					
					
					NOP(); NOP(); NOP();	
				}
				

				NOP(); NOP(); NOP();	

				Length = DataOffSet;	


			break;

		
			case	eGY_rCMD_SET_TER_PHONE_NUM:			 //设置终端手机号 		///// GY S 13800000000	\x00
				Len = DataOffSet;
				if (pRecBuffer[Len] == 'S')
				{
					Len++;
					SetBufferValue(sTelNum.Sim, 16, 0);
					sTelNum.Sim[0] = 0x30;
				    memmove((uchar*)&sTelNum.Sim[1],(uchar*)&pRecBuffer[Len],11);
					Len += 11;
					Length = strlen((char *)&sTelNum.Sim[0]);
					DFE.Write(EE_SIM_NUMBER,16,(uchar*)&sTelNum.Sim[0]);
					Uart.puts("ter phone:"); Uart.puts((char *)&sTelNum.Sim[1]);
					NOP(); NOP(); NOP();
					DecCharToPressBCD(Length,(char *)sTelNum.Sim,SimNumberBCD);
					memmove(sTelNum.SimBCD_JT, SimNumberBCD, 6);
					Length = DataOffSet;	

				}
				else if (pRecBuffer[Len] == 'G')	  //// GY G 
				{
					 Length = strlen((char *)&sTelNum.Sim[1]);
			         memmove((uchar*)&pSendBuffer[DataOffSet],&sTelNum.Sim[1], Length);
					 Length += DataOffSet;	
				}
					
			break;
			
			
			case	eGY_rCMD_SET_BLIND_TEST:
				
				Len = DataOffSet;
				if (pRecBuffer[Len] == 'S')
				{
					SamlePeriod1S = TRUE;
					if (pRecBuffer[Len+1] == 'B')
						TestBlind = TRUE;
					else if (pRecBuffer[Len+1] == 'H')
						TestAccuracy = TRUE;
				}
				else if (pRecBuffer[Len] == 'C')
				{
					SamlePeriod1S = 0;
					TestBlind = 0;
					TestAccuracy = 0;
				}
				
				pSendBuffer[Len] 	= SamlePeriod1S;
				pSendBuffer[Len+1]	= TestBlind;
				pSendBuffer[Len+2]	= TestAccuracy;
				Len += 3;
				
				Length = DataOffSet+3;
			break;



			case  eGY_rCMD_SET_FACTORY_RESET:

			   if(pRecBuffer[DataOffSet]=='A')
				  Sys.FirstPowerOn(TRUE);  				//恢复出厂设置	  重度 ，清所有数据包括IP地址和记录仪数据
				else
				  Sys.FirstPowerOn(FALSE); 
				NVIC_SystemReset();     
			 break;
		default : 
			Error = TRUE;
			break;
	}
	////////////////////////////////////////////////////////////////////////////////////

	if(Error)
	{
		pGy = (pGY_COMM_PROTOCOL)pSendBuffer;
		pGy->Head = 0x3A59472A;		
		pGy->Cmd = 0x0001;				// 通用应答
		pGy->Index = U0_GyIndex;
		
		pGy->Data.B32[0] = RecIndex;
		pGy->Data.B16[2] = Cmd;
		pGy->Data.Buffer[6] = 2;
		
		pGy->Attr.Len = 7;
		DataLength = 15;
		
		if(CheckType==0)
		{
			pGy->Attr.Bits.Check = 0;
			for(Sum=0,i=GY_PROTOCOL_CHECK_START_ADDR;i<(DataLength+GY_PROTOCOL_CHECK_START_ADDR);i++)
			{
				Sum += pSendBuffer[i];
			}
			pGy->Check = Sum;
		}
		else if(CheckType==1)
		{
			pGy->Attr.Bits.Check = 1;
			for(Sum=0,i=GY_PROTOCOL_CHECK_START_ADDR;i<(DataLength+GY_PROTOCOL_CHECK_START_ADDR);i++)
			{
				Sum ^= pSendBuffer[i];
			}
			pGy->Check = Sum;
			
		}
		else if(CheckType==2)
		{
			pGy->Attr.Bits.Check = 2;
			pGy->Check = CalCRC16( (pSendBuffer+GY_PROTOCOL_CHECK_START_ADDR),DataLength);
		}
		else
		{
			
		}
		Length = DataOffSet+7;
	}
	else
	{

		pGy = (pGY_COMM_PROTOCOL)pSendBuffer;
		pGy->Head = 0x3A59472A;
		
		pGy->Cmd = Cmd;
		pGy->Index = U0_GyIndex;

		DataLength = Length-DataOffSet;
		pGy->Attr.Len = DataLength;
		
		DataLength += 8;		// 校验长度
		
		
		if(CheckType==0)
		{
			pGy->Attr.Bits.Check = 0;
			for(Sum=0,i=GY_PROTOCOL_CHECK_START_ADDR;i<(DataLength+GY_PROTOCOL_CHECK_START_ADDR);i++)
			{
				Sum += pSendBuffer[i];
			}
			pGy->Check = Sum;
		}
		else if(CheckType==1)
		{
			pGy->Attr.Bits.Check = 1;
			for(Sum=0,i=GY_PROTOCOL_CHECK_START_ADDR;i<(DataLength+GY_PROTOCOL_CHECK_START_ADDR);i++)
			{
				Sum ^= pSendBuffer[i];
			}
			pGy->Check = Sum;
			
		}
		else if(CheckType==2)
		{
			pGy->Attr.Bits.Check = 2;
			pGy->Check = CalCRC16( (pSendBuffer+GY_PROTOCOL_CHECK_START_ADDR),DataLength);
		}
		else
		{
			
		}
				
	}	
	U0_GyIndex++;
	NOP();	NOP();

	
	return Length;
	
}
#endif








/*
// 国业统一通讯协议帧格式///////////////////////// 协议帧格式
#define		GY_PROTOCOL_CHECK_START_ADDR			6
#define		DATA_OFFSET_M_PACK						18
#define		DATA_OFFSET_1_PACK						14
typedef		struct	_GY_COMM_PROTOCOL_
{
	uint32	Head;					// 4B，*GY:	// 0x3A59472A
	uint16	Check;				// 2B，校验
	uint16	Cmd;					// 2B，命令字
	uint32	Index;				// 4B，流水号
	union
	{
		uint16	Len;
		struct
		{
			uchar		Len:8;			// 11Bit，数据段长度
			uchar		LenH:3;			// 
			uchar		Encry:2;			// 2Bit
			uchar		Pack:1;			// 1Bit
			uchar		Check:2;			// 2Bit
		}Bits;
	}Attr;
	union
	{
		uchar		Buffer[128];		// 实际长度最大2048字节，请使用结构指针指向大缓冲
		uint16	B16[64];				// 
		uint32	B32[32];				// 
		struct
		{
			uint16	Total;
			uint16	Index;
		}Pack;
	}Data;
}GY_COMM_PROTOCOL,*pGY_COMM_PROTOCOL;		*/






uint16		Uart2Cls::ReceiveAudioData(uint16 RecLength,uchar *pRecBuffer,uchar *pTxdBuffer)
{
	uint16	Len=0;
	
	
	
	
	
	
	return 	Len;
}



uint16		Uart2Cls::SendDataToAudioDevice(uint16 Cmd,uint16 Length,uchar *pData)
{
	
	return 0;
	
}















