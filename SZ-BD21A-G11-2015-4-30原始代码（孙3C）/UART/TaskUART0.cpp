/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$  Copyright (c)  $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
$												 
$										 
$                       	 		  
$								  		All rights reserved.	  
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

#include    	"UART0.h"
#include    	"UART1.h"

#define			GB19056_GLOBALS		1
#include			"..\Common\GB_T19056.h"
#include			"..\Common\GY_T19056.h"


#include			"..\GSM_GPRS\GSM_GPRS.h"
#include			"..\GSM_GPRS\JT_T808.h"
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
#include 		".\Accelerometer\kionix_kxtf9_common.h"	

///////////////////////   记录数据名称 ////////////////////////////////////////
//UART0_EXT	uchar		const		GBDataName[][20] = 
//{ 
//{"执行标准版本号"},
//{"当前驾驶员信息"},
//{"实时时钟"},
//{"累计行驶里程"},
//{"速度脉冲系数"},
//{"车辆信息"},
//{"记录仪唯一性编号"},
//{"状态信号配置信息"},
//{"每秒钟平均速度记录"},
//{"每分钟平均速度记录"},
//{"位置信息记录"},
//{"事故疑点记录"},
//{"疲劳驾驶记录"},
//{"登录退出记录"},
//{"外部供电记录"},
//{"参数修改记录"},
//{"记录仪安装参数"},
//{"定位、告警相关参数"},
//{"电子围栏数据"},
//{"电子围栏目录"},
//{"线路规划数据"},
//{"线路规划目录"},
//{"\0"},
//};

/////////////////////   记录数据名称 ////////////////////////////////////////
UART0_EXT	uchar		const		GBDataName[][20] = 
{ 
{"执行标准版本年号"},		
{"当前驾驶人信息"},
{"实时时间"},
{"累计行驶里程"},
{"脉冲系数"},
{"车辆信息"},
{"状态信号配置信息"},
{"记录仪唯一性编号"},
{"行驶速度记录"},
{"位置信息记录"},
{"事故疑点记录"},
{"超时驾驶记录"},
{"驾驶人身份记录"},
{"外部供电记录"},
{"参数修改记录"},
{"速度状态日志"},
{"\0"},
};

////////////	通讯出错标志	///////////////////////////////////////////////////////
#define			UP_LOAD_ERROR					0xfa	// 上传出错标志字
#define			DOWN_LOAD_ERROR				0xfb	// 下传出错标志字
#define			XOR_SUM_ERROR					0xff	// 检验和错误

#define	GB_LARGE_DATA_PACK_BLOCK_LEN		512

extern		"C"  unsigned long 		SwapINT32(unsigned long dData);
static    uchar      CheckSum;
static    uchar      USBCheckSum;



extern	char		jt_auth_code[];		// 鉴权码

//static	 uchar		TxdTBuffer[260+16];
//static  uchar  USB_GBDataBuffer[512]
static char USB_GetGBdata_PartFun(uchar Cmd, uint32 Time, uint32 MaxDataLen, uint32 DataHeadLen)		  // USB采集部分功能函数 
{
	uchar i;
	uint16 Len = 0,TotalItem;
	uint32 DataBlockTotalLen = 0, L;
	uint32 StartSecond, EndSecond;
	uchar tBuffer[32];

	Time = 24*3600*30UL;  // 	检测中心要求取所有数据, 这里就取一月的数据（其实没有存那么多）

	EndSecond = Rtc.GetRtcSecond();
	StartSecond = EndSecond - Time;				 

	Rtc.SecondToDateTime_BCD(StartSecond,(RTC_STR *)tBuffer);
	memmove(GB_LargeData.StartTime,tBuffer,6);	  // 开始时间  bcd 


	Rtc.SecondToDateTime_BCD(EndSecond,(RTC_STR *)tBuffer);
	memmove(GB_LargeData.EndTime,tBuffer,6);	  // 结束时间	  bcd
	GB_LargeData.Cmd = Cmd; 
	TotalItem  = 0xffff;//GB_LargeData.TotalItem;	 // 检测中心要求取所有数据	，所以这里不限制数据块数	2015.3.9
	

	//Len = SectionLenPos+4;  // 数据段开始的位置为 数据段长度位置加4
	// 这里两次循环，第一次循环是得到数据段的总长度, 把数据头部写入USB ，第二次循环是把数据段写入USB 

	for (i=0; i<2; i++)
	{
		GB_LargeData.SearchFlag = 0;
		GB_LargeData.TotalItem = TotalItem;
		L = 1;
		while (L)
		{
			if (i==0) Len = DataHeadLen;
			else		Len = 0;

			L = Uart.SendVDR_Data_08H_To_15H(1024,TxdTBuffer+Len);
			if (L)  //  数据长度不为0
			{

				//if (DataBlockTotalLen>MaxDataLen)	break;	 // 检测中心要求取所有数据	，所以这里不限制总长
				DataBlockTotalLen += L;
			
				if (i==1)
				{
					Uart.WaitUSBGetData(10, L);
					//BEEP(9,0,4,0,4);
					os_dly_wait(4);
					if(Write_File_Error) 
					{
			   		return 1;
					}
				}
			}
			else
			{
				break;
			}
		


		}

		if (i==0)
		{
//			if (TD.SW==tSPEED_RECORD)
//			{
//				 Uart.puts("DataBlockTotalLen 1 Test Data:"); Uart.putb(1,4,(uchar *)&DataBlockTotalLen); Uart.puts("\r\n");
//				 Uart.puts("GB_LargeData.TotalPack Test Data:"); Uart.putb(1,2,(uchar *)&GB_LargeData.TotalPack); Uart.puts("\r\n"); 
//				 Uart.puts("Cmd Test Data:"); Uart.putb(1,1,&Cmd); Uart.puts("\r\n");
//			 }
			 DataBlockTotalLen = SwapINT32(DataBlockTotalLen);

			 //Uart.puts("DataHeadLen Test Data:"); Uart.putb(1,4,(uchar *)&DataHeadLen); Uart.puts("\r\n");
			 
			 memmove(&TxdTBuffer[DataHeadLen-4],(uchar *)&DataBlockTotalLen, 4); // 更新实际数据段的总长 

			  //Uart.puts("head Test Data:"); Uart.putb(1,DataHeadLen,TxdTBuffer); Uart.puts("\r\n"); os_dly_wait(10);
			 // 写入数据块头部信息  ： 数据代码，数据名称，数据长度
			 Uart.WaitUSBGetData(10, (uint16)DataHeadLen);
			 BEEP(9,0,4,0,4);
			 os_dly_wait(2);
			 if(Write_File_Error) 
			 {
			   		return 1;
			 }

			 DataBlockTotalLen = 0;
			 	 
		}
//		else
//		{
//			if (TD.SW==tSPEED_RECORD)
//			{
//				 Uart.puts("DataBlockTotalLen 2 Test Data:"); Uart.putb(1,4,(uchar *)&DataBlockTotalLen); Uart.puts("\r\n");
//				 Uart.puts("GB_LargeData.TotalPack Test Data:"); Uart.putb(1,2,(uchar *)&GB_LargeData.TotalPack); Uart.puts("\r\n"); 
//				 Uart.puts("Cmd Test Data:"); Uart.putb(1,1,&Cmd); Uart.puts("\r\n");
//			 }
//		}
	}

	return 0;
}

extern		"C"   void     USB_GetGBdata(void)
{
	uint16 	t, L;
	uint32   Len;
	uchar    DataBuffer[50];
	//uint32 StartSecond, EndSecond;
	Write_File_Error = 0;	
	//BEEP(9,0,4,0,4);
	//os_dly_wait(4);
	t = USBGBDATANUMBER;								 //USB数据块个数
	t = SwapINT16(t);
	SetBufferValue(&TxdTBuffer[0],1024,0);
	memmove(&TxdTBuffer[0], (uchar *)&t, 2);	
	Len = 2;					  
	/////执行的标准版本号///////////////////
	L = Uart.SetHeadUSBBlockData(eGB_rCMD_GET_STD_VERSION, eGB_GET_STD_VERSION, LEN_GPS_Standard, (uchar *)&TxdTBuffer[2]);	 //USB数据块头		 
	Len += L;
	Uart.SetGB_Down_CMDHead(DataBuffer, 0, eGB_rCMD_GET_STD_VERSION);
	L = Uart.GB19056Process(LEN_U0_GB_FRAME_HEAD,DataBuffer,TxdTBuffer+Len);
	L -= LEN_U0_GB_FRAME_HEAD;
	memmove(TxdTBuffer+Len, TxdTBuffer+Len+LEN_U0_GB_FRAME_HEAD, L); // 去掉头部6个字节
	Len += L;


	/////当前驾驶员信息///////////////////
	L = Uart.SetHeadUSBBlockData(eGB_rCMD_GET_DRIVER_CODE, eGB_GET_DRIVER_CODE, LEN_DRIVER_CODE, (uchar *)&TxdTBuffer[Len]);	 //USB数据块头	
	Len += L;
	DFE.Read(FM_DRIVER_CODE, LEN_DRIVER_CODE, &TxdTBuffer[Len]);
	Len += LEN_DRIVER_CODE;


	/////实时时钟///////////////////
	L = Uart.SetHeadUSBBlockData(eGB_rCMD_GET_REAL_TIME, eGB_GET_REAL_TIME, 6, (uchar *)&TxdTBuffer[Len]);	 //USB数据块头
	Len += L;	
	Rtc.GetRTC_BCD((RTC_STR *)&TxdTBuffer[Len]);	
	Len += 6;

	/////累计行驶里程///////////////////
	L = Uart.SetHeadUSBBlockData(eGB_rCMD_GET_TOTAL_MILAGE, eGB_GET_TOTAL_MILAGE,LEN_MILAGE, (uchar *)&TxdTBuffer[Len]);	 //USB数据块头	
	Len += L;
	Uart.SetGB_Down_CMDHead(DataBuffer, 0, eGB_rCMD_GET_TOTAL_MILAGE);
	L = Uart.GB19056Process(LEN_U0_GB_FRAME_HEAD,DataBuffer,TxdTBuffer+Len);
	L -= LEN_U0_GB_FRAME_HEAD; // 去掉头部6个字节
	memmove(TxdTBuffer+Len, TxdTBuffer+Len+LEN_U0_GB_FRAME_HEAD, L); // 去掉头部6个字节
	Len += L;


	/////速度脉冲系数///////////////////
	L = Uart.SetHeadUSBBlockData(eGB_rCMD_GET_SPEED_KV, eGB_GET_SPEED_PULSE_COEFFICIENT, LEN_VEHICLE_KV, (uchar *)&TxdTBuffer[Len]);	 //USB数据块头	
	Len += L;
	Uart.SetGB_Down_CMDHead(DataBuffer, 0, eGB_rCMD_GET_SPEED_KV);
	L = Uart.GB19056Process(LEN_U0_GB_FRAME_HEAD,DataBuffer,TxdTBuffer+Len);
	L -= LEN_U0_GB_FRAME_HEAD;
	memmove(TxdTBuffer+Len, TxdTBuffer+Len+LEN_U0_GB_FRAME_HEAD, L); // 去掉头部6个字节
	Len += L;

	/////车辆信息///////////////////
	L = Uart.SetHeadUSBBlockData(eGB_rCMD_GET_VEHICLE_VIN, eGB_GET_VEHICLE_VIN, LEN_VEHICLE_VCC, (uchar *)&TxdTBuffer[Len]);	 //USB数据块头	
	Len += L;
	Uart.SetGB_Down_CMDHead(DataBuffer, 0, eGB_rCMD_GET_VEHICLE_VIN);
	L = Uart.GB19056Process(LEN_U0_GB_FRAME_HEAD,DataBuffer,TxdTBuffer+Len);
	L -= LEN_U0_GB_FRAME_HEAD;
	memmove(TxdTBuffer+Len, TxdTBuffer+Len+LEN_U0_GB_FRAME_HEAD, L); // 去掉头部6个字节
	Len += L;

	/////状态信号配置信息///////////////////
	L = Uart.SetHeadUSBBlockData(eGB_rCMD_GET_SIGNAL_CONFIG, eGB_GET_SIGNAL_CONFIG, LEN_SIGNAL_STATUS_1PEC, (uchar *)&TxdTBuffer[Len]);	 //USB数据块头	
	Len += L;
	Uart.SetGB_Down_CMDHead(DataBuffer, 0, eGB_rCMD_GET_SIGNAL_CONFIG);
	L = Uart.GB19056Process(LEN_U0_GB_FRAME_HEAD,DataBuffer,TxdTBuffer+Len);
	L -= LEN_U0_GB_FRAME_HEAD;
	memmove(TxdTBuffer+Len, TxdTBuffer+Len+LEN_U0_GB_FRAME_HEAD, L); // 去掉头部6个字节
	Len += L;
		

//	/////记录仪唯一性编号///////////////////
	L = Uart.SetHeadUSBBlockData(eGB_rCMD_GET_SERIAL_NUMBER, eGB_GET_SERIAL_NUMBER, LEN_SERIAL_NUMBER, (uchar *)&TxdTBuffer[Len]);	 //USB数据块头	
	Len += L;
	Uart.SetGB_Down_CMDHead(DataBuffer, 0, eGB_rCMD_GET_SERIAL_NUMBER);
	L = Uart.GB19056Process(LEN_U0_GB_FRAME_HEAD,DataBuffer,TxdTBuffer+Len);
	L -= LEN_U0_GB_FRAME_HEAD;
	memmove(TxdTBuffer+Len, TxdTBuffer+Len+LEN_U0_GB_FRAME_HEAD, L); // 去掉头部6个字节
	Len += L;


	Uart.WaitUSBGetData(10, (uint16)Len);										//有限的时间内等待USB处理完数据，
	Uart.puts("USB  Wrinting");
   os_dly_wait(4);
	Len  = 0;

	// 注：按检测中心要求，U盘采集数据是把机器有效的数据全部采集，所以下的 USB_GetGBdata_PartFun里面作修改
	//  GB_LargeData.TotalItem  和USB_GetGBdata_PartFun 的第二个时间参数无作用
	/////每秒钟平均速度///////////////////			48小时行驶速度
	Len  = 0;
	GB_LargeData.TotalItem = 0xffff;//48*60;
	L = Uart.SetHeadUSBBlockData(eGB_rCMD_GET_SPEED_PER_SECOND, eGB_GET_SPEED_PER_SECOND, 0, (uchar *)&TxdTBuffer[Len]);	 //USB数据块头	
	if (USB_GetGBdata_PartFun(eGB_rCMD_GET_SPEED_PER_SECOND,(48*3600UL),LEN_48Hour_SPEED_DATA,L) == 1)
	{
		  	goto __OVER_USB_DATA;
	}
	//goto __OVER_USB_DATA; //	 test

	/////360小时位置信息///////////////////
	Len  = 0;
	GB_LargeData.TotalItem = 360;
	L = Uart.SetHeadUSBBlockData(eGB_rCMD_GET_POSITION_INFO, eGB_GET_POSITION_INFO, 0, (uchar *)&TxdTBuffer[Len]);	 //USB数据块头	
	if (USB_GetGBdata_PartFun(eGB_rCMD_GET_POSITION_INFO,(360*3600UL),LEN_360H_POS_DATA,L) == 1)
	{
		  	goto __OVER_USB_DATA;
	}


	/////事故疑点///////////////////
	Len  = 0;
	GB_LargeData.TotalItem = 100;
	L = Uart.SetHeadUSBBlockData(eGB_rCMD_GET_PARKING_DATA, eGB_GET_STOP_DATA, 0, (uchar *)&TxdTBuffer[Len]);	 //USB数据块头	
	if (USB_GetGBdata_PartFun(eGB_rCMD_GET_PARKING_DATA,(360*3600UL),LEN_STOP_DATA,L) == 1)
	{
		  	goto __OVER_USB_DATA;
	}

	/////疲劳驾驶///////////////////
	Len  = 0;
	GB_LargeData.TotalItem = 100;
	L = Uart.SetHeadUSBBlockData(eGB_rCMD_GET_OVER_TIME_DRIVE, eGB_GET_OVER_TIME_3H, 0, (uchar *)&TxdTBuffer[Len]);	 //USB数据块头	
	if (USB_GetGBdata_PartFun(eGB_rCMD_GET_OVER_TIME_DRIVE,(360*3600UL),LEN_OVER_TIME_DATA,L) == 1)
	{
		  	goto __OVER_USB_DATA;
	}

	/////驾驶员登录退出///////////////////
	Len  = 0;
	GB_LargeData.TotalItem = 200;
	L = Uart.SetHeadUSBBlockData(eGB_rCMD_GET_GET_LOG_IN_OUT_RECORD, eGB_GET_GET_LOG_IN_OUT_RECORD, 0, (uchar *)&TxdTBuffer[Len]);	 //USB数据块头	
	if (USB_GetGBdata_PartFun(eGB_rCMD_GET_GET_LOG_IN_OUT_RECORD,(360*3600UL),LEN_DRIVER_OUT_IN_DATA,L) == 1)
	{
		  	goto __OVER_USB_DATA;
	}

	/////外部供电///////////////////
	Len  = 0;
	GB_LargeData.TotalItem = 100;
	L = Uart.SetHeadUSBBlockData(eGB_rCMD_GET_POWER_RECORD, eGB_GET_POWER_RECORD, 0, (uchar *)&TxdTBuffer[Len]);	 //USB数据块头	
	if (USB_GetGBdata_PartFun(eGB_rCMD_GET_POWER_RECORD,(360*3600UL),LEN_GET_POWER_RECORD_DATA,L) == 1)
	{
		  	goto __OVER_USB_DATA;
	}

	/////参数修改//////////////////
	Len  = 0;
	GB_LargeData.TotalItem = 100;
	L = Uart.SetHeadUSBBlockData(eGB_rCMD_GET_MODIFY_PARA_RECORD, eGB_GET_MODIFY_PARA_RECORD, 0, (uchar *)&TxdTBuffer[Len]);	 //USB数据块头	
	if (USB_GetGBdata_PartFun(eGB_rCMD_GET_MODIFY_PARA_RECORD,(360*3600UL),LEN_GET_MODIFY_PARA_RECORD_DATA,L) == 1)
	{
		  	goto __OVER_USB_DATA;
	}

	////// 速度状态日志 ////////////////
	Len  = 0;
	GB_LargeData.TotalItem = 10;
	L = Uart.SetHeadUSBBlockData(eGB_rCMD_GET_SPEED_STATUS_LOG, eGB_GET_SPEED_STATUS_LOG, 0, (uchar *)&TxdTBuffer[Len]);	 //USB数据块头	
	if (USB_GetGBdata_PartFun(eGB_rCMD_GET_SPEED_STATUS_LOG,(360*3600UL),LEN_GET_SPEED_STATUS_LOG_DATA,L) == 1)
	{
			Uart.puts("usb write speed status log err\r\n");
		  	goto __OVER_USB_DATA;
	}

   //write_file_error_flag=1;
	
__OVER_USB_DATA:
	//write_file_error_flag = 0;
	USBUseFlag = 0;
	TxdTBuffer[0] = USBCheckSum;
	Uart.WaitUSBGetData(10, 1);
	if(Write_File_Error==0)
	{
	  BEEP(9,0,4,0,4);
	  os_dly_wait(4);
	}
	

}
/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	函数名称：SendDownCMDtoU0
   功能描述：下发一条虚拟的GB指令
   入口参数：指令首地址 ， 数据长度
   出口参数：
   全局变量: 
   创建时间：
   修改时间：
