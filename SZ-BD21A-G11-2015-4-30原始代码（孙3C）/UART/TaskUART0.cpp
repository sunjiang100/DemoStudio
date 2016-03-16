/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$  Copyright (c)  $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
$												 
$										 
$                       	 		  
$								  		All rights reserved.	  
$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$  Copyright (c)  $$$$$$$$$$$$$$$$$$$$$$$$$$$ */
/*  
                                      �ļ�����
	�ļ���: 
	����������
	���뻷����
	Ŀ��CPU: 
	���ߣ�
	����ʱ�䣺
	����༭������UltraEdit 11.00a+  �Ʊ�����Ϊ�����ַ�������ɶ��Ը��á�
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

///////////////////////   ��¼�������� ////////////////////////////////////////
//UART0_EXT	uchar		const		GBDataName[][20] = 
//{ 
//{"ִ�б�׼�汾��"},
//{"��ǰ��ʻԱ��Ϣ"},
//{"ʵʱʱ��"},
//{"�ۼ���ʻ���"},
//{"�ٶ�����ϵ��"},
//{"������Ϣ"},
//{"��¼��Ψһ�Ա��"},
//{"״̬�ź�������Ϣ"},
//{"ÿ����ƽ���ٶȼ�¼"},
//{"ÿ����ƽ���ٶȼ�¼"},
//{"λ����Ϣ��¼"},
//{"�¹��ɵ��¼"},
//{"ƣ�ͼ�ʻ��¼"},
//{"��¼�˳���¼"},
//{"�ⲿ�����¼"},
//{"�����޸ļ�¼"},
//{"��¼�ǰ�װ����"},
//{"��λ���澯��ز���"},
//{"����Χ������"},
//{"����Χ��Ŀ¼"},
//{"��·�滮����"},
//{"��·�滮Ŀ¼"},
//{"\0"},
//};

/////////////////////   ��¼�������� ////////////////////////////////////////
UART0_EXT	uchar		const		GBDataName[][20] = 
{ 
{"ִ�б�׼�汾���"},		
{"��ǰ��ʻ����Ϣ"},
{"ʵʱʱ��"},
{"�ۼ���ʻ���"},
{"����ϵ��"},
{"������Ϣ"},
{"״̬�ź�������Ϣ"},
{"��¼��Ψһ�Ա��"},
{"��ʻ�ٶȼ�¼"},
{"λ����Ϣ��¼"},
{"�¹��ɵ��¼"},
{"��ʱ��ʻ��¼"},
{"��ʻ����ݼ�¼"},
{"�ⲿ�����¼"},
{"�����޸ļ�¼"},
{"�ٶ�״̬��־"},
{"\0"},
};

////////////	ͨѶ�����־	///////////////////////////////////////////////////////
#define			UP_LOAD_ERROR					0xfa	// �ϴ������־��
#define			DOWN_LOAD_ERROR				0xfb	// �´������־��
#define			XOR_SUM_ERROR					0xff	// ����ʹ���

#define	GB_LARGE_DATA_PACK_BLOCK_LEN		512

extern		"C"  unsigned long 		SwapINT32(unsigned long dData);
static    uchar      CheckSum;
static    uchar      USBCheckSum;



extern	char		jt_auth_code[];		// ��Ȩ��

//static	 uchar		TxdTBuffer[260+16];
//static  uchar  USB_GBDataBuffer[512]
static char USB_GetGBdata_PartFun(uchar Cmd, uint32 Time, uint32 MaxDataLen, uint32 DataHeadLen)		  // USB�ɼ����ֹ��ܺ��� 
{
	uchar i;
	uint16 Len = 0,TotalItem;
	uint32 DataBlockTotalLen = 0, L;
	uint32 StartSecond, EndSecond;
	uchar tBuffer[32];

	Time = 24*3600*30UL;  // 	�������Ҫ��ȡ��������, �����ȡһ�µ����ݣ���ʵû�д���ô�ࣩ

	EndSecond = Rtc.GetRtcSecond();
	StartSecond = EndSecond - Time;				 

	Rtc.SecondToDateTime_BCD(StartSecond,(RTC_STR *)tBuffer);
	memmove(GB_LargeData.StartTime,tBuffer,6);	  // ��ʼʱ��  bcd 


	Rtc.SecondToDateTime_BCD(EndSecond,(RTC_STR *)tBuffer);
	memmove(GB_LargeData.EndTime,tBuffer,6);	  // ����ʱ��	  bcd
	GB_LargeData.Cmd = Cmd; 
	TotalItem  = 0xffff;//GB_LargeData.TotalItem;	 // �������Ҫ��ȡ��������	���������ﲻ�������ݿ���	2015.3.9
	

	//Len = SectionLenPos+4;  // ���ݶο�ʼ��λ��Ϊ ���ݶγ���λ�ü�4
	// ��������ѭ������һ��ѭ���ǵõ����ݶε��ܳ���, ������ͷ��д��USB ���ڶ���ѭ���ǰ����ݶ�д��USB 

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
			if (L)  //  ���ݳ��Ȳ�Ϊ0
			{

				//if (DataBlockTotalLen>MaxDataLen)	break;	 // �������Ҫ��ȡ��������	���������ﲻ�����ܳ�
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
			 
			 memmove(&TxdTBuffer[DataHeadLen-4],(uchar *)&DataBlockTotalLen, 4); // ����ʵ�����ݶε��ܳ� 

			  //Uart.puts("head Test Data:"); Uart.putb(1,DataHeadLen,TxdTBuffer); Uart.puts("\r\n"); os_dly_wait(10);
			 // д�����ݿ�ͷ����Ϣ  �� ���ݴ��룬�������ƣ����ݳ���
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
	t = USBGBDATANUMBER;								 //USB���ݿ����
	t = SwapINT16(t);
	SetBufferValue(&TxdTBuffer[0],1024,0);
	memmove(&TxdTBuffer[0], (uchar *)&t, 2);	
	Len = 2;					  
	/////ִ�еı�׼�汾��///////////////////
	L = Uart.SetHeadUSBBlockData(eGB_rCMD_GET_STD_VERSION, eGB_GET_STD_VERSION, LEN_GPS_Standard, (uchar *)&TxdTBuffer[2]);	 //USB���ݿ�ͷ		 
	Len += L;
	Uart.SetGB_Down_CMDHead(DataBuffer, 0, eGB_rCMD_GET_STD_VERSION);
	L = Uart.GB19056Process(LEN_U0_GB_FRAME_HEAD,DataBuffer,TxdTBuffer+Len);
	L -= LEN_U0_GB_FRAME_HEAD;
	memmove(TxdTBuffer+Len, TxdTBuffer+Len+LEN_U0_GB_FRAME_HEAD, L); // ȥ��ͷ��6���ֽ�
	Len += L;


	/////��ǰ��ʻԱ��Ϣ///////////////////
	L = Uart.SetHeadUSBBlockData(eGB_rCMD_GET_DRIVER_CODE, eGB_GET_DRIVER_CODE, LEN_DRIVER_CODE, (uchar *)&TxdTBuffer[Len]);	 //USB���ݿ�ͷ	
	Len += L;
	DFE.Read(FM_DRIVER_CODE, LEN_DRIVER_CODE, &TxdTBuffer[Len]);
	Len += LEN_DRIVER_CODE;


	/////ʵʱʱ��///////////////////
	L = Uart.SetHeadUSBBlockData(eGB_rCMD_GET_REAL_TIME, eGB_GET_REAL_TIME, 6, (uchar *)&TxdTBuffer[Len]);	 //USB���ݿ�ͷ
	Len += L;	
	Rtc.GetRTC_BCD((RTC_STR *)&TxdTBuffer[Len]);	
	Len += 6;

	/////�ۼ���ʻ���///////////////////
	L = Uart.SetHeadUSBBlockData(eGB_rCMD_GET_TOTAL_MILAGE, eGB_GET_TOTAL_MILAGE,LEN_MILAGE, (uchar *)&TxdTBuffer[Len]);	 //USB���ݿ�ͷ	
	Len += L;
	Uart.SetGB_Down_CMDHead(DataBuffer, 0, eGB_rCMD_GET_TOTAL_MILAGE);
	L = Uart.GB19056Process(LEN_U0_GB_FRAME_HEAD,DataBuffer,TxdTBuffer+Len);
	L -= LEN_U0_GB_FRAME_HEAD; // ȥ��ͷ��6���ֽ�
	memmove(TxdTBuffer+Len, TxdTBuffer+Len+LEN_U0_GB_FRAME_HEAD, L); // ȥ��ͷ��6���ֽ�
	Len += L;


	/////�ٶ�����ϵ��///////////////////
	L = Uart.SetHeadUSBBlockData(eGB_rCMD_GET_SPEED_KV, eGB_GET_SPEED_PULSE_COEFFICIENT, LEN_VEHICLE_KV, (uchar *)&TxdTBuffer[Len]);	 //USB���ݿ�ͷ	
	Len += L;
	Uart.SetGB_Down_CMDHead(DataBuffer, 0, eGB_rCMD_GET_SPEED_KV);
	L = Uart.GB19056Process(LEN_U0_GB_FRAME_HEAD,DataBuffer,TxdTBuffer+Len);
	L -= LEN_U0_GB_FRAME_HEAD;
	memmove(TxdTBuffer+Len, TxdTBuffer+Len+LEN_U0_GB_FRAME_HEAD, L); // ȥ��ͷ��6���ֽ�
	Len += L;

	/////������Ϣ///////////////////
	L = Uart.SetHeadUSBBlockData(eGB_rCMD_GET_VEHICLE_VIN, eGB_GET_VEHICLE_VIN, LEN_VEHICLE_VCC, (uchar *)&TxdTBuffer[Len]);	 //USB���ݿ�ͷ	
	Len += L;
	Uart.SetGB_Down_CMDHead(DataBuffer, 0, eGB_rCMD_GET_VEHICLE_VIN);
	L = Uart.GB19056Process(LEN_U0_GB_FRAME_HEAD,DataBuffer,TxdTBuffer+Len);
	L -= LEN_U0_GB_FRAME_HEAD;
	memmove(TxdTBuffer+Len, TxdTBuffer+Len+LEN_U0_GB_FRAME_HEAD, L); // ȥ��ͷ��6���ֽ�
	Len += L;

	/////״̬�ź�������Ϣ///////////////////
	L = Uart.SetHeadUSBBlockData(eGB_rCMD_GET_SIGNAL_CONFIG, eGB_GET_SIGNAL_CONFIG, LEN_SIGNAL_STATUS_1PEC, (uchar *)&TxdTBuffer[Len]);	 //USB���ݿ�ͷ	
	Len += L;
	Uart.SetGB_Down_CMDHead(DataBuffer, 0, eGB_rCMD_GET_SIGNAL_CONFIG);
	L = Uart.GB19056Process(LEN_U0_GB_FRAME_HEAD,DataBuffer,TxdTBuffer+Len);
	L -= LEN_U0_GB_FRAME_HEAD;
	memmove(TxdTBuffer+Len, TxdTBuffer+Len+LEN_U0_GB_FRAME_HEAD, L); // ȥ��ͷ��6���ֽ�
	Len += L;
		

//	/////��¼��Ψһ�Ա��///////////////////
	L = Uart.SetHeadUSBBlockData(eGB_rCMD_GET_SERIAL_NUMBER, eGB_GET_SERIAL_NUMBER, LEN_SERIAL_NUMBER, (uchar *)&TxdTBuffer[Len]);	 //USB���ݿ�ͷ	
	Len += L;
	Uart.SetGB_Down_CMDHead(DataBuffer, 0, eGB_rCMD_GET_SERIAL_NUMBER);
	L = Uart.GB19056Process(LEN_U0_GB_FRAME_HEAD,DataBuffer,TxdTBuffer+Len);
	L -= LEN_U0_GB_FRAME_HEAD;
	memmove(TxdTBuffer+Len, TxdTBuffer+Len+LEN_U0_GB_FRAME_HEAD, L); // ȥ��ͷ��6���ֽ�
	Len += L;


	Uart.WaitUSBGetData(10, (uint16)Len);										//���޵�ʱ���ڵȴ�USB���������ݣ�
	Uart.puts("USB  Wrinting");
   os_dly_wait(4);
	Len  = 0;

	// ע�����������Ҫ��U�̲ɼ������ǰѻ�����Ч������ȫ���ɼ��������µ� USB_GetGBdata_PartFun�������޸�
	//  GB_LargeData.TotalItem  ��USB_GetGBdata_PartFun �ĵڶ���ʱ�����������
	/////ÿ����ƽ���ٶ�///////////////////			48Сʱ��ʻ�ٶ�
	Len  = 0;
	GB_LargeData.TotalItem = 0xffff;//48*60;
	L = Uart.SetHeadUSBBlockData(eGB_rCMD_GET_SPEED_PER_SECOND, eGB_GET_SPEED_PER_SECOND, 0, (uchar *)&TxdTBuffer[Len]);	 //USB���ݿ�ͷ	
	if (USB_GetGBdata_PartFun(eGB_rCMD_GET_SPEED_PER_SECOND,(48*3600UL),LEN_48Hour_SPEED_DATA,L) == 1)
	{
		  	goto __OVER_USB_DATA;
	}
	//goto __OVER_USB_DATA; //	 test

	/////360Сʱλ����Ϣ///////////////////
	Len  = 0;
	GB_LargeData.TotalItem = 360;
	L = Uart.SetHeadUSBBlockData(eGB_rCMD_GET_POSITION_INFO, eGB_GET_POSITION_INFO, 0, (uchar *)&TxdTBuffer[Len]);	 //USB���ݿ�ͷ	
	if (USB_GetGBdata_PartFun(eGB_rCMD_GET_POSITION_INFO,(360*3600UL),LEN_360H_POS_DATA,L) == 1)
	{
		  	goto __OVER_USB_DATA;
	}


	/////�¹��ɵ�///////////////////
	Len  = 0;
	GB_LargeData.TotalItem = 100;
	L = Uart.SetHeadUSBBlockData(eGB_rCMD_GET_PARKING_DATA, eGB_GET_STOP_DATA, 0, (uchar *)&TxdTBuffer[Len]);	 //USB���ݿ�ͷ	
	if (USB_GetGBdata_PartFun(eGB_rCMD_GET_PARKING_DATA,(360*3600UL),LEN_STOP_DATA,L) == 1)
	{
		  	goto __OVER_USB_DATA;
	}

	/////ƣ�ͼ�ʻ///////////////////
	Len  = 0;
	GB_LargeData.TotalItem = 100;
	L = Uart.SetHeadUSBBlockData(eGB_rCMD_GET_OVER_TIME_DRIVE, eGB_GET_OVER_TIME_3H, 0, (uchar *)&TxdTBuffer[Len]);	 //USB���ݿ�ͷ	
	if (USB_GetGBdata_PartFun(eGB_rCMD_GET_OVER_TIME_DRIVE,(360*3600UL),LEN_OVER_TIME_DATA,L) == 1)
	{
		  	goto __OVER_USB_DATA;
	}

	/////��ʻԱ��¼�˳�///////////////////
	Len  = 0;
	GB_LargeData.TotalItem = 200;
	L = Uart.SetHeadUSBBlockData(eGB_rCMD_GET_GET_LOG_IN_OUT_RECORD, eGB_GET_GET_LOG_IN_OUT_RECORD, 0, (uchar *)&TxdTBuffer[Len]);	 //USB���ݿ�ͷ	
	if (USB_GetGBdata_PartFun(eGB_rCMD_GET_GET_LOG_IN_OUT_RECORD,(360*3600UL),LEN_DRIVER_OUT_IN_DATA,L) == 1)
	{
		  	goto __OVER_USB_DATA;
	}

	/////�ⲿ����///////////////////
	Len  = 0;
	GB_LargeData.TotalItem = 100;
	L = Uart.SetHeadUSBBlockData(eGB_rCMD_GET_POWER_RECORD, eGB_GET_POWER_RECORD, 0, (uchar *)&TxdTBuffer[Len]);	 //USB���ݿ�ͷ	
	if (USB_GetGBdata_PartFun(eGB_rCMD_GET_POWER_RECORD,(360*3600UL),LEN_GET_POWER_RECORD_DATA,L) == 1)
	{
		  	goto __OVER_USB_DATA;
	}

	/////�����޸�//////////////////
	Len  = 0;
	GB_LargeData.TotalItem = 100;
	L = Uart.SetHeadUSBBlockData(eGB_rCMD_GET_MODIFY_PARA_RECORD, eGB_GET_MODIFY_PARA_RECORD, 0, (uchar *)&TxdTBuffer[Len]);	 //USB���ݿ�ͷ	
	if (USB_GetGBdata_PartFun(eGB_rCMD_GET_MODIFY_PARA_RECORD,(360*3600UL),LEN_GET_MODIFY_PARA_RECORD_DATA,L) == 1)
	{
		  	goto __OVER_USB_DATA;
	}

	////// �ٶ�״̬��־ ////////////////
	Len  = 0;
	GB_LargeData.TotalItem = 10;
	L = Uart.SetHeadUSBBlockData(eGB_rCMD_GET_SPEED_STATUS_LOG, eGB_GET_SPEED_STATUS_LOG, 0, (uchar *)&TxdTBuffer[Len]);	 //USB���ݿ�ͷ	
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
	�������ƣ�SendDownCMDtoU0
   �����������·�һ�������GBָ��
   ��ڲ�����ָ���׵�ַ �� ���ݳ���
   ���ڲ�����
   ȫ�ֱ���: 
   ����ʱ�䣺
   �޸�ʱ�䣺
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
	�������ƣ�CmdForGetGBdata
   ��������������һ��GB�·�ָ�ֻ����ȡ������ݼ�¼
   ��ڲ�����ָ������ ��Ҫȡ�����ݿ����
   ���ڲ�����
   ȫ�ֱ���: 
   ����ʱ�䣺
   �޸�ʱ�䣺
*/ 	
//void      UartCls::CmdForGetGBdata(uchar  CmdType, uint16 DataBlocks)
//{	 
//	 uchar   DataBuffer[20];
//	 uint32  Len;
//	 SetGB_Down_CMDHead(U0_RecBuffer, 14, CmdType);
//	 Len = Rtc.GetRtcSecond() - 2*ONE_YEAR_SECOND;									            //��������2�������ȡ48Сʱ�ٶ�����
//	 Rtc.SecondToDateTime_BCD(Len, (RTC_STR *)DataBuffer);					               //��ʼʱ��
//	 memmove(&U0_RecBuffer[LEN_U0_GB_FRAME_HEAD], DataBuffer, 6);
//	 Rtc.GetRTC_BCD((RTC_STR *)&DataBuffer[0]);									               //����ʱ��
//	 memmove(&U0_RecBuffer[LEN_U0_GB_FRAME_HEAD+6], DataBuffer, 6);
//	 U0_RecBuffer[LEN_U0_GB_FRAME_HEAD+12] = (DataBlocks>>8)&0xff;									//48Сʱ���ݿ��� 48*60=2880=0x0B40
//	 U0_RecBuffer[LEN_U0_GB_FRAME_HEAD+13] = DataBlocks&0xff;
//	 SendDownCMDtoU0(U0_RecBuffer, LEN_U0_GB_FRAME_HEAD+14);
//	 
//
//}
/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	�������ƣ�SetGB_Down_CMDHead
   ������������������·�ָ���ͷ
   ��ڲ�����
   ���ڲ�����
   ȫ�ֱ���: 
   ����ʱ�䣺
   �޸�ʱ�䣺