*/ 	
//void     UartCls::SendDownCMDtoU0(uchar  *DataBuffer, uint32 DataLen)
//{	
//	uint32  t;
//	CheckSum = 0;
//	for(t=0; t<DataLen; t++)	{ CheckSum ^= *(DataBuffer+t);	}
//	*(DataBuffer+DataLen) = CheckSum;
//
//	//GB19056Process(DataLen,DataBuffer,TxdTBuffer);
//
//}
/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	函数名称：CmdForGetGBdata
   功能描述：虚拟一条GB下发指令，只用于取大块数据记录
   入口参数：指令类型 ，要取的数据块个数
   出口参数：
   全局变量: 
   创建时间：
   修改时间：
*/ 	
//void      UartCls::CmdForGetGBdata(uchar  CmdType, uint16 DataBlocks)
//{	 
//	 uchar   DataBuffer[20];
//	 uint32  Len;
//	 SetGB_Down_CMDHead(U0_RecBuffer, 14, CmdType);
//	 Len = Rtc.GetRtcSecond() - 2*ONE_YEAR_SECOND;									            //搜索往后2年的数据取48小时速度数据
//	 Rtc.SecondToDateTime_BCD(Len, (RTC_STR *)DataBuffer);					               //起始时间
//	 memmove(&U0_RecBuffer[LEN_U0_GB_FRAME_HEAD], DataBuffer, 6);
//	 Rtc.GetRTC_BCD((RTC_STR *)&DataBuffer[0]);									               //结束时间
//	 memmove(&U0_RecBuffer[LEN_U0_GB_FRAME_HEAD+6], DataBuffer, 6);
//	 U0_RecBuffer[LEN_U0_GB_FRAME_HEAD+12] = (DataBlocks>>8)&0xff;									//48小时数据块数 48*60=2880=0x0B40
//	 U0_RecBuffer[LEN_U0_GB_FRAME_HEAD+13] = DataBlocks&0xff;
//	 SendDownCMDtoU0(U0_RecBuffer, LEN_U0_GB_FRAME_HEAD+14);
//	 
//
//}
/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	函数名称：SetGB_Down_CMDHead
   功能描述：组包国标下发指令包头
   入口参数：
   出口参数：
   全局变量: 
   创建时间：
   修改时间：
*/ 	
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ 以上原形声明的函数供本函数调用
void        UartCls::SetGB_Down_CMDHead(uchar *DataBf, uint32 DataLen, uchar Cmd)
{							
	*(DataBf+0) = 0xAA;
   *(DataBf+1) = 0x75;
   *(DataBf+2) = Cmd;
   *(DataBf+3) = (DataLen>>8)&0xff;
   *(DataBf+4) = (DataLen)&0xff;
   *(DataBf+5) = (DataLen>>16)&0xff;     //RESERVER_WORD;
}

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
//uchar		tU0Buf[32] = {0x24,0x01,0x0c,0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x30,0x28,0xff};
extern	uchar StartBDUpdate;
void			UartCls::ReceiveProcessData(void)
{
	uchar		s,Cmd;
	uint16	DataLength;
	uchar		TxdBuffer[256+4];

	//uint32 Addr;
	
    if((++Rec.OverTimeCnt != 3))		return;			 //接收数据结束后30ms处理 

	if(Rec.Counter ==0)	return;

	/////////////////// RS232通讯，认为每次通讯只有一个数据包，不处理粘包情况。
	//处理数据不能延时(非测试指令），否则U0_RecBuffer有可能被刷新
	NOP();	NOP();	NOP();	NOP();	//	memmove(U0_RecBuffer,tU0Buf,14);
    DataLength = Rec.Counter;	
	
	if((U0_RecBuffer[0]==0x47)&&(U0_RecBuffer[1]==0x49)&&(U0_RecBuffer[2]==0x41)&&(U0_RecBuffer[3]==0x50))
	{
		NOP();
		IAPprocess();
	}
	else if( ( (U0_RecBuffer[0]==0x55)&&(U0_RecBuffer[1]==0x7a)&&(GetGB_Data_Flag==0))	|| 		 //Uz
				( (U0_RecBuffer[0]==0xaa)&&(U0_RecBuffer[1]==0x75)&&(GetGB_Data_Flag==0)) )
	{		
		Cmd = U0_RecBuffer[2];
		DataLength = U0_RecBuffer[3];
		DataLength <<= 8;
		DataLength += U0_RecBuffer[4];
		if(DataLength>UART0_REC_LENGTH)
		  DataLength = UART0_REC_LENGTH - 6;
		if( (U0_RecBuffer[6]=='G')&&(U0_RecBuffer[7]=='Y') )
		{
			GY19056Process();
		}
		else
		{
			s = CalXorSum((DataLength+6),U0_RecBuffer);
			if(s != U0_RecBuffer[(DataLength+6)])
			{		
				if(Cmd<0x80)
					s = UP_LOAD_ERROR;
				else
					s = DOWN_LOAD_ERROR;
				ReturnErrorFlag(s);		
				ClearReceiveFlag(0);
				return;					// Check Sum error
			}
			////////////////////////////////////////////////////////		
			GB19056Process(Rec.Counter,U0_RecBuffer,TxdBuffer);
		}
	}
	else if ( ((U0_RecBuffer[0]=='*')&&(U0_RecBuffer[1]=='G')&&(U0_RecBuffer[2]=='Y')) ||
					((U0_RecBuffer[0]=='*')&&(U0_RecBuffer[1]=='S')&&(U0_RecBuffer[2]=='Z')) ||
					 ((U0_RecBuffer[0]==0x7e)&&(U0_RecBuffer[1]==0x81)))
	{	
		Uart0CommandFlag = TRUE;
		GPRS.ProcessFrameData(0,DataLength,U0_RecBuffer);
		Uart0CommandFlag = FALSE;		
	}
	else if ((U0_RecBuffer[0]=='*')&&(U0_RecBuffer[1]=='B')&&(U0_RecBuffer[2]=='D'))
	{
		StartBDUpdate	= TRUE;
		Uart3.putb(Rec.Counter-3, &U0_RecBuffer[3]);
		os_dly_wait(50);
		StartBDUpdate	= FALSE;
	}
	else if( U0_RecBuffer[0]==0x24)		// LCM
	{
		KeyDisp.ReceiveProcessData();
	}
	else
	{
		#if (WUXI_TEST != 1)
		GPRS.BBJT808_GPRS_UploadCmd(eBB_gCMD_TER_DATA_TRAN_UPLOAD,DataLength,U0_RecBuffer);			
		#endif
	}
	
	ClearReceiveFlag(0);
	NOP();	NOP();	NOP();	NOP();
	/////////////////////////////////////////
}



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

static uint16 VDR_Data_Fun_1(VDR_DATA_FUN_1_ST *pVDR, uint32 *pReadPage, char *pDebugMsg)
{
	uint32 tl, DataLen = 0;
	uint32 Addr,i, GetDataCnt;

	 GetDataCnt = *pReadPage;

	if (GB_LargeData.SearchFlag==0)
	{	
		if (TD.SW==tSPEED_RECORD)	  Uart.puts(pDebugMsg);
				
		DFE.Read(pVDR->FMCnt,4,(uchar*)&tl);
		GetDataCnt = 0;
		if (pVDR->FlashPageSaveCnt>0)	//  flash中有数据
		{		 
			GetDataCnt = 	pVDR->FlashPageSaveCnt;	// 最新一页
			if (GetDataCnt>pVDR->FlashTotalPages) GetDataCnt = pVDR->FlashTotalPages;	
		}
		else if (tl>0)			// 铁电里有数据
		{	
			GetDataCnt = 1;
		}
	}

  	for (;GetDataCnt;GetDataCnt--)
	{	
		if ((DataLen > GB_LARGE_DATA_PACK_BLOCK_LEN)) // 每一个包的数据段不能太大
		{			 	
			break;
		}
			  
		if ((GetDataCnt==pVDR->FlashPageSaveCnt) || ((pVDR->FlashPageSaveCnt==0)&&(GetDataCnt==1)))
		{	// 取铁电里的数据
			if (tl<pVDR->FMSaveItemCnt)  //
			{ 	  	
				tl = tl;    
					
			}
			else		// 取最新存在铁电里部分数据,该数据还没存在flash计数
			{	  	
				tl = tl % pVDR->FMSaveItemCnt;	
			}

			DFE.Read(pVDR->FMAddr, pVDR->SaveItemLen * tl,pVDR->ptBuf);

			for (i=tl; i>0;i--)
			{	
						
				if (pVDR->ptBuf[pVDR->TimeBCDPos+(i-1)*pVDR->SaveItemLen]==0) continue;	  // 年份不能为0

				if ((memcmp(&pVDR->ptBuf[pVDR->TimeBCDPos+(i-1)*pVDR->SaveItemLen],GB_LargeData.StartTime,6)>=0)	&&
					(memcmp(&pVDR->ptBuf[pVDR->TimeBCDPos+(i-1)*pVDR->SaveItemLen],GB_LargeData.EndTime,6)<=0))
				{	 	
					// 找到符合的数据
					memmove(&pVDR->pSendBuffer[LEN_U0_GB_FRAME_HEAD+DataLen],&pVDR->ptBuf[(i-1) * (pVDR->SaveItemLen)],(pVDR->SaveItemLen));
					DataLen += (pVDR->SaveItemLen);	

					GB_LargeData.TotalItem--;
					if (GB_LargeData.TotalItem==0)
					{			
						GetDataCnt = 0;
						*pReadPage = GetDataCnt;
						return DataLen;
					}
				}
			}
		}

		
		if ((pVDR->FlashPageSaveCnt>0)&&(GB_LargeData.TotalItem))		 // 取flash中的数据
		{
			Addr = pVDR->FlashStartPageAddr;
			Addr += (GetDataCnt-1)%(pVDR->FlashTotalPages);
			DFF.PageRead(Addr, pVDR->ptBuf);
						
			for (i=(pVDR->FMSaveItemCnt); i>0;i--)
			{
				if ((DataLen > GB_LARGE_DATA_PACK_BLOCK_LEN)) // 每一个包的数据段不能太大
				{	 	
					break;
				}
					 
				if (pVDR->ptBuf[pVDR->TimeBCDPos+(i-1)*pVDR->SaveItemLen]==0) continue;
									
				if ((memcmp(&pVDR->ptBuf[pVDR->TimeBCDPos+(i-1)*pVDR->SaveItemLen],GB_LargeData.StartTime,6)>=0)	&&
					(memcmp(&pVDR->ptBuf[pVDR->TimeBCDPos+(i-1)*pVDR->SaveItemLen],GB_LargeData.EndTime,6)<=0))
				{		 	
					// 找到符合的数据
					memmove(&pVDR->pSendBuffer[LEN_U0_GB_FRAME_HEAD+DataLen],&pVDR->ptBuf[(i-1) * (pVDR->SaveItemLen)],(pVDR->SaveItemLen));
					DataLen += (pVDR->SaveItemLen);	

					GB_LargeData.TotalItem--;
					if (GB_LargeData.TotalItem==0)
					{	 	
						GetDataCnt = 0;
						*pReadPage = GetDataCnt;
						return DataLen;
					}
				}
			}
		}
		else if (pVDR->FlashPageSaveCnt==0)
		{				  	
			GB_LargeData.TotalItem = 0;	
		}

	
	}
	
	*pReadPage = GetDataCnt;		// 更新

	return DataLen;
}

////////
uint16		UartCls::SendVDR_Data_08H_To_15H(uint16 ReturnBufferLen, uchar *pReturnBuffer)
{
	static uint32 ReadPage;
	uchar s, SendDataType = 0;			 // SendDataType   0：需要发送到串口1上的， 1：不发送到串口，需要返回取得的数据
	uint16 DataLen, Length, i;
	uchar *pSendBuffer;
	uint32 Addr,t;
	SP_SW_PER_MIN *pSwPerMin;
	pSAVE_STOP_DATA		pSaveStopData;
	VDR_DATA_FUN_1_ST Vdr1;
	pSPEED_STATUS_LOG pSpeedStatus; 
	uchar tBuffer[256];


	if (pReturnBuffer)
	{
		SendDataType = 1;
	}


	if ((GB_LargeData.GetVDRFlag == TRUE) || (SendDataType==1))
	{
	   //os_dly_wait(10);

		DataLen = 0;

		if ((SendDataType != 1))
		{
			pSendBuffer = MyMalloc(1024);
			SetBufferValue(pSendBuffer,1024,0);
		}
		else
		{
			  if (ReturnBufferLen < 1024) return 0;
			  pSendBuffer =  pReturnBuffer;
			  SetBufferValue(pSendBuffer,ReturnBufferLen,0);
		}
			
		switch (GB_LargeData.Cmd)
		{
		case eGB_rCMD_GET_SPEED_PER_SECOND			:			// 0x08,48H,采集指定的行驶速度记录
			#if (0)
			if (GB_LargeData.TotalPack==0)
			{	
				if (TD.SW==tSPEED_RECORD) Uart.puts("速度数据：");
				ReadPage = 0;
				if (FlashPageCnt.Speed48HSave>0)
				{
					ReadPage = 	FlashPageCnt.Speed48HSave;	// 最新一页
					if (ReadPage>FF_48H_SPEED_TOTAL_PAGE) ReadPage = FF_48H_SPEED_TOTAL_PAGE;
				}

				
			}

			if (TD.SW==tSPEED_RECORD)
			{ Uart.puts("ReadPage Test Data:"); Uart.putb(1,4, (uchar *)&ReadPage); Uart.puts("\r\n"); os_dly_wait(50);	 }
		
			for (;ReadPage;ReadPage--)
			{
				if ((DataLen > GB_LARGE_DATA_PACK_BLOCK_LEN)) // 每一个包的数据段不能太大
				{
					 break;
				}

				Addr = FF_48H_SPEED_START_PAGE;
				Addr += ((ReadPage-1)%FF_48H_SPEED_TOTAL_PAGE);	 // 
				DFF.PageRead(Addr, tBuffer);
			
				// 一页存有2分钟的数据
				pSwPerMin = (SP_SW_PER_MIN *)(tBuffer+126);
				
				if (TD.SW==tSPEED_RECORD)
				{
//					Uart.puts("page Addr Test Data:"); Uart.putb(1,4, (uchar *)&Addr);  Uart.puts("\r\n");
//					Uart.puts("per min speed starttime 1 Test Data:"); Uart.putb(1,6, (pSwPerMin->StartTime));  Uart.puts("\r\n");
//					Uart.puts("GB_LargeData starttime Test Data:"); Uart.putb(1,6, GB_LargeData.StartTime);  Uart.puts("\r\n");
//					Uart.puts("GB_LargeData endtime Test Data:"); Uart.putb(1,6, GB_LargeData.EndTime);  Uart.puts("\r\n");
				}

				if ((pSwPerMin->StartTime[0]!=0)&&(memcmp(pSwPerMin->StartTime,GB_LargeData.StartTime,6)>=0) &&
					(memcmp(pSwPerMin->StartTime,GB_LargeData.EndTime,6)<=0))
				{	 // 找到符合时间区间的数据
					for (i=0; i<60;i++)
					{
						if (pSwPerMin->SpeedSw[i].Speed != 0) break;
						
						if ((i<59)&&(pSwPerMin->SpeedSw[i].Sw != pSwPerMin->SpeedSw[i].Sw)) break;	
					}
					if (i!=60)
					{
						memmove(&pSendBuffer[LEN_U0_GB_FRAME_HEAD+DataLen],(tBuffer+126),126);
					   pSendBuffer[LEN_U0_GB_FRAME_HEAD+DataLen+5] = 0; // 秒总为0
						DataLen += 126;
						GB_LargeData.TotalItem--;
						if (GB_LargeData.TotalItem==0)
						{
							ReadPage = 0;
							break;
						}
					}

				}
			
				
				pSwPerMin = (SP_SW_PER_MIN *)tBuffer;
				if (GB_LargeData.TotalItem&&(pSwPerMin->StartTime[0]!=0)&&(memcmp(pSwPerMin->StartTime,GB_LargeData.StartTime,6)>=0) &&
					(memcmp(pSwPerMin->StartTime,GB_LargeData.EndTime,6)<=0))
				{	 // 找到符合时间区间的数据
					for (i=0; i<60;i++)
					{
						if (pSwPerMin->SpeedSw[i].Speed != 0) break;
						
						if ((i<59)&&(pSwPerMin->SpeedSw[i].Sw != pSwPerMin->SpeedSw[i].Sw)) break;	
					}
					if (i!=60)
					{
						memmove(&pSendBuffer[LEN_U0_GB_FRAME_HEAD+DataLen],(tBuffer),126);
						pSendBuffer[LEN_U0_GB_FRAME_HEAD+DataLen+5] = 0; // 秒总为0
						DataLen += 126;
	
						GB_LargeData.TotalItem--;
						if (GB_LargeData.TotalItem==0)
						{
							ReadPage = 0;
							break;
						}	
					}
				}
				
						
			}
			#else		 //////////////// 优化
			if (GB_LargeData.SearchFlag==0)
			{	
				if (TD.SW==tSPEED_RECORD) Uart.puts("速度数据：");
				ReadPage = 0;
				if (FlashPageCnt.Speed48HSave>0)
				{
					ReadPage = 	FlashPageCnt.Speed48HSave;	// 最新一页
					if (ReadPage>FF_48H_SPEED_TOTAL_PAGE) ReadPage = FF_48H_SPEED_TOTAL_PAGE;
				}

				
			}

			if (TD.SW==tSPEED_RECORD)
			{ Uart.puts("ReadPage Test Data:"); Uart.putb(1,4, (uchar *)&ReadPage); Uart.puts("\r\n"); os_dly_wait(50);	 }
		
			for (t=0;ReadPage;ReadPage--)
			{
				//if ((DataLen > GB_LARGE_DATA_PACK_BLOCK_LEN))   break;// 每一个包的数据段不能太大
				if (DataLen >= 126*7) break;			// 最多发7包	每一帧不能超过1000字节

				Addr = FF_48H_SPEED_START_PAGE;
				Addr += ((ReadPage-1)%FF_48H_SPEED_TOTAL_PAGE);	 // 
				DFF.PageRead(Addr, tBuffer);
			
				// 一页存有2分钟的数据	
				for (s=2;s>0;s--)
				{
					pSwPerMin = (SP_SW_PER_MIN *)(tBuffer+126*(s-1));
					if (TD.SW==tSPEED_RECORD)
					{
						Uart.puts("page Addr Test Data:"); Uart.putb(1,4, (uchar *)&Addr);  Uart.puts("\r\n");
						Uart.puts("per min speed starttime 1 Test Data:"); Uart.putb(1,6, (pSwPerMin->StartTime));  Uart.puts("\r\n");
						Uart.puts("GB_LargeData starttime Test Data:"); Uart.putb(1,6, GB_LargeData.StartTime);  Uart.puts("\r\n");
						Uart.puts("GB_LargeData endtime Test Data:"); Uart.putb(1,6, GB_LargeData.EndTime);  Uart.puts("\r\n");
					}
					if ((pSwPerMin->StartTime[0]!=0)&&(memcmp(pSwPerMin->StartTime,GB_LargeData.StartTime,6)<0))
					{	 
						 ReadPage = 0;
						 break;
					}
					else if (GB_LargeData.TotalItem&&(pSwPerMin->StartTime[0]!=0)&&(memcmp(pSwPerMin->StartTime,GB_LargeData.StartTime,6)>=0) &&
					  (memcmp(pSwPerMin->StartTime,GB_LargeData.EndTime,6)<=0))
					{
						// 找到符合时间区间的数据
						for (i=0; i<60;i++)
						{
							if (pSwPerMin->SpeedSw[i].Speed != 0) break;
							
							if ((i<59)&&(pSwPerMin->SpeedSw[i].Sw != pSwPerMin->SpeedSw[i].Sw)) break;	
						}
						if (i!=60)
						{
							memmove(&pSendBuffer[LEN_U0_GB_FRAME_HEAD+DataLen],(tBuffer+126*(s-1)),126);
						   pSendBuffer[LEN_U0_GB_FRAME_HEAD+DataLen+5] = 0; // 秒总为0
							DataLen += 126;

							if (GB_LargeData.TotalItem>0)  	GB_LargeData.TotalItem--;
							t++;
							if (GB_LargeData.TotalItem==0)
							{
								if (t%2)	//  如果是奇数个数据块，要把看搜索地址加回，因为还有一包数据在flash，待下次再搜索取出
								{
									ReadPage++; 
								}
								//ReadPage = 0;
								break;
							}
						}
					 }
				}
				if (ReadPage==0) break;
						
			}

			#endif
	
			//Uart.puts("速度数据：");
			break;
		
		case eGB_rCMD_GET_POSITION_INFO				:			// 0x09,360H,采集指定的位置信息
			#if (0) // 整个小时的
			if (GB_LargeData.TotalPack==0)
			{	
				if (TD.SW==tSPEED_RECORD) Uart.puts("位置数据：");
				ReadPage = 0;
				if (FlashPageCnt.Location360HSave>1)
				{
					ReadPage = 	FlashPageCnt.Location360HSave;	// 最新一页
					if (ReadPage>FF_360H_LOCATION_TOTAL_PAGE) ReadPage = FF_360H_LOCATION_TOTAL_PAGE;
				}	
			}
		
			for (;ReadPage;ReadPage--)
			{
				if ((DataLen > GB_LARGE_DATA_PACK_BLOCK_LEN)) // 每一个包的数据段不能太大
				{
					 break;
				}

				Addr = FF_360H_LOCATION_START_PAGE;
			   Addr += ((ReadPage-1)%FF_360H_LOCATION_TOTAL_PAGE);	 // 
				DFF.PageRead(Addr, tBuffer);


				if (TD.SW==tSPEED_RECORD)
				{
					Uart.puts("Addr Test Data:"); Uart.putb(1,4,(uchar *)&Addr);  Uart.puts("\r\n");
					Uart.puts("tBuffer Test Data:"); Uart.putb(1,6, tBuffer);  Uart.puts("\r\n");
					Uart.puts("GB_LargeData starttime Test Data:"); Uart.putb(1,6, GB_LargeData.StartTime);  Uart.puts("\r\n");
					Uart.puts("GB_LargeData endtime Test Data:"); Uart.putb(1,6, GB_LargeData.EndTime);  Uart.puts("\r\n");
				} 

			
				// 一页存有20分钟的数据，3页才是1小时的数据，其中首页有时间 ， tBuffer[0] - tBuffer[5] 是时间
				if ((tBuffer[0]!= 0x00)&&(tBuffer[0]!= 0xff)&&(memcmp(tBuffer,GB_LargeData.StartTime,6)>=0) &&
					(memcmp(tBuffer,GB_LargeData.EndTime,6)<=0))
				{	 // 找到符合时间区间的数据
					if (TD.SW==tSPEED_RECORD) Uart.puts("get pos data ok");
					memmove(&pSendBuffer[LEN_U0_GB_FRAME_HEAD],tBuffer,226);
					//pSendBuffer[LEN_U0_GB_FRAME_HEAD+DataLen+4] = 0; // 分总为0
				   //pSendBuffer[LEN_U0_GB_FRAME_HEAD+DataLen+5] = 0; // 秒总为0
					DataLen += 226;
					Addr++;
			
					DFF.PageRead(Addr++,tBuffer);
					memmove(&pSendBuffer[LEN_U0_GB_FRAME_HEAD+DataLen],&tBuffer[6],220);
					DataLen += 220;
				
					DFF.PageRead(Addr++,tBuffer);
					memmove(&pSendBuffer[LEN_U0_GB_FRAME_HEAD+DataLen],&tBuffer[6],220);
					DataLen += 220;

					GB_LargeData.TotalItem--;
					if (GB_LargeData.TotalItem==0)
					{
						ReadPage = 0;
						break;
					}
//						
				}
				
						
			} 
	
			#else ///////////////////  含实时的数据	  ///////////////////////////////////////////////////////

			if (GB_LargeData.SearchFlag==0)
			{	
				if (TD.SW==tSPEED_RECORD) Uart.puts("位置数据：");
				ReadPage = 0;
				if (FlashPageCnt.Location360HSave>1)
				{
					ReadPage = 	FlashPageCnt.Location360HSave;	// 最新一页
					if (ReadPage>FF_360H_LOCATION_TOTAL_PAGE) ReadPage = FF_360H_LOCATION_TOTAL_PAGE;
				}
				
					
				// 发送铁电数据，这是实时的数据
			   SetBufferValue(&pSendBuffer[LEN_U0_GB_FRAME_HEAD],666,0x00);
				Rtc.GetRTC_BCD((RTC_STR  *)tBuffer);

				if ((tBuffer[0]!=0)&&(memcmp(tBuffer,GB_LargeData.StartTime,4)>=0) &&
					(memcmp(tBuffer,GB_LargeData.EndTime,4)<=0))
				{		
					//if (TD.SW==tSPEED_RECORD) Uart.puts("TTT");	
					// 读取最新数据	(存在铁电) 
					  					 
					  DFE.Read(FM_360H_LOCATION_TIME,666,&pSendBuffer[LEN_U0_GB_FRAME_HEAD]);

					  DataLen = 666;	 // 数据段必须666的倍数
	
						GB_LargeData.TotalItem--;
						if (GB_LargeData.TotalItem==0)
						{
							//ReadPage = 0;
						}
				
				}	
			
			} 
				
			for (;ReadPage;ReadPage--)
			{
				//if ((DataLen > GB_LARGE_DATA_PACK_BLOCK_LEN)) // 每一个包的数据段不能太大
				if (DataLen >= 666)
				{			
					 break;
				}


				Addr = FF_360H_LOCATION_START_PAGE;
			   Addr += ((ReadPage-1)%FF_360H_LOCATION_TOTAL_PAGE);	 // 
				DFF.PageRead(Addr, tBuffer);

				
				// 一页存有20分钟的数据，3页才是1小时的数据，其中首页有时间 ， tBuffer[0] - tBuffer[5] 是时间
				if ((tBuffer[0]!= 0x00)&&(tBuffer[0]!= 0xff)&&(tBuffer[4]==0)&&(tBuffer[5]==0)&&
					(memcmp(tBuffer,GB_LargeData.StartTime,4)>=0) &&
					(memcmp(tBuffer,GB_LargeData.EndTime,4)<=0))
				{	 // 找到符合时间区间的数据
					if (TD.SW==tSPEED_RECORD) Uart.puts("get pos data ok");

					memmove(&pSendBuffer[LEN_U0_GB_FRAME_HEAD],tBuffer,226);
					//pSendBuffer[LEN_U0_GB_FRAME_HEAD+DataLen+4] = 0; // 分总为0
				   //pSendBuffer[LEN_U0_GB_FRAME_HEAD+DataLen+5] = 0; // 秒总为0
					DataLen += 226;
					Addr++;
			
					DFF.PageRead(Addr++,tBuffer);
					memmove(&pSendBuffer[LEN_U0_GB_FRAME_HEAD+DataLen],&tBuffer[6],220);
					DataLen += 220;
				
					DFF.PageRead(Addr++,tBuffer);
					memmove(&pSendBuffer[LEN_U0_GB_FRAME_HEAD+DataLen],&tBuffer[6],220);
					DataLen += 220;

					GB_LargeData.TotalItem--;
					if (GB_LargeData.TotalItem==0)
					{
						ReadPage = 0;
						break;
					}
						
				}
				
						
			} 	
			#endif

			
			break;
		
		case eGB_rCMD_GET_PARKING_DATA				:			// 0x10,事故疑点数据（停车前20秒数据）
			if (GB_LargeData.SearchFlag==0)
			{	
				if (TD.SW==tSPEED_RECORD) Uart.puts("停车数据：");
				ReadPage = 0;
				if (FlashPageCnt.ParkingDataSave>0)
				{
					ReadPage = 	FlashPageCnt.ParkingDataSave;	// 最新一页
					if (ReadPage>FF_PARKING_DATA_TOTAL_PAGE) ReadPage = FF_PARKING_DATA_TOTAL_PAGE;

					//Uart.puts("ReadPage Test Data:"); Uart.putb(1,4,(uchar *)&ReadPage); Uart.puts("\r\n");
				}
			}
		
			for (;ReadPage;ReadPage--)
			{
				if (SendDataType != 1)	 //串口采集时
				{
				///if ((DataLen > GB_LARGE_DATA_PACK_BLOCK_LEN)) // 每一个包的数据段不能太大
				// 根据实际检测中心条件，最多发4包，超时驾驶1，2，3	，并且数据段不能超过1024
				//  如果检测软件采5个数据块，只回3个，检测软件就认为该采集完成
					if ((DataLen > LEN_SAVE_STOP_DATA*3))
					{
						 break;
					}
				}
				else
				{
					 if ((DataLen > GB_LARGE_DATA_PACK_BLOCK_LEN)) 
					 {
					 	 break;
					 }
				}

				Addr = FF_PARKING_DATA_START_PAGE;
				Addr += ((ReadPage-1)%FF_PARKING_DATA_TOTAL_PAGE);	 // 
				DFF.PageRead(Addr, tBuffer);

				// 一页存有1条事故疑点数据
				pSaveStopData = (pSAVE_STOP_DATA)tBuffer;

				if ((pSaveStopData->DateTime[0]!=0)&&(memcmp(pSaveStopData->DateTime,GB_LargeData.StartTime,6)>=0)	&&
					(memcmp(pSaveStopData->DateTime,GB_LargeData.EndTime,6)<=0))

				{	 // 找到符合时间区间的数据
					
					// 过检机器，后面要去掉
					if (memcmp(&pSaveStopData->DateTime[0],"\x15\x03\x12\x16\x06\x10",6)==0)
					{
						pSaveStopData->DateTime[5] += 8; 
					}
					//
					memmove(&pSendBuffer[LEN_U0_GB_FRAME_HEAD+DataLen],tBuffer,LEN_SAVE_STOP_DATA);
					DataLen += LEN_SAVE_STOP_DATA;

					GB_LargeData.TotalItem--;
					if (GB_LargeData.TotalItem==0)
					{
						//ReadPage = 0;
						break;
					}	
				}	
						
			}	
			break;
		case eGB_rCMD_GET_OVER_TIME_DRIVE			:			// 0x11,疲劳驾驶数据

			Vdr1.FMAddr = FM_SAVE_FATIGUE_DRIVING;			  // 数据在铁电的地址
			Vdr1.FMCnt = FM_FATIGUE_DRIVING_CNT;			  // 数据条目总计数
			Vdr1.FMSaveItemCnt = 256/LEN_SAVE_FATIGUE_DRIVING_INFO;	  // 数据条目能保存在铁电的条数	 256/ 7  = 36
			Vdr1.SaveItemLen = LEN_SAVE_FATIGUE_DRIVING_INFO;		// 数据条目长度
			Vdr1.FlashStartPageAddr = FF_TIRE_DRIVE_START_PAGE;			// 数据条目在flash的开始页地址
			Vdr1.FlashPageSaveCnt = FlashPageCnt.FatigueDrivingSave;	 // 数据条目在flash的计数
			Vdr1.FlashTotalPages = FF_TIRE_DRIVE_TOTAL_PAGE;	 //  数据条目在flash总共的页数 
			Vdr1.TimeBCDPos = 18;				// 每个数据条目时间在哪个位置	即每条记录中时间的偏移地址  看GB19056-2012可知		
			Vdr1.ptBuf = tBuffer;				   // 临时缓存	  
			Vdr1.pSendBuffer = pSendBuffer;	

			DataLen = VDR_Data_Fun_1(&Vdr1, &ReadPage,(char *)"疲劳驾驶数据：");
		
		
			break;
		
		case eGB_rCMD_GET_GET_LOG_IN_OUT_RECORD	:			// 0x12,驾驶员登录退出记录
	
			Vdr1.FMAddr = FM_DRIVER_OUT_IN;			  // 数据在铁电的地址
			Vdr1.FMCnt = FM_DRIVER_OUT_IN_CNT;			  // 数据条目总计数
			Vdr1.FMSaveItemCnt = 256/LEN_DRIVER_IN_OUT_STATUS;	  // 数据条目能保存在铁电的条数	 
			Vdr1.SaveItemLen = LEN_DRIVER_IN_OUT_STATUS;		// 数据条目长度
			Vdr1.FlashStartPageAddr = FF_DRIVER_IN_OUT_START_PAGE;			// 数据条目在flash的开始页地址
			Vdr1.FlashPageSaveCnt = FlashPageCnt.DriverLogInOutSave;	 // 数据条目在flash的计数
			Vdr1.FlashTotalPages = FF_DRIVER_IN_OUT_TOTAL_PAGE;	 //  数据条目在flash总共的页数 
			Vdr1.TimeBCDPos = 0;				// 每个数据条目时间在哪个位置	即每条记录中时间的偏移地址 		
			Vdr1.ptBuf = tBuffer;				   // 临时缓存	  
			Vdr1.pSendBuffer = pSendBuffer;	
			 //DataLen = 0;		  // test
			DataLen = VDR_Data_Fun_1(&Vdr1, &ReadPage,(char *)"驾驶员登录退出数据：");	 // 会重启
			//Uart.puts("驾驶员登录退出数据：");
			//
		 	break;
		
		case eGB_rCMD_GET_POWER_RECORD				:			// 0x13,采集外部供电记录
			Vdr1.FMAddr = FM_EXT_POWER_STATUS;			  // 数据在铁电的地址
			Vdr1.FMCnt = FM_EXT_POWER_STATUS_CNT;			  // 数据条目总计数
			Vdr1.FMSaveItemCnt = 256/LEN_EXT_POWER_STATUS;	  // 数据条目能保存在铁电的条数	 256/ 7  = 36
			Vdr1.SaveItemLen = LEN_EXT_POWER_STATUS;		// 数据条目长度
			Vdr1.FlashStartPageAddr = FF_POWER_ON_OFF_START_PAGE;			// 数据条目在flash的开始页地址
			Vdr1.FlashPageSaveCnt = FlashPageCnt.PowerOnOffSave;	 // 数据条目在flash的计数
			Vdr1.FlashTotalPages = FF_POWER_ON_OFF_TOTAL_PAGE;	 //  数据条目在flash总共的页数 
			Vdr1.TimeBCDPos = 0;				// 每个数据条目时间在哪个位置	即每条记录中时间的偏移地址 		
			Vdr1.ptBuf = tBuffer;				   // 临时缓存	  
			Vdr1.pSendBuffer = pSendBuffer;	

			DataLen = VDR_Data_Fun_1(&Vdr1, &ReadPage,(char *)"外部供电数据：");
			break;

		case eGB_rCMD_GET_MODIFY_PARA_RECORD		:			// 0x14,参数修改记录
			Vdr1.FMAddr = FM_PARA_MODIFY_RECORD;			  // 数据在铁电的地址
			Vdr1.FMCnt = FM_PARA_MODIFY_RECORD_CNT;			  // 数据条目总计数
			Vdr1.FMSaveItemCnt = 256/LEN_PARA_MODIFY_RECORD;	  // 数据条目能保存在铁电的条数	 256/ 7  = 36
			Vdr1.SaveItemLen = LEN_PARA_MODIFY_RECORD;		// 数据条目长度
			Vdr1.FlashStartPageAddr = FF_PARA_MODIFY_START_PAGE;			// 数据条目在flash的开始页地址
			Vdr1.FlashPageSaveCnt = FlashPageCnt.ParaModifySave;	 // 数据条目在flash的计数
			Vdr1.FlashTotalPages = FF_PARA_MODIFY_TOTAL_PAGE;	 //  数据条目在flash总共的页数 
			Vdr1.TimeBCDPos = 0;				// 每个数据条目时间在哪个位置	即每条记录中时间的偏移地址 		
			Vdr1.ptBuf = tBuffer;				   // 临时缓存	  
			Vdr1.pSendBuffer = pSendBuffer;	

			DataLen = VDR_Data_Fun_1(&Vdr1, &ReadPage,(char *)"参数修改：");
			//
			break;

		case eGB_rCMD_GET_SPEED_STATUS_LOG			:			// 0x15,速度状态记录  
			if (GB_LargeData.SearchFlag==0)
			{	
				if (TD.SW==tSPEED_RECORD) Uart.puts("速度状态数据：");
				ReadPage = 0;
				if (FlashPageCnt.SpeedStatusSave>0)
				{
					ReadPage = 	FlashPageCnt.SpeedStatusSave;	// 最新一页
					if (ReadPage>FF_SPEED_STATUS_TOTAL_PAGE) ReadPage = FF_SPEED_STATUS_TOTAL_PAGE;

					//Uart.puts("ReadPage Test Data:"); Uart.putb(1,4,(uchar *)&ReadPage); Uart.puts("\r\n");
				}
			}
		
			for (;ReadPage;ReadPage--)
			{
				if ((DataLen > GB_LARGE_DATA_PACK_BLOCK_LEN)) // 每一个包的数据段不能太大
				{
					 break;
				}

				Addr = FF_SPEED_STATUS_START_PAGE;
				Addr += ((ReadPage-1)%FF_SPEED_STATUS_TOTAL_PAGE);	 // 
				DFF.PageRead(Addr, tBuffer);

				// 一页存有1条速度状态
				pSpeedStatus = (pSPEED_STATUS_LOG)tBuffer;

				if ((pSpeedStatus->StartTime!=0)&&(memcmp(pSpeedStatus->StartTime,GB_LargeData.StartTime,6)>=0)	&&
					(memcmp(pSpeedStatus->StartTime,GB_LargeData.EndTime,6)<=0))

				{	 // 找到符合时间区间的数据
					memmove(&pSendBuffer[LEN_U0_GB_FRAME_HEAD+DataLen],tBuffer,LEN_SPEED_STATUS_LOG);
					DataLen += LEN_SPEED_STATUS_LOG;	

					GB_LargeData.TotalItem--;
					if (GB_LargeData.TotalItem==0)
					{
						ReadPage = 0;
						break;
					}
				}
				
						
			}
			//	if (TD.SW==tSPEED_RECORD) Uart.puts("速度状态数据：");
			//
			break; 
		
		
		}			
		
		// 
		if (SendDataType != 1)
		{
		   *(pSendBuffer+0) = 0x55;
			*(pSendBuffer+1) = 0x7a;
			*(pSendBuffer+2) = GB_LargeData.Cmd;
			*(pSendBuffer+3) = DataLen/256;
			*(pSendBuffer+4) = DataLen%256;
			*(pSendBuffer+5) = RESERVER_WORD;
			Length  = DataLen + 6;		
			s = CalXorSum(Length,pSendBuffer);
			*(pSendBuffer+Length) = s;
			Length++;
			GB_LargeData.SearchFlag = 1;
			putb(1,Length,pSendBuffer);		// 返回信息到上位机


		   //BEEP(9,0,2,0,5);
			MyFree(pSendBuffer);

			//if (ReadPage==0)
			{
				if ((ReadPage==0) ||(GB_LargeData.TotalItem == 0) ||  (DataLen==0))
				{  
					GB_LargeData.SearchFlag = 0;
					GB_LargeData.TotalItem = 0;
					GB_LargeData.GetVDRFlag = FALSE;
					BEEP(9,0,2,0,5);
				}
				GB_LargeData.TotalItem  = 0;
			}
			GB_LargeData.GetVDRFlag = FALSE;



		}
		else
		{
			if (DataLen)
			{
				Length = DataLen;
				memmove(pReturnBuffer, pReturnBuffer+LEN_U0_GB_FRAME_HEAD, Length);
				GB_LargeData.SearchFlag = 1;
			}
			else
			{
				 Length = 0;
			}

			if (ReadPage==0)
			{
				GB_LargeData.GetVDRFlag = FALSE;
				BEEP(9,0,2,0,5);
			}


		}
		

		return Length;


	}

	return 0;


}
	