*/ 	
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ ����ԭ�������ĺ���������������
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
   �������ƣ�
   ����������
   ��ڲ�����
   ���ڲ�����
   ȫ�ֱ���: 
   ����ʱ�䣺
   �޸�ʱ�䣺
*/ 	
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ ����ԭ�������ĺ���������������
//uchar		tU0Buf[32] = {0x24,0x01,0x0c,0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x30,0x28,0xff};
extern	uchar StartBDUpdate;
void			UartCls::ReceiveProcessData(void)
{
	uchar		s,Cmd;
	uint16	DataLength;
	uchar		TxdBuffer[256+4];

	//uint32 Addr;
	
    if((++Rec.OverTimeCnt != 3))		return;			 //�������ݽ�����30ms���� 

	if(Rec.Counter ==0)	return;

	/////////////////// RS232ͨѶ����Ϊÿ��ͨѶֻ��һ�����ݰ���������ճ�������
	//�������ݲ�����ʱ(�ǲ���ָ�������U0_RecBuffer�п��ܱ�ˢ��
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
	�������ƣ�
   ����������
   ��ڲ�����
   ���ڲ�����
   ȫ�ֱ���: 
   ����ʱ�䣺
   �޸�ʱ�䣺
*/ 	
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ ����ԭ�������ĺ���������������		

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
		if (pVDR->FlashPageSaveCnt>0)	//  flash��������
		{		 
			GetDataCnt = 	pVDR->FlashPageSaveCnt;	// ����һҳ
			if (GetDataCnt>pVDR->FlashTotalPages) GetDataCnt = pVDR->FlashTotalPages;	
		}
		else if (tl>0)			// ������������
		{	
			GetDataCnt = 1;
		}
	}

  	for (;GetDataCnt;GetDataCnt--)
	{	
		if ((DataLen > GB_LARGE_DATA_PACK_BLOCK_LEN)) // ÿһ���������ݶβ���̫��
		{			 	
			break;
		}
			  
		if ((GetDataCnt==pVDR->FlashPageSaveCnt) || ((pVDR->FlashPageSaveCnt==0)&&(GetDataCnt==1)))
		{	// ȡ�����������
			if (tl<pVDR->FMSaveItemCnt)  //
			{ 	  	
				tl = tl;    
					
			}
			else		// ȡ���´��������ﲿ������,�����ݻ�û����flash����
			{	  	
				tl = tl % pVDR->FMSaveItemCnt;	
			}

			DFE.Read(pVDR->FMAddr, pVDR->SaveItemLen * tl,pVDR->ptBuf);

			for (i=tl; i>0;i--)
			{	
						
				if (pVDR->ptBuf[pVDR->TimeBCDPos+(i-1)*pVDR->SaveItemLen]==0) continue;	  // ��ݲ���Ϊ0

				if ((memcmp(&pVDR->ptBuf[pVDR->TimeBCDPos+(i-1)*pVDR->SaveItemLen],GB_LargeData.StartTime,6)>=0)	&&
					(memcmp(&pVDR->ptBuf[pVDR->TimeBCDPos+(i-1)*pVDR->SaveItemLen],GB_LargeData.EndTime,6)<=0))
				{	 	
					// �ҵ����ϵ�����
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

		
		if ((pVDR->FlashPageSaveCnt>0)&&(GB_LargeData.TotalItem))		 // ȡflash�е�����
		{
			Addr = pVDR->FlashStartPageAddr;
			Addr += (GetDataCnt-1)%(pVDR->FlashTotalPages);
			DFF.PageRead(Addr, pVDR->ptBuf);
						
			for (i=(pVDR->FMSaveItemCnt); i>0;i--)
			{
				if ((DataLen > GB_LARGE_DATA_PACK_BLOCK_LEN)) // ÿһ���������ݶβ���̫��
				{	 	
					break;
				}
					 
				if (pVDR->ptBuf[pVDR->TimeBCDPos+(i-1)*pVDR->SaveItemLen]==0) continue;
									
				if ((memcmp(&pVDR->ptBuf[pVDR->TimeBCDPos+(i-1)*pVDR->SaveItemLen],GB_LargeData.StartTime,6)>=0)	&&
					(memcmp(&pVDR->ptBuf[pVDR->TimeBCDPos+(i-1)*pVDR->SaveItemLen],GB_LargeData.EndTime,6)<=0))
				{		 	
					// �ҵ����ϵ�����
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
	
	*pReadPage = GetDataCnt;		// ����

	return DataLen;
}

////////
uint16		UartCls::SendVDR_Data_08H_To_15H(uint16 ReturnBufferLen, uchar *pReturnBuffer)
{
	static uint32 ReadPage;
	uchar s, SendDataType = 0;			 // SendDataType   0����Ҫ���͵�����1�ϵģ� 1�������͵����ڣ���Ҫ����ȡ�õ�����
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
		case eGB_rCMD_GET_SPEED_PER_SECOND			:			// 0x08,48H,�ɼ�ָ������ʻ�ٶȼ�¼
			#if (0)
			if (GB_LargeData.TotalPack==0)
			{	
				if (TD.SW==tSPEED_RECORD) Uart.puts("�ٶ����ݣ�");
				ReadPage = 0;
				if (FlashPageCnt.Speed48HSave>0)
				{
					ReadPage = 	FlashPageCnt.Speed48HSave;	// ����һҳ
					if (ReadPage>FF_48H_SPEED_TOTAL_PAGE) ReadPage = FF_48H_SPEED_TOTAL_PAGE;
				}

				
			}

			if (TD.SW==tSPEED_RECORD)
			{ Uart.puts("ReadPage Test Data:"); Uart.putb(1,4, (uchar *)&ReadPage); Uart.puts("\r\n"); os_dly_wait(50);	 }
		
			for (;ReadPage;ReadPage--)
			{
				if ((DataLen > GB_LARGE_DATA_PACK_BLOCK_LEN)) // ÿһ���������ݶβ���̫��
				{
					 break;
				}

				Addr = FF_48H_SPEED_START_PAGE;
				Addr += ((ReadPage-1)%FF_48H_SPEED_TOTAL_PAGE);	 // 
				DFF.PageRead(Addr, tBuffer);
			
				// һҳ����2���ӵ�����
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
				{	 // �ҵ�����ʱ�����������
					for (i=0; i<60;i++)
					{
						if (pSwPerMin->SpeedSw[i].Speed != 0) break;
						
						if ((i<59)&&(pSwPerMin->SpeedSw[i].Sw != pSwPerMin->SpeedSw[i].Sw)) break;	
					}
					if (i!=60)
					{
						memmove(&pSendBuffer[LEN_U0_GB_FRAME_HEAD+DataLen],(tBuffer+126),126);
					   pSendBuffer[LEN_U0_GB_FRAME_HEAD+DataLen+5] = 0; // ����Ϊ0
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
				{	 // �ҵ�����ʱ�����������
					for (i=0; i<60;i++)
					{
						if (pSwPerMin->SpeedSw[i].Speed != 0) break;
						
						if ((i<59)&&(pSwPerMin->SpeedSw[i].Sw != pSwPerMin->SpeedSw[i].Sw)) break;	
					}
					if (i!=60)
					{
						memmove(&pSendBuffer[LEN_U0_GB_FRAME_HEAD+DataLen],(tBuffer),126);
						pSendBuffer[LEN_U0_GB_FRAME_HEAD+DataLen+5] = 0; // ����Ϊ0
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
			#else		 //////////////// �Ż�
			if (GB_LargeData.SearchFlag==0)
			{	
				if (TD.SW==tSPEED_RECORD) Uart.puts("�ٶ����ݣ�");
				ReadPage = 0;
				if (FlashPageCnt.Speed48HSave>0)
				{
					ReadPage = 	FlashPageCnt.Speed48HSave;	// ����һҳ
					if (ReadPage>FF_48H_SPEED_TOTAL_PAGE) ReadPage = FF_48H_SPEED_TOTAL_PAGE;
				}

				
			}

			if (TD.SW==tSPEED_RECORD)
			{ Uart.puts("ReadPage Test Data:"); Uart.putb(1,4, (uchar *)&ReadPage); Uart.puts("\r\n"); os_dly_wait(50);	 }
		
			for (t=0;ReadPage;ReadPage--)
			{
				//if ((DataLen > GB_LARGE_DATA_PACK_BLOCK_LEN))   break;// ÿһ���������ݶβ���̫��
				if (DataLen >= 126*7) break;			// ��෢7��	ÿһ֡���ܳ���1000�ֽ�

				Addr = FF_48H_SPEED_START_PAGE;
				Addr += ((ReadPage-1)%FF_48H_SPEED_TOTAL_PAGE);	 // 
				DFF.PageRead(Addr, tBuffer);
			
				// һҳ����2���ӵ�����	
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
						// �ҵ�����ʱ�����������
						for (i=0; i<60;i++)
						{
							if (pSwPerMin->SpeedSw[i].Speed != 0) break;
							
							if ((i<59)&&(pSwPerMin->SpeedSw[i].Sw != pSwPerMin->SpeedSw[i].Sw)) break;	
						}
						if (i!=60)
						{
							memmove(&pSendBuffer[LEN_U0_GB_FRAME_HEAD+DataLen],(tBuffer+126*(s-1)),126);
						   pSendBuffer[LEN_U0_GB_FRAME_HEAD+DataLen+5] = 0; // ����Ϊ0
							DataLen += 126;

							if (GB_LargeData.TotalItem>0)  	GB_LargeData.TotalItem--;
							t++;
							if (GB_LargeData.TotalItem==0)
							{
								if (t%2)	//  ��������������ݿ飬Ҫ�ѿ�������ַ�ӻأ���Ϊ����һ��������flash�����´�������ȡ��
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
	
			//Uart.puts("�ٶ����ݣ�");
			break;
		
		case eGB_rCMD_GET_POSITION_INFO				:			// 0x09,360H,�ɼ�ָ����λ����Ϣ
			#if (0) // ����Сʱ��
			if (GB_LargeData.TotalPack==0)
			{	
				if (TD.SW==tSPEED_RECORD) Uart.puts("λ�����ݣ�");
				ReadPage = 0;
				if (FlashPageCnt.Location360HSave>1)
				{
					ReadPage = 	FlashPageCnt.Location360HSave;	// ����һҳ
					if (ReadPage>FF_360H_LOCATION_TOTAL_PAGE) ReadPage = FF_360H_LOCATION_TOTAL_PAGE;
				}	
			}
		
			for (;ReadPage;ReadPage--)
			{
				if ((DataLen > GB_LARGE_DATA_PACK_BLOCK_LEN)) // ÿһ���������ݶβ���̫��
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

			
				// һҳ����20���ӵ����ݣ�3ҳ����1Сʱ�����ݣ�������ҳ��ʱ�� �� tBuffer[0] - tBuffer[5] ��ʱ��
				if ((tBuffer[0]!= 0x00)&&(tBuffer[0]!= 0xff)&&(memcmp(tBuffer,GB_LargeData.StartTime,6)>=0) &&
					(memcmp(tBuffer,GB_LargeData.EndTime,6)<=0))
				{	 // �ҵ�����ʱ�����������
					if (TD.SW==tSPEED_RECORD) Uart.puts("get pos data ok");
					memmove(&pSendBuffer[LEN_U0_GB_FRAME_HEAD],tBuffer,226);
					//pSendBuffer[LEN_U0_GB_FRAME_HEAD+DataLen+4] = 0; // ����Ϊ0
				   //pSendBuffer[LEN_U0_GB_FRAME_HEAD+DataLen+5] = 0; // ����Ϊ0
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
	
			#else ///////////////////  ��ʵʱ������	  ///////////////////////////////////////////////////////

			if (GB_LargeData.SearchFlag==0)
			{	
				if (TD.SW==tSPEED_RECORD) Uart.puts("λ�����ݣ�");
				ReadPage = 0;
				if (FlashPageCnt.Location360HSave>1)
				{
					ReadPage = 	FlashPageCnt.Location360HSave;	// ����һҳ
					if (ReadPage>FF_360H_LOCATION_TOTAL_PAGE) ReadPage = FF_360H_LOCATION_TOTAL_PAGE;
				}
				
					
				// �����������ݣ�����ʵʱ������
			   SetBufferValue(&pSendBuffer[LEN_U0_GB_FRAME_HEAD],666,0x00);
				Rtc.GetRTC_BCD((RTC_STR  *)tBuffer);

				if ((tBuffer[0]!=0)&&(memcmp(tBuffer,GB_LargeData.StartTime,4)>=0) &&
					(memcmp(tBuffer,GB_LargeData.EndTime,4)<=0))
				{		
					//if (TD.SW==tSPEED_RECORD) Uart.puts("TTT");	
					// ��ȡ��������	(��������) 
					  					 
					  DFE.Read(FM_360H_LOCATION_TIME,666,&pSendBuffer[LEN_U0_GB_FRAME_HEAD]);

					  DataLen = 666;	 // ���ݶα���666�ı���
	
						GB_LargeData.TotalItem--;
						if (GB_LargeData.TotalItem==0)
						{
							//ReadPage = 0;
						}
				
				}	
			
			} 
				
			for (;ReadPage;ReadPage--)
			{
				//if ((DataLen > GB_LARGE_DATA_PACK_BLOCK_LEN)) // ÿһ���������ݶβ���̫��
				if (DataLen >= 666)
				{			
					 break;
				}


				Addr = FF_360H_LOCATION_START_PAGE;
			   Addr += ((ReadPage-1)%FF_360H_LOCATION_TOTAL_PAGE);	 // 
				DFF.PageRead(Addr, tBuffer);

				
				// һҳ����20���ӵ����ݣ�3ҳ����1Сʱ�����ݣ�������ҳ��ʱ�� �� tBuffer[0] - tBuffer[5] ��ʱ��
				if ((tBuffer[0]!= 0x00)&&(tBuffer[0]!= 0xff)&&(tBuffer[4]==0)&&(tBuffer[5]==0)&&
					(memcmp(tBuffer,GB_LargeData.StartTime,4)>=0) &&
					(memcmp(tBuffer,GB_LargeData.EndTime,4)<=0))
				{	 // �ҵ�����ʱ�����������
					if (TD.SW==tSPEED_RECORD) Uart.puts("get pos data ok");

					memmove(&pSendBuffer[LEN_U0_GB_FRAME_HEAD],tBuffer,226);
					//pSendBuffer[LEN_U0_GB_FRAME_HEAD+DataLen+4] = 0; // ����Ϊ0
				   //pSendBuffer[LEN_U0_GB_FRAME_HEAD+DataLen+5] = 0; // ����Ϊ0
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
		
		case eGB_rCMD_GET_PARKING_DATA				:			// 0x10,�¹��ɵ����ݣ�ͣ��ǰ20�����ݣ�
			if (GB_LargeData.SearchFlag==0)
			{	
				if (TD.SW==tSPEED_RECORD) Uart.puts("ͣ�����ݣ�");
				ReadPage = 0;
				if (FlashPageCnt.ParkingDataSave>0)
				{
					ReadPage = 	FlashPageCnt.ParkingDataSave;	// ����һҳ
					if (ReadPage>FF_PARKING_DATA_TOTAL_PAGE) ReadPage = FF_PARKING_DATA_TOTAL_PAGE;

					//Uart.puts("ReadPage Test Data:"); Uart.putb(1,4,(uchar *)&ReadPage); Uart.puts("\r\n");
				}
			}
		
			for (;ReadPage;ReadPage--)
			{
				if (SendDataType != 1)	 //���ڲɼ�ʱ
				{
				///if ((DataLen > GB_LARGE_DATA_PACK_BLOCK_LEN)) // ÿһ���������ݶβ���̫��
				// ����ʵ�ʼ��������������෢4������ʱ��ʻ1��2��3	���������ݶβ��ܳ���1024
				//  �����������5�����ݿ飬ֻ��3��������������Ϊ�òɼ����
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

				// һҳ����1���¹��ɵ�����
				pSaveStopData = (pSAVE_STOP_DATA)tBuffer;

				if ((pSaveStopData->DateTime[0]!=0)&&(memcmp(pSaveStopData->DateTime,GB_LargeData.StartTime,6)>=0)	&&
					(memcmp(pSaveStopData->DateTime,GB_LargeData.EndTime,6)<=0))

				{	 // �ҵ�����ʱ�����������
					
					// �������������Ҫȥ��
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
		case eGB_rCMD_GET_OVER_TIME_DRIVE			:			// 0x11,ƣ�ͼ�ʻ����

			Vdr1.FMAddr = FM_SAVE_FATIGUE_DRIVING;			  // ����������ĵ�ַ
			Vdr1.FMCnt = FM_FATIGUE_DRIVING_CNT;			  // ������Ŀ�ܼ���
			Vdr1.FMSaveItemCnt = 256/LEN_SAVE_FATIGUE_DRIVING_INFO;	  // ������Ŀ�ܱ��������������	 256/ 7  = 36
			Vdr1.SaveItemLen = LEN_SAVE_FATIGUE_DRIVING_INFO;		// ������Ŀ����
			Vdr1.FlashStartPageAddr = FF_TIRE_DRIVE_START_PAGE;			// ������Ŀ��flash�Ŀ�ʼҳ��ַ
			Vdr1.FlashPageSaveCnt = FlashPageCnt.FatigueDrivingSave;	 // ������Ŀ��flash�ļ���
			Vdr1.FlashTotalPages = FF_TIRE_DRIVE_TOTAL_PAGE;	 //  ������Ŀ��flash�ܹ���ҳ�� 
			Vdr1.TimeBCDPos = 18;				// ÿ��������Ŀʱ�����ĸ�λ��	��ÿ����¼��ʱ���ƫ�Ƶ�ַ  ��GB19056-2012��֪		
			Vdr1.ptBuf = tBuffer;				   // ��ʱ����	  
			Vdr1.pSendBuffer = pSendBuffer;	

			DataLen = VDR_Data_Fun_1(&Vdr1, &ReadPage,(char *)"ƣ�ͼ�ʻ���ݣ�");
		
		
			break;
		
		case eGB_rCMD_GET_GET_LOG_IN_OUT_RECORD	:			// 0x12,��ʻԱ��¼�˳���¼
	
			Vdr1.FMAddr = FM_DRIVER_OUT_IN;			  // ����������ĵ�ַ
			Vdr1.FMCnt = FM_DRIVER_OUT_IN_CNT;			  // ������Ŀ�ܼ���
			Vdr1.FMSaveItemCnt = 256/LEN_DRIVER_IN_OUT_STATUS;	  // ������Ŀ�ܱ��������������	 
			Vdr1.SaveItemLen = LEN_DRIVER_IN_OUT_STATUS;		// ������Ŀ����
			Vdr1.FlashStartPageAddr = FF_DRIVER_IN_OUT_START_PAGE;			// ������Ŀ��flash�Ŀ�ʼҳ��ַ
			Vdr1.FlashPageSaveCnt = FlashPageCnt.DriverLogInOutSave;	 // ������Ŀ��flash�ļ���
			Vdr1.FlashTotalPages = FF_DRIVER_IN_OUT_TOTAL_PAGE;	 //  ������Ŀ��flash�ܹ���ҳ�� 
			Vdr1.TimeBCDPos = 0;				// ÿ��������Ŀʱ�����ĸ�λ��	��ÿ����¼��ʱ���ƫ�Ƶ�ַ 		
			Vdr1.ptBuf = tBuffer;				   // ��ʱ����	  
			Vdr1.pSendBuffer = pSendBuffer;	
			 //DataLen = 0;		  // test
			DataLen = VDR_Data_Fun_1(&Vdr1, &ReadPage,(char *)"��ʻԱ��¼�˳����ݣ�");	 // ������
			//Uart.puts("��ʻԱ��¼�˳����ݣ�");
			//
		 	break;
		
		case eGB_rCMD_GET_POWER_RECORD				:			// 0x13,�ɼ��ⲿ�����¼
			Vdr1.FMAddr = FM_EXT_POWER_STATUS;			  // ����������ĵ�ַ
			Vdr1.FMCnt = FM_EXT_POWER_STATUS_CNT;			  // ������Ŀ�ܼ���
			Vdr1.FMSaveItemCnt = 256/LEN_EXT_POWER_STATUS;	  // ������Ŀ�ܱ��������������	 256/ 7  = 36
			Vdr1.SaveItemLen = LEN_EXT_POWER_STATUS;		// ������Ŀ����
			Vdr1.FlashStartPageAddr = FF_POWER_ON_OFF_START_PAGE;			// ������Ŀ��flash�Ŀ�ʼҳ��ַ
			Vdr1.FlashPageSaveCnt = FlashPageCnt.PowerOnOffSave;	 // ������Ŀ��flash�ļ���
			Vdr1.FlashTotalPages = FF_POWER_ON_OFF_TOTAL_PAGE;	 //  ������Ŀ��flash�ܹ���ҳ�� 
			Vdr1.TimeBCDPos = 0;				// ÿ��������Ŀʱ�����ĸ�λ��	��ÿ����¼��ʱ���ƫ�Ƶ�ַ 		
			Vdr1.ptBuf = tBuffer;				   // ��ʱ����	  
			Vdr1.pSendBuffer = pSendBuffer;	

			DataLen = VDR_Data_Fun_1(&Vdr1, &ReadPage,(char *)"�ⲿ�������ݣ�");
			break;

		case eGB_rCMD_GET_MODIFY_PARA_RECORD		:			// 0x14,�����޸ļ�¼
			Vdr1.FMAddr = FM_PARA_MODIFY_RECORD;			  // ����������ĵ�ַ
			Vdr1.FMCnt = FM_PARA_MODIFY_RECORD_CNT;			  // ������Ŀ�ܼ���
			Vdr1.FMSaveItemCnt = 256/LEN_PARA_MODIFY_RECORD;	  // ������Ŀ�ܱ��������������	 256/ 7  = 36
			Vdr1.SaveItemLen = LEN_PARA_MODIFY_RECORD;		// ������Ŀ����
			Vdr1.FlashStartPageAddr = FF_PARA_MODIFY_START_PAGE;			// ������Ŀ��flash�Ŀ�ʼҳ��ַ
			Vdr1.FlashPageSaveCnt = FlashPageCnt.ParaModifySave;	 // ������Ŀ��flash�ļ���
			Vdr1.FlashTotalPages = FF_PARA_MODIFY_TOTAL_PAGE;	 //  ������Ŀ��flash�ܹ���ҳ�� 
			Vdr1.TimeBCDPos = 0;				// ÿ��������Ŀʱ�����ĸ�λ��	��ÿ����¼��ʱ���ƫ�Ƶ�ַ 		
			Vdr1.ptBuf = tBuffer;				   // ��ʱ����	  
			Vdr1.pSendBuffer = pSendBuffer;	

			DataLen = VDR_Data_Fun_1(&Vdr1, &ReadPage,(char *)"�����޸ģ�");
			//
			break;

		case eGB_rCMD_GET_SPEED_STATUS_LOG			:			// 0x15,�ٶ�״̬��¼  
			if (GB_LargeData.SearchFlag==0)
			{	
				if (TD.SW==tSPEED_RECORD) Uart.puts("�ٶ�״̬���ݣ�");
				ReadPage = 0;
				if (FlashPageCnt.SpeedStatusSave>0)
				{
					ReadPage = 	FlashPageCnt.SpeedStatusSave;	// ����һҳ
					if (ReadPage>FF_SPEED_STATUS_TOTAL_PAGE) ReadPage = FF_SPEED_STATUS_TOTAL_PAGE;

					//Uart.puts("ReadPage Test Data:"); Uart.putb(1,4,(uchar *)&ReadPage); Uart.puts("\r\n");
				}
			}
		
			for (;ReadPage;ReadPage--)
			{
				if ((DataLen > GB_LARGE_DATA_PACK_BLOCK_LEN)) // ÿһ���������ݶβ���̫��
				{
					 break;
				}

				Addr = FF_SPEED_STATUS_START_PAGE;
				Addr += ((ReadPage-1)%FF_SPEED_STATUS_TOTAL_PAGE);	 // 
				DFF.PageRead(Addr, tBuffer);

				// һҳ����1���ٶ�״̬
				pSpeedStatus = (pSPEED_STATUS_LOG)tBuffer;

				if ((pSpeedStatus->StartTime!=0)&&(memcmp(pSpeedStatus->StartTime,GB_LargeData.StartTime,6)>=0)	&&
					(memcmp(pSpeedStatus->StartTime,GB_LargeData.EndTime,6)<=0))

				{	 // �ҵ�����ʱ�����������
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
			//	if (TD.SW==tSPEED_RECORD) Uart.puts("�ٶ�״̬���ݣ�");
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
			putb(1,Length,pSendBuffer);		// ������Ϣ����λ��


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
   ������: ReturnErrorFlag
   ���ܣ�  UART0���ش����־
   ��ڣ�  ��
   ���ڣ�  ��
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
		putb(0,5,tBuffer);		// ������Ϣ����λ��	
	}
}


/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	�������ƣ�
   ����������
   ��ڲ�����
   ���ڲ�����
   ȫ�ֱ���: 
   ����ʱ�䣺
   �޸�ʱ�䣺
*/ 	
//void			GPRS_PushAlignment(uint16 Len,uchar *pData);
//uchar			NF_Format(uint16 Affirm);
extern		"C"		uchar			HalfByteHexCharToInt(uchar Hex);

//static    uint16      ULen;
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ ����ԭ�������ĺ���������������
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
	
	
	/////////////////// RS232ͨѶ����Ϊÿ��ͨѶֻ��һ�����ݰ���������ճ�������
	//�������ݲ�����ʱ(�ǲ���ָ�������U0_RecBuffer�п��ܱ�ˢ��
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
		//����������	//////////////////////////////////////////////////////////
		case eGB_rCMD_GET_STD_VERSION:			// �ɼ���¼��ִ�б�׼
		   
		   *(pSendBuffer+LEN_U0_GB_FRAME_HEAD) = 0x12;  		// ��¼��ִ�б�׼��ź�2λ BCD�룬 GB/T19056-2012 
		   *(pSendBuffer+LEN_U0_GB_FRAME_HEAD+1) = 0x00;  		// �޸ĵ��� ���޸Ļ���Ӧ����Ĭ��Ϊ0
		   Length = LEN_U0_GB_FRAME_HEAD + 2;
			
		break;
		//////////////////
		case eGB_rCMD_GET_DRIVER_CODE:			// ��������ʻ֤��
			
			SetBufferValue( (uchar*)(pSendBuffer+LEN_U0_GB_FRAME_HEAD),20,0);
			DFE.Read(FM_DRIVER_CODE, 18, (pSendBuffer+LEN_U0_GB_FRAME_HEAD));	
			if( strlen( (char*)(pSendBuffer+LEN_U0_GB_FRAME_HEAD))!=18)
				memmove( (uchar*)(pSendBuffer+LEN_U0_GB_FRAME_HEAD),"500382197911253837\0",18);
			//
			Length = LEN_U0_GB_FRAME_HEAD + 18;
		break;
		
		//		$$$$$$$$$$$$$$$$$$$$  ��ȡʱʵʱ��
		case eGB_rCMD_GET_REAL_TIME:
			NOP();	NOP();
			Length = LEN_U0_GB_FRAME_HEAD+6;								
			Rtc.GetRTC_BCD((RTC_STR *)(pSendBuffer+LEN_U0_GB_FRAME_HEAD));										
			break;
			
		////////////////////////////// ��ʼ��������ʵ�ʲ�ʹ�ã�ֻ���úͶ�ȡ�������ο�
		case eGB_rCMD_GET_TOTAL_MILAGE:			// �Ӱ�װ��ʼ���ۼ���ʻ�������
			/*Rtc.GetRTC_BCD((RTC_STR *)(pSendBuffer+LEN_U0_GB_FRAME_HEAD));				//��ǰʱ��
			DFE.Read(FM_INSTALL_DATE,6,(pSendBuffer+LEN_U0_GB_FRAME_HEAD+6));			// ��װʱ��
			DFE.Read(FM_INIT_MILAGE_BCD, 4, (pSendBuffer+LEN_U0_GB_FRAME_HEAD+12));	// ��ʼ���
			tl = SystemLog.VDR.SpeedSensorPulsCnt;
			tl /= VehiclePulseKV;			
			Int2Bcd8( tl , (pSendBuffer+LEN_U0_GB_FRAME_HEAD+16));						// ��ʻ�ۼ����

			Length = LEN_U0_GB_FRAME_HEAD+20;	 */

			Rtc.GetRTC_BCD((RTC_STR *)(pSendBuffer+LEN_U0_GB_FRAME_HEAD));				//��ǰʱ��
			DFE.Read(FM_INSTALL_DATE,6,(pSendBuffer+LEN_U0_GB_FRAME_HEAD+6));			// ��װʱ��
			DFE.Read(FM_INIT_MILAGE_BCD, 4, (pSendBuffer+LEN_U0_GB_FRAME_HEAD+12));	// ��ʼ���
			tl = SystemLog.VDR.SpeedSensorPulsCnt;
			tl *= 10;
			tl /= VehiclePulseKV;
			UINT32HexToBcd( tl , (pSendBuffer+LEN_U0_GB_FRAME_HEAD+16));						// ��ʻ�ۼ����

			Length = LEN_U0_GB_FRAME_HEAD+20;
		break;
		//////////////////
		case eGB_rCMD_GET_SPEED_KV:				// *Vehicle Characteristic Coefficients		�ٶ�����ϵ��
			Rtc.GetRTC_BCD((RTC_STR *)(pSendBuffer+LEN_U0_GB_FRAME_HEAD));				//��ǰʱ��
			DFE.Read( FM_VEHICLE_PULSE_KV,4,(uchar*)&VehiclePulseKV);
			Tp.B32[0] = VehiclePulseKV;
			*(pSendBuffer+LEN_U0_GB_FRAME_HEAD+6) = Tp.Buffer[1];
			*(pSendBuffer+LEN_U0_GB_FRAME_HEAD+7) = Tp.Buffer[0];
			Length = LEN_U0_GB_FRAME_HEAD+8;
		break;

		//////////////////
		case eGB_rCMD_GET_VEHICLE_VIN					:			// *����VIN�ţ����ƺ��룬��������
			//DFE.Read(FM_VEHICLE_VIN,LEN_VEHICLE_VIN + LEN_VEHICLE_CODE + LEN_VEHICLE_CLASS,(pSendBuffer+LEN_U0_GB_FRAME_HEAD));
			DFE.Read(FM_VEHICLE_VIN,LEN_VEHICLE_VIN,(pSendBuffer+LEN_U0_GB_FRAME_HEAD));
			DFE.Read(FM_VEHICLE_CODE,LEN_VEHICLE_CODE,(pSendBuffer+LEN_U0_GB_FRAME_HEAD+LEN_VEHICLE_VIN) );
			DFE.Read(FM_VEHICLE_CLASS,LEN_VEHICLE_CLASS,(pSendBuffer+LEN_U0_GB_FRAME_HEAD+LEN_VEHICLE_VIN+LEN_VEHICLE_CODE) );
			Length = LEN_VEHICLE_VIN + LEN_VEHICLE_CODE + LEN_VEHICLE_CLASS + LEN_U0_GB_FRAME_HEAD;	
		break;		
		
		//////////////////
		case eGB_rCMD_GET_SIGNAL_CONFIG:			// �ɼ��ź�״̬������Ϣ 
						SetBufferValue(pSendBuffer,100,0);
			Rtc.GetRTC_BCD((RTC_STR *)(pSendBuffer+LEN_U0_GB_FRAME_HEAD));				//��ǰʱ��
			*(pSendBuffer+LEN_U0_GB_FRAME_HEAD+6) = 1;		// ֻ��1�ֽ�
			//memmove((pSendBuffer+LEN_U0_GB_FRAME_HEAD+7), (uchar*)"�Զ���1\x00\x00\x00�Զ���2\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00����\x00\x00\x00\x00\x00\x00Զ��\x00\x00\x00\x00\x00\x00��ת��\x00\x00\x00\x00��ת��\x00\x00\x00\x00�ƶ�\x00\x00\x00\x00\x00\x00",80);
			DFE.Read(FM_SIGNAL_CONFIG,80,(pSendBuffer+LEN_U0_GB_FRAME_HEAD+7));
			Length = LEN_U0_GB_FRAME_HEAD+87;			
		break;
		
		//////////////////////////////////// Ψһ���к�
		case eGB_rCMD_GET_SERIAL_NUMBER:
			
			#if (WUXI_TEST==1)
			Length = LEN_U0_GB_FRAME_HEAD;	 
			SetBufferValue(pSendBuffer,35,0);
			memmove((uchar*)(pSendBuffer+Length),"0000000",7);   // ������CCC��֤����
			Length += 7;
			strcpy((char*)(pSendBuffer+Length),"SZ-BD21A"); // ʹ�ñ��˵�ƽ̨��������ֱ�Ӹ��ͺţ���ƽ̨��ѯ����ȷ�� 2015.3.24
			//DFE.Read(EE_TERMINAL_TYPE,16, (uchar*)(pSendBuffer+Length));      			//  ��֤��Ʒ�ͺ�	 ������ͺ�ʹ�����
		
			Length += 16;
			memmove((uchar*)(pSendBuffer+Length),"\x14\x11\x01",3); 				// BCD�� ��������������
			Length += 3;
			//memmove((uchar*)(pSendBuffer+Length),"\x00\x00\x00\x01",4);    // ��ˮ��
			memmove((uchar*)(pSendBuffer+Length),"\x59\xF9\x62\xE1",4); 	  // 0x59F962E1	������48Сʱ�����豸�ı�� 1509516001 
			Length += 4;															  			
			Length += 5;             // ����5���ֽڵı���	
			#else
			DFE.Read(FM_SERIAL_NUMBER, LEN_SERIAL_NUMBER, (uchar*)(pSendBuffer+LEN_U0_GB_FRAME_HEAD) );
			Length = LEN_U0_GB_FRAME_HEAD+LEN_SERIAL_NUMBER;
			#endif
		break;
		
		case eGB_rCMD_GET_SPEED_PER_SECOND			:			// 0x08,48H,�ɼ�ָ������ʻ�ٶȼ�¼
		case eGB_rCMD_GET_POSITION_INFO				:			// 0x09,360H,�ɼ�ָ����λ����Ϣ
		case eGB_rCMD_GET_PARKING_DATA				:			// 0x10,�¹��ɵ����ݣ�ͣ��ǰ20�����ݣ�
		case eGB_rCMD_GET_OVER_TIME_DRIVE			:			// 0x11,ƣ�ͼ�ʻ����
		case eGB_rCMD_GET_GET_LOG_IN_OUT_RECORD	:			// 0x12,��ʻԱ��¼�˳���¼
		case eGB_rCMD_GET_POWER_RECORD				:			// 0x13,�ɼ��ⲿ�����¼
		case eGB_rCMD_GET_MODIFY_PARA_RECORD		:			// 0x14,�����޸ļ�¼
		case eGB_rCMD_GET_SPEED_STATUS_LOG			:			// 0x15,�ٶ�״̬��¼
			
	

		if (memcmp(GB_LargeData.StartTime,(pRecBuffer+LEN_U0_GB_FRAME_HEAD+6),6) !=0)
		{
			s = 1;
			memmove(GB_LargeData.StartTime,(pRecBuffer+LEN_U0_GB_FRAME_HEAD),6);	  // ��ʼʱ��  bcd
		}
		else 
		{
			s = 0;
		}
		memmove(GB_LargeData.EndTime,(pRecBuffer+LEN_U0_GB_FRAME_HEAD+6),6);	  // ����ʱ��	  bcd
		// Ӧ�������������⣬һ��һ��Ϊ����ߴ����ٶȣ�ֻ�п�ʼ����һ�������������ֲ�һ����  GB_LargeData.TotalPack
		// ����0��������SendVDR_Data_08H_To_15H����������flash
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
		GB_LargeData.OverTimeSearch = 10; //   10��

		return 0;
		//break;


		
		//////////////////
		//���У�����涨������ 0x01~0x07��7�������������������ʾ����ʾ


		/////////////////�춨����////////////////////////////////////////////////////////////////
		case eGB_rCMD_SET_CHECK:			// �춨����
			
			Length = LEN_U0_GB_FRAME_HEAD;
		break;

		case eGB_rCMD_SET_CHECK_CONTINUSE:			// ��춨ģʽ����
		
		break;
		


		////////////	����������	//////////////////////////////////////////////////////////
		/*case eGB_rCMD_SET_DRIVER_CODE:			// ���ü�ʻԱ���룬��ʻ֤����		������Ҫ��

			Length = LEN_U0_GB_FRAME_HEAD;
		break;	*/
		
		///////////////////////////////////////////////// ����ϵ��
		case eGB_rCMD_SET_VEHICLE_PULSE_KV:
			// ǰ6�ֽ�Ϊ����ʱ��
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
		case eGB_rCMD_SET_VEHICLE_VIN:			// ���ó���VIN�ţ����ƺţ���������
			
			DFE.Write(FM_VEHICLE_VIN,17,(pRecBuffer+LEN_U0_GB_FRAME_HEAD));
			SetBufferValue(Tp.Buffer, 12, 0);
			memmove(Tp.Buffer,(pRecBuffer+LEN_U0_GB_FRAME_HEAD+17),12);
			DFE.Write(FM_VEHICLE_CODE,9,Tp.Buffer);  // ���ƺ�������ֻ��9���ֽڣ�����д12���ֽ�
			DFE.Write(FM_VEHICLE_CLASS,12,(pRecBuffer+LEN_U0_GB_FRAME_HEAD+17+12));
			Para_Modi_Recode(Cmd);
			Length = LEN_U0_GB_FRAME_HEAD;
		break;
		
		//////////////////
		case eGB_rCMD_SET_SETUP_DATE:			// ��װ����
			DFE.Write(FM_INSTALL_DATE,6,(pRecBuffer+LEN_U0_GB_FRAME_HEAD));
			Para_Modi_Recode(Cmd);
		   Length = LEN_U0_GB_FRAME_HEAD;
		break;
		
		//////////////////
		case eGB_rCMD_SET_SIGNAL_STATUS:			// �����ź�״̬����
			
			DFE.Write(FM_SIGNAL_CONFIG,80,(pRecBuffer+LEN_U0_GB_FRAME_HEAD));
			Para_Modi_Recode(Cmd);
			Length = LEN_U0_GB_FRAME_HEAD;		
		break;
		
		///////////////////////////// ����Ψһ���кţ�����ʹ�ã�ʵ�ʲ�ʹ�ã�
		/*case eGB_rCMD_SET_SERIAL_NUMBER:
		
			DFE.Write( FM_SERIAL_NUMBER, LEN_SERIAL_NUMBER, (pRecBuffer+LEN_U0_GB_FRAME_HEAD) );
			Length = LEN_U0_GB_FRAME_HEAD;
		break;*/
		
				
		
		
		/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
		*/
		//		$$$$$$$$$$$$$$$$$$$$  ����ʱʵʱ��
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

		////////////////////////////// ��ʼ��������ʵ�ʲ�ʹ�ã�ֻ���úͶ�ȡ�������ο�
		case	eGB_rCMD_SET_INIT_MIL:				 // ���ó�ʼ���    // added 2013.5
			
			/*for (i = 0; i<4; i++)
			{	s =  *(pRecBuffer+LEN_U0_GB_FRAME_HEAD+i);
				if (((s & 0x0f) > 0x09) || (((s >> 4) & 0x0f) > 0x09))		 // ����BCD�룺 00 - 99 99 99 99 
				{
					Error = TRUE;
					break;	
				} 					
			}
			DFE.Write(FM_INIT_MILAGE_BCD, 4, (pRecBuffer+LEN_U0_GB_FRAME_HEAD)); 
			NOP(); NOP(); NOP();
			
			Length = LEN_U0_GB_FRAME_HEAD;*/
			Length = LEN_U0_GB_FRAME_HEAD;
			// ʵʱʱ��
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
				if (((s & 0x0f) > 0x09) || (((s >> 4) & 0x0f) > 0x09))		 // ����BCD�룺 00 - 99 99 99 99 
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
			// ����ʹ�ò�����û�����ʱ�����ã���Ϊ���ʱ���·�������̫�󣬵��� SystemLog.VDR.SpeedSensorPulsCnt �����
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
	if(USBUseFlag==0)															     //�Ƿ�USBҪд������
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
			  putb(0,Length,pSendBuffer);		// ������Ϣ����λ��
			  BEEP(9,0,2,0,5);
		   }	
		}
	}
	
	NOP();	NOP();
	return Length;	
}
		



/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	�������ƣ�SetU0FramHead
   �������������U0ָ���ͷ
   ��ڲ�����
   ���ڲ�����
   ȫ�ֱ���: 
   ����ʱ�䣺
   �޸�ʱ�䣺
*/ 	
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ ����ԭ�������ĺ���������������
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
	�������ƣ�Para_Modi_Recode
   ������������¼�����޸ļ�¼
   ��ڲ�����
   ���ڲ�����
   ȫ�ֱ���: 
   ����ʱ�䣺
   �޸�ʱ�䣺
*/ 	
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ ����ԭ�������ĺ���������������
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
	Addr = tl%36;							  // 	 36��
	Addr *= LEN_PARA_MODIFY_RECORD;
	Addr += FM_PARA_MODIFY_RECORD;		  	
	DFE.Write( Addr, LEN_PARA_MODIFY_RECORD,(uchar *)&Modify);
	tl++;
	DFE.Write(FM_PARA_MODIFY_RECORD_CNT,4,(uchar*)&tl);	 	// ��ʻԱ����ǳ���������1��洢������	
	//
	Uart.puts("���²����޸ĵ����� "); 
	if ((tl)&&(tl%36)==0)
	{
		// ���������һҳ��Flash�Ŀռ䣬��ŵ�Flash��
		if( (FlashPageCnt.ParaModifySave%FF_PAGE_PER_SECTOR)==0)
		{
			Addr = FF_PARA_MODIFY_START_PAGE;
			Addr += FlashPageCnt.ParaModifySave%FF_PARA_MODIFY_TOTAL_PAGE;
			
			DFF.SectorEraseWaitOK(Addr/FF_PAGE_PER_SECTOR,10);
		}				
		///////////////////////////////////////////�洢��Flash  ����1ҳflash����36���ⲿ�����¼
		DFE.Read(FM_PARA_MODIFY_RECORD,LEN_PARA_MODIFY_RECORD*36,tBuffer); // 
	
		Addr = FF_PARA_MODIFY_START_PAGE;
		Addr += FlashPageCnt.ParaModifySave%FF_PARA_MODIFY_TOTAL_PAGE;
		DFF.PageProgramWaitOK(Addr,tBuffer,3);
		FlashPageCnt.ParaModifySave++;
	
		DFE.Write( FM_FLASH_PAGE_CNT, LEN_FLASH_PAGE_CNT, (uchar*)&FlashPageCnt);
		Uart.puts("��������޸ļ�¼��Flash"); 
		//Uart.puts("Addr Test Data:"); Uart.putb(1,4, (uchar *)&Addr); Uart.puts("\r\n");
	
	}
}


#if(1)
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

	/////////////////// RS232ͨѶ����Ϊÿ��ͨѶֻ��һ�����ݰ���������ճ�������
	//�������ݲ�����ʱ(�ǲ���ָ�������U0_RecBuffer�п��ܱ�ˢ��
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
		////////////////////////////////////////////// ����ƣ�ͼ�ʻ��ز���
		case	eGY_rCMD_DRIVING_PARA:	// 0x80 // 300 120 1800 600 30
			// GYS \x2c\x01\x00\x00 \x78\x00\x00\x00 \x08\x07\x00\x00 \x58\x02\x00\x00 \x1e\x00\x00\x00 
			if( (U0_RecBuffer[LEN_U0_GY_FRAME_HEAD]=='S') )			// ����ȫ��ͬʱ���ã�������ܳ���
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
		////////////////////////////////////////////// ���ó�����ز��� 
		case	eGY_rCMD_OVER_SPEED_PARA:	// 0x81 // GY G 1 \030 \005 \003 \015\000 \000\000
			// GY S 1 \030 \005 \003 \015\000 \000\000 
			if( (U0_RecBuffer[LEN_U0_GY_FRAME_HEAD]=='S') )	  		// ����ȫ��ͬʱ���ã�������ܳ���
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
		
		//		$$$$$$$$$$$$$$$$$$$$  ��ȡʱʵʱ��
		case eGY_rCMD_GET_REAL_TIME:
			NOP();	NOP();
			if(U0_RecBuffer[LEN_U0_GY_FRAME_HEAD]=='I')	// CPU����ʱ��
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
		//		$$$$$$$$$$$$$$$$$$$$  ����ʱʵʱ��
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
			t = ((RTCD.D+2*RTCD.M+3*(RTCD.M+1)/5+RTCD.Y+RTCD.Y/4-RTCD.Y/100+RTCD.Y/400)+1)%7;	 //������
			
			LPC_RTC->DOW = t;
						
			U0_RecBuffer[LEN_U0_GY_FRAME_HEAD+10] = t;			
			Rtc.SetRtcR8025((RTC_STR *)&U0_RecBuffer[LEN_U0_GY_FRAME_HEAD]); 
			
			break;
		
		
		case eGY_rCMD_SELECT_SIM_CARD:				// ѡ��SIM��
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

		case	eGY_rCMD_SET_SIM_NUMBER:		//����SIM������
					
			memmove((uchar*)sTelNum.Sim,(uchar*)&U0_RecBuffer[LEN_U0_GY_FRAME_HEAD],16);
			sTelNum.Sim[16] = 0;
			Length = strlen((char *)sTelNum.Sim);
			DFE.Write(EE_SIM_NUMBER,16,(uchar*)sTelNum.Sim);
			DecCharToPressBCD(Length,(char *)sTelNum.Sim,SimNumberBCD);
			for(i=0;i<8;i++)	{ if(SimNumberBCD[i]==0)	SimNumberBCD[i] = 0x0f;	}
			Length = LEN_U0_GY_FRAME_HEAD;
		break;


		case	eGY_rCMD_GET_SIM_NUMBER:	  //�ɼ�SIM������
			
	      strcpy((char*)&TxdTBuffer[LEN_U0_GY_FRAME_HEAD],(char*)sTelNum.Sim);
			Length = strlen((char*)sTelNum.Sim);			
			Length += LEN_U0_GY_FRAME_HEAD;
		break;

		//  	$$$$$$$$$$$$$$$$$$$$ ����дָ��ҳ��Nand Flash������
		//    ���ڽ��ո�ʽ xx xx xxxx xx *x	   
		//    GYE-Test I \x00 \x00 \x00 \x00 \x00 \x00 \x00 \x00 \x00 \x00 \x00 \x00 \x00 

		case eGY_rCMD_GET_SET_EE_DATA:
		
			pEE = (pRW_EEP)&U0_RecBuffer[LEN_U0_GY_FRAME_HEAD];

			t = U0_RecBuffer[LEN_U0_GY_FRAME_HEAD];

			if( t=='S')		// 	SPI����  GY S \x00 R \xa0 \x00\x00 \x10\x00
			{
				if(pEE->RW == 'E')
				{
					EreadFRAM_RecodeMem( pEE->Addr,  pEE->Length);
				}				
				else if(pEE->RW == 'W')			// 'W' д
				{
					DFE.Write(pEE->Addr,pEE->Length,&U0_RecBuffer[LEN_U0_GY_FRAME_HEAD+8]);		 /////////// change by wrc  ///////////////
				}
				else	//if(pEE->RW == 'R')			  	// 'R' ��
				{
					Length = pEE->Length;
					DFE.Read(pEE->Addr,Length,&TxdTBuffer[LEN_U0_GY_FRAME_HEAD]);					
				}		
			}
			else if( t=='I')			// I2C 	
			{
				if(pEE->RW == 'W')			// 'W' д	  	GY I \x00 W \xa0 \x00\x00 \x10\x00
				{
					I2C0.WriteEEPROM(pEE->DS,pEE->PageLen,pEE->Addr,pEE->Length,&U0_RecBuffer[LEN_U0_GY_FRAME_HEAD+8]);  //pEE->PageLen		/////////// change by wrc  ///////////////
				}
				else if(pEE->RW == 'R')			  	// 'R' ��		GY I \x00 R \xa0 \x00\x00 \x10\x00
				{
					Length = pEE->Length;
					I2C0.ReadEEPROM(pEE->DS,pEE->Addr,Length,&TxdTBuffer[LEN_U0_GY_FRAME_HEAD]);
				}
				
			}			
			else
			  Error = TRUE;
			Length += LEN_U0_GY_FRAME_HEAD;	
			break;		
		//		$$$$$$$$$$$$$$$$$$$$  ��дȡ���ݴ洢�� SPI-Flash
		//    ��ʽ x x xx xx    GY F \x00 R D \xE0\x1E \x10\x00 
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
				LPC_UART0->THR  = *pTxd++;						// ��������		
				Txd.Sending = TRUE;
				os_dly_wait(20);
				return;
			}
			
			pEE = (pRW_EEP)&U0_RecBuffer[LEN_U0_GY_FRAME_HEAD];
			
			t = U0_RecBuffer[LEN_U0_GY_FRAME_HEAD];			

			if( t == 'F' )							// SPI Flash GY F \x00 E S \xE0\x1E \x10\x00 
			{
				if(pEE->RW == 'E')			// 'W' ����
				{
					if(pEE->DS == 'B')		// ������
					{
						DFF.BlockErase(pEE->Addr);						
					}
					else if(pEE->DS == 'S')	// ��������
					{
						DFF.SectorEraseWaitOK(pEE->Addr,10);
					}
					Length = 0;
				}
				else if(pEE->RW == 'W')			// 'W' д
				{
					DFF.PageProgramWaitOK(pEE->Addr,pEE->Buffer,3);
					Length = 0;
				}
				else	//	if(pEE->RW == 'R')		// 'R' ��
				{
					if(pEE->DS == 'D')			//  ������
					{
						DFF.PageRead(pEE->Addr,&TxdTBuffer[LEN_U0_GY_FRAME_HEAD]);
						Length = 256;
					}
					else if(pEE->DS == 'S')		// ��״̬�Ĵ�����ID
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
		 	
		 	
		 	
		case 	eGY_rCMD_SET_INIT_SYS:       //�ָ���������		 ͬ����ͨ������һ��
		  	/*DFE.Read(EE_JT_MAIN_SERVER, LEN_JT808_IP_DNS, (uchar *)&JT808_IP_DNS ); //Bf.tBuffer);
			os_dly_wait(1);
			EreadFRAM_RecodeMem(16, 0x2000-16);									   //������ƬFram	  ������������ַ
			os_dly_wait(1);
			DFE.Write(EE_JT_MAIN_SERVER, LEN_JT808_IP_DNS, (uchar *)&JT808_IP_DNS);
		   EreadFirstBlock();	
			Uart.puts("����ָ��ָ�����ֵ������ϵͳ ");
			os_dly_wait(100);
			DisableIRQ();
	      NOP();	NOP();	NOP();	NOP();
	      NOP();	NOP();	NOP();	NOP();
			for(;;)											     //�ָ��������û���Ҫ����һ��
	      {
	      	while(1);	
	      } */
			
		  	break;	
		case  eGY_rCMD_ERASE_FLASH:	
			
			if ( (U0_RecBuffer[LEN_U0_GY_FRAME_HEAD]=='S')&&(U0_RecBuffer[LEN_U0_GY_FRAME_HEAD]=='E') )	// ����ָ��������
			{
				
			}
			else if ( (U0_RecBuffer[LEN_U0_GY_FRAME_HEAD]=='B')&&(U0_RecBuffer[LEN_U0_GY_FRAME_HEAD]=='L') )	// ָ����
			{
				
			}
			
			break;
			
		case eGY_rCMD_ERASE_FLASH_RECORD_DATA:																			
			
			

			Length = LEN_U0_GY_FRAME_HEAD;
		break;	
		
		//////////////////////////////////////// ����������ȫ��Flashҳ�洢������
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
					i = U0_RecBuffer[LEN_U0_GY_FRAME_HEAD+1];		// ��ʼ����
					t = U0_RecBuffer[LEN_U0_GY_FRAME_HEAD+2];		// �ܸ���
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
			else	// ��
			{
				if( U0_RecBuffer[LEN_U0_GY_FRAME_HEAD+1]=='A') 
			   {
			   	memmove(&TxdTBuffer[LEN_U0_GY_FRAME_HEAD], (uchar*)&FlashPageCnt,LEN_FLASH_PAGE_CNT);
			   	Length += LEN_FLASH_PAGE_CNT;
			   }
			   else if((U0_RecBuffer[LEN_U0_GY_FRAME_HEAD+1]+U0_RecBuffer[LEN_U0_GY_FRAME_HEAD+2]) < FLASH_PAGE_CNT_NUM )
				{					
					i = U0_RecBuffer[LEN_U0_GY_FRAME_HEAD+1];		// ��ʼ����
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
		
		////////////////////////////////////// ��ȡFlashҳ�洢������
		case eGY_rCMD_GET_FLASH_PAGE_CNT:
			
			memmove( &TxdTBuffer[LEN_U0_GY_FRAME_HEAD],(uchar*)&FlashPageCnt,LEN_FLASH_PAGE_CNT);
			DFE.Read( FM_FLASH_PAGE_CNT,LEN_FLASH_PAGE_CNT,(uchar*)&TxdTBuffer[LEN_U0_GY_FRAME_HEAD+LEN_FLASH_PAGE_CNT]);
			Length = LEN_U0_GY_FRAME_HEAD+LEN_FLASH_PAGE_CNT*2;
		break;


		case	eGY_rCMD_GET_DRIVER_IC_CARD_INFO:		// ��ȡ��ʻԱIC����Ϣ128B
		 	if((GET_IC_CARD_STATUS)==0)		// IC���Ѳ��� �����ڳ���������  
			//if((GET_IC_CARD_STATUS)!=0)		// IC���Ѳ���	(���ڳ��տ���)
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
				 	//Uart.puts("����ʧ��");
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
				
				 //Uart.puts("δ�忨");
			}
					


		break;
		
		//		$$$$$$$$$$$$$$$$$$$$  8����־λ
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
				  SimuSpeed.Enable = 0;					  //������������⣬Ҳ�����ٶ�ģ��
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
			��ȡGSM/GPRS�����Ϣ
		*/
		//		$$$$$$$$$$$$$$$$$$$$  ��ȡ����Ϣ�������ĺ���
		case eGY_rCMD_GET_SMS_CENTER_NUMBER:
			NOP();	NOP();
			memmove(&TxdTBuffer[LEN_U0_GY_FRAME_HEAD],sTelNum.SmsCenter,16);
			//strcpy((char*)sTelNum.SmsCenter,(char*)&U0_RecBuffer[LEN_U0_GB_FRAME_HEAD]);
			//DFE.Read(EE_SMS_CENTER_NUMBER,16,&TxdTBuffer[LEN_U0_GY_FRAME_HEAD+16]);
			Length = LEN_U0_GY_FRAME_HEAD+16;
			break;
		//		$$$$$$$$$$$$$$$$$$$$  ��������ĺ���
		case eGY_rCMD_GET_CENTER_NUMBER:
			NOP();	NOP();
			DFE.Read(EE_CENTER_NUMBER,48,&TxdTBuffer[LEN_U0_GY_FRAME_HEAD]);
			
			Length = LEN_U0_GY_FRAME_HEAD+64;
			break;
		//		$$$$$$$$$$$$$$$$$$$$  ��ȡGPRS������IP�Ͷ˿ں�
		case eGY_rCMD_GET_GPRS_SERVER_IP:						
			NOP();	NOP();
			SetBufferValue(TxdTBuffer,160,0);
			strcpy((char*)&TxdTBuffer[6],(char*)sIP.TCP);			
			DFE.Read(EE_IP_PORT,96,&TxdTBuffer[6+32]);			
			Length = LEN_U0_GY_FRAME_HEAD+32*4;						
			break;

		//		$$$$$$$$$$$$$$$$$$$$  ��ȡ�ͺź��ն�ID
		case eGY_rCMD_GET_OEM_ID:
		case eGY_rCMD_GET_SOLE_NUMBER:
			
			SetBufferValue(TxdTBuffer,160,0);
			strcpy((char*)&TxdTBuffer[6],(char*)sOEM_ID);			
			Length = LEN_U0_GY_FRAME_HEAD+16;
			break;
		
		//		$$$$$$$$$$$$$$$$$$$$  ��ȡ������Web
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
						
					
		//		$$$$$$$$$$$$$$$$$$$$  ���ö���Ϣ���ĺ���
		case eGY_rCMD_SET_SMS_CENTER_NUMBER:
			SetBufferValue(sTelNum.SmsCenter, 16, 0);
			strcpy((char*)sTelNum.SmsCenter,(char*)&U0_RecBuffer[LEN_U0_GY_FRAME_HEAD]);
			DFE.Write(EE_SMS_CENTER_NUMBER,16,sTelNum.SmsCenter);			
			Length = LEN_U0_GY_FRAME_HEAD;
			#if(DFE_DEBUG==TRUE)
			if(TD.MainTask==tMAIN_WRITE_DFE)	Uart.puts("DFE08\r\n\0");
			#endif
			break;
		//		$$$$$$$$$$$$$$$$$$$$  ���ü�����ĺ���
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
		//		$$$$$$$$$$$$$$$$$$$$  ����GPRS������IP�Ͷ˿ں�
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
			

		//		$$$$$$$$$$$$$$$$$$$$  ���͵�����1
		case eGY_rCMD_SET_SEND_TO_UART:
			if (U0_RecBuffer[LEN_U0_GY_FRAME_HEAD]==0) // ͸����U0
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
				
				DFE.Write(EE_SYS_GPS_SPEED_CALCULUS,12,(uchar *)&SystemLog.GPS.SpeedCaculus);		// д��
				os_dly_wait(1);
				DFE.Write(EE_SYS_VDR_MOTOR_RUN_TIME,16,(uchar *)&SystemLog.VDR.HighSpeedPulsCnt);		// д��
				//DFE.Write(EE_SPEED_CALCULUS,16,(uchar *)&SystemLog.GPS.SpeedCaculus);		// д��
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
				for(i=0;i<EE_TOTAL_PAGE;i++)			// �����Lengyh ���� Address
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
				Idle.EnableInterIdleMode = U0_RecBuffer[LEN_U0_GB_FRAME_HEAD+8];	// �������ʡ��ģʽ
				Idle.IdleModeFlag = 0;
				Idle.InterIdleModeTime = U0_RecBuffer[LEN_U0_GB_FRAME_HEAD+9];		// ����ʱ������ʡ��ģʽ
				Idle.WakeUpTime = U0_RecBuffer[LEN_U0_GB_FRAME_HEAD+10];				// ����ʱ�����
				Idle.WakeUpLongTime = U0_RecBuffer[LEN_U0_GB_FRAME_HEAD+11];		// ���ѳ���ʱ��
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
			Uart.puts("���ڲ�����ֱ������ϵͳָ��\r\n\0");
			Uart.PopAlignmentSend();
			os_dly_wait(10);	

			GPRS.RestoreFactorySetting(0);
			DisableIRQ();
			while(1);
			//break;
	

		
		////////////////////////////////////////////// 	���ڷ��Ͷ���
		case  eGY_rCMD_SEND_SMS:							  
		   
		   if(U0_RecBuffer[LEN_U0_GY_FRAME_HEAD+0]=='P')		 //����PDU����
			{
				t = U0_RecBuffer[LEN_U0_GY_FRAME_HEAD+1];	
				Length = U0_RecBuffer[LEN_U0_GY_FRAME_HEAD+2];	
				memmove(TxdTBuffer,&U0_RecBuffer[LEN_U0_GY_FRAME_HEAD+3], Length);
				i = U0_RecBuffer[LEN_U0_GY_FRAME_HEAD+3+Length];
				SMS.SetPduDataMessage(t, (char *)TxdTBuffer, &U0_RecBuffer[LEN_U0_GY_FRAME_HEAD+4+Length], i);
			}
			else if(U0_RecBuffer[LEN_U0_GY_FRAME_HEAD+0]=='T')	  //����Text����
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
			
			// ע��
			// GY 44 01 00000 BAJ-A1 \x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00 C088888 \x01 ��C88888 \x00 U A 1234567890A \x00 \x00
			// GY \x2c\x00 \x69\x00 00000 BAJ-A1 \x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00 C088888 \x01 ��C88888 \x00 U A 13886088888 \x00 \x00
			// GY \x2c\x00 \x69\x00 00000 GY-G11A \x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00 2280582 \x02 ��A80582 \x00 U A 22280582 \x00 \x00 \x00 \x00 
			case eGY_rCMD_SET_REG_CODE:
				
				// ʡ��
				Len = 0;
				DFE.Write(EE_PROVINCE_REGION_ID, 2, (uchar *)&U0_RecBuffer[LEN_U0_GY_FRAME_HEAD]);
				NOP(); NOP(); NOP();
				Len += 2;
			
				// ������
				DFE.Write(EE_PREFECTURE_REGION_ID, 2, (uchar *)&U0_RecBuffer[LEN_U0_GY_FRAME_HEAD+Len]);
				NOP(); NOP(); NOP();
				Len += 2;
			
				// ������ID��5B)���ͺţ�20B)���ն�ID��7B��
				DFE.Write(EE_VENDOR_ID, 32, &U0_RecBuffer[LEN_U0_GY_FRAME_HEAD+Len]);
				NOP(); NOP(); NOP();	 
				Len += 32; 
				
				// ������ɫ
				DFE.Write(FM_CAR_COLOUR, 1, &U0_RecBuffer[LEN_U0_GY_FRAME_HEAD+Len]);	
				NOP(); NOP(); NOP();	      
				Len++;
			
				// ���ƺ� //���ﲻ��Ҫ���
				DFE.Write(FM_VEHICLE_CODE, 9, &U0_RecBuffer[LEN_U0_GY_FRAME_HEAD+Len]);
				NOP(); NOP(); NOP();	   
				Len += 9;


				if( U0_RecBuffer[LEN_U0_GY_FRAME_HEAD+Len]=='U')
				{
					CreateUserFlag = OK;		  // ����
					DFE.Write(EE_CREATE_USER_FLAG, 1, &CreateUserFlag);
					NOP(); NOP(); NOP();	
				}
				Len++;

				if( U0_RecBuffer[LEN_U0_GY_FRAME_HEAD+Len]=='A')
				{
					// ��ȡ��Ȩ�룬����ʱ������0����
					Len++;
					NeedRegServer = 0;	
					DFE.Write(EE_NEED_REG_SERVER, 1, &NeedRegServer);	
					
					SetBufferValue(  (uchar*)jt_auth_code, LEN_AUTH_CODE,0);
					strcpy((char*)&jt_auth_code[0],(char*)&U0_RecBuffer[LEN_U0_GY_FRAME_HEAD+Len]);
					DFE.Write(EE_AUTH_CODE, LEN_AUTH_CODE,(uchar *)&jt_auth_code[0]);			//  д��Ȩ�뵽����
										
					NOP(); NOP(); NOP();	
				}				

				NOP(); 	NOP(); 	NOP();	NOP();
				Length = LEN_U0_GY_FRAME_HEAD;	


			break;

			case	eGY_rCMD_SET_CREAT_USER_TEST:
				if( (U0_RecBuffer[LEN_U0_GY_FRAME_HEAD]=='U') && (U0_RecBuffer[LEN_U0_GY_FRAME_HEAD+1]=='L'))
				{
					CreateUserFlag = OK;		  // ����
					DFE.Write(EE_CREATE_USER_FLAG, 1, &CreateUserFlag);
					NOP(); NOP(); NOP();	
					Uart.puts("�����ɹ�");
				}

			break;
		
			case	eGY_rCMD_SET_TER_PHONE_NUM:			 //�����ն��ֻ��� 		///// GY S 13800000000	\x00
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
				
				strcpy( (char*)pData,"�洢��ʼҳ��ַ: ");
				
				strcat( (char*)pData,"\r\n48H: ");	
				Len = strlen( (char*)pData);
				Int2Str( FF_48H_SPEED_START_PAGE, (char*)(pData+Len));
				
				strcat( (char*)pData,"\r\n360H: ");	
				Len = strlen( (char*)pData);
				Int2Str( FF_360H_LOCATION_START_PAGE, (char*)(pData+Len));
				
				strcat( (char*)pData,"\r\nͣ������: ");	
				Len = strlen( (char*)pData);
				Int2Str( FF_PARKING_DATA_START_PAGE, (char*)(pData+Len));
				
								
				strcat( (char*)pData,"\r\nCAN BUS: ");	
				Len = strlen( (char*)pData);
				Int2Str( FF_CAN_DATA_START_PAGE, (char*)(pData+Len));
				
				strcat( (char*)pData,"\r\n������Ϣ: ");	
				Len = strlen( (char*)pData);
				Int2Str( FF_ALARM_MSG_START_PAGE, (char*)(pData+Len));
				
				strcat( (char*)pData,"\r\n��ͨ��Ϣ: ");	
				Len = strlen( (char*)pData);
				Int2Str( FF_NORMAL_MSG_START_PAGE, (char*)(pData+Len));
				
				strcat( (char*)pData,"\r\n��ʷ�켣: ");	
				Len = strlen( (char*)pData);
				Int2Str( FF_GPS_MESSAGE_START_PAGE, (char*)(pData+Len));
				
				strcat( (char*)pData,"\r\nä���켣: ");
				Len = strlen( (char*)pData);
				Int2Str( FF_GPS_BLIND_MESSAGE_START_PAGE, (char*)(pData+Len));
								
				strcat( (char*)pData,"\r\nGNSS: ");				
				Len = strlen( (char*)pData);
				Int2Str( FF_GNSS_DATA_START_PAGE, (char*)(pData+Len));
				
				strcat( (char*)pData,"\r\nͼƬ����: ");	
				Len = strlen( (char*)pData);
				Int2Str( FF_PIC_START_PAGE, (char*)(pData+Len));
				
				
													
				Len = strlen( (char*)pData);
				
				
				Length = LEN_U0_GY_FRAME_HEAD+Len;
				
				
				
			break;


			case	eGY_rCMD_SET_DRIVER_IC_CARD_INFO:	 // ���ü�ʻԱIC����Ϣ128B
				if((GET_IC_CARD_STATUS)==0)		// IC���Ѳ��� �����ڳ���������  
				//if((GET_IC_CARD_STATUS)!=0)		// IC���Ѳ���	(���ڳ��տ���)
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
						 	//Uart.puts("д��ʧ��");
	
							break;
						 }	 	
						
					 } 
					
					if (s != ERROR)
					{
						TxdTBuffer[LEN_U0_GY_FRAME_HEAD] = 0;
						Length = LEN_U0_GY_FRAME_HEAD+1;
			 			//Uart.puts("д���ɹ�");
		
					}
		
					ICCardBusing = FALSE;
		
					
				
				}
				else
				{
					 TxdTBuffer[LEN_U0_GY_FRAME_HEAD] = 2;
					 Length = LEN_U0_GY_FRAME_HEAD+1;
					
					 //Uart.puts("δ�忨");
				}
					

			break;

			
			            
			case  eGY_rCMD_SET_FACTORY_RESET:
            
				Length = 0;
            s = U0_RecBuffer[LEN_U0_GY_FRAME_HEAD];
            //�ָ���������	  �ض� �����������ݰ���IP��ַ�ͼ�¼������
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
		putb(0,Length,TxdTBuffer);		// ������Ϣ����λ��		
	}	
				
	NOP();	NOP();
}
#endif	



/************************************************************************************************
** �������ƣ�EreadFRAM_RecodeMem()
** ������������������ĳЩ�ռ�
** ��ڲ�������ʼ��ַ����������
** ���ڲ�����
** ȫ�ֱ���: ��
** ȫ�ֺ���: 
** ����ʱ�䣺
** �޸�ʱ�䣺2011.09.17
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
** �������ƣ�WaitUSBGetData()
** �����������ȴ�SUBд������
** ��ڲ������ȴ�ʱ�䣬 ���ݳ���
** ���ڲ����������ɹ�������
** ȫ�ֱ���: ��
** ȫ�ֺ���: 
** ����ʱ�䣺
** �޸�ʱ�䣺2011.09.17
**************************************************************************************************/ 	
void				UartCls::WaitUSBGetData(uchar DlyTime, uint16 DataLen)
{				
	uchar  n, *buffer;
	uint16 t;
	n = DlyTime;
	LenDataForUSB = DataLen;

	buffer = &TxdTBuffer[0];
	for(t=0; t<LenDataForUSB; t++)	{ USBCheckSum ^= *(buffer+t);	}
   
	//while((LenDataForUSB>0)&&(n>0))					            //���޵�ʱ���ڵȴ�USB���������ݣ�
	if(Write_File_Error==0)
	{
		WrintDataToFile();
		//os_dly_wait(1);
	   n--;
	}
}
/************************************************************************************************
** �������ƣ�SetHeadUSBBlockData()
** ��������������SUB�������ͷ
** ��ڲ������������ͣ� ������ţ����ݳ���
** ���ڲ���������ͷ����
** ȫ�ֱ���: ��
** ȫ�ֺ���: 
** ����ʱ�䣺
** �޸�ʱ�䣺2011.09.17
**************************************************************************************************/ 	
uint32   UartCls::SetHeadUSBBlockData(uchar DataType, uchar DataSierieNb, uint32 DataLen, uchar *DataBuffer)
{
	uint32 Len, L;
														 
  *DataBuffer = DataType;							  //���ݴ���
  	Len = 1;

   SetBufferValue(DataBuffer+Len, 18,0);
	strcpy((char *)(DataBuffer+Len), (char *)&GBDataName[DataSierieNb][0]);		 //������ 
	Len += 18;			 // ��������Ϊ�̶�Ϊ18�ֽ�
	L = DataLen;												    //���ݼ�¼����
	L = SwapINT32(L);
	memmove(DataBuffer+Len, (uchar *)&L, 4);
	Len += 4;

	return Len;
}













/*
 $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$  End of File $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
 */