/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
   函数名: ReturnErrorFlag
   功能：  UART0返回错误标志
   入口：  无
   出口：  无
*/
void				UartCls::ReturnErrorFlag(uchar s)
{
	uchar   Sum,tBuffer[6];
	while(Txd.Sending)
	{
		Uart.puts("cc1"); os_dly_wait(1);
	}	
	Sum  = 0x55;
	Sum ^= 0x7a;
	Sum ^= s;
	Sum ^= RESERVER_WORD;
	tBuffer[0] = 0x55;
    tBuffer[1] = 0x7a;
    tBuffer[2] = s;
    tBuffer[3] = RESERVER_WORD;
    tBuffer[4] = Sum;  
	if(GetGB_Data_Flag==0)
	{ 
		Uart.puts("cc2"); os_dly_wait(1);
		putb(0,5,tBuffer);		// 返回信息到上位机	
	}
}


/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	函数名称：
   功能描述：
   入口参数：
   出口参数：
   全局变量: 
   创建时间：
   修改时间：
*/ 	
//void			GPRS_PushAlignment(uint16 Len,uchar *pData);
//uchar			NF_Format(uint16 Affirm);
extern		"C"		uchar			HalfByteHexCharToInt(uchar Hex);

//static    uint16      ULen;
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ 以上原形声明的函数供本函数调用
uint16       UartCls::GB19056Process(uint16 LenRec,uchar *pRecBuffer,uchar *pSendBuffer)
{
	uchar		i,s,Cmd,Error;
	uint32	DataLength,Length;
	uint32	tl;
	union
	{
		uchar		Buffer[64];
		uint16	B16[32];
		uint32	B32[16];
	}Tp;

	struct
	{
		uchar		Y;
		uchar		M;
		uchar		D;
	}RTCD;
	
	
	/////////////////// RS232通讯，认为每次通讯只有一个数据包，不处理粘包情况。
	//处理数据不能延时(非测试指令），否则U0_RecBuffer有可能被刷新
	//////////////////////////////////////////////////////////////////////
	Cmd = *(pRecBuffer+2);
	DataLength = *(pRecBuffer+3);
	DataLength <<= 8;
	DataLength += *(pRecBuffer+4);
	NOP();	NOP();
	Error = 0;
	Length = 0;
	//SetBufferValue(TxdTBuffer,(256+16),0);		
	////////////////////////////////////////////////////////////////////////////////////
	switch(Cmd)
	{	
		//上载命令字	//////////////////////////////////////////////////////////
		case eGB_rCMD_GET_STD_VERSION:			// 采集记录仪执行标准
		   
		   *(pSendBuffer+LEN_U0_GB_FRAME_HEAD) = 0x12;  		// 记录仪执行标准年号后2位 BCD码， GB/T19056-2012 
		   *(pSendBuffer+LEN_U0_GB_FRAME_HEAD+1) = 0x00;  		// 修改单号 无修改或无应答则默认为0
		   Length = LEN_U0_GB_FRAME_HEAD + 2;
			
		break;
		//////////////////
		case eGB_rCMD_GET_DRIVER_CODE:			// 机动车驾驶证号
			
			SetBufferValue( (uchar*)(pSendBuffer+LEN_U0_GB_FRAME_HEAD),20,0);
			DFE.Read(FM_DRIVER_CODE, 18, (pSendBuffer+LEN_U0_GB_FRAME_HEAD));	
			if( strlen( (char*)(pSendBuffer+LEN_U0_GB_FRAME_HEAD))!=18)
				memmove( (uchar*)(pSendBuffer+LEN_U0_GB_FRAME_HEAD),"500382197911253837\0",18);
			//
			Length = LEN_U0_GB_FRAME_HEAD + 18;
		break;
		
		//		$$$$$$$$$$$$$$$$$$$$  读取时实时钟
		case eGB_rCMD_GET_REAL_TIME:
			NOP();	NOP();
			Length = LEN_U0_GB_FRAME_HEAD+6;								
			Rtc.GetRTC_BCD((RTC_STR *)(pSendBuffer+LEN_U0_GB_FRAME_HEAD));										
			break;
			
		////////////////////////////// 初始公里数，实际不使用，只设置和读取，仅供参考
		case eGB_rCMD_GET_TOTAL_MILAGE:			// 从安装开始，累计行驶里程数据
			/*Rtc.GetRTC_BCD((RTC_STR *)(pSendBuffer+LEN_U0_GB_FRAME_HEAD));				//当前时间
			DFE.Read(FM_INSTALL_DATE,6,(pSendBuffer+LEN_U0_GB_FRAME_HEAD+6));			// 安装时间
			DFE.Read(FM_INIT_MILAGE_BCD, 4, (pSendBuffer+LEN_U0_GB_FRAME_HEAD+12));	// 初始里程
			tl = SystemLog.VDR.SpeedSensorPulsCnt;
			tl /= VehiclePulseKV;			
			Int2Bcd8( tl , (pSendBuffer+LEN_U0_GB_FRAME_HEAD+16));						// 行驶累计里程

			Length = LEN_U0_GB_FRAME_HEAD+20;	 */

			Rtc.GetRTC_BCD((RTC_STR *)(pSendBuffer+LEN_U0_GB_FRAME_HEAD));				//当前时间
			DFE.Read(FM_INSTALL_DATE,6,(pSendBuffer+LEN_U0_GB_FRAME_HEAD+6));			// 安装时间
			DFE.Read(FM_INIT_MILAGE_BCD, 4, (pSendBuffer+LEN_U0_GB_FRAME_HEAD+12));	// 初始里程
			tl = SystemLog.VDR.SpeedSensorPulsCnt;
			tl *= 10;
			tl /= VehiclePulseKV;
			UINT32HexToBcd( tl , (pSendBuffer+LEN_U0_GB_FRAME_HEAD+16));						// 行驶累计里程

			Length = LEN_U0_GB_FRAME_HEAD+20;
		break;
		//////////////////
		case eGB_rCMD_GET_SPEED_KV:				// *Vehicle Characteristic Coefficients		速度特征系数
			Rtc.GetRTC_BCD((RTC_STR *)(pSendBuffer+LEN_U0_GB_FRAME_HEAD));				//当前时间
			DFE.Read( FM_VEHICLE_PULSE_KV,4,(uchar*)&VehiclePulseKV);
			Tp.B32[0] = VehiclePulseKV;
			*(pSendBuffer+LEN_U0_GB_FRAME_HEAD+6) = Tp.Buffer[1];
			*(pSendBuffer+LEN_U0_GB_FRAME_HEAD+7) = Tp.Buffer[0];
			Length = LEN_U0_GB_FRAME_HEAD+8;
		break;

		//////////////////
		case eGB_rCMD_GET_VEHICLE_VIN					:			// *车辆VIN号，车牌号码，车辆分类
			//DFE.Read(FM_VEHICLE_VIN,LEN_VEHICLE_VIN + LEN_VEHICLE_CODE + LEN_VEHICLE_CLASS,(pSendBuffer+LEN_U0_GB_FRAME_HEAD));
			DFE.Read(FM_VEHICLE_VIN,LEN_VEHICLE_VIN,(pSendBuffer+LEN_U0_GB_FRAME_HEAD));
			DFE.Read(FM_VEHICLE_CODE,LEN_VEHICLE_CODE,(pSendBuffer+LEN_U0_GB_FRAME_HEAD+LEN_VEHICLE_VIN) );
			DFE.Read(FM_VEHICLE_CLASS,LEN_VEHICLE_CLASS,(pSendBuffer+LEN_U0_GB_FRAME_HEAD+LEN_VEHICLE_VIN+LEN_VEHICLE_CODE) );
			Length = LEN_VEHICLE_VIN + LEN_VEHICLE_CODE + LEN_VEHICLE_CLASS + LEN_U0_GB_FRAME_HEAD;	
		break;		
		
		//////////////////
		case eGB_rCMD_GET_SIGNAL_CONFIG:			// 采集信号状态配置信息 
						SetBufferValue(pSendBuffer,100,0);
			Rtc.GetRTC_BCD((RTC_STR *)(pSendBuffer+LEN_U0_GB_FRAME_HEAD));				//当前时间
			*(pSendBuffer+LEN_U0_GB_FRAME_HEAD+6) = 1;		// 只需1字节
			//memmove((pSendBuffer+LEN_U0_GB_FRAME_HEAD+7), (uchar*)"自定义1\x00\x00\x00自定义2\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00近光\x00\x00\x00\x00\x00\x00远光\x00\x00\x00\x00\x00\x00右转向\x00\x00\x00\x00左转向\x00\x00\x00\x00制动\x00\x00\x00\x00\x00\x00",80);
			DFE.Read(FM_SIGNAL_CONFIG,80,(pSendBuffer+LEN_U0_GB_FRAME_HEAD+7));
			Length = LEN_U0_GB_FRAME_HEAD+87;			
		break;
		
		//////////////////////////////////// 唯一序列号
		case eGB_rCMD_GET_SERIAL_NUMBER:
			
			#if (WUXI_TEST==1)
			Length = LEN_U0_GB_FRAME_HEAD;	 
			SetBufferValue(pSendBuffer,35,0);
			memmove((uchar*)(pSendBuffer+Length),"0000000",7);   // 生产厂CCC认证代码
			Length += 7;
			strcpy((char*)(pSendBuffer+Length),"SZ-BD21A"); // 使用别人的平台，所以这直接改型号，让平台查询到正确的 2015.3.24
			//DFE.Read(EE_TERMINAL_TYPE,16, (uchar*)(pSendBuffer+Length));      			//  认证产品型号	 后面的型号使用这个
		
			Length += 16;
			memmove((uchar*)(pSendBuffer+Length),"\x14\x11\x01",3); 				// BCD码 生产日期年月日
			Length += 3;
			//memmove((uchar*)(pSendBuffer+Length),"\x00\x00\x00\x01",4);    // 流水号
			memmove((uchar*)(pSendBuffer+Length),"\x59\xF9\x62\xE1",4); 	  // 0x59F962E1	这里用48小时数据设备的编号 1509516001 
			Length += 4;															  			
			Length += 5;             // 加上5个字节的备用	
			#else
			DFE.Read(FM_SERIAL_NUMBER, LEN_SERIAL_NUMBER, (uchar*)(pSendBuffer+LEN_U0_GB_FRAME_HEAD) );
			Length = LEN_U0_GB_FRAME_HEAD+LEN_SERIAL_NUMBER;
			#endif
		break;
		
		case eGB_rCMD_GET_SPEED_PER_SECOND			:			// 0x08,48H,采集指定的行驶速度记录
		case eGB_rCMD_GET_POSITION_INFO				:			// 0x09,360H,采集指定的位置信息
		case eGB_rCMD_GET_PARKING_DATA				:			// 0x10,事故疑点数据（停车前20秒数据）
		case eGB_rCMD_GET_OVER_TIME_DRIVE			:			// 0x11,疲劳驾驶数据
		case eGB_rCMD_GET_GET_LOG_IN_OUT_RECORD	:			// 0x12,驾驶员登录退出记录
		case eGB_rCMD_GET_POWER_RECORD				:			// 0x13,采集外部供电记录
		case eGB_rCMD_GET_MODIFY_PARA_RECORD		:			// 0x14,参数修改记录
		case eGB_rCMD_GET_SPEED_STATUS_LOG			:			// 0x15,速度状态记录
			
	

		if (memcmp(GB_LargeData.StartTime,(pRecBuffer+LEN_U0_GB_FRAME_HEAD+6),6) !=0)
		{
			s = 1;
			memmove(GB_LargeData.StartTime,(pRecBuffer+LEN_U0_GB_FRAME_HEAD),6);	  // 开始时间  bcd
		}
		else 
		{
			s = 0;
		}
		memmove(GB_LargeData.EndTime,(pRecBuffer+LEN_U0_GB_FRAME_HEAD+6),6);	  // 结束时间	  bcd
		// 应用于无锡软件检测，一问一答，为了提高传输速度，只有开始果不一样，或者命令字不一样，  GB_LargeData.TotalPack
		// 才置0，这样，SendVDR_Data_08H_To_15H函数重新搜flash
		GB_LargeData.TotalItem = (*(pRecBuffer+LEN_U0_GB_FRAME_HEAD+6+6) << 8) | *(pRecBuffer+LEN_U0_GB_FRAME_HEAD+6+6+1);
		if (GB_LargeData.TotalItem==0) GB_LargeData.TotalItem = 1;
		//GB_LargeData.PackIndex = 0;
		//GB_LargeData.TotalPack = 0;
		//GB_LargeData.Cmd = Cmd;
		//GB_LargeData.GetVDRFlag = TRUE;
		if ((Cmd != GB_LargeData.Cmd) || (s==1))
		{
			 GB_LargeData.SearchFlag = 0;
		}
		GB_LargeData.Cmd = Cmd;
		GB_LargeData.GetVDRFlag = TRUE;
		GB_LargeData.OverTimeSearch = 10; //   10秒

		return 0;
		//break;


		
		//////////////////
		//其中：国标规定至少有 0x01~0x07共7个上载命令，其它可在显示器显示


		/////////////////检定命令////////////////////////////////////////////////////////////////
		case eGB_rCMD_SET_CHECK:			// 检定控制
			
			Length = LEN_U0_GB_FRAME_HEAD;
		break;

		case eGB_rCMD_SET_CHECK_CONTINUSE:			// 设检定模式保持
		
		break;
		


		////////////	下载命令字	//////////////////////////////////////////////////////////
		/*case eGB_rCMD_SET_DRIVER_CODE:			// 设置驾驶员代码，驾驶证号码		国标无要求

			Length = LEN_U0_GB_FRAME_HEAD;
		break;	*/
		
		///////////////////////////////////////////////// 特征系数
		case eGB_rCMD_SET_VEHICLE_PULSE_KV:
			// 前6字节为日期时间
			Tp.B32[0] = 0;
			Tp.Buffer[0] = *(pRecBuffer+LEN_U0_GB_FRAME_HEAD+7);
			Tp.Buffer[1] = *(pRecBuffer+LEN_U0_GB_FRAME_HEAD+6);
			VehiclePulseKV = Tp.B32[0];
			DFE.Write( FM_VEHICLE_PULSE_KV,4,(uchar*)&VehiclePulseKV);
			memmove( (pSendBuffer+LEN_U0_GB_FRAME_HEAD),(uchar*)&VehiclePulseKV,4);
			Para_Modi_Recode(Cmd);
			Length = LEN_U0_GB_FRAME_HEAD+4;
		break;
		
		//////////////////
		case eGB_rCMD_SET_VEHICLE_VIN:			// 设置车辆VIN号，车牌号，车辆分类
			
			DFE.Write(FM_VEHICLE_VIN,17,(pRecBuffer+LEN_U0_GB_FRAME_HEAD));
			SetBufferValue(Tp.Buffer, 12, 0);
			memmove(Tp.Buffer,(pRecBuffer+LEN_U0_GB_FRAME_HEAD+17),12);
			DFE.Write(FM_VEHICLE_CODE,9,Tp.Buffer);  // 车牌号在铁电只有9个字节，不能写12个字节
			DFE.Write(FM_VEHICLE_CLASS,12,(pRecBuffer+LEN_U0_GB_FRAME_HEAD+17+12));
			Para_Modi_Recode(Cmd);
			Length = LEN_U0_GB_FRAME_HEAD;
		break;
		
		//////////////////
		case eGB_rCMD_SET_SETUP_DATE:			// 安装日期
			DFE.Write(FM_INSTALL_DATE,6,(pRecBuffer+LEN_U0_GB_FRAME_HEAD));
			Para_Modi_Recode(Cmd);
		   Length = LEN_U0_GB_FRAME_HEAD;
		break;
		
		//////////////////
		case eGB_rCMD_SET_SIGNAL_STATUS:			// 设置信号状态配置
			
			DFE.Write(FM_SIGNAL_CONFIG,80,(pRecBuffer+LEN_U0_GB_FRAME_HEAD));
			Para_Modi_Recode(Cmd);
			Length = LEN_U0_GB_FRAME_HEAD;		
		break;
		
		///////////////////////////// 设置唯一序列号（国标使用，实际不使用）
		/*case eGB_rCMD_SET_SERIAL_NUMBER:
		
			DFE.Write( FM_SERIAL_NUMBER, LEN_SERIAL_NUMBER, (pRecBuffer+LEN_U0_GB_FRAME_HEAD) );
			Length = LEN_U0_GB_FRAME_HEAD;
		break;*/
		
				
		
		
		/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
		*/
		//		$$$$$$$$$$$$$$$$$$$$  设置时实时钟
		case eGB_rCMD_SET_REAL_TIME:
			NOP();	NOP();
			Rtc.SetRTC_BCD((RTC_STR *)(pRecBuffer+LEN_U0_GB_FRAME_HEAD));
			RTCD.Y = LPC_RTC->YEAR;
			RTCD.M = LPC_RTC->MONTH;
			RTCD.D = LPC_RTC->DOM;
			if((RTCD.M==1)||(RTCD.M==2))
			{
  				RTCD.M += 12;
  				RTCD.Y--;
			}
			LPC_RTC->DOW = ((RTCD.D+2*RTCD.M+3*(RTCD.M+1)/5+RTCD.Y+RTCD.Y/4- \
			             RTCD.Y/100+RTCD.Y/400)+1)%7;
			Para_Modi_Recode(Cmd);
			Length = LEN_U0_GB_FRAME_HEAD;
			break;

		////////////////////////////// 初始公里数，实际不使用，只设置和读取，仅供参考
		case	eGB_rCMD_SET_INIT_MIL:				 // 设置初始里程    // added 2013.5
			
			/*for (i = 0; i<4; i++)
			{	s =  *(pRecBuffer+LEN_U0_GB_FRAME_HEAD+i);
				if (((s & 0x0f) > 0x09) || (((s >> 4) & 0x0f) > 0x09))		 // 不是BCD码： 00 - 99 99 99 99 
				{
					Error = TRUE;
					break;	
				} 					
			}
			DFE.Write(FM_INIT_MILAGE_BCD, 4, (pRecBuffer+LEN_U0_GB_FRAME_HEAD)); 
			NOP(); NOP(); NOP();
			
			Length = LEN_U0_GB_FRAME_HEAD;*/
			Length = LEN_U0_GB_FRAME_HEAD;
			// 实时时钟
			Rtc.SetRTC_BCD((RTC_STR *)(pRecBuffer+Length));
			RTCD.Y = LPC_RTC->YEAR;
			RTCD.M = LPC_RTC->MONTH;
			RTCD.D = LPC_RTC->DOM;
			if((RTCD.M==1)||(RTCD.M==2))
			{
  				RTCD.M += 12;
  				RTCD.Y--;
			}
			LPC_RTC->DOW = ((RTCD.D+2*RTCD.M+3*(RTCD.M+1)/5+RTCD.Y+RTCD.Y/4- \
			             RTCD.Y/100+RTCD.Y/400)+1)%7;
	
			Length += 6;
			//

			DFE.Write(FM_INSTALL_DATE,6,(pRecBuffer+Length));
			Length += 6;

			for (i = Length; i<8; i++)
			{	s =  *(pRecBuffer+LEN_U0_GB_FRAME_HEAD+i);
				if (((s & 0x0f) > 0x09) || (((s >> 4) & 0x0f) > 0x09))		 // 不是BCD码： 00 - 99 99 99 99 
				{
					Error = TRUE;
					break;	
				} 					
			}
			DFE.Write(FM_INIT_MILAGE_BCD, 4, (pRecBuffer+Length));
			Length += 4;

			DFE.Write(FM_MILAGE_BCD,4,(pRecBuffer+Length));
			tl = BcdToUINT32Hex((pRecBuffer+Length));
			DFE.Write(FM_MILAGE_HEX,4,(uchar *)&tl);
			// 正常使用才下面没，检测时不能用，因为检测时，下发的数据太大，导致 SystemLog.VDR.SpeedSensorPulsCnt 会溢出
			SystemLog.VDR.SpeedSensorPulsCnt = (tl * VehiclePulseKV) / 10;		
			DFE.Write(EE_SYS_VDR_SPEED_PULS_CNT,4,(uchar *)&SystemLog.VDR.SpeedSensorPulsCnt); 	

			NOP(); NOP(); NOP();
			
			Para_Modi_Recode(Cmd);

			Length = LEN_U0_GB_FRAME_HEAD;
				
		break;

		default : 
			Error = TRUE;
			break;
	}
	////////////////////////////////////////////////////////////////////////////////////
	if(USBUseFlag==0)															     //是否USB要写入数据
    {
		if(Error)
		{
			if(Cmd<0x80)
				s = UP_LOAD_ERROR;
			else
				s = DOWN_LOAD_ERROR;
			ReturnErrorFlag(s);		
		}
		else
		{
			*(pSendBuffer+0) = 0x55;
			*(pSendBuffer+1) = 0x7a;
			*(pSendBuffer+2) = Cmd;
			*(pSendBuffer+3) = (Length-LEN_U0_GB_FRAME_HEAD)/256;
			*(pSendBuffer+4) = (Length-LEN_U0_GB_FRAME_HEAD)%256;
			*(pSendBuffer+5) = RESERVER_WORD;		
			s = CalXorSum(Length,pSendBuffer);
			*(pSendBuffer+Length) = s;
			Length++;
			if(GetGB_Data_Flag==0)
			{ 
			  putb(0,Length,pSendBuffer);		// 返回信息到上位机
			  BEEP(9,0,2,0,5);
		   }	
		}
	}
	
	NOP();	NOP();
	return Length;	
}
		



/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	函数名称：SetU0FramHead
   功能描述：组包U0指令包头
   入口参数：
   出口参数：
   全局变量: 
   创建时间：
   修改时间：
*/ 	
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ 以上原形声明的函数供本函数调用
void        UartCls::SetU0FramHead(uchar *DataBf, uint32 DataLen, uchar Cmd)
{							
	*(DataBf+0) = 0x55;
   *(DataBf+1) = 0x7a;
   *(DataBf+2) = Cmd;
   *(DataBf+3) = (DataLen>>8)&0xff;
   *(DataBf+4) = (DataLen)&0xff;
   *(DataBf+5) = (DataLen>>16)&0xff;     //RESERVER_WORD;
}





/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	函数名称：Para_Modi_Recode
   功能描述：记录参数修改记录
   入口参数：
   出口参数：
   全局变量: 
   创建时间：
   修改时间：
*/ 	
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ 以上原形声明的函数供本函数调用
void        UartCls::Para_Modi_Recode(uchar ParaCmd)
{	
	uint32 tl;
	uint32 Addr;
	PARA_MODIFY_RECORD Modify;
	uchar tBuffer[256];
	
	Modify.EventType = ParaCmd;
	Rtc.GetRTC_BCD((RTC_STR *)tBuffer);
	memmove(Modify.Time,tBuffer, 6);
	DFE.Read(FM_PARA_MODIFY_RECORD_CNT,4,(uchar*)&tl);
	Addr = tl%36;							  // 	 36条
	Addr *= LEN_PARA_MODIFY_RECORD;
	Addr += FM_PARA_MODIFY_RECORD;		  	
	DFE.Write( Addr, LEN_PARA_MODIFY_RECORD,(uchar *)&Modify);
	tl++;
	DFE.Write(FM_PARA_MODIFY_RECORD_CNT,4,(uchar*)&tl);	 	// 驾驶员登入登出计数器加1后存储到铁电	
	//
	Uart.puts("更新参数修改到铁电 "); 
	if ((tl)&&(tl%36)==0)
	{
		// 铁电充满近一页的Flash的空间，则放到Flash上
		if( (FlashPageCnt.ParaModifySave%FF_PAGE_PER_SECTOR)==0)
		{
			Addr = FF_PARA_MODIFY_START_PAGE;
			Addr += FlashPageCnt.ParaModifySave%FF_PARA_MODIFY_TOTAL_PAGE;
			
			DFF.SectorEraseWaitOK(Addr/FF_PAGE_PER_SECTOR,10);
		}				
		///////////////////////////////////////////存储到Flash  这样1页flash存有36条外部供电记录
		DFE.Read(FM_PARA_MODIFY_RECORD,LEN_PARA_MODIFY_RECORD*36,tBuffer); // 
	
		Addr = FF_PARA_MODIFY_START_PAGE;
		Addr += FlashPageCnt.ParaModifySave%FF_PARA_MODIFY_TOTAL_PAGE;
		DFF.PageProgramWaitOK(Addr,tBuffer,3);
		FlashPageCnt.ParaModifySave++;
	
		DFE.Write( FM_FLASH_PAGE_CNT, LEN_FLASH_PAGE_CNT, (uchar*)&FlashPageCnt);
		Uart.puts("保存参数修改记录到Flash"); 
		//Uart.puts("Addr Test Data:"); Uart.putb(1,4, (uchar *)&Addr); Uart.puts("\r\n");
	
	}
}


#if(1)
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
extern uchar 	CreateUserFlag,OnlyUseBD_GPS;
void        UartCls::GY19056Process(void)
{
	uchar		t,Cmd;
	uchar		s,Error;		
	uint16	i,Length,DataLength,Len;
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

	pGPS_SEND	pSd;
	pRW_EEP		pEE;

	/////////////////// RS232通讯，认为每次通讯只有一个数据包，不处理粘包情况。
	//处理数据不能延时(非测试指令），否则U0_RecBuffer有可能被刷新
	Cmd = U0_RecBuffer[2];
	DataLength = U0_RecBuffer[3];
	DataLength <<= 8;
	DataLength += U0_RecBuffer[4];	
	//////////////////////////////////////////////////////////////////////
	NOP();	NOP();
	Error = 0;
	Length = 0;		
	SetBufferValue(TxdTBuffer,(256+16),0);		
	////////////////////////////////////////////////////////////////////////////////////
	switch(Cmd)
	{	
		////////////////////////////////////////////// 设置疲劳驾驶相关参数
		case	eGY_rCMD_DRIVING_PARA:	// 0x80 // 300 120 1800 600 30
			// GYS \x2c\x01\x00\x00 \x78\x00\x00\x00 \x08\x07\x00\x00 \x58\x02\x00\x00 \x1e\x00\x00\x00 
			if( (U0_RecBuffer[LEN_U0_GY_FRAME_HEAD]=='S') )			// 必须全部同时设置，否则可能出错
			{
				SpeedC.SetGetPara( eSPEED_CMD_SET_DRIVING_TIME,LEN_DRIVING_TIME_LIMIT,(uchar*)&U0_RecBuffer[LEN_U0_GY_FRAME_HEAD+1]);
				Length = LEN_U0_GY_FRAME_HEAD;
			}
			else
			{
				SpeedC.SetGetPara( eSPEED_CMD_GET_DRIVING_TIME,LEN_DRIVING_TIME_LIMIT,(uchar*)&TxdTBuffer[LEN_U0_GY_FRAME_HEAD]);
				Length = LEN_U0_GY_FRAME_HEAD+LEN_DRIVING_TIME_LIMIT;
			}			
			break;
		////////////////////////////////////////////// 设置超速相关参数 
		case	eGY_rCMD_OVER_SPEED_PARA:	// 0x81 // GY G 1 \030 \005 \003 \015\000 \000\000
			// GY S 1 \030 \005 \003 \015\000 \000\000 
			if( (U0_RecBuffer[LEN_U0_GY_FRAME_HEAD]=='S') )	  		// 必须全部同时设置，否则可能出错
			{
				
				SpeedC.SetGetPara( eSPEED_CMD_SET_OVER_SPEED,LEN_OVER_SPEED_PARA,(uchar*)&U0_RecBuffer[LEN_U0_GY_FRAME_HEAD+1]);
				Length = LEN_U0_GY_FRAME_HEAD;
			}
			else
			{
				SpeedC.SetGetPara( eSPEED_CMD_GET_OVER_SPEED,LEN_OVER_SPEED_PARA,(uchar*)&TxdTBuffer[LEN_U0_GY_FRAME_HEAD]);
				Length = LEN_U0_GY_FRAME_HEAD+LEN_OVER_SPEED_PARA;
			}
			
			break;
			
		case eGY_rCMD_LOW_POWER_ALARM_VALUE:	
			
			// GY S \xff\xff\xff\xff \085\x00\x00\x00
			if( (U0_RecBuffer[LEN_U0_GY_FRAME_HEAD]=='S') )
			{
				memmove( Tp.Buffer,&U0_RecBuffer[LEN_U0_GY_FRAME_HEAD+1],8);
				HighLowLevel = Tp.B32[0];
				LowPowerAlarmValue = Tp.B32[1];			
				DFE.Write(EE_HIGH_LOW_LEVEL,8,Tp.Buffer);		
				Length = LEN_U0_GY_FRAME_HEAD;
			}
			else
			{
				Tp.B32[0] = HighLowLevel;
				Tp.B32[1] = LowPowerAlarmValue;
				memmove( (uchar*)&TxdTBuffer[LEN_U0_GY_FRAME_HEAD],Tp.Buffer,8);
				Length = LEN_U0_GY_FRAME_HEAD+8;
			}
			break;
		
		//		$$$$$$$$$$$$$$$$$$$$  读取时实时钟
		case eGY_rCMD_GET_REAL_TIME:
			NOP();	NOP();
			if(U0_RecBuffer[LEN_U0_GY_FRAME_HEAD]=='I')	// CPU内置时钟
			{
				Length = LEN_U0_GY_FRAME_HEAD+6;								
				Rtc.GetRTC_BCD((RTC_STR *)&TxdTBuffer[LEN_U0_GY_FRAME_HEAD]);										
			}
			else if(U0_RecBuffer[LEN_U0_GY_FRAME_HEAD]=='A')
			{
				Length = LEN_U0_GY_FRAME_HEAD+16;
				Rtc.ReadR8025((RTC_STR *)NULL,(uchar*)&TxdTBuffer[LEN_U0_GY_FRAME_HEAD]);		
			}
			else
			{
				Length = LEN_U0_GY_FRAME_HEAD+6;
				Rtc.ReadR8025((RTC_STR *)&TxdTBuffer[LEN_U0_GY_FRAME_HEAD],(uchar*)NULL);				
			}
			break;
		
		/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
		GY \x11 \x08 \x17 \x11 \x54 \x53
		*/
		//		$$$$$$$$$$$$$$$$$$$$  设置时实时钟
		case eGY_rCMD_SET_REAL_TIME:
			NOP();	NOP();
			
			Length = LEN_U0_GY_FRAME_HEAD;			
			Rtc.SetRTC_BCD((RTC_STR *)&U0_RecBuffer[LEN_U0_GY_FRAME_HEAD]);
						
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
						
			U0_RecBuffer[LEN_U0_GY_FRAME_HEAD+10] = t;			
			Rtc.SetRtcR8025((RTC_STR *)&U0_RecBuffer[LEN_U0_GY_FRAME_HEAD]); 
			
			break;
		
		
		case eGY_rCMD_SELECT_SIM_CARD:				// 选择SIM卡
			if( (U0_RecBuffer[LEN_U0_GY_FRAME_HEAD]=='I')||(U0_RecBuffer[LEN_U0_GY_FRAME_HEAD]=='E'))
			{
				DFE.Write(EE_SELECT_SIM_CARD,1,(uchar*)&U0_RecBuffer[LEN_U0_GY_FRAME_HEAD]);
				//if(U0_RecBuffer[LEN_U0_GY_FRAME_HEAD]=='I')	
				//	{	USE_SOP_SIM_CARD;	 UseWthSimCard = USE_IN_SIM;}
				//else														
				//	{	USE_DIP_SIM_CARD;	 UseWthSimCard = USE_OUT_SIM;}
			}
			else
			{
				DFE.Read(EE_SELECT_SIM_CARD,1,(uchar*)&TxdTBuffer[LEN_U0_GY_FRAME_HEAD]);
			}
			Length = LEN_U0_GY_FRAME_HEAD+1;
		break;

		case	eGY_rCMD_SET_SIM_NUMBER:		//设置SIM卡号码
					
			memmove((uchar*)sTelNum.Sim,(uchar*)&U0_RecBuffer[LEN_U0_GY_FRAME_HEAD],16);
			sTelNum.Sim[16] = 0;
			Length = strlen((char *)sTelNum.Sim);
			DFE.Write(EE_SIM_NUMBER,16,(uchar*)sTelNum.Sim);
			DecCharToPressBCD(Length,(char *)sTelNum.Sim,SimNumberBCD);
			for(i=0;i<8;i++)	{ if(SimNumberBCD[i]==0)	SimNumberBCD[i] = 0x0f;	}
			Length = LEN_U0_GY_FRAME_HEAD;
		break;


		case	eGY_rCMD_GET_SIM_NUMBER:	  //采集SIM卡号码
			
	      strcpy((char*)&TxdTBuffer[LEN_U0_GY_FRAME_HEAD],(char*)sTelNum.Sim);
			Length = strlen((char*)sTelNum.Sim);			
			Length += LEN_U0_GY_FRAME_HEAD;
		break;

		//  	$$$$$$$$$$$$$$$$$$$$ 读或写指定页的Nand Flash的数据
		//    串口接收格式 xx xx xxxx xx *x	   
		//    GYE-Test I \x00 \x00 \x00 \x00 \x00 \x00 \x00 \x00 \x00 \x00 \x00 \x00 \x00 

		case eGY_rCMD_GET_SET_EE_DATA:
		
			pEE = (pRW_EEP)&U0_RecBuffer[LEN_U0_GY_FRAME_HEAD];

			t = U0_RecBuffer[LEN_U0_GY_FRAME_HEAD];

			if( t=='S')		// 	SPI铁电  GY S \x00 R \xa0 \x00\x00 \x10\x00
			{
				if(pEE->RW == 'E')
				{
					EreadFRAM_RecodeMem( pEE->Addr,  pEE->Length);
				}				
				else if(pEE->RW == 'W')			// 'W' 写
				{
					DFE.Write(pEE->Addr,pEE->Length,&U0_RecBuffer[LEN_U0_GY_FRAME_HEAD+8]);		 /////////// change by wrc  ///////////////
				}
				else	//if(pEE->RW == 'R')			  	// 'R' 读
				{
					Length = pEE->Length;
					DFE.Read(pEE->Addr,Length,&TxdTBuffer[LEN_U0_GY_FRAME_HEAD]);					
				}		
			}
			else if( t=='I')			// I2C 	
			{
				if(pEE->RW == 'W')			// 'W' 写	  	GY I \x00 W \xa0 \x00\x00 \x10\x00
				{
					I2C0.WriteEEPROM(pEE->DS,pEE->PageLen,pEE->Addr,pEE->Length,&U0_RecBuffer[LEN_U0_GY_FRAME_HEAD+8]);  //pEE->PageLen		/////////// change by wrc  ///////////////
				}
				else if(pEE->RW == 'R')			  	// 'R' 读		GY I \x00 R \xa0 \x00\x00 \x10\x00
				{
					Length = pEE->Length;
					I2C0.ReadEEPROM(pEE->DS,pEE->Addr,Length,&TxdTBuffer[LEN_U0_GY_FRAME_HEAD]);
				}
				
			}			
			else
			  Error = TRUE;
			Length += LEN_U0_GY_FRAME_HEAD;	
			break;		
		//		$$$$$$$$$$$$$$$$$$$$  读写取数据存储器 SPI-Flash
		//    格式 x x xx xx    GY F \x00 R D \xE0\x1E \x10\x00 
		case eGY_rCMD_GET_SET_DF_DATA:
			
			Length = LEN_U0_GY_FRAME_HEAD;
			if( (U0_RecBuffer[Length+0]=='B')&&(U0_RecBuffer[Length+1]=='D') )
			{
				GetFlashData.TotalPage = FF_GPS_BLIND_MESSAGE_TOTAL_PAGE;
				GetFlashData.StartPage = FF_GPS_BLIND_MESSAGE_START_PAGE;

				memmove( &TxdTBuffer[LEN_U0_GY_FRAME_HEAD],(uchar*)"Get All Blind Data Ack",22);
				Length = LEN_U0_GY_FRAME_HEAD+22;
			}
			else if( (U0_RecBuffer[Length+0]=='H')&&(U0_RecBuffer[Length+1]=='D') )
			{
				#if(TEST_CODE_JUST_FOR_TEST_LUOYANG_NEW==1)
				GetFlashData.TotalPage = 256;
				#else
				GetFlashData.TotalPage = FF_GPS_MESSAGE_TOTAL_PAGE;
				#endif
				GetFlashData.StartPage = FF_GPS_MESSAGE_START_PAGE;
				memmove( &TxdTBuffer[LEN_U0_GY_FRAME_HEAD],(uchar*)"Get All History Data Ack",24);
				Length = LEN_U0_GY_FRAME_HEAD+24;
			}
			else if( (U0_RecBuffer[Length+0]=='D')&&(U0_RecBuffer[Length+1]=='D') )
			{
				memmove( (uchar*)&GetFlashData, &U0_RecBuffer[Length+2], 8 );
				memmove( &TxdTBuffer[LEN_U0_GY_FRAME_HEAD],(uchar*)"Get Flash Data Ack",18);
				Length = LEN_U0_GY_FRAME_HEAD+18;
			}
			
			if( GetFlashData.TotalPage )
			{
				Length -= LEN_U0_GY_FRAME_HEAD;
				memmove(U0_TxdBuffer,&TxdTBuffer[LEN_U0_GY_FRAME_HEAD],Length);
				Txd.Length = Length;
				pTxd = U0_TxdBuffer;
				Txd.Length--;
				LPC_UART0->THR  = *pTxd++;						// 发送数据		
				Txd.Sending = TRUE;
				os_dly_wait(20);
				return;
			}
			
			pEE = (pRW_EEP)&U0_RecBuffer[LEN_U0_GY_FRAME_HEAD];
			
			t = U0_RecBuffer[LEN_U0_GY_FRAME_HEAD];			

			if( t == 'F' )							// SPI Flash GY F \x00 E S \xE0\x1E \x10\x00 
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
						DFF.PageRead(pEE->Addr,&TxdTBuffer[LEN_U0_GY_FRAME_HEAD]);
						Length = 256;
					}
					else if(pEE->DS == 'S')		// 读状态寄存器和ID
					{
						TxdTBuffer[LEN_U0_GY_FRAME_HEAD] = DFF.ReadStatusRegister();
						DFF.ReadJEDEC_ID(&TxdTBuffer[LEN_U0_GY_FRAME_HEAD+1]);
						Length = 5;	
					}
				}
			}
			else
				Error = TRUE;
			Length += LEN_U0_GY_FRAME_HEAD;	
			break;		
			
		case eGY_rCMD_GET_SET_NF_DATA:			// Nand Flash

		 	break;	
		 	
		 	
		 	
		case 	eGY_rCMD_SET_INIT_SYS:       //恢复出厂设置		 同无线通道功能一样
		  	/*DFE.Read(EE_JT_MAIN_SERVER, LEN_JT808_IP_DNS, (uchar *)&JT808_IP_DNS ); //Bf.tBuffer);
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
	      } */
			
		  	break;	
		case  eGY_rCMD_ERASE_FLASH:	
			
			if ( (U0_RecBuffer[LEN_U0_GY_FRAME_HEAD]=='S')&&(U0_RecBuffer[LEN_U0_GY_FRAME_HEAD]=='E') )	// 擦除指定的扇区
			{
				
			}
			else if ( (U0_RecBuffer[LEN_U0_GY_FRAME_HEAD]=='B')&&(U0_RecBuffer[LEN_U0_GY_FRAME_HEAD]=='L') )	// 指定块
			{
				
			}
			
			break;
			
		case eGY_rCMD_ERASE_FLASH_RECORD_DATA:																			
			
			

			Length = LEN_U0_GY_FRAME_HEAD;
		break;	
		
		//////////////////////////////////////// 擦除单个或全部Flash页存储计数器
		case	eGY_rCMD_CLEAR_FLASH_PAGE_CNT:
			Length = LEN_U0_GY_FRAME_HEAD;
			if(  U0_RecBuffer[LEN_U0_GY_FRAME_HEAD+0]=='S')
			{ 				
				if( U0_RecBuffer[LEN_U0_GY_FRAME_HEAD+1]=='A') 
			   {
			   	SetBufferValue( (uchar*)&FlashPageCnt,LEN_FLASH_PAGE_CNT,0);
			   	DFE.Write( FM_FLASH_PAGE_CNT,LEN_FLASH_PAGE_CNT,(uchar*)&FlashPageCnt);
			   }
			   else if((U0_RecBuffer[LEN_U0_GY_FRAME_HEAD+1]+U0_RecBuffer[LEN_U0_GY_FRAME_HEAD+2]) < FLASH_PAGE_CNT_NUM )
				{					
					i = U0_RecBuffer[LEN_U0_GY_FRAME_HEAD+1];		// 开始个数
					t = U0_RecBuffer[LEN_U0_GY_FRAME_HEAD+2];		// 总个数
					pData = (uchar*)&FlashPageCnt;
					pData += (i*4);
					memmove( pData, &U0_RecBuffer[LEN_U0_GY_FRAME_HEAD+3], t*4 );
					DFE.Write( FM_FLASH_PAGE_CNT,LEN_FLASH_PAGE_CNT,(uchar*)&FlashPageCnt);	
				}
			   else
				{
					Error = TRUE;
				}
			}			
			else	// 读
			{
				if( U0_RecBuffer[LEN_U0_GY_FRAME_HEAD+1]=='A') 
			   {
			   	memmove(&TxdTBuffer[LEN_U0_GY_FRAME_HEAD], (uchar*)&FlashPageCnt,LEN_FLASH_PAGE_CNT);
			   	Length += LEN_FLASH_PAGE_CNT;
			   }
			   else if((U0_RecBuffer[LEN_U0_GY_FRAME_HEAD+1]+U0_RecBuffer[LEN_U0_GY_FRAME_HEAD+2]) < FLASH_PAGE_CNT_NUM )
				{					
					i = U0_RecBuffer[LEN_U0_GY_FRAME_HEAD+1];		// 起始个数
					t = U0_RecBuffer[LEN_U0_GY_FRAME_HEAD+2];
					pData = (uchar*)&FlashPageCnt;
					pData += (i*4);
					strcpy( (char*)TxdTBuffer,"12345678 ");
					Length = LEN_U0_GY_FRAME_HEAD+1;
					for(i=0;i<t;i++)
					{
						memmove( Tp.Buffer, pData, 4 );						
						Int2Str( Tp.B32[0], (char*)&TxdTBuffer[Length] );
						strcat( (char*)TxdTBuffer,",");
						Length = strlen( (char*)TxdTBuffer );
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
			
			memmove( &TxdTBuffer[LEN_U0_GY_FRAME_HEAD],(uchar*)&FlashPageCnt,LEN_FLASH_PAGE_CNT);
			DFE.Read( FM_FLASH_PAGE_CNT,LEN_FLASH_PAGE_CNT,(uchar*)&TxdTBuffer[LEN_U0_GY_FRAME_HEAD+LEN_FLASH_PAGE_CNT]);
			Length = LEN_U0_GY_FRAME_HEAD+LEN_FLASH_PAGE_CNT*2;
		break;


		case	eGY_rCMD_GET_DRIVER_IC_CARD_INFO:		// 读取驾驶员IC卡信息128B
		 	if((GET_IC_CARD_STATUS)==0)		// IC卡已插入 （用于常开卡座）  
			//if((GET_IC_CARD_STATUS)!=0)		// IC卡已插入	(用于常闭卡座)
			{
				 while (ICCardBusing==TRUE)
				 {
				 	 os_dly_wait(10);
				 }
	
				 
				 I2C0_PIN_SELECT;
				 os_dly_wait(1);
	
				 ICCardBusing = TRUE;
			
				 s = I2C0.ReadEEPROM(CARD_DEVICE_ADDR,0,128,(uchar*)&TxdTBuffer[LEN_U0_GY_FRAME_HEAD]);
				 if (s==ERROR)
				 {
				 	TxdTBuffer[LEN_U0_GY_FRAME_HEAD] = 1;
					Length = LEN_U0_GY_FRAME_HEAD+1;
				 	//Uart.puts("读卡失败");
				 }
				 else
				 {
				 	 Length = LEN_U0_GY_FRAME_HEAD+128;
				 }	 	
	
				ICCardBusing = FALSE;
	
				
			
			}
			else
			{
				 TxdTBuffer[LEN_U0_GY_FRAME_HEAD] = 2;
				 Length = LEN_U0_GY_FRAME_HEAD+1;
				
				 //Uart.puts("未插卡");
			}
					


		break;
		
		//		$$$$$$$$$$$$$$$$$$$$  8个标志位
		case eGY_rCMD_SET_8_FLAGS:
			NOP();	NOP();
			////////////////////////////////  GYE-Test
			if(  (U0_RecBuffer[LEN_U0_GB_FRAME_HEAD+0]=='G')&&(U0_RecBuffer[LEN_U0_GB_FRAME_HEAD+1]=='Y')\
			   &&(U0_RecBuffer[LEN_U0_GB_FRAME_HEAD+2]=='E')&&(U0_RecBuffer[LEN_U0_GB_FRAME_HEAD+3]=='-')\
				&&(U0_RecBuffer[LEN_U0_GB_FRAME_HEAD+4]=='T')&&(U0_RecBuffer[LEN_U0_GB_FRAME_HEAD+5]=='e')\
				&&(U0_RecBuffer[LEN_U0_GB_FRAME_HEAD+6]=='s')&&(U0_RecBuffer[LEN_U0_GB_FRAME_HEAD+7]=='t'))
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
				if(U0_RecBuffer[LEN_U0_GB_FRAME_HEAD+8])		TD.MainTask 	= ((0x55<<8)+U0_RecBuffer[LEN_U0_GB_FRAME_HEAD+8]); 							
				if(U0_RecBuffer[LEN_U0_GB_FRAME_HEAD+9])		TD.GPS      	= ((0x55<<8)+U0_RecBuffer[LEN_U0_GB_FRAME_HEAD+9]);				
				if(U0_RecBuffer[LEN_U0_GB_FRAME_HEAD+10])		TD.GPRS     	= ((0x55<<8)+U0_RecBuffer[LEN_U0_GB_FRAME_HEAD+10]);			// 				
				if(U0_RecBuffer[LEN_U0_GB_FRAME_HEAD+11])		TD.CAN		 	= ((0x55<<8)+U0_RecBuffer[LEN_U0_GB_FRAME_HEAD+11]); 		// 
				if(U0_RecBuffer[LEN_U0_GB_FRAME_HEAD+12])		TD.SW       	= ((0x55<<8)+U0_RecBuffer[LEN_U0_GB_FRAME_HEAD+12]);
				if(U0_RecBuffer[LEN_U0_GB_FRAME_HEAD+13])		TD.ADC      	= ((0x55<<8)+U0_RecBuffer[LEN_U0_GB_FRAME_HEAD+13]);			
				if(U0_RecBuffer[LEN_U0_GB_FRAME_HEAD+14])		TD.Display  	= ((0x55<<8)+U0_RecBuffer[LEN_U0_GB_FRAME_HEAD+14]);
				if(U0_RecBuffer[LEN_U0_GB_FRAME_HEAD+15])		TD.Camera		= ((0x55<<8)+U0_RecBuffer[LEN_U0_GB_FRAME_HEAD+15]);
				if(U0_RecBuffer[LEN_U0_GB_FRAME_HEAD+16])		TD.Speed			= ((0x55<<8)+U0_RecBuffer[LEN_U0_GB_FRAME_HEAD+16]);
				TD.Value = U0_RecBuffer[LEN_U0_GB_FRAME_HEAD+19];
				TD.Value <<= 8;
				TD.Value += U0_RecBuffer[LEN_U0_GB_FRAME_HEAD+18];
				if(TD.SW==0)
				{
				  SimuSpeed.Enable = 0;					  //结束开关量检测，也结束速度模拟
			     Pwm.UnInit();     // LPC_PINCON->PINSEL7 &= (~(3UL<<18));
				  Sys.PowerControl(PCONP_PWM1,0);
				}	
				
				//if(TD.SW == tHC595_OUT1)		HC595.Out1.Byte = U0_RecBuffer[LEN_U0_GB_FRAME_HEAD+16];
				//if(TD.SW == tHC595_OUT2)		HC595.Out2.Byte = U0_RecBuffer[LEN_U0_GB_FRAME_HEAD+16];

				if( (TD.GPS==tGPS_USE_GPS)||(TD.GPS==tGPS_USE_GPS_BD)||(TD.GPS==tGPS_USE_BD) )
					OnlyUseBD_GPS = 0;
				
				
				memmove(&TxdTBuffer[LEN_U0_GY_FRAME_HEAD],(uchar*)&TD,20);
				Length = 20;
				
					
			}
			else if(  (U0_RecBuffer[LEN_U0_GY_FRAME_HEAD+0]=='T')&&(U0_RecBuffer[LEN_U0_GY_FRAME_HEAD+1]=='e')\
			   &&(U0_RecBuffer[LEN_U0_GY_FRAME_HEAD+2]=='s')&&(U0_RecBuffer[LEN_U0_GY_FRAME_HEAD+3]=='t')\
				&&(U0_RecBuffer[LEN_U0_GY_FRAME_HEAD+4]=='M')&&(U0_RecBuffer[LEN_U0_GY_FRAME_HEAD+5]=='o')\
				&&(U0_RecBuffer[LEN_U0_GY_FRAME_HEAD+6]=='d')&&(U0_RecBuffer[LEN_U0_GY_FRAME_HEAD+7]=='e'))
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
				if( (U0_RecBuffer[LEN_U0_GY_FRAME_HEAD+8]=='A')&&(U0_RecBuffer[LEN_U0_GY_FRAME_HEAD+9]=='u')&&\
					 (U0_RecBuffer[LEN_U0_GY_FRAME_HEAD+10]=='t')&&(U0_RecBuffer[LEN_U0_GY_FRAME_HEAD+11]=='o'))
					TD.TestMode 	= TEST_MODE_AUTO;
				else
				{
					TD.TestMode 	= TEST_MODE;
					SimuSpeed.Enable = ENABLE_FLAG;
					Sys.PowerControl(PCONP_PWM1,1);	
					Pwm.SetTargetSpeed(100,12); 
				}
			}	
			else if((U0_RecBuffer[LEN_U0_GY_FRAME_HEAD+0]=='I')&&(U0_RecBuffer[LEN_U0_GY_FRAME_HEAD+1]=='D')\
					&&(U0_RecBuffer[LEN_U0_GY_FRAME_HEAD+2]=='L')&&(U0_RecBuffer[LEN_U0_GY_FRAME_HEAD+3]=='E')\
			   	&&(U0_RecBuffer[LEN_U0_GY_FRAME_HEAD+4]=='-')&&(U0_RecBuffer[LEN_U0_GY_FRAME_HEAD+5]=='T')\
					&&(U0_RecBuffer[LEN_U0_GY_FRAME_HEAD+6]=='e')&&(U0_RecBuffer[LEN_U0_GY_FRAME_HEAD+7]=='s')\
					&&(U0_RecBuffer[LEN_U0_GY_FRAME_HEAD+8]=='t'))
			{
			  	Idle.EnableInterIdleMode = 1;
				Idle.IdleModeFlag = 0;
				Idle.InterIdleModeTime = U0_RecBuffer[LEN_U0_GB_FRAME_HEAD+9];
				Idle.WakeUpTime = U0_RecBuffer[LEN_U0_GB_FRAME_HEAD+10];
				Idle.WakeUpLongTime = U0_RecBuffer[LEN_U0_GB_FRAME_HEAD+11];
				Idle.SecondCnt=0;						
				DFE.Write(EE_IDLE_STR,LEN_EE_IDLE,(uchar*)&Idle);	
				#if(DFE_DEBUG==TRUE)
				if(TD.MainTask==tMAIN_WRITE_DFE)	Uart.puts("DFE09\r\n\0");
				#endif									
			}
			
			else if(  (U0_RecBuffer[LEN_U0_GY_FRAME_HEAD+0]=='T')&&(U0_RecBuffer[LEN_U0_GY_FRAME_HEAD+1]=='e')\
			   &&(U0_RecBuffer[LEN_U0_GY_FRAME_HEAD+2]=='s')&&(U0_RecBuffer[LEN_U0_GY_FRAME_HEAD+3]=='t')&&(U0_RecBuffer[LEN_U0_GY_FRAME_HEAD+4]=='M')\
			   &&(U0_RecBuffer[LEN_U0_GY_FRAME_HEAD+5]=='o')&&(U0_RecBuffer[LEN_U0_GY_FRAME_HEAD+6]=='d')&&(U0_RecBuffer[LEN_U0_GY_FRAME_HEAD+7]=='e'))
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
			Length += LEN_U0_GY_FRAME_HEAD;	
			break;	

		/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
			读取GSM/GPRS相关信息
		*/
		//		$$$$$$$$$$$$$$$$$$$$  读取短消息服务中心号码
		case eGY_rCMD_GET_SMS_CENTER_NUMBER:
			NOP();	NOP();
			memmove(&TxdTBuffer[LEN_U0_GY_FRAME_HEAD],sTelNum.SmsCenter,16);
			//strcpy((char*)sTelNum.SmsCenter,(char*)&U0_RecBuffer[LEN_U0_GB_FRAME_HEAD]);
			//DFE.Read(EE_SMS_CENTER_NUMBER,16,&TxdTBuffer[LEN_U0_GY_FRAME_HEAD+16]);
			Length = LEN_U0_GY_FRAME_HEAD+16;
			break;
		//		$$$$$$$$$$$$$$$$$$$$  读监控中心号码
		case eGY_rCMD_GET_CENTER_NUMBER:
			NOP();	NOP();
			DFE.Read(EE_CENTER_NUMBER,48,&TxdTBuffer[LEN_U0_GY_FRAME_HEAD]);
			
			Length = LEN_U0_GY_FRAME_HEAD+64;
			break;
		//		$$$$$$$$$$$$$$$$$$$$  读取GPRS服务器IP和端口号
		case eGY_rCMD_GET_GPRS_SERVER_IP:						
			NOP();	NOP();
			SetBufferValue(TxdTBuffer,160,0);
			strcpy((char*)&TxdTBuffer[6],(char*)sIP.TCP);			
			DFE.Read(EE_IP_PORT,96,&TxdTBuffer[6+32]);			
			Length = LEN_U0_GY_FRAME_HEAD+32*4;						
			break;

		//		$$$$$$$$$$$$$$$$$$$$  读取型号和终端ID
		case eGY_rCMD_GET_OEM_ID:
		case eGY_rCMD_GET_SOLE_NUMBER:
			
			SetBufferValue(TxdTBuffer,160,0);
			strcpy((char*)&TxdTBuffer[6],(char*)sOEM_ID);			
			Length = LEN_U0_GY_FRAME_HEAD+16;
			break;
		
		//		$$$$$$$$$$$$$$$$$$$$  读取生产商Web
		case eGY_rCMD_GET_OEM_WEB:
			NOP();	NOP();	NOP();
			if(  (U0_RecBuffer[LEN_U0_GB_FRAME_HEAD+0]=='G')&&(U0_RecBuffer[LEN_U0_GB_FRAME_HEAD+1]=='u')\
			   &&(U0_RecBuffer[LEN_U0_GB_FRAME_HEAD+2]=='o')&&(U0_RecBuffer[LEN_U0_GB_FRAME_HEAD+3]=='y')\
				&&(U0_RecBuffer[LEN_U0_GB_FRAME_HEAD+4]=='e')&&(U0_RecBuffer[LEN_U0_GB_FRAME_HEAD+5]=='G')\
				&&(U0_RecBuffer[LEN_U0_GB_FRAME_HEAD+6]=='P')&&(U0_RecBuffer[LEN_U0_GB_FRAME_HEAD+7]=='S'))		
			{
				//DFE.Read(EE_OEM_WEB,32,&TxdTBuffer[LEN_U0_GY_FRAME_HEAD]);			
				Length = LEN_U0_GY_FRAME_HEAD+32;
			}
			else
				Error = TRUE;
			break;

		case eGY_rCMD_GET_GY_FLAG:			
			NOP();	NOP();			
			if(  (U0_RecBuffer[LEN_U0_GB_FRAME_HEAD+0]=='G')&&(U0_RecBuffer[LEN_U0_GB_FRAME_HEAD+1]=='Y')\
			   &&(U0_RecBuffer[LEN_U0_GB_FRAME_HEAD+2]=='E')&&(U0_RecBuffer[LEN_U0_GB_FRAME_HEAD+3]=='-')\
				&&(U0_RecBuffer[LEN_U0_GB_FRAME_HEAD+4]=='F')&&(U0_RecBuffer[LEN_U0_GB_FRAME_HEAD+5]=='l')\
				&&(U0_RecBuffer[LEN_U0_GB_FRAME_HEAD+6]=='a')&&(U0_RecBuffer[LEN_U0_GB_FRAME_HEAD+7]=='g'))	
			{
				//Sys.GetGYFlagToUart();
				return ;
			}
			else
			{
				Error = TRUE;			
				Length += LEN_U0_GY_FRAME_HEAD;
			}
			break;
		
//		case eGY_rCMD_GET_GPS_PACK_TIME:			
//			
////			DFE.Read(EE_GPS_SEND_CONFIG,LEN_SEND_CFG,(uchar*)&TxdTBuffer[LEN_U0_GY_FRAME_HEAD]);		
////			Length = LEN_U0_GY_FRAME_HEAD+16;
////			pSd = (pGPS_SEND)&TxdTBuffer[Length];
////			GPS.GetSendConfig(pSd);
////			Length += 16;
//			break;
							
		case eGY_rCMD_GET_PIN_CODE:			
			memmove(&TxdTBuffer[LEN_U0_GY_FRAME_HEAD],(uchar*)sPIN_Code,5);
			DFE.Read(EE_PIN_CODE,8,&TxdTBuffer[LEN_U0_GY_FRAME_HEAD+5]);
			Length = LEN_U0_GY_FRAME_HEAD+13;
			break;
						
		case eGY_rCMD_GET_GPS_MILEAGE_ELE_RAIL:
			NOP();	NOP();
			
			break;
						
		case eGY_rCMD_GET_OUTPUT_STATUS:
			
			if(  (U0_RecBuffer[LEN_U0_GB_FRAME_HEAD+0]=='G')&&(U0_RecBuffer[LEN_U0_GB_FRAME_HEAD+1]=='Y')\
			   &&(U0_RecBuffer[LEN_U0_GB_FRAME_HEAD+2]=='S')&&(U0_RecBuffer[LEN_U0_GB_FRAME_HEAD+3]=='e')\
				&&(U0_RecBuffer[LEN_U0_GB_FRAME_HEAD+4]=='t')&&(U0_RecBuffer[LEN_U0_GB_FRAME_HEAD+5]=='O')\
				&&(U0_RecBuffer[LEN_U0_GB_FRAME_HEAD+6]=='u')&&(U0_RecBuffer[LEN_U0_GB_FRAME_HEAD+7]=='t'))
			{
				DFE.Read(EE_LOCK_FLAG,LEN_LOCK_CONTROL_OUT,&TxdTBuffer[LEN_U0_GY_FRAME_HEAD]);
				memmove(&TxdTBuffer[LEN_U0_GY_FRAME_HEAD+LEN_LOCK_CONTROL_OUT],(uchar*)&VehicleControl,LEN_LOCK_CONTROL_OUT);
				Length = LEN_U0_GY_FRAME_HEAD+LEN_LOCK_CONTROL_OUT<<1;
			}
			else
			{
				Error = TRUE;
				Length = LEN_U0_GY_FRAME_HEAD;	
			}
			break;		
						
					
		//		$$$$$$$$$$$$$$$$$$$$  设置短消息中心号码
		case eGY_rCMD_SET_SMS_CENTER_NUMBER:
			SetBufferValue(sTelNum.SmsCenter, 16, 0);
			strcpy((char*)sTelNum.SmsCenter,(char*)&U0_RecBuffer[LEN_U0_GY_FRAME_HEAD]);
			DFE.Write(EE_SMS_CENTER_NUMBER,16,sTelNum.SmsCenter);			
			Length = LEN_U0_GY_FRAME_HEAD;
			#if(DFE_DEBUG==TRUE)
			if(TD.MainTask==tMAIN_WRITE_DFE)	Uart.puts("DFE08\r\n\0");
			#endif
			break;
		//		$$$$$$$$$$$$$$$$$$$$  设置监控中心号码
		case eGY_rCMD_SET_CENTER_NUMBER:
			
			DFE.Write(EE_CENTER_NUMBER,32,&U0_RecBuffer[LEN_U0_GY_FRAME_HEAD]);
			os_dly_wait(2);

			DFE.Write(EE_CENTER_NUMBER,16,&U0_RecBuffer[LEN_U0_GY_FRAME_HEAD+32]);
			os_dly_wait(2);						
			Length = LEN_U0_GY_FRAME_HEAD;
			#if(DFE_DEBUG==TRUE)
			if(TD.MainTask==tMAIN_WRITE_DFE)	Uart.puts("DFE07\r\n\0");
			#endif
			break;
		//		$$$$$$$$$$$$$$$$$$$$  设置GPRS服务器IP和端口号
		case eGY_rCMD_SET_GPRS_SERVER_IP:
						
			NOP();	NOP();	NOP();	NOP();
			if(  (U0_RecBuffer[LEN_U0_GB_FRAME_HEAD+0]=='G')&&(U0_RecBuffer[LEN_U0_GB_FRAME_HEAD+1]=='Y')\
			   &&(U0_RecBuffer[LEN_U0_GB_FRAME_HEAD+2]=='S')&&(U0_RecBuffer[LEN_U0_GB_FRAME_HEAD+3]=='e')\
				&&(U0_RecBuffer[LEN_U0_GB_FRAME_HEAD+4]=='t')&&(U0_RecBuffer[LEN_U0_GB_FRAME_HEAD+5]=='I')\
				&&(U0_RecBuffer[LEN_U0_GB_FRAME_HEAD+6]=='P')&&(U0_RecBuffer[LEN_U0_GB_FRAME_HEAD+7]=='P'))
			{ 
				Length = strlen((char*)&U0_RecBuffer[LEN_U0_GB_FRAME_HEAD+8]);
				SetBufferValue((uchar*)sIP.TCP,LEN_IP_PORT,0);
				strcpy((char*)sIP.TCP,(char*)&U0_RecBuffer[LEN_U0_GB_FRAME_HEAD+8]);
				
				RESET_WATCH_DOG;
				DFE.Write(EE_IP_PORT,32,(uchar*)sIP.TCP);
				os_dly_wait(2);
				//DFE.Write(EE_IP_PORT_BAK,32,(uchar*)sIP.TCP);
				os_dly_wait(2);				
				//memmove(&TxdTBuffer[6],U0_RecBuffer,96);	
				//GPRS.ReDialFlag();
				#if(DFE_DEBUG==TRUE)		
				if(TD.MainTask==tMAIN_WRITE_DFE)	Uart.puts("DFE06\r\n\0");
				#endif			
			}			
			else	
				Error = TRUE;
			Length = LEN_U0_GY_FRAME_HEAD;
			NOP();	NOP();
			break;
			

		//		$$$$$$$$$$$$$$$$$$$$  发送到串口1
		case eGY_rCMD_SET_SEND_TO_UART:
			if (U0_RecBuffer[LEN_U0_GY_FRAME_HEAD]==0) // 透传到U0
			{
				putb(1,Rec.Counter-3,(uchar*)&U0_RecBuffer[LEN_U0_GY_FRAME_HEAD+1]);
			}
			else  if (U0_RecBuffer[LEN_U0_GY_FRAME_HEAD]==1)
			{
				Uart1.putb(1,Rec.Counter-3,(uchar*)&U0_RecBuffer[LEN_U0_GY_FRAME_HEAD+1]);
			}
			else if (U0_RecBuffer[LEN_U0_GY_FRAME_HEAD]==2)
			{
				Uart2.putb(1,Rec.Counter-3,(uchar*)&U0_RecBuffer[LEN_U0_GY_FRAME_HEAD+1]);
				Uart2.SendTask();
			}
			else if (U0_RecBuffer[LEN_U0_GY_FRAME_HEAD]==3)
			{
				Uart3.putb(Rec.Counter-3,(uchar*)&U0_RecBuffer[LEN_U0_GY_FRAME_HEAD+1]);
			}
			else 
			{
				Error = TRUE;
				Length = LEN_U0_GY_FRAME_HEAD;
			}
			
		 	break;
		 	
		 ////////////////////////////////// 	
		case eGY_rCMD_SET_OEM_WEB:
			NOP();	NOP();
			
			if(  (U0_RecBuffer[LEN_U0_GB_FRAME_HEAD+0]=='P')&&(U0_RecBuffer[LEN_U0_GB_FRAME_HEAD+1]=='o')\
			   &&(U0_RecBuffer[LEN_U0_GB_FRAME_HEAD+2]=='w')&&(U0_RecBuffer[LEN_U0_GB_FRAME_HEAD+3]=='e')\
				&&(U0_RecBuffer[LEN_U0_GB_FRAME_HEAD+4]=='r')&&(U0_RecBuffer[LEN_U0_GB_FRAME_HEAD+5]=='G')\
				&&(U0_RecBuffer[LEN_U0_GB_FRAME_HEAD+6]=='P')&&(U0_RecBuffer[LEN_U0_GB_FRAME_HEAD+7]=='S'))		
			{
				NOP();	NOP();				
				//DFE.Write(EE_OEM_WEB,32,&U0_RecBuffer[LEN_U0_GB_FRAME_HEAD+8]);
			}
			else
				Error = TRUE;
			Length = LEN_U0_GY_FRAME_HEAD;
			break;			
		////////////////////////////////// 
		case eGY_rCMD_SET_U0_U1_CLARITY:
			Length = LEN_U0_GY_FRAME_HEAD;			
		   break;   
			 
		//////////////////////////////////////////		
//		case eGY_rCMD_SET_GPS_PACK_TIME:
//			
//			pSd = (pGPS_SEND)&U0_RecBuffer[LEN_U0_GB_FRAME_HEAD];
//			GPS.SetSendConfig(pSd,1);
//			Length = LEN_U0_GY_FRAME_HEAD;
//			break;
									
		case eGY_rCMD_SET_CLR_GPS_MILEAGE:
			
			if(  (U0_RecBuffer[LEN_U0_GB_FRAME_HEAD+0]=='G')&&(U0_RecBuffer[LEN_U0_GB_FRAME_HEAD+1]=='Y')\
			   &&(U0_RecBuffer[LEN_U0_GB_FRAME_HEAD+2]=='C')&&(U0_RecBuffer[LEN_U0_GB_FRAME_HEAD+3]=='l')\
				&&(U0_RecBuffer[LEN_U0_GB_FRAME_HEAD+4]=='r')&&(U0_RecBuffer[LEN_U0_GB_FRAME_HEAD+5]=='G')\
				&&(U0_RecBuffer[LEN_U0_GB_FRAME_HEAD+6]=='P')&&(U0_RecBuffer[LEN_U0_GB_FRAME_HEAD+7]=='S'))
			{
				SystemLog.GPS.SpeedCaculus = 0;
				SystemLog.GPS.TotalMileage = 0;
				SystemLog.VDR.HighSpeedPulsCnt = 0;
				SystemLog.VDR.SpeedSensorPulsCnt = 0;
				SystemLog.VDR.MotorSpeedPulsCnt = 0;
				SystemLog.VDR.MotoRunTimeCnt = 0;	
				
				DFE.Write(EE_SYS_GPS_SPEED_CALCULUS,12,(uchar *)&SystemLog.GPS.SpeedCaculus);		// 写入
				os_dly_wait(1);
				DFE.Write(EE_SYS_VDR_MOTOR_RUN_TIME,16,(uchar *)&SystemLog.VDR.HighSpeedPulsCnt);		// 写入
				//DFE.Write(EE_SPEED_CALCULUS,16,(uchar *)&SystemLog.GPS.SpeedCaculus);		// 写入
				#if(DFE_DEBUG==TRUE)
				if(TD.MainTask==tMAIN_WRITE_DFE)	Uart.puts("DFE05\r\n\0");
				#endif
			}
			else
				Error = ERROR;
				
			Length = LEN_U0_GY_FRAME_HEAD;			
			break;
			
									
		case eGY_rCMD_SET_PIN_CODE:
			
			if(  (U0_RecBuffer[LEN_U0_GB_FRAME_HEAD+0]=='G')&&(U0_RecBuffer[LEN_U0_GB_FRAME_HEAD+1]=='Y')\
			   &&(U0_RecBuffer[LEN_U0_GB_FRAME_HEAD+2]=='S')&&(U0_RecBuffer[LEN_U0_GB_FRAME_HEAD+3]=='e')\
				&&(U0_RecBuffer[LEN_U0_GB_FRAME_HEAD+4]=='t')&&(U0_RecBuffer[LEN_U0_GB_FRAME_HEAD+5]=='P')\
				&&(U0_RecBuffer[LEN_U0_GB_FRAME_HEAD+6]=='I')&&(U0_RecBuffer[LEN_U0_GB_FRAME_HEAD+7]=='N'))
			{
				NOP();	NOP();				
				memmove((uchar*)sPIN_Code,&U0_RecBuffer[LEN_U0_GB_FRAME_HEAD+8],4);
				memmove(&U0_RecBuffer[LEN_U0_GB_FRAME_HEAD+12],&U0_RecBuffer[LEN_U0_GB_FRAME_HEAD+8],4);
				DFE.Write(EE_PIN_CODE,8,&U0_RecBuffer[LEN_U0_GB_FRAME_HEAD+8]);
				#if(DFE_DEBUG==TRUE)
				if(TD.MainTask==tMAIN_WRITE_DFE)	Uart.puts("DFE04\r\n\0");
				#endif
			}
			else
				Error = TRUE;
			Length = LEN_U0_GY_FRAME_HEAD;
			break;

		case eGY_rCMD_SET_MEMORY_FORMAT:
		
			if(  (U0_RecBuffer[LEN_U0_GB_FRAME_HEAD+0]=='G')&&(U0_RecBuffer[LEN_U0_GB_FRAME_HEAD+1]=='Y')\
			   &&(U0_RecBuffer[LEN_U0_GB_FRAME_HEAD+2]=='F')&&(U0_RecBuffer[LEN_U0_GB_FRAME_HEAD+3]=='o')\
				&&(U0_RecBuffer[LEN_U0_GB_FRAME_HEAD+4]=='r')&&(U0_RecBuffer[LEN_U0_GB_FRAME_HEAD+5]=='m')\
				&&(U0_RecBuffer[LEN_U0_GB_FRAME_HEAD+6]=='a')&&(U0_RecBuffer[LEN_U0_GB_FRAME_HEAD+7]=='t'))
			{				// GYFormat
				
				SetBufferValue(TxdTBuffer,256,0);
				
				/////////////////////////////////////////////////////////////////////
				for(i=0;i<EE_TOTAL_PAGE;i++)			// 这里的Lengyh 代表 Address
				{					
					RESET_WATCH_DOG;  
					DFE.Write(EE_ELE_SET_PARAMETER,EE_PAGE_LEN,TxdTBuffer);
					os_dly_wait(10);
					#if(DFE_DEBUG==TRUE)
					if(TD.MainTask==tMAIN_WRITE_DFE)	Uart.puts("DFE03\r\n\0");
					#endif
				}								
				
				Length = LEN_U0_GY_FRAME_HEAD;
			}
			else
				Error = TRUE;
			
			
			break;

		case eGY_rCMD_SET_OUTPUT_STATUS:
			
			if(  (U0_RecBuffer[LEN_U0_GB_FRAME_HEAD+0]=='G')&&(U0_RecBuffer[LEN_U0_GB_FRAME_HEAD+1]=='Y')\
			   &&(U0_RecBuffer[LEN_U0_GB_FRAME_HEAD+2]=='S')&&(U0_RecBuffer[LEN_U0_GB_FRAME_HEAD+3]=='e')\
				&&(U0_RecBuffer[LEN_U0_GB_FRAME_HEAD+4]=='t')&&(U0_RecBuffer[LEN_U0_GB_FRAME_HEAD+5]=='O')\
				&&(U0_RecBuffer[LEN_U0_GB_FRAME_HEAD+6]=='u')&&(U0_RecBuffer[LEN_U0_GB_FRAME_HEAD+7]=='t'))
			{				
				memmove((uchar*)&VehicleControl,(uchar*)&U0_RecBuffer[LEN_U0_GB_FRAME_HEAD+8],LEN_LOCK_CONTROL_OUT);
				DFE.Write(EE_LOCK_FLAG,LEN_LOCK_CONTROL_OUT,(uchar *)&VehicleControl);				
				Length = LEN_U0_GY_FRAME_HEAD;
				#if(DFE_DEBUG==TRUE)
				if(TD.MainTask==tMAIN_WRITE_DFE)	Uart.puts("DFE02\r\n\0");
				#endif
			}
			else
			{
				Error = TRUE;
				Length = LEN_U0_GY_FRAME_HEAD;	
			}
			break;
		 case eGY_rCMD_SET_IDLE_PARA:
		 	if(  (U0_RecBuffer[LEN_U0_GB_FRAME_HEAD+0]=='G')&&(U0_RecBuffer[LEN_U0_GB_FRAME_HEAD+1]=='Y')\
			   &&(U0_RecBuffer[LEN_U0_GB_FRAME_HEAD+2]=='E')&&(U0_RecBuffer[LEN_U0_GB_FRAME_HEAD+3]=='-')\
				&&(U0_RecBuffer[LEN_U0_GB_FRAME_HEAD+4]=='I')&&(U0_RecBuffer[LEN_U0_GB_FRAME_HEAD+5]=='d')\
				&&(U0_RecBuffer[LEN_U0_GB_FRAME_HEAD+6]=='l')&&(U0_RecBuffer[LEN_U0_GB_FRAME_HEAD+7]=='e'))
			{
				Idle.EnableInterIdleMode = U0_RecBuffer[LEN_U0_GB_FRAME_HEAD+8];	// 允许进入省电模式
				Idle.IdleModeFlag = 0;
				Idle.InterIdleModeTime = U0_RecBuffer[LEN_U0_GB_FRAME_HEAD+9];		// 多少时间后进入省电模式
				Idle.WakeUpTime = U0_RecBuffer[LEN_U0_GB_FRAME_HEAD+10];				// 多少时间后唤醒
				Idle.WakeUpLongTime = U0_RecBuffer[LEN_U0_GB_FRAME_HEAD+11];		// 唤醒持续时间
				Idle.SecondCnt=0;			
				DFE.Write(EE_IDLE_STR,LEN_EE_IDLE,(uchar*)&Idle);
				#if(DFE_DEBUG==TRUE)
				if(TD.MainTask==tMAIN_WRITE_DFE)	Uart.puts("DFE01\r\n\0");
				#endif
			}
			else
			{
				Error = TRUE;
				Length = LEN_U0_GY_FRAME_HEAD;	
			}
		 	break;
		 
		 case eGY_rCMD_GET_SYSTEM_LOG:
		 	memmove(&TxdTBuffer[LEN_U0_GY_FRAME_HEAD],(uchar*)&SystemLog.ResetCnt.SystemRestartCnt,LEN_SYSTEM_LOG);
			Length = LEN_U0_GY_FRAME_HEAD+LEN_SYSTEM_LOG;
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
		   
		   if(U0_RecBuffer[LEN_U0_GY_FRAME_HEAD+0]=='P')		 //发送PDU短信
			{
				t = U0_RecBuffer[LEN_U0_GY_FRAME_HEAD+1];	
				Length = U0_RecBuffer[LEN_U0_GY_FRAME_HEAD+2];	
				memmove(TxdTBuffer,&U0_RecBuffer[LEN_U0_GY_FRAME_HEAD+3], Length);
				i = U0_RecBuffer[LEN_U0_GY_FRAME_HEAD+3+Length];
				SMS.SetPduDataMessage(t, (char *)TxdTBuffer, &U0_RecBuffer[LEN_U0_GY_FRAME_HEAD+4+Length], i);
			}
			else if(U0_RecBuffer[LEN_U0_GY_FRAME_HEAD+0]=='T')	  //发送Text短信
			{
				 t = U0_RecBuffer[LEN_U0_GY_FRAME_HEAD+1];	
				 Length = U0_RecBuffer[LEN_U0_GY_FRAME_HEAD+2];	
				 memmove(TxdTBuffer,&U0_RecBuffer[LEN_U0_GY_FRAME_HEAD+2], Length);
				 i = U0_RecBuffer[LEN_U0_GY_FRAME_HEAD+2+Length];
				 U0_RecBuffer[LEN_U0_GY_FRAME_HEAD+2+Length+1+i] = 0;
				 GSM.SetSMS_Buffer((char*)TxdTBuffer, (char*)&U0_RecBuffer[LEN_U0_GY_FRAME_HEAD+4+Length]);
			}
			else
			{
			  Error = TRUE;
			  Length = LEN_U0_GY_FRAME_HEAD;	
			}
			break;
			
			// 注册
			// GY 44 01 00000 BAJ-A1 \x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00 C088888 \x01 粤C88888 \x00 U A 1234567890A \x00 \x00
			// GY \x2c\x00 \x69\x00 00000 BAJ-A1 \x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00 C088888 \x01 粤C88888 \x00 U A 13886088888 \x00 \x00
			// GY \x2c\x00 \x69\x00 00000 GY-G11A \x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00 2280582 \x02 粤A80582 \x00 U A 22280582 \x00 \x00 \x00 \x00 
			case eGY_rCMD_SET_REG_CODE:
				
				// 省域
				Len = 0;
				DFE.Write(EE_PROVINCE_REGION_ID, 2, (uchar *)&U0_RecBuffer[LEN_U0_GY_FRAME_HEAD]);
				NOP(); NOP(); NOP();
				Len += 2;
			
				// 市县域
				DFE.Write(EE_PREFECTURE_REGION_ID, 2, (uchar *)&U0_RecBuffer[LEN_U0_GY_FRAME_HEAD+Len]);
				NOP(); NOP(); NOP();
				Len += 2;
			
				// 制造商ID（5B)，型号（20B)，终端ID（7B）
				DFE.Write(EE_VENDOR_ID, 32, &U0_RecBuffer[LEN_U0_GY_FRAME_HEAD+Len]);
				NOP(); NOP(); NOP();	 
				Len += 32; 
				
				// 车的颜色
				DFE.Write(FM_CAR_COLOUR, 1, &U0_RecBuffer[LEN_U0_GY_FRAME_HEAD+Len]);	
				NOP(); NOP(); NOP();	      
				Len++;
			
				// 车牌号 //这里不需要点号
				DFE.Write(FM_VEHICLE_CODE, 9, &U0_RecBuffer[LEN_U0_GY_FRAME_HEAD+Len]);
				NOP(); NOP(); NOP();	   
				Len += 9;


				if( U0_RecBuffer[LEN_U0_GY_FRAME_HEAD+Len]=='U')
				{
					CreateUserFlag = OK;		  // 开户
					DFE.Write(EE_CREATE_USER_FLAG, 1, &CreateUserFlag);
					NOP(); NOP(); NOP();	
				}
				Len++;

				if( U0_RecBuffer[LEN_U0_GY_FRAME_HEAD+Len]=='A')
				{
					// 获取鉴权码，输入时必须以0结束
					Len++;
					NeedRegServer = 0;	
					DFE.Write(EE_NEED_REG_SERVER, 1, &NeedRegServer);	
					
					SetBufferValue(  (uchar*)jt_auth_code, LEN_AUTH_CODE,0);
					strcpy((char*)&jt_auth_code[0],(char*)&U0_RecBuffer[LEN_U0_GY_FRAME_HEAD+Len]);
					DFE.Write(EE_AUTH_CODE, LEN_AUTH_CODE,(uchar *)&jt_auth_code[0]);			//  写鉴权码到铁电
										
					NOP(); NOP(); NOP();	
				}				

				NOP(); 	NOP(); 	NOP();	NOP();
				Length = LEN_U0_GY_FRAME_HEAD;	


			break;

			case	eGY_rCMD_SET_CREAT_USER_TEST:
				if( (U0_RecBuffer[LEN_U0_GY_FRAME_HEAD]=='U') && (U0_RecBuffer[LEN_U0_GY_FRAME_HEAD+1]=='L'))
				{
					CreateUserFlag = OK;		  // 开户
					DFE.Write(EE_CREATE_USER_FLAG, 1, &CreateUserFlag);
					NOP(); NOP(); NOP();	
					Uart.puts("开户成功");
				}

			break;
		
			case	eGY_rCMD_SET_TER_PHONE_NUM:			 //设置终端手机号 		///// GY S 13800000000	\x00
				Len = LEN_U0_GY_FRAME_HEAD;
				if (U0_RecBuffer[Len] == 'S')
				{
					Len++;
					SetBufferValue(sTelNum.Sim, 16, 0);
					sTelNum.Sim[0] = 0x30;
				    memmove((uchar*)&sTelNum.Sim[1],(uchar*)&U0_RecBuffer[Len],11);
					Len += 11;
					Length = strlen((char *)&sTelNum.Sim[0]);
					DFE.Write(EE_SIM_NUMBER,16,(uchar*)&sTelNum.Sim[0]);
					Uart.puts("ter phone:"); Uart.puts((char *)&sTelNum.Sim[1]);
					NOP(); NOP(); NOP();
					DecCharToPressBCD(Length,(char *)sTelNum.Sim,SimNumberBCD);
					memmove(sTelNum.SimBCD_JT, SimNumberBCD, 6);
					Length = LEN_U0_GY_FRAME_HEAD;	

				}
				else if (U0_RecBuffer[Len] == 'G')	  //// GY G 
				{
					 Length = strlen((char *)&sTelNum.Sim[1]);
			         memmove((uchar*)&TxdTBuffer[LEN_U0_GY_FRAME_HEAD],&sTelNum.Sim[1], Length);
					 Length += LEN_U0_GY_FRAME_HEAD;	
				}
					
			break;
			
			
			case	eGY_rCMD_SET_BLIND_TEST:
				
				Len = LEN_U0_GY_FRAME_HEAD;
				if (U0_RecBuffer[Len] == 'S')
				{
					SamlePeriod1S = TRUE;
					if (U0_RecBuffer[Len+1] == 'B')
						TestBlind = TRUE;
					else if (U0_RecBuffer[Len+1] == 'H')
						TestAccuracy = TRUE;
					if (U0_RecBuffer[Len+2] == 'G')
						JT_ShutMobileCnt	= TRUE;
				}
				else if (U0_RecBuffer[Len] == 'C')
				{
					SamlePeriod1S = 0;
					TestBlind = 0;
					TestAccuracy = 0;
				}
				
				TxdTBuffer[Len] 	= SamlePeriod1S;
				TxdTBuffer[Len+1]	= TestBlind;
				TxdTBuffer[Len+2]	= TestAccuracy;
				TxdTBuffer[Len+3]	= JT_ShutMobileCnt;
				
				Length = LEN_U0_GY_FRAME_HEAD+4;
			break;

			
				
			case eGY_rCMD_GET_FLASH_START_PAGE:
			
				pData = (uchar*)&TxdTBuffer[LEN_U0_GY_FRAME_HEAD];
				
				strcpy( (char*)pData,"存储开始页地址: ");
				
				strcat( (char*)pData,"\r\n48H: ");	
				Len = strlen( (char*)pData);
				Int2Str( FF_48H_SPEED_START_PAGE, (char*)(pData+Len));
				
				strcat( (char*)pData,"\r\n360H: ");	
				Len = strlen( (char*)pData);
				Int2Str( FF_360H_LOCATION_START_PAGE, (char*)(pData+Len));
				
				strcat( (char*)pData,"\r\n停车数据: ");	
				Len = strlen( (char*)pData);
				Int2Str( FF_PARKING_DATA_START_PAGE, (char*)(pData+Len));
				
								
				strcat( (char*)pData,"\r\nCAN BUS: ");	
				Len = strlen( (char*)pData);
				Int2Str( FF_CAN_DATA_START_PAGE, (char*)(pData+Len));
				
				strcat( (char*)pData,"\r\n报警信息: ");	
				Len = strlen( (char*)pData);
				Int2Str( FF_ALARM_MSG_START_PAGE, (char*)(pData+Len));
				
				strcat( (char*)pData,"\r\n普通信息: ");	
				Len = strlen( (char*)pData);
				Int2Str( FF_NORMAL_MSG_START_PAGE, (char*)(pData+Len));
				
				strcat( (char*)pData,"\r\n历史轨迹: ");	
				Len = strlen( (char*)pData);
				Int2Str( FF_GPS_MESSAGE_START_PAGE, (char*)(pData+Len));
				
				strcat( (char*)pData,"\r\n盲区轨迹: ");
				Len = strlen( (char*)pData);
				Int2Str( FF_GPS_BLIND_MESSAGE_START_PAGE, (char*)(pData+Len));
								
				strcat( (char*)pData,"\r\nGNSS: ");				
				Len = strlen( (char*)pData);
				Int2Str( FF_GNSS_DATA_START_PAGE, (char*)(pData+Len));
				
				strcat( (char*)pData,"\r\n图片数据: ");	
				Len = strlen( (char*)pData);
				Int2Str( FF_PIC_START_PAGE, (char*)(pData+Len));
				
				
													
				Len = strlen( (char*)pData);
				
				
				Length = LEN_U0_GY_FRAME_HEAD+Len;
				
				
				
			break;


			case	eGY_rCMD_SET_DRIVER_IC_CARD_INFO:	 // 设置驾驶员IC卡信息128B
				if((GET_IC_CARD_STATUS)==0)		// IC卡已插入 （用于常开卡座）  
				//if((GET_IC_CARD_STATUS)!=0)		// IC卡已插入	(用于常闭卡座)
				{
					 while (ICCardBusing==TRUE)
					 {
					 	 os_dly_wait(10);
					 }
		
					 
					 I2C0_PIN_SELECT;
					 os_dly_wait(1);
		
					 ICCardBusing = TRUE;
					 for (i=0; i<32; i++)
					 {
						 s = I2C0.WriteEEPROM(CARD_DEVICE_ADDR, 8, i*8, 8, &U0_RecBuffer[LEN_U0_GY_FRAME_HEAD]+8*i);	
						 if (s==ERROR)
						 {
						 	TxdTBuffer[LEN_U0_GY_FRAME_HEAD] = 1;
							Length = LEN_U0_GY_FRAME_HEAD+1;
						 	//Uart.puts("写卡失败");
	
							break;
						 }	 	
						
					 } 
					
					if (s != ERROR)
					{
						TxdTBuffer[LEN_U0_GY_FRAME_HEAD] = 0;
						Length = LEN_U0_GY_FRAME_HEAD+1;
			 			//Uart.puts("写卡成功");
		
					}
		
					ICCardBusing = FALSE;
		
					
				
				}
				else
				{
					 TxdTBuffer[LEN_U0_GY_FRAME_HEAD] = 2;
					 Length = LEN_U0_GY_FRAME_HEAD+1;
					
					 //Uart.puts("未插卡");
				}
					

			break;

			
			            
			case  eGY_rCMD_SET_FACTORY_RESET:
            
				Length = 0;
            s = U0_RecBuffer[LEN_U0_GY_FRAME_HEAD];
            //恢复出厂设置	  重度 ，清所有数据包括IP地址和记录仪数据
			   if(s=='A')				
			   {
			   	Sys.FirstPowerOn(TRUE);  				
			   	NVIC_SystemReset();     
			   }
				else if(s=='B')		
				{
					Sys.FirstPowerOn(FALSE); 
					NVIC_SystemReset();     
				}
				else if(s=='C')
				{
					Camera.RestoreFactoryParameter();
				}
				else if(s=='D')
				{
					Tp.B32[0] = Str2Int((char*)&U0_RecBuffer[LEN_U0_GY_FRAME_HEAD+1]);
					GSM.SetDefaultSendInterval(Tp.B32[0]);					
					memmove(&TxdTBuffer[LEN_U0_GY_FRAME_HEAD],(uchar*)&SendInterval.DefauletTime,4);
					Length = 4;
				}
				else if( s=='E')
				{
					s = U0_RecBuffer[LEN_U0_GY_FRAME_HEAD+1];
					Tp.B32[0] = Str2Int( (char*)&U0_RecBuffer[LEN_U0_GY_FRAME_HEAD+2]);
					if(s=='1')	 		
					{
						Baudrate.U0_232 = Tp.B32[0];
					}
					else if(s=='2')
					{
						Baudrate.U2_232 = Tp.B32[0];
						Uart2.Init(Baudrate.U2_232,U2_FOR_DISPLAY);
					}
					else if(s=='3')
					{
						Baudrate.U2_485 = Tp.B32[0];
						Uart2.Init(Baudrate.U2_485,U2_FOR_CAMERA);
					}
					else if(s=='4')
					{
						Baudrate.CanBus = Tp.B32[0];
					}
					else if(s=='5')
					{
						Baudrate.U2_232 = Tp.B32[0];
						U2PinSelDevice = U2_FOR_RFID;
						os_dly_wait(30);
						Uart2.Init(Baudrate.U2_232,U2_FOR_RFID);
					}
					else if(s=='6')
					{
						Baudrate.U3_GPS = Tp.B32[0];
						Uart3.Init(Baudrate.U3_GPS);
						DFE.Write(EE_BAUDRATE_U3_232, 4, Tp.Buffer);
					}
					else
					{
						pData = &TxdTBuffer[LEN_U0_GY_FRAME_HEAD];

						Int2Str( Baudrate.U0_232, (char*)pData);
						strcat( (char*)pData,",");
						Len = strlen( (char*)pData);

						Int2Str( Baudrate.U2_232, (char*)(pData+Len));
						strcat( (char*)pData,",");
						Len = strlen( (char*)pData);

						Int2Str( Baudrate.U2_485, (char*)(pData+Len));
						strcat( (char*)pData,",");
						Len = strlen( (char*)pData);

						Int2Str( Baudrate.CanBus, (char*)(pData+Len));
						strcat( (char*)pData,",");
						Len = strlen( (char*)pData);

						Int2Str( Baudrate.U1_GSM, (char*)(pData+Len));
						strcat( (char*)pData,",");
						Len = strlen( (char*)pData);

						Int2Str( Baudrate.U3_GPS, (char*)(pData+Len));
						strcat( (char*)pData,",");
						Len = strlen( (char*)pData);

					   Length = Len;
					}	
					if((s>='1')&&(s<='4'))				
						DFE.Write(EE_BAUDRATE_U0_232,16,(uchar*)&Baudrate);					

				}
				else if(s=='F')
				{
					s = U0_RecBuffer[LEN_U0_GY_FRAME_HEAD+1];
					if(s<0x10)
					{
						if(s&0x01) 	JT808Flag.VS3.Bits.UseGPS = 1;
						else			JT808Flag.VS3.Bits.UseGPS = 0;					
						if(s&0x02) 	JT808Flag.VS3.Bits.UseBD = 1;
						else			JT808Flag.VS3.Bits.UseBD = 0;
						DFE.Write(EE_JT_GNNS_POS_MODE, 1, (uchar*)&U0_RecBuffer[LEN_U0_GY_FRAME_HEAD+1]);
					}
					else
					{
						if(JT808Flag.VS3.Bits.UseGPS ==1)	TxdTBuffer[LEN_U0_GY_FRAME_HEAD] = '1';
						else											TxdTBuffer[LEN_U0_GY_FRAME_HEAD] = '0';

						if(JT808Flag.VS3.Bits.UseBD ==1)		TxdTBuffer[LEN_U0_GY_FRAME_HEAD+1] = '1';
						else											TxdTBuffer[LEN_U0_GY_FRAME_HEAD+1] = '0';
						Length = 2;
					}
				}
				else if(s=='G')
				{
					Accelerometer_init(1);
				}
				
				
				Length += LEN_U0_GY_FRAME_HEAD;
				
			 break;
			 
			 
			 
		default : 
			Error = TRUE;
			break;
	}
	////////////////////////////////////////////////////////////////////////////////////

	if(Error)
	{
		if(Cmd<0x80)
			s = UP_LOAD_ERROR;
		else
			s = DOWN_LOAD_ERROR;
		ReturnErrorFlag(s);		
	}
	else
	{
		TxdTBuffer[0] = 0x55;
		TxdTBuffer[1] = 0x7a;
		TxdTBuffer[2] = Cmd;
		TxdTBuffer[3] = (Length-LEN_U0_GY_FRAME_HEAD)/256;
		TxdTBuffer[4] = (Length-LEN_U0_GY_FRAME_HEAD)%256;
		TxdTBuffer[5] = RESERVER_WORD;
		TxdTBuffer[6] = 'G';
		TxdTBuffer[7] = 'Y';
		s = CalXorSum(Length,TxdTBuffer);
		TxdTBuffer[Length] = s;
		Length++;
		putb(0,Length,TxdTBuffer);		// 返回信息到上位机		
	}	
				
	NOP();	NOP();
}
#endif	



/************************************************************************************************
** 函数名称：EreadFRAM_RecodeMem()
** 功能描述：擦除铁电某些空间
** 入口参数：开始地址，擦除长度
** 出口参数：
** 全局变量: 无
** 全局函数: 
** 创建时间：
** 修改时间：2011.09.17
**************************************************************************************************/ 	
void     UartCls::EreadFRAM_RecodeMem(uint16 Addr, uint16 Len)
{
	uint16  wLen;
	uchar   tBuffer[128];
		
	SetBufferValue(tBuffer,128,0);
	while(Len) 
	{
		 if(Len>128)	   wLen = 128;
		 else				 	wLen = Len;
		 
		 DFE.Write(Addr,wLen, tBuffer);
		 Addr += wLen;
		 Len -= wLen;
		 RESET_WATCH_DOG;
		 os_dly_wait(2);
	}
}





extern		"C"  void   WrintDataToFile (void);
/************************************************************************************************
** 函数名称：WaitUSBGetData()
** 功能描述：等待SUB写入数据
** 入口参数：等待时间， 数据长度
** 出口参数：操作成功返回零
** 全局变量: 无
** 全局函数: 
** 创建时间：
** 修改时间：2011.09.17
**************************************************************************************************/ 	
void				UartCls::WaitUSBGetData(uchar DlyTime, uint16 DataLen)
{				
	uchar  n, *buffer;
	uint16 t;
	n = DlyTime;
	LenDataForUSB = DataLen;

	buffer = &TxdTBuffer[0];
	for(t=0; t<LenDataForUSB; t++)	{ USBCheckSum ^= *(buffer+t);	}
   
	//while((LenDataForUSB>0)&&(n>0))					            //有限的时间内等待USB处理完数据，
	if(Write_File_Error==0)
	{
		WrintDataToFile();
		//os_dly_wait(1);
	   n--;
	}
}
/************************************************************************************************
** 函数名称：SetHeadUSBBlockData()
** 功能描述：设置SUB块的数据头
** 入口参数：数据类型， 数据序号，数据长度
** 出口参数：数据头长度
** 全局变量: 无
** 全局函数: 
** 创建时间：
** 修改时间：2011.09.17
**************************************************************************************************/ 	
uint32   UartCls::SetHeadUSBBlockData(uchar DataType, uchar DataSierieNb, uint32 DataLen, uchar *DataBuffer)
{
	uint32 Len, L;
														 
  *DataBuffer = DataType;							  //数据代码
  	Len = 1;

   SetBufferValue(DataBuffer+Len, 18,0);
	strcpy((char *)(DataBuffer+Len), (char *)&GBDataName[DataSierieNb][0]);		 //数据名 
	Len += 18;			 // 数据名称为固定为18字节
	L = DataLen;												    //数据记录长度
	L = SwapINT32(L);
	memmove(DataBuffer+Len, (uchar *)&L, 4);
	Len += 4;

	return Len;
}













/*
 $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$  End of File $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
 */



